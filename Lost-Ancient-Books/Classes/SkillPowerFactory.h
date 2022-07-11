#pragma once
#include "SkillPower.h"
class SkillPowerFactory
{
public:
	SkillPowerFactory():_price(0){}
	virtual ~SkillPowerFactory() {};
	virtual SkillPower* getSkillPower() = 0;
	cocos2d::Sprite* getSkillSp();
	cocos2d::SpriteFrame* getSkillFrame();

	const std::string& getInfo() { return _skillInfo; }
	const std::string& getSkillName() { return _skillName; }
	void setInfo(const std::string& str) { _skillInfo = str; }
	void setSkillName(const std::string& str) { _skillName = str; }
	void setPrice(int p) { _price = p; }
	int getPrice() { return _price; }

protected:
	std::string _frameName;

	std::string _skillName;
	std::string _skillInfo;
	int _price;
};

class RushSPFactory :public SkillPowerFactory
{
public:
	RushSPFactory();
	virtual SkillPower* getSkillPower()override;
};

class BoomSPFactory :public SkillPowerFactory
{
public:
	BoomSPFactory();

	virtual SkillPower* getSkillPower()override;
};

class BlockSPFactory :public SkillPowerFactory
{
public:
	BlockSPFactory();

	virtual SkillPower* getSkillPower()override;
};