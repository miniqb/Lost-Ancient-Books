#pragma once
#include "cocos2d.h"
class SkillInfo:public cocos2d::Node
{
public:
	static SkillInfo* create(const std::string& name, const std::string& info, int price, cocos2d::SpriteFrame* frame);
	bool initSkillNode(const std::string& name, const std::string& info, int price, cocos2d::SpriteFrame* frame);

	void setSName(const std::string& name);
	void setInfo(const std::string& info);
	void setPrice(int price);
	void setSkill(cocos2d::SpriteFrame* frame);

	void cantPay();
private:
	cocos2d::Sprite* _background;
	cocos2d::Sprite* _skill;
	cocos2d::Label* _info;
	cocos2d::Label* _sname;
	cocos2d::Label* _price;
public:
	SkillInfo();
	~SkillInfo();
};

