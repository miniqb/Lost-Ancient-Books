#pragma once
#include "cocos2d.h"
class Seesaw :
    public cocos2d::Node
{
public:
    static Seesaw* create(cocos2d::Sprite* sp, const cocos2d::Size& size, int tag, const cocos2d::PhysicsMaterial& pm, float mass);
    
    void onEnter()override;

    bool initWithSize(cocos2d::Sprite* sp, const cocos2d::Size& size, int tag, const cocos2d::PhysicsMaterial& pm, float mass);

    void addJoint(cocos2d::PhysicsWorld* world);
private:
    cocos2d::Sprite* _wall;
    Node* _fix;
};

