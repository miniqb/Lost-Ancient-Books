#pragma once
#include "cocos2d.h"
class GameHelper
{
public:
	static void throwAway(std::vector<cocos2d::Node*>& nodes, float angleMin, float angleMax, float speedLen, bool isPhy=true);
	static cocos2d::Vector<cocos2d::SpriteFrame*> getAnimation(const char* format, int count);
	static cocos2d::Label* getHanziLabel(bool isRight, const std::string str);
};

