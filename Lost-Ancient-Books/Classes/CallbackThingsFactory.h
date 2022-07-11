#pragma once
#include "CallbackThing.h"
#include "Weapon.h"
class CallbackThingsFactory
{
public:
	virtual ~CallbackThingsFactory(){}
	virtual CallbackThing* getCallbackThing(const cocos2d::Size& size) = 0;
	static CallbackThing* staticGetCallbackThing(const cocos2d::Size& size, cocos2d::EventKeyboard::KeyCode key, const std::string& str, const std::function<void()>& func);
public:
	static cocos2d::PhysicsBody* getBody(const cocos2d::Size& size);
};

class DoorFactory:public CallbackThingsFactory
{
public:
	virtual ~DoorFactory() {}
	virtual CallbackThing* getCallbackThing(const cocos2d::Size& size)override;
};

class BirdBoardFactory :public CallbackThingsFactory
{
public:
	BirdBoardFactory(const std::function<void()>& func) :CallbackThingsFactory(), _func(func) {}
	virtual ~BirdBoardFactory() {}
	virtual CallbackThing* getCallbackThing(const cocos2d::Size& size)override;
private:
	std::function<void()> _func;
};