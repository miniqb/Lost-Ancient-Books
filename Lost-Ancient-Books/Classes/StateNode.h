#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"

class StateNode :public cocos2d::Node
{
public:
	static StateNode* create(int nowSkill, int nowb, int allb);
	bool initStateNode(int nowSkill, int nowb, int allb);
	
	void setSkill(cocos2d::SpriteFrame* frame);
	void setBlod(int nowb, int allb);
	void setCooling(float time);
private:
	cocos2d::Sprite* _state;
	cocos2d::ui::LoadingBar* _loadingBar;
	cocos2d::Label* _blodLabel;
	cocos2d::Sprite* _skill;
	int _nowSkill;
	cocos2d::Label* _coolLabel;
public:
	StateNode();
	~StateNode();
};

