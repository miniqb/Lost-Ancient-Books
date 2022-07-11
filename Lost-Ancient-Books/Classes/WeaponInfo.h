#pragma once
#include "cocos2d.h"
class WeaponInfo:public cocos2d::Node
{
public:
	static WeaponInfo* create(const std::string& name, const std::string& info, int price);
	bool initStateNode(const std::string& name, const std::string& info, int price);

	void setWName(const std::string& name);
	void setInfo(const std::string& info);
	void setPrice(int price);

	void cantPay();
private:
	cocos2d::Sprite* _background;
	cocos2d::Label* _info;
	cocos2d::Label* _wname;
	cocos2d::Label* _price;
public:
	WeaponInfo();
	~WeaponInfo();
};

