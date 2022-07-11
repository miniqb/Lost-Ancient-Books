#pragma once
#include "AtkArea.h"

class AtkArea;
class AtkAreaFactory
{
public:
	AtkAreaFactory(bool isBoy):_isBoy(isBoy){}
	virtual ~AtkAreaFactory(){}
	virtual AtkArea* getAtkArea() { return nullptr; };
	cocos2d::PhysicsBody* createBody(const cocos2d::Size& area, bool dynamic);
	cocos2d::PhysicsBody* createBody(float r, bool dynamic);
protected:
	bool _isBoy;
};

class FireAtkAreaFactory :public AtkAreaFactory
{
public:
	FireAtkAreaFactory(bool isBoy) :AtkAreaFactory(isBoy) {}
	virtual AtkArea* getAtkArea()override;
};

class MushroomAreaFactory :public AtkAreaFactory
{
public:
	MushroomAreaFactory(bool isBoy) :AtkAreaFactory(isBoy) {}
	virtual AtkArea* getAtkArea()override;
};

class InkBottleFactory :public AtkAreaFactory
{
public:
	InkBottleFactory(bool isBoy) :AtkAreaFactory(isBoy) {}
	virtual AtkArea* getAtkArea()override;
};

class lnkBoomFactory :public AtkAreaFactory
{
public:
	lnkBoomFactory(bool isBoy) :AtkAreaFactory(isBoy) {}
	virtual AtkArea* getAtkArea()override;
};

class EraserFactory :public AtkAreaFactory
{
public:
	EraserFactory(bool isBoy) :AtkAreaFactory(isBoy) {}
	virtual AtkArea* getAtkArea()override;
};

class CloseAtkAreaFactory :public AtkAreaFactory
{
public:
	CloseAtkAreaFactory(bool isBoy) :AtkAreaFactory(isBoy) {}
	virtual AtkArea* getAtkArea()override;
};

class PencilAtkAreaFactory :public AtkAreaFactory
{
public:
	PencilAtkAreaFactory(bool isBoy) :AtkAreaFactory(isBoy) {}
	virtual AtkArea* getAtkArea()override;
};

class PenAtkAreaFactory :public AtkAreaFactory
{
public:
	PenAtkAreaFactory(bool isBoy) :AtkAreaFactory(isBoy) {}
	virtual AtkArea* getAtkArea()override;
};

class FirecrackerAreaFactory :public AtkAreaFactory
{
public:
	FirecrackerAreaFactory(bool isBoy) :AtkAreaFactory(isBoy) {}
	virtual AtkArea* getAtkArea()override;
};

class FirecrackerBoomFactory :public AtkAreaFactory
{
public:
	FirecrackerBoomFactory(bool isBoy) :AtkAreaFactory(isBoy) {}
	virtual AtkArea* getAtkArea()override;
};

class BoomSkillBoomFactory :public AtkAreaFactory
{
public:
	BoomSkillBoomFactory(bool isBoy,float r) :AtkAreaFactory(isBoy),_boomR(r) {}
	virtual AtkArea* getAtkArea()override;
private:
	float _boomR;
};

class BlockSkillBoomFactory :public AtkAreaFactory
{
public:
	BlockSkillBoomFactory(bool isBoy) :AtkAreaFactory(isBoy) {}
	virtual AtkArea* getAtkArea()override;
};