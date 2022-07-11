#pragma once
#include "cocos2d.h"
struct MonsterAnimation
{
    cocos2d::Animation* _anRun;
    cocos2d::Animation* _anAttack;
    cocos2d::Animation* _anStand;
    cocos2d::Animation* _anDead;
    cocos2d::Animation* _anVig;
};
struct BoyAnimation
{
    cocos2d::Animation* _anRun;
    cocos2d::Animation* _anAttackClose;
    cocos2d::Animation* _anAttackFar;
    cocos2d::Animation* _anStand;
    cocos2d::Animation* _anDead;
    cocos2d::Animation* _anJump;
    cocos2d::Animation* _anHurt;
    cocos2d::Animation* _anRush;
    cocos2d::Animation* _anSkill;
};
struct BoomAnimation
{
    cocos2d::Animation* _boom24;
    cocos2d::Animation* _boom15;
    cocos2d::Animation* _boom7;
};
struct BulletAnimation
{
    cocos2d::Animation* _firecracker;
};
class AnimationLoader
{
public:
    static AnimationLoader* getInstance();
    static void release();
public:
    void loadAll();
    bool loadBoy();
    bool loadMushroom();
    bool loadBird();
    bool loadBottle();
    bool loadDinosaur();
    bool loadBoom();
    bool loadBullet();
public:
    BoyAnimation boy;
    MonsterAnimation mushroom;
    MonsterAnimation bird;
    MonsterAnimation bottle;
    MonsterAnimation dinosaur;
    BoomAnimation boom;
    BulletAnimation bullet;
private:
    cocos2d::Vector<cocos2d::Animation*> _animationContainer;
private:
    static AnimationLoader* animationLoader;
};

