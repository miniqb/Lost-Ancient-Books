#pragma once
#include "cocos2d.h"
class Crash :public cocos2d::Sprite
{
public:
	virtual ~Crash() {}
	virtual bool crash(cocos2d::Node* other) = 0;
	virtual void outCrash(cocos2d::Node* other) {};
};

class Drop :public Crash
{
public:
	Drop();
	void update(float dt)override;
	virtual bool crash(cocos2d::Node* other)override { ++_sumIn; return false; };
	virtual void outCrash(cocos2d::Node* other)override { --_sumIn; };

	void setTakeTime(float t) { _takeTime = t; }

	virtual void take() = 0;
protected:
	bool canTake() { return _nowTime >= _takeTime; };

protected:
	float _nowTime;
	float _takeTime;
	int _sumIn;
};
class Shell :public Drop
{
public:
	static Shell* createWithSpriteFrame(cocos2d::SpriteFrame* frame);
	virtual void take()override;
};

class KnowPiece :public Drop
{
public:
	static KnowPiece* createWithSpriteFrame(cocos2d::SpriteFrame* frame);
	virtual void take()override;
};