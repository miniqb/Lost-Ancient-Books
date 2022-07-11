#pragma once
#include "cocos2d.h"
class FirstScene:public cocos2d::Scene
{
public:
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(FirstScene)
};

