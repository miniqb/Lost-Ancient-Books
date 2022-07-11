#include "Monster.h"
#include "GlobalWorld.h"
#include "CallbackThingsFactory.h"
#include "Package.h"
#include "GameHelper.h"
#include "CrashFactory.h"
#include "InfoHelper.h"
USING_NS_CC;
GameSprite* Monster::boy = nullptr;

Monster* Monster::create(cocos2d::SpriteFrame* frame, float fmt, float fwt, int atkLen, int life, float splen)
{
    Monster* monster = new (std::nothrow) Monster();
    if (monster && monster->initWithSpriteFrame(frame) && monster->Init(fmt, fwt, atkLen, life, splen,4,100,1,1))
    {
        monster->autorelease();
        return monster;
    }
    CC_SAFE_DELETE(monster);
    return nullptr;
}

bool Monster::foundBoy()
{
    return _foundForce||
        boy->getStandWall() == this->getStandWall() &&
        boy->getPositionX() > this->getPositionX() == _dir &&
        boy->getPosition().getDistanceSq(this->getPosition()) <= _radarLenSq;
}

bool Monster::canAtk()
{
    float len = boy->getPositionX() - this->getPositionX();
    return _atkLen > std::fabsf(len);
}

void Monster::changeState(MonsterState* state)
{
    if (_state)
    {
        _state->outState();
    }
    _state = state;
    _state->enterState();
}

CallbackThing* Monster::addCallbackThing()
{
    auto& infoMap = InfoHelper::getInstance()->getInfoMap();
    auto press = infoMap["press"].asString();
    auto erase = infoMap["wipe"].asString();
    auto colloct = infoMap["collect"].asString();
    auto info = press + "Q" + colloct + '\n' + press + "E" + erase;
    auto callback = CallbackThingsFactory::staticGetCallbackThing(this->getContentSize(), EventKeyboard::KeyCode::KEY_Q, info,
        [this]() {//收集
            this->runAction(RemoveSelf::create());
            if (_hanzi.size() > 0)
            {
                if (_isGood)//选对了，把汉字加入背包
                {
                    Package::getInstance()->addHanziButton(_hanzi);
                    this->boy->addLife(4);
                    createDrops();
                }
                else//选错了，制造分身
                {
                    this->createClons(false);
                }
            }
        });
    callback->addKey(EventKeyboard::KeyCode::KEY_E, 
        [this]() {//擦除
        this->runAction(RemoveSelf::create());
        if (_hanzi.size() > 0)
        {
            if (!_isGood)//选对了，把汉字加入背包
            {
                Package::getInstance()->addHanziButton(_hanzi);
                this->boy->addLife(4);
                createDrops();
            }
            else//选错了，制造分身
            {
                this->createClons(true);
            }
        }
        });
    callback->getPhysicsBody()->setDynamic(false);
    callback->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    return callback;
    //this->addChild(callback);
}

void Monster::updateHanziLabel()
{
    if (_hanziLabel)
    {
        Rect rect = getBoundingBox();
        float offx = isFlippedX() ? 1 - _hanziOffect.x : _hanziOffect.x;
        Vec2 pos = rect.origin + Vec2(rect.size.width * offx, rect.size.height * _hanziOffect.y);
        _hanziLabel->setPosition(pos);
        _hanziLabel->setScale(getScale());
    }
}

Node* Monster::thowRightHanzi()
{
    auto board = CallbackThing::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("ui/n.png"));
    board->setTag(T_CALLBACK_THINGS);
    auto body = CallbackThingsFactory::getBody(board->getContentSize());
    body->setRotationEnable(false);
    body->setVelocity(Vec2(0, 20));
    board->setPhysicsBody(body);

    Label* hanzi = GameHelper::getHanziLabel(true, _hanzi);
    Batchs::getInstance()->getHanziNode()->addChild(hanzi);
    hanzi->runAction(FllowNode::create(board));

    auto& strMap = InfoHelper::getInstance()->getInfoMap();
    auto pressStr = strMap["press"].asString();
    auto colloctStr = strMap["collect"].asString();
    board->initWithFunc(Input::KEY_Q, pressStr + 'Q' + colloctStr, [board, hanzi]() {
        Package::getInstance()->addHanziButton(hanzi->getString());
        hanzi->runAction(RemoveSelf::create());
        board->runAction(RemoveSelf::create());
        });

    board->setPosition(getPosition());
    board->setScale(1.6f);
    hanzi->setPosition(getPosition());

    return board;
}

void Monster::strengthSelf()
{
    _allLife *= 3;
    _life *= 3;
    _atk *= 1.5;
    _speedLen *= 1.8f;
    _beforeTime *= 0.5f;
    _afterTime *= 0.5f;

    auto particle = AngerFireParticleFactory().getParticleSystem();
    Size size = getBoundingBox().size;
    size = size * 0.6f;
    particle->setPosVar(size);
    particle->setPosition(getPosition());
    particle->setAutoRemoveOnFinish(true);
    particle->setPositionType(ParticleSystem::PositionType::RELATIVE);
    Batchs::getInstance()->getParticleNode()->addChild(particle);
    particle->runAction(FllowNode::create(this));
    _particle = particle;
}

//暂时没有用
std::vector<Monster*> Monster::getClons()
{
    auto res = std::vector<Monster*>(_clonSum);
    for (size_t i = 0; i < _clonSum; i++)
    {
        res.push_back(_clonFactory->getMonster(0));
    }
    return res;
}

void Monster::createClons(bool s)
{
    if (s == false)
    {
        if (_clonSum == 0)
            return;
        std::vector<Node*> monsters;
        monsters.reserve(_clonSum);
        int* sum = new int(_clonSum);
        for (size_t i = 0; i < _clonSum; i++)
        {
            auto clon = _clonFactory->getMonster(0);
            clon->setColor(Color3B::GREEN);
            clon->_oriColor = Color3B::GREEN;
            clon->setPosition(_position);
            clon->setDir(random() % 2);
            clon->setFSSum(sum);
            clon->setHanzi(_hanzi);
            clon->_foundForce++;
            clon->randomSelf(0.5f,1.f);
            _parent->addChild(clon, this->getLocalZOrder());
            monsters.push_back(clon);
        }
        bool isPhy = monsters.front()->getPhysicsBody()->isDynamic();
        GameHelper::throwAway(monsters, 30, 150, 200, isPhy);
    }
    else
    {
        int* sum = new int(1);
        std::vector<Node*> monsters;
        auto clon = _clonFactory->getMonster(0);
        monsters.push_back(clon);
        clon->setPosition(_position);
        clon->setDir(random() % 2);
        clon->setFSSum(sum);
        clon->setHanzi(_hanzi);
        clon->strengthSelf();
        clon->_foundForce++;
        _parent->addChild(clon, this->getLocalZOrder());
        bool isPhy = monsters.front()->getPhysicsBody()->isDynamic();
        GameHelper::throwAway(monsters, 30, 150, 200, isPhy);
    }
}

void Monster::createDrops()
{
    if (_shell > 0)
    {
        size_t sum = _shell * random<double>(0.6, 1.4);
        std::vector<Node*> shells;
        shells.reserve(sum);
        auto factory = ShellFactory();
        for (size_t i = 0; i < sum; ++i)
        {
            auto s = factory.getCrash();
            s->setPosition(getPosition());
            getParent()->addChild(s, getLocalZOrder());
            shells.push_back(s);
        }
        GameHelper::throwAway(shells, 40, 140, 120);
    }

    if (_knowPiece > 0)
    {
        size_t sum = _knowPiece * random<double>(0.5, 1.2);
        std::vector<Node*> pieces;
        pieces.reserve(sum);
        auto factory = KonwPieceFactory();
        for (size_t i = 0; i < sum; ++i)
        {
            auto s = factory.getCrash();
            s->setPosition(getPosition());
            getParent()->addChild(s, getLocalZOrder());
            pieces.push_back(s);
        }
        GameHelper::throwAway(pieces, 40, 140, 120);
    }
 
}

Monster::Monster():
    GameSprite(),
    _state(nullptr),
    _station(),
    _freeMoveTime(0),
    _freeWaitTime(0),
    _radarLen(0),
    _atkLen(0),
    _vigSpeedRate(1.2f),
    _beforeTime(0),
    _afterTime(0),
    _foundForce(0),
    _isGood(true),
    _hanziLabel(nullptr),
    _clonSum(0),
    _clonFactory(nullptr),
    _shell(10),
    _knowPiece(2),
    _nowAnimate(nullptr),
    _nowAnimateRepeat(nullptr),
    _sumFS(nullptr),
    _particle(nullptr),
    _blodStrand(nullptr),
    _hanziColor(255,255,255),
    _oriColor(255,255,255)
{
}

Monster::~Monster()
{
    if (_state)
    {
        _state->outState();
    }
    if (_clonFactory)
    {
        CC_SAFE_DELETE(_clonFactory);
    }
    if (_hanziLabel)
    {
        _hanziLabel->runAction(RemoveSelf::create());
        _hanziLabel = nullptr;
    }
}

bool Monster::Init(float fmt, float fwt, int atkLen, int life, float splen, int atk, int ranLen, float beforeT, float afterT)
{
    _freeMoveTime = fmt;
    _freeWaitTime = fwt;
    _atkLen = atkLen;
    _allLife = life;
    _life = life;
    _speedLen = splen;
    _atk = atk;
    _radarLen = ranLen;
    _radarLenSq = ranLen * ranLen;
    _beforeTime = beforeT;
    _afterTime = afterT;
    this->initState();
    this->changeState(_station._stop);

    _blodStrand = BlodStrand::create(_life, _allLife);
    addChild(_blodStrand);
    Size size = getContentSize();
    _blodStrand->setPosition(Vec2(size.width * 0.5f, size.height));

    return true;
}

void Monster::randomSelf(float min, float max)
{
    _freeMoveTime *= random(min, max);
    _freeWaitTime *= random(min, max);
    _life *= random(min, max);
    _allLife = _life;
    _speedLen *= random(min, max);
    _atk *= random(min, max);
    _radarLen *= random(min, max);
    _radarLenSq = _radarLen * _radarLen;
    _beforeTime *= random(min, max);
    _afterTime *= random(min, max);
}

void Monster::attack()
{
}

void Monster::stopAttack()
{
}

void Monster::move()
{
    float dp = getFreeMoveTime() * getSpeedLen();
    if (isOnOutRight()||(!isOnOutLeft() && random() % 2))
        dp = -dp;

    auto moveBy = MoveByWithWall::create(
        this->getFreeMoveTime(),
        Vec2(dp, 0),
        true, false);
    moveBy->setTag(tagMove);
    runAction(moveBy);
}

bool Monster::isMoveing()
{
    Action* act = getActionByTag(tagMove);
    if (act == nullptr)
        return false;

    return !act->isDone();
}

void Monster::stopMove()
{
    stopActionByTag(tagMove);
}

void Monster::stand()
{
}

void Monster::stopStand()
{
}

void Monster::vig()
{
    auto act = MoveToNode::create(Monster::boy, this->getSpeedLen() * _vigSpeedRate, true, 0.f);
    act->setTag(tagVig);
    runAction(act);
}

void Monster::stopVig()
{
    stopActionByTag(tagVig);
}

void Monster::beforeAtk()
{
}

void Monster::stopBeforeAtk()
{
}

void Monster::afterAtk()
{
}

void Monster::stopAfterAtk()
{
}

void Monster::dead()
{
    if (_sumFS == nullptr)
    {
        addChild(addCallbackThing());
    }
    else
    {
        (*_sumFS)--;
        if (*_sumFS == 0)
        {
            auto board = thowRightHanzi();
            getParent()->addChild(board,this->getLocalZOrder());
            CC_SAFE_DELETE(_sumFS);
        }
        runAction(Sequence::create(DelayTime::create(1.5f), FadeOut::create(1), RemoveSelf::create(), nullptr));
    }
}

void Monster::beAttack(int h, bool isRight)
{
    this->getHurt(h);
    auto tin = TintTo::create(0.1f, 255, 155, 155);
    auto rtin = TintTo::create(0.1f, _oriColor);
    auto sqe = Sequence::create(tin, rtin, nullptr);
    sqe->setTag(tagTiny);
    runAction(sqe);
    tempFoundForce(5);
    if (_life <= 0)
    {
        this->setSpeed(0, 0);
        this->runAction(CallFunc::create([this, isRight]() {
            _physicsBody->setDynamic(true);
            _physicsBody->setRotationEnable(false);
            _physicsBody->applyImpulse(Vec2(isRight ? 200000 : -200000, 220000));
            }));
    }
}

void Monster::onExit()
{
    GameSprite::onExit();
    if (_particle)
    {
        if (_particle->getParticleCount() == 0)
            _particle->removeFromParent();
        else
        {
            _particle->setDuration(0);
            _particle->stopAllActions();
            _particle->stopSystem();
        }
    }
}

void Monster::insideUpdate(float dt)
{
    MonsterState* new_state = _state->update(dt);
    if (new_state)
    {
        this->changeState(new_state);
    }
    this->setFlippedX(!_dir);
    updateHanziLabel();
    _blodStrand->setBlod(_life, _allLife);
}

void Monster::suitAtkArea(AtkArea* area)
{
    area->setHarm(_atk);
    area->setFlippedX(isFlippedX());
    area->setAtkDir(!isFlippedX());
}

void Monster::initState()
{
    _station._move = new MonsterMove(this);
    _station._stop = new StopState(this);
    _station._vig = new VigState(this);
    _station._before = new BeforeAtkState(this);
    _station._atk = new AtkState(this);
    _station._after = new AfterAtkState(this);
    _station._dead = new DeadState(this);
}

void Monster::tempFoundForce(float time)
{
    ++_foundForce;
    runAction(Sequence::create(DelayTime::create(time), CallFunc::create([this]() {--_foundForce; }), nullptr));
}

void Monster::runRunAnimate(bool isForver)
{
    ActionInterval* an = Animate::create(_animations._anRun);
    if (isForver)
        an = RepeatForever::create(an);
    an->setTag(tagRunAnimate);
    runAction(an);
}

void Monster::stopRunAnimate()
{
    stopActionByTag(tagRunAnimate);
}

void Monster::runVigAnimate(bool isForver)
{
    ActionInterval* an = Animate::create(_animations._anVig);
    if (isForver)
        an = RepeatForever::create(an);
    an->setTag(tagVigAnimate);
    runAction(an);
}

void Monster::stopVigAnimate()
{
    stopActionByTag(tagVigAnimate);
}

void Monster::runAttackAnimate(bool autoAtk, bool isForver)
{
    ActionInterval* an = Animate::create(_animations._anAttack);
    if (isForver)
        an = RepeatForever::create(an);
    else if (autoAtk)
    {
        setAtk(true);
        an = Sequence::create(an, CallFunc::create(CC_CALLBACK_0(Monster::setAtk, this, false)), nullptr);
    }
    an->setTag(tagAtkAnimate);
    runAction(an);
}

void Monster::stopAttackAnimate()
{
    stopActionByTag(tagAtkAnimate);
}

void Monster::runStandAnimate(bool isForver)
{
    ActionInterval* an = Animate::create(_animations._anStand);
    if (isForver)
        an = RepeatForever::create(an);
    an->setTag(tagStandAnimate);
    runAction(an);
}

void Monster::runAfterAtkAnimate()
{
    runStandAnimate();
}

void Monster::stopAfterAtkAnimate()
{
    stopStandAnimate();
}

void Monster::stopStandAnimate()
{
    stopActionByTag(tagStandAnimate);
}

void Monster::runDeadAnimate(bool isForver)
{
    ActionInterval* an = Animate::create(_animations._anDead);
    if (isForver)
        an = RepeatForever::create(an);
    an->setTag(tagDeadAnimate);
    runAction(an);
}

void Monster::stopDeadAnimate()
{
    stopActionByTag(tagDeadAnimate);
}

/*****************************Vig*****************************************************/
void VigState::enterState()
{
    _monster->runVigAnimate();
    _monster->vig();
}

void VigState::outState()
{
    _monster->stopVigAnimate();
    _monster->stopVig();
}

MonsterState* VigState::insideUpdate(float dt)
{
    if (!_monster->foundBoy())//失去目标，转回闲逛状态
        return _monster->getStation()._stop;
    if (_monster->canAtk())//可以攻击，转向前摇状态
        return _monster->getStation()._before;
    return nullptr;
}

/****************************BeforeAtkState**************************************/
void BeforeAtkState::enterState()
{
    _nowTime = 0;
    _monster->runAttackAnimate(false, true);
    _monster->beforeAtk();
}

void BeforeAtkState::outState()
{
    _nowTime = 0;
    _monster->stopAttackAnimate();
    _monster->stopBeforeAtk();
}

MonsterState* BeforeAtkState::insideUpdate(float dt)
{
    if (_nowTime >= _monster->getBeforeTime())//前摇结束，进行攻击动作
        return _monster->getStation()._atk;
    _nowTime += dt;
    return nullptr;
}

/*****************************Atk*****************************************************/
void AtkState::enterState()
{
    _monster->runAttackAnimate(true);
    _monster->attack();
}

void AtkState::outState()
{
    _monster->stopAttackAnimate();
    _monster->stopAttack();
}

MonsterState* AtkState::insideUpdate(float dt)
{
    if (!_monster->isAtk())//结束攻击
        return _monster->getStation()._after;
    return nullptr;
}

/************************After****************************/
void AfterAtkState::enterState()
{
    _nowTime = 0;
    _monster->runAfterAtkAnimate();
    _monster->afterAtk();
}

void AfterAtkState::outState()
{
    _nowTime = 0;
    _monster->stopAfterAtkAnimate();
    _monster->stopAfterAtk();
}

MonsterState* AfterAtkState::insideUpdate(float dt)
{
    if (_nowTime >= _monster->getBeforeTime())//后摇结束，进入追击状态
        return _monster->getStation()._vig;
    _nowTime += dt;
    return nullptr;
}

/************************************Dead**************************************************/
void DeadState::enterState()
{
    _monster->dead();
    _monster->runDeadAnimate();
}

/***********************************NormalState************************************/
MonsterState* MonsterState::update(float dt)
{
    if (_monster->isDead())
        return _monster->getStation()._dead;
    return insideUpdate(dt);
}

void MonsterState::outState()
{
    _monster->stopDeadAnimate();
}

/************************************Move*****************************************************/
void MonsterMove::enterState()
{
    _monster->runRunAnimate();
    _monster->move();
}

void MonsterMove::outState()
{
    _monster->stopRunAnimate();
    _monster->stopMove();
}

MonsterState* MonsterMove::insideUpdate(float dt)
{
    if (_monster->foundBoy())
        return _monster->getStation()._vig;
    if (!_monster->isMoveing())
        return _monster->getStation()._stop;
    return nullptr;
}


/******************************stand***********************************************/
void StopState::enterState()
{
    _nowTime = 0;
    _monster->runStandAnimate();
    _monster->stand();
}

void StopState::outState()
{
    _nowTime = 0;
    _monster->stopStandAnimate();
    _monster->stopStand();
}

MonsterState* StopState::insideUpdate(float dt)
{
    if (_monster->foundBoy())
        return _monster->getStation()._vig;
    if (_nowTime >= _monster->getFreeWaitTime())//站立结束，进入移动状态
        return _monster->getStation()._move;
    _nowTime += dt;
    return nullptr;
}
