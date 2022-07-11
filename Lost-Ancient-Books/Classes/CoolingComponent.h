#pragma once
#include "cocos2d.h"

class CoolingComponent :
    public cocos2d::Component
{
public:
	void setCoolingTime(float t) { _coolingTime = t; }
	float getCoolingTime() { return _coolingTime; }

	void setNowCoolingTime(float t) { _nowCooling = t; }
	void resetNowCoolingTime() { _nowCooling = _coolingTime; }
	float getNowCoolingTime() { return _nowCooling; }

	bool isCoolingOk() { return _nowCooling <= 0; }

	//overrides
	virtual void update(float delta)override;
protected:
	virtual void updateCoolingTime(float delta);
	//overrides
	virtual void insideUpdate(float delta) {/*override me*/ };
public:
	CoolingComponent();
	virtual ~CoolingComponent();
protected:
	float _coolingTime;
	float _nowCooling;

	//GameSprite* _gameOwner;
};

