#pragma once
#include "cocos2d.h"
class InfoHelper
{
public:
	static InfoHelper* getInstance();
	static void release();
	cocos2d::ValueMap& getInfoMap() { return _infoMap; }
private:
	void init(std::string fileName);
private:
	static InfoHelper* infoHelper;
	cocos2d::ValueMap _infoMap;
};

