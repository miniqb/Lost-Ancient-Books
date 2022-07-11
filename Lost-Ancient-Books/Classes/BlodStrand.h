#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"

class BlodStrand :public cocos2d::Node
{
public:
	static BlodStrand* create(int nowb, int allb);
	bool initStateNode(int nowb, int allb);

	void setBlod(int nowb, int allb);
private:
	cocos2d::Sprite* _background;
	cocos2d::ui::LoadingBar* _blod;
	cocos2d::Label* _blodLabel;
public:
	BlodStrand();
	~BlodStrand();
};

