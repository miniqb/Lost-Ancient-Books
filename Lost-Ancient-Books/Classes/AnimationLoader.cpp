#include "AnimationLoader.h"
#include "GameHelper.h"
#define GETANIMATION(get,give,con) \
do{\
get=give;\
con.pushBack(get);\
}while(0)
USING_NS_CC;


AnimationLoader* AnimationLoader::animationLoader = nullptr;;
AnimationLoader* AnimationLoader::getInstance()
{
    if (!animationLoader)
    {
        animationLoader = new AnimationLoader();
        animationLoader->loadAll();
    }
    return animationLoader;
}

void AnimationLoader::release()
{
    CC_SAFE_DELETE(animationLoader);
}

void AnimationLoader::loadAll()
{
    loadBoy();
    loadMushroom();
    loadBird();
    loadBottle();
    loadDinosaur();
    loadBoom();
    loadBullet();
}

bool AnimationLoader::loadBoy()
{
    GETANIMATION(boy._anAttackClose, Animation::createWithSpriteFrames(GameHelper::getAnimation("Boy/attack/%02d.png", 5), 1.f / 24.f), _animationContainer);
    GETANIMATION(boy._anAttackClose,Animation::createWithSpriteFrames(GameHelper::getAnimation("Boy/attack/%02d.png", 5), 1.f / 24.f), _animationContainer);
    GETANIMATION(boy._anAttackFar , Animation::createWithSpriteFrames(GameHelper::getAnimation("Boy/hold gun/%02d.png", 1), 1), _animationContainer);
    GETANIMATION(boy._anDead , Animation::createWithSpriteFrames(GameHelper::getAnimation("Boy/dead/%02d.png", 3), 1.f / 6.f), _animationContainer);
    GETANIMATION(boy._anHurt , Animation::createWithSpriteFrames(GameHelper::getAnimation("Boy/hurt/%02d.png", 1), 0.2f), _animationContainer);
    GETANIMATION(boy._anJump , Animation::createWithSpriteFrames(GameHelper::getAnimation("Boy/jump/%02d.png", 2), 1.f / 12.f), _animationContainer);
    GETANIMATION(boy._anRun , Animation::createWithSpriteFrames(GameHelper::getAnimation("Boy/run/%02d.png", 10), 1.f / 20.f), _animationContainer);
    GETANIMATION(boy._anRush , Animation::createWithSpriteFrames(GameHelper::getAnimation("Boy/rush/%02d.png", 1), 0.2f), _animationContainer);
    GETANIMATION(boy._anSkill , Animation::createWithSpriteFrames(GameHelper::getAnimation("Boy/skill/%02d.png", 1), 0.5f), _animationContainer);
    GETANIMATION(boy._anStand , Animation::createWithSpriteFrames(GameHelper::getAnimation("Boy/stand/%02d.png", 4), 1.f / 7.f), _animationContainer);
    return true;
}

bool AnimationLoader::loadMushroom()
{
    GETANIMATION(mushroom._anAttack , Animation::createWithSpriteFrames(GameHelper::getAnimation("mushroom/attack/%02d.png", 1), 1.f), _animationContainer);
    GETANIMATION(mushroom._anDead , Animation::createWithSpriteFrames(GameHelper::getAnimation("mushroom/dead/%02d.png", 1), 10.f), _animationContainer);
    GETANIMATION(mushroom._anVig , Animation::createWithSpriteFrames(GameHelper::getAnimation("mushroom/run/%02d.png", 4), 1.f / 18.f), _animationContainer);
    GETANIMATION(mushroom._anRun , Animation::createWithSpriteFrames(GameHelper::getAnimation("mushroom/run/%02d.png", 4), 1.f / 10.f), _animationContainer);
    GETANIMATION(mushroom._anStand , Animation::createWithSpriteFrames(GameHelper::getAnimation("mushroom/stand/%02d.png", 1), 10.f), _animationContainer);
    return true;
}

bool AnimationLoader::loadBird()
{
    GETANIMATION(bird._anAttack , Animation::createWithSpriteFrames(GameHelper::getAnimation("bird/attack/%02d.png", 1), 0.5f), _animationContainer);
    GETANIMATION(bird._anDead , Animation::createWithSpriteFrames(GameHelper::getAnimation("bird/dead/%02d.png", 1), 10.f), _animationContainer);
    GETANIMATION(bird._anVig , Animation::createWithSpriteFrames(GameHelper::getAnimation("bird/run/%02d.png", 3), 1.f / 18.f), _animationContainer);
    GETANIMATION(bird._anRun , Animation::createWithSpriteFrames(GameHelper::getAnimation("bird/run/%02d.png", 3), 1.f / 10.f), _animationContainer);
    GETANIMATION(bird._anStand , Animation::createWithSpriteFrames(GameHelper::getAnimation("bird/stand/%02d.png", 3), 1.f / 8.f), _animationContainer);
    return true;
}

bool AnimationLoader::loadBottle()
{
    GETANIMATION(bottle._anAttack, Animation::createWithSpriteFrames(GameHelper::getAnimation("bottle/attack/%02d.png", 1), 1.f), _animationContainer);
    GETANIMATION(bottle._anDead, Animation::createWithSpriteFrames(GameHelper::getAnimation("bottle/dead/%02d.png", 1), 10.f), _animationContainer);
    GETANIMATION(bottle._anVig, Animation::createWithSpriteFrames(GameHelper::getAnimation("bottle/run/%02d.png", 4), 1.f / 18.f), _animationContainer);
    GETANIMATION(bottle._anRun, Animation::createWithSpriteFrames(GameHelper::getAnimation("bottle/run/%02d.png", 4), 1.f / 10.f), _animationContainer);
    GETANIMATION(bottle._anStand, Animation::createWithSpriteFrames(GameHelper::getAnimation("bottle/stand/%02d.png", 1), 10.f), _animationContainer);
    return true;
}

bool AnimationLoader::loadDinosaur()
{
    GETANIMATION(dinosaur._anAttack, Animation::createWithSpriteFrames(GameHelper::getAnimation("dinosaur/attack/%02d.png", 1), 1.f), _animationContainer);
    GETANIMATION(dinosaur._anDead, Animation::createWithSpriteFrames(GameHelper::getAnimation("dinosaur/dead/%02d.png", 1), 10.f), _animationContainer);
    GETANIMATION(dinosaur._anVig, Animation::createWithSpriteFrames(GameHelper::getAnimation("dinosaur/run/%02d.png", 4), 1.f / 18.f), _animationContainer);
    GETANIMATION(dinosaur._anRun, Animation::createWithSpriteFrames(GameHelper::getAnimation("dinosaur/run/%02d.png", 4), 1.f / 10.f), _animationContainer);
    GETANIMATION(dinosaur._anStand, Animation::createWithSpriteFrames(GameHelper::getAnimation("dinosaur/stand/%02d.png", 1), 10.f), _animationContainer);
    return true;
}

bool AnimationLoader::loadBoom()
{
    GETANIMATION(boom._boom24, Animation::createWithSpriteFrames(GameHelper::getAnimation("boom/b24/%02d.png", 24), 1.f/60.f), _animationContainer);
    GETANIMATION(boom._boom15, Animation::createWithSpriteFrames(GameHelper::getAnimation("boom/b15/%02d.png", 15), 1.f/50.f), _animationContainer);
    GETANIMATION(boom._boom7, Animation::createWithSpriteFrames(GameHelper::getAnimation("boom/b7/%02d.png", 7), 1.f / 30.f), _animationContainer);
    return true;
}

bool AnimationLoader::loadBullet()
{
    GETANIMATION(bullet._firecracker, Animation::createWithSpriteFrames(GameHelper::getAnimation("bullet/firecracker/%02d.png", 2), 1.f / 30.f), _animationContainer);
    return true;
}
