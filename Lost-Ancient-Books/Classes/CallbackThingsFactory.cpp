#include "CallbackThingsFactory.h"
#include "GlobalWorld.h"
#include "HanziScene.h"
#include "WeaponInfo.h"
#include "InfoHelper.h"
USING_NS_CC;

CallbackThing* DoorFactory::getCallbackThing(const cocos2d::Size& size)
{
    auto callback = CallbackThing::create();
    callback->setTag(T_CALLBACK_THINGS);
    callback->setContentSize(size);
    auto body = getBody(callback->getContentSize());
    callback->setPhysicsBody(body);
    return callback;
}

CallbackThing* CallbackThingsFactory::staticGetCallbackThing(const cocos2d::Size& size, EventKeyboard::KeyCode key, const std::string& str, const std::function<void()>& func)
{
    auto callback = CallbackThing::creat(key, str, func);
    callback->setTag(T_CALLBACK_THINGS);
    callback->setContentSize(size);
    auto body = getBody(size);
    callback->setPhysicsBody(body);
    return callback;
}

cocos2d::PhysicsBody* CallbackThingsFactory::getBody(const cocos2d::Size& size)
{
    auto body= PhysicsBody::createBox(size);
    body->setCategoryBitmask(CAT_CALLBACK_THINGS);
    body->setCollisionBitmask(COL_CALLBACK_THINGS);
    body->setContactTestBitmask(CON_CALLBACK_THINGS);
    return body;
}

CallbackThing* BirdBoardFactory::getCallbackThing(const cocos2d::Size& size)
{
    auto callback = CallbackThing::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("bird/dead/board.png"));
    callback->setTag(T_CALLBACK_THINGS);
    auto body = getBody(callback->getContentSize());
    callback->setPhysicsBody(body);
    return callback;
}
