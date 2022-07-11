#include "MonsterFactory.h"
#include "GlobalWorld.h"
#include "otherMonsters.h"
#include "AnimationLoader.h"
USING_NS_CC;
Monster* CloseMonsterFactory::getMonster(int clonSum)
{
    //auto monster = Monster::create("boy.png", 2.f, 3.f, 120, 100, 100);
    //monster->setTag(T_MONSTER);
    //monster->changeState(new NormalState());
    //monster->setGoodHanzi(false);

    //monster->setClonSum(clonSum);
    //monster->setClonFactory(new CloseMonsterFactory());

    //AtkArea* area = FireAtkAreaFactory(false).getAtkArea();
    //area->setExistTime(0.09f);
    //Weapon* weapon = CloseWeapon::createWeapon(0.09f, 120.f, 50, 2.f, area);
    //monster->setWeapon(weapon);

    //PhysicsBody* body = PhysicsBody::createBox(monster->getContentSize(), PhysicsMaterial(1, 0, 1));
    //body->setContactTestBitmask(CON_MONSTER);
    //body->setCategoryBitmask(CAT_MONSTER);
    //body->setCollisionBitmask(COL_MONSTER);
    //body->setRotationEnable(false);
    //monster->setPhysicsBody(body);

    return nullptr;
}

Monster* MushroomFactory::getMonster(int clonSum)
{
    auto animation = AnimationLoader::getInstance()->mushroom;
    auto monster = Mushroom::createWithSpriteFrame(animation._anStand->getFrames().at(0)->getSpriteFrame());
    monster->setAnimations(animation);
    monster->Init(2, 1, 3000, 90, 100, 10, 3000, 1, 0.5);
    monster->setTag(T_MONSTER);
    monster->setClonSum(clonSum);
    monster->setClonFactory(new MushroomFactory());
    monster->setScale(GlobalWorld::scale);
    monster->setHanziOffect(Vec2(0.49779, 0.69675));

    Size box = monster->getContentSize();
    box.width *= 0.5f;
    PhysicsBody* body = PhysicsBody::createBox(box, PhysicsMaterial(1, 0, 1));
    body->setRotationEnable(false);
    body->setDynamic(true);
    body->setContactTestBitmask(CON_MONSTER);
    body->setCategoryBitmask(CAT_MONSTER);
    body->setCollisionBitmask(COL_MONSTER);
    monster->setPhysicsBody(body);
    return monster;
}

Monster* BirdFactory::getMonster(int clonSum)
{
    auto animation = AnimationLoader::getInstance()->bird;
    auto monster = Bird::createWithSpriteFrame(animation._anStand->getFrames().front()->getSpriteFrame());
    monster->setAnimations(animation);
    monster->Init(4, 1, 300, 70, 100, 10, 600, 1, 2);
    monster->setHanziOffect(Vec2(0.52778, 0.22705));
    Vec2 anchPos(0.5, 0.73913);
    monster->setAnchorPoint(anchPos);

    monster->setTag(T_MONSTER);
    monster->setClonSum(clonSum);
    monster->setClonFactory(new BirdFactory());

    Size msize = monster->getContentSize();
    Vec2 offect = anchPos - Vec2(0.5f, 0.5f);
    offect.x *= msize.width;
    offect.y *= msize.height;
    float radiu = (64.f / 259.f) * msize.height;
    //添加物理
    PhysicsBody* body = PhysicsBody::createCircle(radiu, PhysicsMaterial(1, 0, 1), offect);
    body->setRotationEnable(false);
    body->setDynamic(false);
    body->setContactTestBitmask(CON_BIRD);
    body->setCategoryBitmask(CAT_BIRD);
    body->setCollisionBitmask(COL_BIRD);
    monster->setPhysicsBody(body);
    monster->setScale(GlobalWorld::scale);

    //这个0.5可能要改
    monster->initBird(700, radiu);

    return monster;
}

Monster* BottleFactory::getMonster(int clonSum)
{
    auto animation = AnimationLoader::getInstance()->bottle;
    auto monster = Bottle::createWithSpriteFrame(animation._anStand->getFrames().at(0)->getSpriteFrame());
    monster->setAnimations(animation);
    monster->Init(1, 5, 2000, 100, 40, 10, 2000, 2, 2);
    monster->setTag(T_MONSTER);
    monster->setClonSum(clonSum);
    monster->setClonFactory(new BottleFactory());
    monster->setScale(GlobalWorld::scale);
    monster->setShootDir(60);
    monster->setHanziOffect(Vec2(0.55214, 0.46952));

    Size box = monster->getContentSize();
    box.width *= 0.7f;
    PhysicsBody* body = PhysicsBody::createBox(box, PhysicsMaterial(1, 0, 1));
    body->setRotationEnable(false);
    body->setDynamic(true);
    body->setContactTestBitmask(CON_MONSTER);
    body->setCategoryBitmask(CAT_MONSTER);
    body->setCollisionBitmask(COL_MONSTER);
    monster->setPhysicsBody(body);
    return monster;
}

Monster* DinosaurFactory::getMonster(int clonSum)
{
    auto animation = AnimationLoader::getInstance()->dinosaur;
    auto monster = Dinosaur::createWithSpriteFrame(animation._anStand->getFrames().front()->getSpriteFrame());
    monster->setScale(GlobalWorld::scale);
    monster->setAnimations(animation);
    monster->Init(3, 2, 200, 150, 100, 15, 1000, 0.5, 2);
    monster->setHanziOffect(Vec2(0.61585, 0.48291));
    monster->initDinosar();

    monster->setTag(T_MONSTER);
    monster->setClonSum(clonSum);
    monster->setClonFactory(new DinosaurFactory());

    //添加物理
    Size box = monster->getContentSize();
    box.width *= 0.8f;
    PhysicsBody* body = PhysicsBody::createBox(box, PhysicsMaterial(1, 0, 1));
    body->setRotationEnable(false);
    body->setDynamic(true);
    body->setContactTestBitmask(CON_MONSTER);
    body->setCategoryBitmask(CAT_MONSTER);
    body->setCollisionBitmask(COL_MONSTER);
    monster->setPhysicsBody(body);
    return monster;
}
