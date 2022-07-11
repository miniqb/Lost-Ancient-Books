#pragma once
#include "Monster.h"

class Mushroom :public  Monster
{
public:
	static Mushroom* createWithSpriteFrame(cocos2d::SpriteFrame* frame);
	Mushroom();

	//overrides
	virtual void attack()override;
	virtual void afterAtk()override;
	virtual void stopAfterAtk()override;
protected:
	float _bulletTime;
	float _bulletLen;
};

class Bird :public Monster
{
public:
	static Bird* createWithSpriteFrame(cocos2d::SpriteFrame* frame);
	Bird();
	~Bird();
	void initBird(float rushSpd, float radiu);

	//overrides
	virtual void runAfterAtkAnimate();
	virtual void stopAfterAtkAnimate();

	virtual bool foundBoy()override;
	virtual bool canAtk()override;
	virtual void beAttack(int h, bool isRight)override;
	virtual void dead()override;
	virtual void attack()override;
	virtual void vig()override;
	virtual void stopAttack()override;
	virtual void afterAtk()override;
	virtual void stopAfterAtk()override;
	virtual void move()override;
	virtual void stopMove()override;
	virtual void stand()override;
	virtual void stopStand()override;
protected:
	void initAtkArea();
	CallbackThing* getCallbackThing();

	//overrides
	virtual void updateHanziLabel()override;
protected:
	float _rushSpeed;

	float _bodyRadius;
	AtkArea* _atkArea;
	CallbackThing* _myCallbackThing;

	bool _lastDir;

	cocos2d::MotionStreak* _streak;
};

class Bottle:public Monster
{
public:
	static Bottle* createWithSpriteFrame(cocos2d::SpriteFrame* frame);
	Bottle();
	void setShootDir(float d) { _shootDir = d; }
	//overrides
	virtual bool foundBoy()override;
	virtual bool canAtk()override;
	virtual void attack()override;
	virtual void afterAtk()override;
	virtual void stopAfterAtk()override;
	virtual void dead();
protected:
	float _shootDir;
};

class Dinosaur :public Monster
{
public:
	static Dinosaur* createWithSpriteFrame(cocos2d::SpriteFrame* frame);
	Dinosaur();
	~Dinosaur();
	//overrides
	virtual void attack()override;
	virtual void stopAttack()override;
	virtual void afterAtk()override;
	virtual void stopAfterAtk()override;
	bool initDinosar();
protected:
	void initAtkArea();
protected:
	AtkArea* _atkArea;
};