#pragma once
#include "cocos2d.h"
class TempScene:public cocos2d::Scene
{
public:
    static TempScene* create(float time, bool isEnd=false, bool isDead=false);
    bool initTemp(float time, bool isEnd = false, bool isDead = false);
};

