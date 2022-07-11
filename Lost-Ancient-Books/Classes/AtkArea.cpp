#include "AtkArea.h"
#include "GameAction.h"
#include "GlobalWorld.h"
#include "AnimationLoader.h"
USING_NS_CC;

ParticleSystem* AtkArea::openParticle()
{
	_particle = _particleFactory->getParticleSystem();
    _particle->setPosition(getPosition());
    //_particle->setTotalParticles(100);
    Node* batch = Batchs::getInstance()->getParticleNode();
    _particle->setAutoRemoveOnFinish(true);
    _particle->setPositionType(ParticleSystem::PositionType::RELATIVE);
	batch->addChild(_particle);
    return _particle;
}

void AtkArea::onExit()
{
    Crash::onExit();
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
        _particle = nullptr;
    }
}

void AtkArea::_attack(cocos2d::Node* other)
{
    ((GameSprite*)other)->beAttack(_harm, _atkDir);
    if (_btTime > 0 && _btLen > 0)
        other->runAction(MoveBy::create(_btTime, Vec2(this->getAtkDir() ? _btLen : -_btLen, 0)));
}

void AtkArea::attack(Node* other)
{
    if (other->getTag() & (T_BOY | T_MONSTER))
    {
        if (!((GameSprite*)other)->isDead())
        {
            _attack(other);
        }
    }
}

void AtkArea::boom()
{
    if (_boomFactory)
    {
        auto boom = getBoomAtkArea();
        boom->setScale(boom->getScale() * this->getScale());
        getParent()->addChild(boom, 10);
    }
}

AtkArea* AtkArea::getBoomAtkArea()
{
    auto boom = _boomFactory->getAtkArea();
    boom->setPosition(getPosition());
    boom->setHarm(_boomHarm);
    return boom;
}

AtkArea::AtkArea() :
    _harm(0),
    _existTime(0.2f),
    _particleFactory(nullptr),
    _particle(nullptr),
    _boomFactory(nullptr),
    _btLen(0),
    _btTime(0),
    _boomHarm(10),
    _atkDir(false)
{
}

AtkArea::~AtkArea()
{
    CC_SAFE_DELETE(_particleFactory);
    CC_SAFE_DELETE(_boomFactory);
}

////////////////////////ThroughAtkArea///////////////////////////////////////////////////////////////////////////

ThroughAtkArea* ThroughAtkArea::create(const std::string& filename)
{
    ThroughAtkArea* area = new (std::nothrow) ThroughAtkArea();
    if (area && area->initWithFile(filename))
    {
        area->autorelease();
        return area;
    }
    CC_SAFE_DELETE(area);
    return nullptr;
}

ThroughAtkArea* ThroughAtkArea::createWithSpriteFrame(cocos2d::SpriteFrame* frame)
{
    ThroughAtkArea* area = new (std::nothrow) ThroughAtkArea();
    if (area && area->initWithSpriteFrame(frame))
    {
        area->autorelease();
        return area;
    }
    CC_SAFE_DELETE(area);
    return nullptr;
}

void ThroughAtkArea::endCrash()
{
    this->runAction(CallFunc::create([this]() {this->boom(); }));
}

ThroughAtkArea* ThroughAtkArea::create()
{
    ThroughAtkArea* area = new (std::nothrow) ThroughAtkArea();
    if (area && area->init())
    {
        area->autorelease();
        return area;
    }
    CC_SAFE_DELETE(area);
    return nullptr;
}

bool ThroughAtkArea::crash(cocos2d::Node* other)
{
    this->attack(other);
    this->endCrash();
    return true;
}

/////////////////////////CantThroughAtkArea///////////////////////////////////////////////////////////////
CantThroughAtkArea::CantThroughAtkArea()
    :AtkArea()
    , _crashTime(0)
    , _canCrashTime(1)
{
}

CantThroughAtkArea* CantThroughAtkArea::create(const std::string& filename)
{
    CantThroughAtkArea* area = new (std::nothrow) CantThroughAtkArea();
    if (area && area->initWithFile(filename))
    {
        area->autorelease();
        return area;
    }
    CC_SAFE_DELETE(area);
    return nullptr;
}

CantThroughAtkArea* CantThroughAtkArea::createWithSpriteFrame(cocos2d::SpriteFrame* spriteFrame)
{
    CantThroughAtkArea* area = new (std::nothrow) CantThroughAtkArea();
    if (area && area->initWithSpriteFrame(spriteFrame))
    {
        area->autorelease();
        return area;
    }
    CC_SAFE_DELETE(area);
    return nullptr;
}

void CantThroughAtkArea::endCrash(cocos2d::Node* other)
{
    auto boomFun = CallFunc::create([this]() {this->boom(); });
    if (_crashTime >= _canCrashTime || other->getTag() & (T_WALL | T_PLATFORM))
        this->runAction(Sequence::create(boomFun, RemoveSelf::create(), nullptr));
    else
        this->runAction(boomFun);
}

bool CantThroughAtkArea::crash(cocos2d::Node* other)
{
    if (_crashTime >= _canCrashTime)
        return true;
    _crashTime++;

    this->attack(other);
    this->endCrash(other);

    return true;
}

/****************************************/
BoomAtkArea::BoomAtkArea():_hasAtk(1)
{}

bool BoomAtkArea::crash(cocos2d::Node* other)
{
    this->attack(other);
    return true;
}

void BoomAtkArea::update(float)
{
    if (_hasAtk>0)
        --_hasAtk;
    else if (_hasAtk == 0)
    {
        --_hasAtk;
        removeComponent(_physicsBody);
        this->_physicsBody = nullptr;
    }
}

void BoomAtkArea::_attack(cocos2d::Node* other)
{
    float midX1 = getBoundingBox().getMidX();
    float midX2 = other->getBoundingBox().getMidX();
    bool atkDir = midX1 <= midX2;
    ((GameSprite*)other)->beAttack(_harm, atkDir);
    if (_btTime > 0 && _btLen > 0)
        other->runAction(MoveBy::create(_btTime, Vec2(atkDir ? _btLen : -_btLen, 0)));
}

BoomAtkArea* BoomAtkArea::create(const std::string& filename)
{
    BoomAtkArea* area = new (std::nothrow) BoomAtkArea();
    if (area && area->initWithFile(filename))
    {
        area->autorelease();
        return area;
    }
    CC_SAFE_DELETE(area);
    return nullptr;
}

BoomAtkArea* BoomAtkArea::createWithSpriteFrame(cocos2d::SpriteFrame* spriteFrame)
{
    BoomAtkArea* area = new (std::nothrow) BoomAtkArea();
    if (area && area->initWithSpriteFrame(spriteFrame))
    {
        area->autorelease();
        return area;
    }
    CC_SAFE_DELETE(area);
    return nullptr;
}

/**********************EraserAtkArea****************************************************/

bool EraserAtkArea::crash(cocos2d::Node* other)
{
    if (_crashTime >= _canCrashTime)
        return true;

    this->attack(other);

    if (_crashTime >= _canCrashTime)
        this->runAction(RemoveSelf::create());

    return true;
}

EraserAtkArea* EraserAtkArea::create(const std::string& filename)
{
    EraserAtkArea* area = new (std::nothrow) EraserAtkArea();
    if (area && area->initWithFile(filename))
    {
        area->autorelease();
        return area;
    }
    CC_SAFE_DELETE(area);
    return nullptr;
}

EraserAtkArea::EraserAtkArea():CantThroughAtkArea()
{
}

void EraserAtkArea::_attack(cocos2d::Node* other)
{
    bool atkDir = getBoundingBox().getMidX() < other->getBoundingBox().getMidX();
    ((GameSprite*)other)->beAttack(_harm, atkDir);
    if (_btTime > 0 && _btLen > 0)
        other->runAction(MoveBy::create(_btTime, Vec2(_flippedX ? -_btLen : _btLen, 0)));
    ++_crashTime;
}

/********************************FirecrackerAtkArea***************************************************/
bool FirecrackerAtkArea::crash(cocos2d::Node* other)
{
    ++_isIn;
    if (!_hasAtk)
    {
        if (other == _aim  || !_canThrough)
        {
            this->runAction(Sequence::create(CallFunc::create(CC_CALLBACK_0(FirecrackerAtkArea::myBoom, this)), RemoveSelf::create(), nullptr));
            _hasAtk = true;
            //CCLOG("Boom!");
        }
    }
    return false;
}

void FirecrackerAtkArea::update(float dt)
{
    AtkArea::update(dt);
    if (_isIn && !_hasAtk && !_canThrough)
    {
        this->runAction(Sequence::create(CallFunc::create(CC_CALLBACK_0(FirecrackerAtkArea::myBoom, this)), RemoveSelf::create(), nullptr));
        _hasAtk = true;
    }
}

void FirecrackerAtkArea::myBoom()
{
    if (_boomFactory)
    {
        auto area = getBoomAtkArea();
        area->setScale(getScale() * 4.f);
        getParent()->addChild(area);
    }
}

void FirecrackerAtkArea::outCrash(cocos2d::Node* other)
{
    if(_isIn>0)
        --_isIn;
}

FirecrackerAtkArea::FirecrackerAtkArea()
    :_aim(nullptr)
    ,_wall(nullptr)
    ,_hasAtk(false)
    , _canThrough(true)
    ,_isIn(0)
{}

void FirecrackerAtkArea::setThroughTime(float t)
{
    runAction(Sequence::create(DelayTime::create(t), CallFunc::create([this]() {
        _canThrough = false;
        }), nullptr));
}

FirecrackerAtkArea* FirecrackerAtkArea::createWithSpriteFrame(SpriteFrame* spriteFrame)
{
    auto area = new(std::nothrow) FirecrackerAtkArea();
    if (area && area->initWithSpriteFrame(spriteFrame))
    {
        area->autorelease();
        return area;
    }
    CC_SAFE_DELETE(area);
    return nullptr;
}

EraserAtkArea* EraserAtkArea::createWithSpriteFrame(SpriteFrame* frame)
{
    auto area = new(std::nothrow) EraserAtkArea();
    if (area && area->initWithSpriteFrame(frame))
    {
        area->autorelease();
        return area;
    }
    CC_SAFE_DELETE(area);
    return nullptr;
}
