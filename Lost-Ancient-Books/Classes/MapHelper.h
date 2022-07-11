#pragma once
#include "cocos2d.h"
class MapHelper
{
public:
	static cocos2d::Vec2 getStanderPosition(float x, float y, const cocos2d::Size& tiledSize);
	static cocos2d::Rect getStanderRect(float x, float y, float width, float height, const cocos2d::Size& tiledSize);
};

