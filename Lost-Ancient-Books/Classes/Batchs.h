#pragma once
#include "cocos2d.h"
class Batchs
{
public:
	enum class ParticleKind
	{
		FIRE = 0
	};
public:
	static Batchs* getInstance();
	static void release();
	cocos2d::Node* getParticleNode() { return _particleNode; }
	cocos2d::Node* getHanziNode() { return _hanziNode; }
	bool initBatchs();
	~Batchs();
private:
	cocos2d::Node* _particleNode;
	cocos2d::Node* _hanziNode;
private:
	static Batchs* _batchs;
	Batchs():_particleNode(nullptr),_hanziNode(nullptr) {};
};

