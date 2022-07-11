#pragma once
#include "cocos2d.h"
class ParticleFactory
{
public:
	ParticleFactory() { _zorder = -1; }
	virtual cocos2d::ParticleSystem* getParticleSystem() = 0;
protected:
	int _zorder;
};

class FireParticleFactory :public ParticleFactory
{
public:
	FireParticleFactory() { _zorder = 0; }
	virtual cocos2d::ParticleSystem* getParticleSystem()override;
};

class OutFireParticleFactory :public ParticleFactory
{
public:
	OutFireParticleFactory() { _zorder = 1; }
	virtual cocos2d::ParticleSystem* getParticleSystem()override;
};

class AngerFireParticleFactory :public ParticleFactory
{
public:
	AngerFireParticleFactory() { _zorder = 2; }
	virtual cocos2d::ParticleSystem* getParticleSystem()override;
};

class MushGoustParticleFactory :public ParticleFactory
{
public:
	MushGoustParticleFactory() { _zorder = 3; }
	virtual cocos2d::ParticleSystem* getParticleSystem()override;
};