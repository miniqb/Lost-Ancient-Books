#pragma once
#include "Crash.h"
class CrashFactory
{
public:
	virtual ~CrashFactory(){}
	virtual Crash* getCrash() = 0;
	static cocos2d::PhysicsBody* getBody(float r);
	static cocos2d::PhysicsBody* getBody(const cocos2d::Size& size);
};

class ShellFactory:public CrashFactory
{
public:
	virtual Crash* getCrash();
};

class KonwPieceFactory :public CrashFactory
{
public:
	virtual Crash* getCrash();
};
