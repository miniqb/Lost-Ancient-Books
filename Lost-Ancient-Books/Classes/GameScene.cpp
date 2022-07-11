#include "GameScene.h"
#include "GlobalWorld.h"
#include "Monster.h"
#include "GameAction.h"
#include "CallbackThingsFactory.h"
#include "QuestionHelper.h"
#include "Package.h"
#include "Seesaw.h"
#include "GameConfig.h"
#include "SkillPowerFactory.h"
#include "MapHelper.h"
#include "MapBuilder.h"
#include "GameHelper.h"
#include "AnimationLoader.h"
#include "ParticleFactory.h"
#include "WeaponFactory.h"
#include "InfoHelper.h"
#include "WeaponInfo.h"
#include "SkillInfo.h"
#include "HanziScene.h"
#include "MyTransitionScene.h"
#include "TempScene.h"
#include "AudioEngine.h"

#define CRASH_SELECT(nodeA,nodeB,t,res) \
do{\
    if(nodeA->getTag()==t)\
         res = ((Crash*)nodeA)->crash(nodeB);\
    if(nodeB->getTag()==t)\
         res = ((Crash*)nodeB)->crash(nodeA);\
}while(0)

#define CRASH_OUT_SELECT(nodeA,nodeB,t) \
do{\
    if(nodeA->getTag()==t)\
         ((Crash*)nodeA)->outCrash(nodeB);\
    if(nodeB->getTag()==t)\
         ((Crash*)nodeB)->outCrash(nodeA);\
}while(0)

#define OPEN_BTEETRMAP

USING_NS_CC;
cocos2d::Scene* GameScene::createScene()
{
	return GameScene::create();
}

bool GameScene::init()
{
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }
    if (!this->initWithPhysics())
    {
        return false;
    }

    _physicsWorld->setSpeed(2.f);
    _physicsWorld->setGravity(Vec2(0, GlobalWorld::gravity));

    Size winSize = _director->getWinSize();

    //添加按键响应
    auto keyListener = EventListenerKeyboard::create();
    keyListener->onKeyPressed = CC_CALLBACK_2(GameScene::handleInputKeyDown, this);
    keyListener->onKeyReleased = CC_CALLBACK_2(GameScene::handleInputKeyUp, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

    //设置碰撞回调函数
    auto contactListener = EventListenerPhysicsContact::create();
    //设置监听器的碰撞开始函数
    contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
    contactListener->onContactSeparate = CC_CALLBACK_1(GameScene::onContactEnd, this);
    contactListener->onContactPreSolve = CC_CALLBACK_2(GameScene::onContactPre, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);


    //加载精灵图集
    auto spritecache = SpriteFrameCache::getInstance();
    spritecache->addSpriteFramesWithFile("spriteSheet/scene.plist");
    spritecache->addSpriteFramesWithFile("spriteSheet/queScene.plist");

    //加载动画
    auto animationLoader = AnimationLoader::getInstance();

    //移动图层
    auto para = ParallaxNode::create();
    this->addChild(para);

    auto gameConfig = GameConfig::getInstance();
    auto package = Package::getInstance();
    int nowCut = package->getNowCut();
    int nowBack = package->getNowBack();

    auto nmap = gameConfig->getMaps()[nowBack][nowCut];
    auto nback = gameConfig->getBacks()[nowBack];
    auto nsound = gameConfig->getBackSounds()[nowBack];

    //添加背景音乐
    if (package->getLastSoundName() != nsound)
    {
        AudioEngine::stopAll();
        _backSoundID = AudioEngine::play2d(nsound, true);
        AudioEngine::setVolume(_backSoundID, 0.5);
        package->setLastSoundName(nsound);
    }
    else
        _backSoundID = 0;

    //添加背景等精灵
    auto background = Sprite::create(nback);
    background->setScale(getContentSize().width / background->getContentSize().width);
    background->setAnchorPoint(Vec2());
    para->addChild(background, -1, Vec2(0, 0), Vec2());

    std::vector<Monster*> monsters;
    std::vector<CallbackThing*> weaponTables;
    std::vector<CallbackThing*> skillTables;
    std::vector<CallbackThing*> hanziDoors;
    std::vector<CallbackThing*> normalDoors;
    //_gameMap
    {
        _gameMap = TMXTiledMap::create(nmap);
        _gameMap->setAnchorPoint(Vec2());
        para->addChild(_gameMap, 1, Vec2(1, 1), Vec2());
#ifdef OPEN_BTEETRMAP
        auto& children = _gameMap->getChildren();
        SpriteBatchNode* child = nullptr;

        for (const auto& obj : children) {
            child = static_cast<SpriteBatchNode*>(obj);
            child->getTexture()->setAntiAliasTexParameters();
        }
#endif // OPEN_BTEETRMAP
        auto builder = MapBuilder(_gameMap);
        builder.handleWall();
        builder.handlePlatform();
        builder.handleSeesaw(_seesaws, PhysicsMaterial(5.f, 0, 0.2f), 20);
        builder.handleTrampoline(PhysicsMaterial(0.1f, 2.f, 0.4f));
        builder.handleDoor(hanziDoors, normalDoors);
        builder.handleMonster(monsters, 3);
        builder.handleWeaponTable(weaponTables);
        builder.handleSkillTable(skillTables);

        //精灵层，因为默认地图不遮挡精灵
        _allSpNode = Node::create();
        _allSpNode->setContentSize(_gameMap->getContentSize());
        _gameMap->addChild(_allSpNode, 10);

        //初始化主角
        this->addLittleBoy(builder.getBoyPosition());
        Monster::boy = _littleBoy;

        scheduleUpdate();
    }

    {//添加批处理
        auto batch = Batchs::getInstance();
        _allSpNode->addChild(batch->getParticleNode(), 10);
        _allSpNode->addChild(batch->getHanziNode(), 999);
    }

    {//添加状态栏
        _stateNode = StateNode::create(_littleBoy->getSkillPowerKind(), _littleBoy->getLife(), _littleBoy->getAllLife());
        _stateNode->setScale(2);
        this->addChild(_stateNode);
        //_stateNode->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        Size stateSize = _stateNode->getContentSize() * _stateNode->getScale();
        _stateNode->setPosition(Vec2(stateSize.width * 0.5f, _director->getWinSize().height - stateSize.height * 0.5f));
    }
    {//钱币栏
        auto package = Package::getInstance();

        _gemNode = GemNode::create(package->getShell(), package->getKnowledgePieces());
        _gemNode->setPosition(40, 40);

        addChild(_gemNode);
    }

    _monsterSum = monsters.size();
    //处理怪物，下面的代码非常重要！
    handleQuestion(monsters, 0.5f);
    handleWeaponTables(weaponTables);
    handleSkillTables(skillTables);
    handleHanziDoor(hanziDoors);
    handleNormalDoor(normalDoors);
    for (auto seesaw : _seesaws)
        _allSpNode->addChild(seesaw, 0);

    para->runAction(Follow::create(_littleBoy, _gameMap->getBoundingBox()));


    auto titleBack = DrawNode::create();
    Size titleSize = Size(300, 100);
    titleBack->drawSolidRect(-Vec2(titleSize*0.5f), titleSize*0.5f, Color4F::BLACK);
    titleBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    titleBack->setPosition(Vec2(winSize.width * 0.5f, winSize.height - 100));
    titleBack->setOpacity(200);

    auto title = Label::createWithBMFont(gameConfig->getGameInfoFont(), gameConfig->getNames()[nowBack],TextHAlignment::CENTER);
    //title->setBMFontSize(100);
    title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    titleBack->addChild(title);

    auto titleSqe = Sequence::create(DelayTime::create(2.f), FadeOut::create(2.f), RemoveSelf::create(), nullptr);
    titleBack->runAction(titleSqe);
    title->runAction(Sequence::create(DelayTime::create(2.f), FadeOut::create(2.f), nullptr));
    addChild(titleBack, 99);

    return true;
}

void GameScene::handleInputKeyDown(Input keyCode, cocos2d::Event* /*event*/)
{
#ifdef _DEBUG
    if (keyCode == Input::KEY_T)
        _littleBoy->setScale(_littleBoy->getScale() + 0.1);
    if (keyCode == Input::KEY_G)
        _littleBoy->setScale(_littleBoy->getScale() - 0.1);
    if (keyCode == Input::KEY_ENTER)
    {
        if (_physicsWorld->getDebugDrawMask() == PhysicsWorld::DEBUGDRAW_ALL)
        {
            _physicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
        }
        else
        {
            _physicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
        }
    }
    else if (keyCode == Input::KEY_SHIFT)
    {
        _littleBoy->setWeapon(_weaponFactorys[_weaIndex]->getWeapon());
        _weaIndex++;
        _weaIndex %= 4;
    }
    else if (keyCode == Input::KEY_Z)
    {
        _littleBoy->setSkillPower(_skillFactorys[_skillIndex]->getSkillPower());
        _skillIndex++;
        _skillIndex %= 3;
    }
#endif // DEBUG
    if (keyCode == Input::KEY_ESCAPE)
    {
        pauseFunction();
    }
    else
    {
        auto controller = KeyController::getInstance();
        if (!controller->handleCallBackThingsInput(keyCode, KeyState::Key_Down))
            _littleBoy->handleInput(controller->acceptInput(keyCode, KeyState::Key_Down));
    }
}

void GameScene::handleInputKeyUp(Input keyCode, cocos2d::Event* /*event*/)
{
    _littleBoy->handleInput(KeyController::getInstance()->acceptInput(keyCode, KeyState::Key_Up));
}

bool GameScene::onContactBegin(PhysicsContact& contact)
{
    Node* nodeA = contact.getShapeA()->getBody()->getNode();
    Node* nodeB = contact.getShapeB()->getBody()->getNode();
    if (!(nodeA && nodeB)) 
    {
        CCLOG("has null body");
        return false;
    }
    int tagA = nodeA->getTag();
    int tagB = nodeB->getTag();
    int AorB = tagA | tagB;

    int boyOrMonster = (T_BOY | T_MONSTER);
    bool res = true;
    //掉落物
    if (AorB == (T_DROP | T_BOY))
    {
        CRASH_SELECT(nodeA, nodeB, T_DROP, res);
        return res;
    }
    //小男孩和怪物碰撞，主要是障碍物处理，障碍物处理直接就在这里处理了，不交给小男孩或怪物处理
    if (AorB & boyOrMonster)
    {
        GameSprite* gameSprite = nullptr;
        Node* other = nullptr;
        int oTag = 0;
        if (tagA & boyOrMonster)
        {
            gameSprite = (GameSprite*)nodeA;
            other = nodeB;
            oTag = tagB;
        }
        else
        {
            gameSprite = (GameSprite*)nodeB;
            other = nodeA;
            oTag = tagA;
        }
        //障碍物
        if (oTag & (T_PLATFORM | T_TRAMPOLINE | T_WALL))
        {
            CollisionHelper::DirNormal dirNormal = _collisionHelper.getColDir(contact.getContactData(), gameSprite, other, nodeA);
            if (oTag & T_PLATFORM)//平台
                res = dirNormal._normal.y > 0;
            else if (oTag & T_TRAMPOLINE)//跳板
            {
                if (dirNormal._normal.y > 0.01f)//不阻挡向上
                {
                    gameSprite->jump(400.f);//跳板弹跳力，以后改为非直接编码
                }
            }
            //else {} //墙的情况，已经默认处理了
            return res;//注意，这里目前考虑直接退出，但是不排除有未考虑到的情况
        }
    }

    //有必要单独处理一下平台的情况，后面可能重复处理了，但是反正没什么复杂的运算，问题不大
    if (AorB & T_PLATFORM)
    {
        CollisionHelper::DirNormal dirNormal;
        if (tagA == T_PLATFORM)
            dirNormal = _collisionHelper.getColDir(contact.getContactData(), nodeB, nodeA, nodeA);
        else
            dirNormal = _collisionHelper.getColDir(contact.getContactData(), nodeA, nodeB, nodeA);
        if (dirNormal._normal.y <= 0)
            return false;
        res = true;
    }

    //攻击响应
    if (AorB & T_ATKAREA_B)//玩家攻击
    {
        CRASH_SELECT(nodeA, nodeB, T_ATKAREA_B, res);
        return res;
    }
    if(AorB& T_ATKAREA_M)//敌人攻击
    {
        CRASH_SELECT(nodeA, nodeB, T_ATKAREA_M, res);
        return res;
    }
    if (AorB & T_CALLBACK_THINGS)
    {
        _collisionHelper.getNodeABWithTag(nodeA, nodeB, T_CALLBACK_THINGS);
        CallbackThing* thing = static_cast<CallbackThing*>(nodeA);
        auto controller = KeyController::getInstance();

        //休眠上一个callback
        if (auto t = controller->getNowCallbackThings())
        {
            t->setActive(false);
            t->sleepTip();
        }

        //唤醒当前callback
        thing->crash(nodeB);
        KeyController::getInstance()->addCallbackThing(thing);
        return res;
    }
    return res;
}

bool GameScene::onContactPre(cocos2d::PhysicsContact& contact, cocos2d::PhysicsContactPreSolve& solve)
{
    Node* nodeA = contact.getShapeA()->getBody()->getNode();
    Node* nodeB = contact.getShapeB()->getBody()->getNode();

    if (!(nodeA && nodeB)) {
        CCLOG("has null body");
        return false;
    }
    int tagA = nodeA->getTag();
    int tagB = nodeB->getTag();
    if ((tagA | tagB) & (T_BOY))
    {
        auto n = contact.getContactData()->normal;
    }
    if ((tagA | tagB) & (T_BOY| T_MONSTER))
    {
        Node* other;
        GameSprite* gameSprite;
        if (tagA & (T_BOY | T_MONSTER))
        {
            gameSprite = (GameSprite*)nodeA;
            other = nodeB;
        }
        else
        {
            gameSprite = (GameSprite*)nodeB;
            other = nodeA;
        }
        CollisionHelper::DirNormal dirNormal = _collisionHelper.getColDir(contact.getContactData(), gameSprite, other, nodeA);
        //if (gameSprite->getTag() == T_BOY)
        //{
        //    CCLOG("before dir: (%.2f, %.2f)", contact.getContactData()->normal.x, contact.getContactData()->normal.y);
        //    CCLOG("dir: (%.2f, %.2f)", dirNormal._normal.x, dirNormal._normal.y);

        //}
        this->dealDir(dirNormal, gameSprite, other);
    }

    return true;
}

void GameScene::onContactEnd(cocos2d::PhysicsContact& contact)
{
    Node* nodeA = contact.getShapeA()->getBody()->getNode();
    Node* nodeB = contact.getShapeB()->getBody()->getNode();
    if (!(nodeA && nodeB))
    {
        CCLOG("has null body");
        return;
    }

    int tagA = nodeA->getTag();
    int tagB = nodeB->getTag();
    int AorB = tagA | tagB;

    if (AorB & T_ATKAREA_M)
    {
        CRASH_OUT_SELECT(nodeA, nodeB, T_ATKAREA_M);
        return;
    }

    if (AorB == (T_BOY | T_DROP))
    {
        CRASH_OUT_SELECT(nodeA, nodeB, T_DROP);
        return;
    }

    if (AorB == (T_BOY | T_CALLBACK_THINGS))
    {
        _collisionHelper.getNodeABWithTag(nodeA, nodeB, T_CALLBACK_THINGS);
        CallbackThing* thing = static_cast<CallbackThing*>(nodeA);

        auto controller = KeyController::getInstance();
        bool isNow = controller->getNowCallbackThings() == thing;
        //去除该callback
        thing->outCrash(nodeB);
        controller->removeCallbackThing(thing);

        if(isNow)//如果被去除的callback是当前活跃的callback,则唤醒上一个callback
            if (auto t = controller->getNowCallbackThings())
            {
                t->setActive(true);
                t->activeTip();
            }
    }
}

void GameScene::addLittleBoy(const Vec2& postion)
{
    auto pack = Package::getInstance();

    BoyAnimation boyAnimation = AnimationLoader::getInstance()->boy;
    _littleBoy = LittleBoy::create(boyAnimation._anStand->getFrames().front()->getSpriteFrame());
    _littleBoy->setScale(GlobalWorld::scale);
    LittleBoy::setBoy(_littleBoy);
    _littleBoy->setAnimations(boyAnimation);
    _littleBoy->setTag(T_BOY);
    _littleBoy->setPosition(postion);
    int fullLife = pack->getFullLife();
    int nowLife = pack->getNowLife();
    _littleBoy->setAllLife(fullLife);
    _littleBoy->setLife(nowLife);
    _littleBoy->setJumpTimes(1);
    _littleBoy->setJumpPower(220);

    Size boySize = _littleBoy->getContentSize();

    Size boxSize = boySize;
    boxSize.width *= 0.5f;
    boxSize.height *= 0.8f;
    Vec2 offect(0, boxSize.height - boySize.height);
    offect.y *= 0.5f;
    PhysicsBody* boyBody = PhysicsBody::createBox(boxSize, PhysicsMaterial(1, 0, 1), offect);
    boyBody->setRotationEnable(false);
    boyBody->setContactTestBitmask(CON_BOY);
    boyBody->setCategoryBitmask(CAT_BOY);
    boyBody->setCollisionBitmask(COL_BOY);
    _littleBoy->setPhysicsBody(boyBody);

    float duration = boyAnimation._anAttackClose->getDuration();
    Vec2 woffect = Vec2(boySize.width * 0.1091, boySize.height * 0.0195);

    _weaponFactorys[0] = new PencilFactory(woffect, duration);
    _weaponFactorys[1] = new PenFactory(woffect, duration);
    _weaponFactorys[2] = new InkWeaponFactory(woffect);
    _weaponFactorys[3] = new EraserWeaponFactory(woffect);

    int weaponIndex = pack->getWeapon();
    _littleBoy->setWeapon(_weaponFactorys[weaponIndex]->getWeapon());

    _skillFactorys[0] = new RushSPFactory();
    _skillFactorys[1] = new BoomSPFactory();
    _skillFactorys[2] = new BlockSPFactory();

    int skillIndex = pack->getSkill();
    if (skillIndex >= 0)
        _littleBoy->setSkillPower(_skillFactorys[skillIndex]->getSkillPower());
    _allSpNode->addChild(_littleBoy);

    pack->setBoy(_littleBoy);
}

void GameScene::addMonster(cocos2d::Node* ground, int sum, MonsterFactory* factory)
{
    if (sum < 1)
        return;
    int len = ground->getContentSize().width;
    int left = ground->getBoundingBox().getMinX();
    int top = ground->getBoundingBox().getMaxY();
    int add = len / (sum + 1);
    for (size_t i = 1; i <= sum; i++)
    {
        auto monster = factory->getMonster(3);
        monster->setPosition(left + i * add, top + monster->getContentSize().height * 0.5f);
        monster->setDir(random() % 2 ? true : false);
        _allSpNode->addChild(monster);
        //_monsters.push_back(monster);
    }
}

void GameScene::dealDir(const CollisionHelper::DirNormal& dirNormal, GameSprite* me, cocos2d::Node* other)
{
    if(int(dirNormal._dir) & 
        (int(CollisionHelper::ColDir::DOWN) | 
        int(CollisionHelper::ColDir::LEFT_DOWN)| 
        int(CollisionHelper::ColDir::RIGHT_DOWN)))
    {
        //me->fallToGround();
        me->setOnDown(true);
        me->setGroundDir(dirNormal._normal);
        me->setStandWall(other);
    }
    else if (int(dirNormal._dir) &
        (int(CollisionHelper::ColDir::TOP) |
            int(CollisionHelper::ColDir::LEFT_TOP) |
            int(CollisionHelper::ColDir::RIGHT_TOP)))
    {
        me->setOnTop(true);
    }
    else if (dirNormal._dir==CollisionHelper::ColDir::LEFT)
    {
        me->setOnLeft(true);
    }
    else if (dirNormal._dir == CollisionHelper::ColDir::RIGHT)
    {
        me->setOnRight(true);
    }
}

void GameScene::handleQuestion(std::vector<Monster*>& monsters, float tipPer)
{
    auto helper = QuestionHelper::getInstance();
    ValueMap& pom = helper->getRandomPom().asValueMap();
    ValueVector& pomHanzis = pom["sentence"].asValueVector();
    ValueVector& cants = pom["nz"].asValueVector();
    std::vector<int> cant;
    cant.reserve(cant.size());
    for (Value& val : cants)
    {
        cant.push_back(val.asInt());
    }

    size_t msum = monsters.size();
    size_t ms = pomHanzis.size() - cants.size();
    size_t maxSpace = std::min(size_t(tipPer * ms), ms - 1);
    size_t spaceSum = std::min(msum, maxSpace);

    std::vector<int>& randomIndex = helper->getRandomIndexs(spaceSum, pomHanzis.size(), cant);

    std::random_shuffle(monsters.begin(), monsters.end());

    auto batchs = Batchs::getInstance()->getHanziNode();

    auto gameConfig = GameConfig::getInstance();
    auto rightFont = gameConfig->getRightFont();
    auto wrongFont = gameConfig->getWrongFont();
    for (size_t i = 0; i < spaceSum; i++)
    {
        auto monster = monsters[i];
        _allSpNode->addChild(monster);
        monster->randomSelf(0.8f, 1.2f);
        std::string hanzi = pomHanzis[randomIndex[i]].asString();
        bool isRight = random() % 2;
        monster->setHanzi(hanzi, isRight);
        auto label = Label::createWithBMFont(isRight ? rightFont : wrongFont, hanzi, TextHAlignment::CENTER);
        //label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        monster->setHanziLabel(label);
        label->setColor(monster->getHanziColor());
        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        batchs->addChild(label);
    }
    for (size_t i = spaceSum; i < msum; i++)
    {
        auto monster = monsters[i];
        _allSpNode->addChild(monster);
        monster->randomSelf(0.8f, 1.2f);
        std::string hanzi = helper->getRandomHanzi();
        bool isRight = random() % 2;
        monster->setHanzi(hanzi, isRight);
        auto label = Label::createWithBMFont(isRight ? rightFont : wrongFont, hanzi, TextHAlignment::CENTER);
        //label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        monster->setHanziLabel(label);
        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        label->setColor(monster->getHanziColor());
        batchs->addChild(label);
    }
}

void GameScene::handleWeaponTables(std::vector<CallbackThing*>& tables)
{
    Vec2 offect = Vec2(46.f / 84.f, 66.f / 84.f);
    auto& strMap = InfoHelper::getInstance()->getInfoMap();
    auto pressStr = strMap["press"].asString();
    for (auto table : tables)
    {
        _allSpNode->addChild(table);
        int randomIndex = random() % 4;
        auto sp = _weaponFactorys[randomIndex]->getWeaponSp();
        table->addChild(sp);
        Size tSize = table->getContentSize();
        Vec2 pos = Vec2(tSize.width * offect.x, tSize.height * offect.y);
        sp->setPosition(pos);
        
        Vec2 infoPos = table->convertToWorldSpace(pos);
        infoPos = _allSpNode->convertToNodeSpace(infoPos);
        infoPos.y += 240;
        auto weapon= _weaponFactorys[randomIndex]->getWeapon();
        auto info = WeaponInfo::create(weapon->getWeaponName(), weapon->getWeaponInfo(), weapon->getPrice());
        info->setPosition(infoPos);
        auto moveBy = MoveBy::create(1.f, Vec2(0, 20));
        info->runAction(RepeatForever::create(Sequence::create(moveBy, moveBy->reverse(), nullptr)));
        table->setShowNode(info);

        table->initWithFunc(Input::KEY_C, pressStr + " C", [this,table, randomIndex, info]() {
            auto weapon = _weaponFactorys[randomIndex]->getWeapon();
            auto pack = Package::getInstance();
            if (weapon->getPrice() <= pack->getShell())
            {
                _littleBoy->setWeapon(weapon);
                pack->addShell(-weapon->getPrice());
                pack->setWeapon(randomIndex);
                table->removeFromParent();
            }
            else
            {
                info->cantPay();
            }
            });
    }
}

int GameScene::getSkillIndex(const std::string& name)
{
    if (name == "rush")
        return 0;
    if (name == "boom")
        return 1;
    if (name == "block")
        return 2;
    return -1;
}

void GameScene::handleHanziDoor(std::vector<CallbackThing*>& doors)
{
    auto pressStr = InfoHelper::getInstance()->getInfoMap()["press"].asString();
    for (auto door : doors)
    {
        door->initWithFunc(Input::KEY_C, pressStr + " C", 
            [this]() {
                auto pack = Package::getInstance();
                if (pack->getHanziButtons().size() >= _monsterSum)
                {     
                    auto scene = HanziScene::create(cutScene());
                    Director::getInstance()->pushScene(scene);
                    this->runAction(Sequence::create(DelayTime::create(0.1f), CallFunc::create([this]() {
                        auto pack = Package::getInstance();
                        pack->onSceneEnd();
                        endScene();

                        _director->replaceScene(TransitionFade::create(0.5, TempScene::create(0.5, pack->isEnd()), Color3B(234, 224, 201)));
                        }), nullptr));
                }
                else
                {
                    auto gameConfig = GameConfig::getInstance();
                    auto font = gameConfig->getGameInfoFont();
                    auto tipStr = InfoHelper::getInstance()->getInfoMap()["cant go"].asString();
                    auto tip = Label::createWithBMFont(font, tipStr,TextHAlignment::CENTER);
                    tip->setBMFontSize(50);
                    tip->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                    _allSpNode->addChild(tip, 899);
                    Vec2 pos = _littleBoy->getPosition();
                    pos.y += 30;
                    tip->setPosition(pos);
                    float actTime = 1.5f;
                    tip->runAction(Sequence::create(MoveBy::create(actTime, Vec2(0, 80)), RemoveSelf::create(), nullptr));
                    tip->runAction(FadeOut::create(actTime));
                }
            });
        _allSpNode->addChild(door);
    }
}

void GameScene::handleNormalDoor(std::vector<CallbackThing*>& doors)
{
    auto pressStr = InfoHelper::getInstance()->getInfoMap()["press"].asString();
    for (auto door : doors)
    {
        door->initWithFunc(Input::KEY_C, pressStr + " C",
            [this]() {
                auto pack = Package::getInstance();
                pack->onSceneEnd();
                endScene();
                Director::getInstance()->replaceScene(TransitionFade::create(0.5, TempScene::create(0.5, pack->isEnd()), Color3B(234, 224, 201)));
            });
        _allSpNode->addChild(door);
    }
}

void GameScene::handleSeesaw(std::vector<Seesaw*>& seesaws)
{
    for (auto seesaw : seesaws)
    {
        _allSpNode->addChild(seesaw, 0);
        seesaw->addJoint(this->_physicsWorld);
    }
}

cocos2d::RenderTexture* GameScene::cutScene()
{
    Size visibleSize = _director->getVisibleSize();
    auto renderTexture = RenderTexture::create(visibleSize.width, visibleSize.height);

    renderTexture->begin();
    visit();
    renderTexture->end();

    return renderTexture;
}

void GameScene::pauseFunction()
{
    _director->pushScene(MyTransitionScene::create(cutScene()));
    AudioEngine::pauseAll();
}

void GameScene::handleSkillTables(std::vector<CallbackThing*>& tables)
{
    Vec2 offect = Vec2(46.f / 84.f, 66.f / 84.f);
    auto& strMap = InfoHelper::getInstance()->getInfoMap();
    auto pressStr = strMap["press"].asString();
    for (auto table : tables)
    {
        _allSpNode->addChild(table);
        int index = this->getSkillIndex(table->getName());
        auto factory = _skillFactorys[index];
        auto sp = factory->getSkillSp();
        table->addChild(sp);
        Size tSize = table->getContentSize();
        Vec2 pos = Vec2(tSize.width * offect.x, tSize.height * offect.y);
        sp->setPosition(pos);

        Vec2 infoPos = table->convertToWorldSpace(pos);
        infoPos = _allSpNode->convertToNodeSpace(infoPos);
        infoPos.y += 240;
        auto info = SkillInfo::create(factory->getSkillName(), factory->getInfo(), factory->getPrice(), sp->getSpriteFrame());
        info->setPosition(infoPos);
        auto moveBy = MoveBy::create(1.f, Vec2(0, 20));
        info->runAction(RepeatForever::create(Sequence::create(moveBy, moveBy->reverse(), nullptr)));
        table->setShowNode(info);

        table->initWithFunc(Input::KEY_C, pressStr + " C", [this, table, info, factory, index]() {
            auto skill = factory->getSkillPower();
            auto pack = Package::getInstance();
            if (factory->getPrice() <= pack->getKnowledgePieces())
            {
                _littleBoy->setSkillPower(skill);
                _stateNode->setSkill(factory->getSkillFrame());
                pack->addKnowledgePieces(-factory->getPrice());
                pack->setSkill(index);
                table->removeFromParent();
            }
            else
            {
                info->cantPay();
            }
            });
    }
}

void GameScene::update(float dt)
{
    _stateNode->setBlod(_littleBoy->getLife(), _littleBoy->getAllLife());
    float cool = 0;
    if (_littleBoy->getSkillPower())
        cool = _littleBoy->getSkillPower()->getNowCoolingTime();
    _stateNode->setCooling(cool);

    auto package = Package::getInstance();
    _gemNode->setShell(package->getShell());
    _gemNode->setPiece(package->getKnowledgePieces());
}

void GameScene::onEnter()
{
    Scene::onEnter();
    for(auto seesaw:_seesaws)
        seesaw->addJoint(this->_physicsWorld);
    Package::getInstance()->setBoy(_littleBoy);
    //AudioEngine::resumeAll();
}

void GameScene::onExit()
{
    Scene::onExit();
    //AudioEngine::pauseAll();
}

//bool GameScene::initGameScene(bool isTemp)
//{
//    if(!GameScene::init())
//        return false;
//    if (isTemp)
//        runAction(Sequence::create(DelayTime::create(1.f), CallFunc::create([this]() {Director::getInstance()->pushScene(TempScene::create(cutScene())); }), nullptr));
//    return true;
//}

void GameScene::endScene()
{
    //removeAllChildren();
    Batchs::release();
}

GameScene::~GameScene()
{
    Batchs::release();
    KeyController::release();
    //QuestionHelper::release();
    //AnimationLoader::release();
    for (size_t i = 0; i < 4; i++)
        CC_SAFE_DELETE(_weaponFactorys[i]);
    for (size_t i = 0; i < 3; i++)
        CC_SAFE_DELETE(_skillFactorys[i]);
}

