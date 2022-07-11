#pragma once
#include "cocos2d.h"

class MyTransitionScene :public cocos2d::Scene
{
public:
    static MyTransitionScene* create(cocos2d::RenderTexture* texture);
    bool initPause(cocos2d::RenderTexture* texture);

    void handleInputKeyDown(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
};

