#include "MapBuilder.h"
#include "GlobalWorld.h"
#include "Seesaw.h"
#include "GameConfig.h"
#include "CallbackThingsFactory.h"
USING_NS_CC;

void MapBuilder::handleWall()
{
    auto config = GameConfig::getInstance();
    auto objectsGroup = _map->getObjectGroup(config->getMapObjectsWall());

    if (objectsGroup == nullptr)
    {
        CCLOG("not found the map layer named %s !", config->getMapObjectsWall());
        return;
    }

    auto objects = objectsGroup->getObjects();
    for (auto& obj : objects)
    {
        auto objMap = obj.asValueMap();
        float x = objMap["x"].asFloat();
        float y = objMap["y"].asFloat();
        float width = objMap["width"].asFloat();
        float height = objMap["height"].asFloat();

        Rect rect = getStanderRect(x, y, width, height, _fixTileSize);
        Vec2 pos = rect.origin + rect.size * 0.5f;

        _map->addChild(getWall(pos, rect.size));
    }
}

void MapBuilder::handlePlatform()
{
    auto config = GameConfig::getInstance();
    auto objectsGroup = _map->getObjectGroup(config->getMapObjectsPlatform());

    if (objectsGroup == nullptr)
    {
        CCLOG("not found the map layer named %s !", config->getMapObjectsPlatform());
        return;
    }

    auto objects = objectsGroup->getObjects();
    for (auto& obj : objects)
    {
        auto objMap = obj.asValueMap();
        float x = objMap["x"].asFloat();
        float y = objMap["y"].asFloat();
        float width = objMap["width"].asFloat();
        float height = objMap["height"].asFloat();

        Rect rect = getStanderRect(x, y, width, height, _fixTileSize);
        Vec2 pos = rect.origin + rect.size * 0.5f;

        _map->addChild(getPlatform(pos, rect.size));
    }
}

void MapBuilder::handleSeesaw(std::vector<Seesaw*>& seesaws, const cocos2d::PhysicsMaterial& pm, float mass)
{
    auto config = GameConfig::getInstance();
    auto objectsGroup = _map->getObjectGroup(config->getMapObjectsSeesaw());

    if (objectsGroup == nullptr)
    {
        CCLOG("not found the map layer named %s !", config->getMapObjectsSeesaw());
        return;
    }

    auto objects = objectsGroup->getObjects();
    for (auto& obj : objects)
    {
        auto objMap = obj.asValueMap();
        float x = objMap["x"].asFloat();
        float y = objMap["y"].asFloat();
        float width = objMap["width"].asFloat();
        float height = objMap["height"].asFloat();

        Rect rect = getStanderRect(x, y, width, height, _fixTileSize);
        Vec2 pos = rect.origin + rect.size * 0.5f;

        int gid = objMap["gid"].asInt();
        seesaws.push_back(getSeesaw(getSpriteByGid(gid), pos, rect.size, pm, mass));
    }
}

void MapBuilder::handleTrampoline(const cocos2d::PhysicsMaterial& pm)
{
    auto config = GameConfig::getInstance();
    auto objectsGroup = _map->getObjectGroup(config->getMapObjectsTrampoline());

    if (objectsGroup == nullptr)
    {
        CCLOG("not found the map layer named %s !", config->getMapObjectsWall());
        return;
    }

    auto objects = objectsGroup->getObjects();
    for (auto& obj : objects)
    {
        auto objMap = obj.asValueMap();
        float x = objMap["x"].asFloat();
        float y = objMap["y"].asFloat();
        float width = objMap["width"].asFloat();
        float height = objMap["height"].asFloat();

        Rect rect = getStanderRect(x, y, width, height, _fixTileSize);
        Vec2 pos = rect.origin + rect.size * 0.5f;

        _map->addChild(getTrampoline(pos, rect.size, pm));
    }
}

void MapBuilder::handleMap(const cocos2d::PhysicsMaterial& seesawPm, float mass, const cocos2d::PhysicsMaterial& TramPm)
{
    handleWall();
    handlePlatform();
    //handleSeesaw(seesawPm, mass);
    handleTrampoline(TramPm);
}

void MapBuilder::handleMonster(std::vector<Monster*>& monsters, int clonSum)
{
    auto config = GameConfig::getInstance();
    auto objectsGroup = _map->getObjectGroup(config->getMapObjectsMonster());

    if (objectsGroup == nullptr)
    {
        CCLOG("not found the map layer named %s !", config->getMapObjectsMonster());
        return;
    }

    auto objects = objectsGroup->getObjects();
    for (auto& obj : objects)
    {
        auto objMap = obj.asValueMap();
        float x = objMap["x"].asFloat();
        float y = objMap["y"].asFloat();
        std::string name = objMap["name"].asString();
        
        MonsterFactory* factory = nullptr;
        if (name == "bird")
            factory = new BirdFactory();
        else if (name == "mushroom")
            factory = new MushroomFactory();
        else if (name == "dinosaur")
            factory = new DinosaurFactory();
        else if (name == "bottle")
            factory = new BottleFactory();
        
        if (factory)
        {
            Monster* monster = getMonster(Vec2(x, y), factory, clonSum);
            monsters.push_back(monster);
        }
    }
}

void MapBuilder::handleDoor(std::vector<CallbackThing*>& hanziDoors, std::vector<CallbackThing*>& normalDoors)
{
    auto config = GameConfig::getInstance();
    auto objectsGroup = _map->getObjectGroup(config->getMapObjectsDoor());

    if (objectsGroup == nullptr)
    {
        CCLOG("not found the map layer named %s !", config->getMapObjectsDoor());
        return;
    }

    auto objects = objectsGroup->getObjects();
    for (auto& obj : objects)
    {
        auto objMap = obj.asValueMap();
        float x = objMap["x"].asFloat();
        float y = objMap["y"].asFloat();
        float width = objMap["width"].asFloat();
        float height = objMap["height"].asFloat();

        bool isHanzi = objMap["name"].isNull();

        Rect rect = getStanderRect(x, y, width, height, _fixTileSize);
        Vec2 pos = rect.origin + rect.size * 0.5f;
        auto door = DoorFactory().getCallbackThing(rect.size);
        door->setPosition(pos);
        if (isHanzi)
            hanziDoors.push_back(door);
        else
            normalDoors.push_back(door);
    }
}

void MapBuilder::handleWeaponTable(std::vector<CallbackThing*>& tables)
{
    auto config = GameConfig::getInstance();
    auto objectsGroup = _map->getObjectGroup(config->getMapObjectsWeaponTable());

    if (objectsGroup == nullptr)
    {
        CCLOG("not found the map layer named %s !", config->getMapObjectsWeaponTable());
        return;
    }

    auto objects = objectsGroup->getObjects();
    auto frameCache = SpriteFrameCache::getInstance();
    for (auto& obj : objects)
    {
        auto objMap = obj.asValueMap();
        float x = objMap["x"].asFloat();
        float y = objMap["y"].asFloat();
        float width = objMap["width"].asFloat();
        float height = objMap["height"].asFloat();

        Rect rect = getStanderRect(x, y, width, height, _fixTileSize);
        Vec2 pos = rect.origin + rect.size * 0.5f;

        auto table = CallbackThing::create();
        table->setContentSize(rect.size);
        table->setPhysicsBody(CallbackThingsFactory::getBody(rect.size));
        table->setPosition(pos);
        table->setTag(T_CALLBACK_THINGS);

        tables.push_back(table);
    }
}

void MapBuilder::handleSkillTable(std::vector<CallbackThing*>& tables)
{
    auto config = GameConfig::getInstance();
    auto objectsGroup = _map->getObjectGroup(config->getMapObjectsSkillTable());

    if (objectsGroup == nullptr)
    {
        CCLOG("not found the map layer named %s !", config->getMapObjectsSkillTable());
        return;
    }

    auto objects = objectsGroup->getObjects();
    auto frameCache = SpriteFrameCache::getInstance();
    for (auto& obj : objects)
    {
        auto objMap = obj.asValueMap();
        float x = objMap["x"].asFloat();
        float y = objMap["y"].asFloat();
        float width = objMap["width"].asFloat();
        float height = objMap["height"].asFloat();
        auto name = objMap["name"].asString();

        Rect rect = getStanderRect(x, y, width, height, _fixTileSize);
        Vec2 pos = rect.origin + rect.size * 0.5f;

        auto table = CallbackThing::create();
        table->setContentSize(rect.size);
        table->setPhysicsBody(CallbackThingsFactory::getBody(rect.size));
        table->setPosition(pos);
        table->setTag(T_CALLBACK_THINGS);
        table->setName(name);

        tables.push_back(table);
    }
}

cocos2d::Vec2 MapBuilder::getBoyPosition()
{
    auto config = GameConfig::getInstance();
    auto objectsGroup = _map->getObjectGroup(config->getMapObjectsBoy());

    CC_ASSERT((objectsGroup != nullptr && objectsGroup->getObjects().size() == 1), "wrong information of boy layer in map !");

    auto objMap = objectsGroup->getObjects().front().asValueMap();
    float x = objMap["x"].asFloat();
    float y = objMap["y"].asFloat();

    return Vec2(x, y);
}

Node* MapBuilder::getWall(const Vec2& pos, const Size& size)
{
    PhysicsBody* body = PhysicsBody::createBox(size);
    body->setDynamic(false);
    body->setCategoryBitmask(CAT_WALL);
    body->setContactTestBitmask(CON_WALL);
    body->setCollisionBitmask(COL_WALL);

    Node* block = Node::create();
    block->setTag(T_WALL);
    block->setContentSize(size);
    block->setAnchorPoint(Vec2(0.5f, 0.5f));
    block->setPhysicsBody(body);
    block->setPosition(pos);

    return block;
}

Node* MapBuilder::getPlatform(const Vec2& pos, const Size& size)
{
    PhysicsBody* body = PhysicsBody::createBox(size);
    body->setDynamic(false);
    body->setCategoryBitmask(CAT_PLATFORM);
    body->setContactTestBitmask(CON_PLATFORM);
    body->setCollisionBitmask(COL_PLATFORM);

    Node* block = Node::create();
    block->setTag(T_PLATFORM);
    block->setContentSize(size);
    block->setAnchorPoint(Vec2(0.5f, 0.5f));
    block->setPhysicsBody(body);
    block->setPosition(pos);

    return block;
}

Seesaw* MapBuilder::getSeesaw(Sprite* sp, const Vec2& pos, const Size& size, const PhysicsMaterial& pm, float mass)
{
    auto seesaw = Seesaw::create(sp, size, T_SEESAW, pm, mass);
    seesaw->setPosition(pos);
    return seesaw;
}

Node* MapBuilder::getTrampoline(const Vec2& pos, const cocos2d::Size& size, const cocos2d::PhysicsMaterial& pm)
{
    PhysicsBody* body = PhysicsBody::createBox(size, pm);
    body->setDynamic(false);
    body->setCategoryBitmask(CAT_WALL);
    body->setContactTestBitmask(CON_WALL);
    body->setCollisionBitmask(COL_WALL);

    Node* block = Node::create();
    block->setTag(T_TRAMPOLINE);
    block->setContentSize(size);
    block->setAnchorPoint(Vec2(0.5f, 0.5f));
    block->setPhysicsBody(body);
    block->setPosition(pos);

    return block;
}

cocos2d::Node* MapBuilder::getDoor(const cocos2d::Vec2& pos, const cocos2d::Size& size)
{
    auto door = DoorFactory().getCallbackThing(size);
    door->setPosition(pos);
    return door;
}


Monster* MapBuilder::getMonster(const cocos2d::Vec2& pos, MonsterFactory* factory, int clonSum)
{
    auto monster = factory->getMonster(clonSum);
    monster->setPosition(pos);
    monster->setDir(random() % 2 ? true : false);
    return monster;
}

Sprite* MapBuilder::getSpriteByGid(int gid)
{
    auto layer = static_cast<TMXLayer*>(_map->getChildren().at(0));
    auto tileSet = layer->getTileSet();
    auto text = layer->getTexture();
    Rect sRect = tileSet->getRectForGID(gid);
    sRect = CC_RECT_PIXELS_TO_POINTS(sRect);

    //第一种创建方法
    //Sprite* sp = Sprite::createWithSpriteFrame(SpriteFrame::createWithTexture(text, sRect));
    // 
    //第二种创建方法，似乎会更简单
    Sprite* sp = Sprite::create();
    sp->initWithTexture(text, sRect);
    return sp;
}

Vec2 MapBuilder::getStanderPosition(float x, float y, const Size& tiledSize)
{
    return Vec2(roundf(x / tiledSize.width) * tiledSize.width, roundf(y / tiledSize.height) * tiledSize.height);
}

Rect MapBuilder::getStanderRect(float x, float y, float width, float height, const cocos2d::Size& tiledSize)
{
    Vec2 standerPos = getStanderPosition(x, y, tiledSize);
    Vec2 standerMPos = getStanderPosition(x + width, y + height, tiledSize);
    return Rect(standerPos.x, standerPos.y, standerMPos.x - standerPos.x, standerMPos.y - standerPos.y);
}

MapBuilder::MapBuilder(cocos2d::TMXTiledMap* map)
    :_map(map)
{
    CCASSERT(_map != nullptr, "The map for MapBuilder can't be null!");

    Size tileSize = _map->getTileSize();
    float orMapWidth = _map->getMapSize().width * tileSize.width;
    float scale = _map->getContentSize().width / orMapWidth;
    _fixTileSize = tileSize * scale;
}
