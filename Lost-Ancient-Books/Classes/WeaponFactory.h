#pragma once
#include "Weapon.h"
class WeaponFactory
{
public:
	WeaponFactory(const cocos2d::Vec2& offect) :_offect(offect) {}
	virtual Weapon* getWeapon()=0;
	virtual ~WeaponFactory() {}
	void setOffect(const cocos2d::Vec2& offect) { _offect = offect; }
	cocos2d::Sprite* getWeaponSp();
protected:
	cocos2d::Vec2 _offect;
	std::string _frameName;
};

class PenFactory:public WeaponFactory
{
public:
	PenFactory(const cocos2d::Vec2& offect, float t) :WeaponFactory(offect), _time(t) { _frameName = "weapon/pen.png"; }

	virtual Weapon* getWeapon()override;

private:
	float _time;
};

class PencilFactory :public WeaponFactory
{
public:
	PencilFactory(const cocos2d::Vec2& offect, float t) :WeaponFactory(offect), _time(t) { _frameName = "weapon/pencil.png"; }

	virtual Weapon* getWeapon()override;
private:
	float _time;
};

class InkWeaponFactory :public WeaponFactory
{
public:
	InkWeaponFactory(const cocos2d::Vec2& offect) :WeaponFactory(offect){ _frameName = "weapon/ink.png"; }

	virtual Weapon* getWeapon()override;
};

class EraserWeaponFactory :public WeaponFactory
{
public:
	EraserWeaponFactory(const cocos2d::Vec2& offect) :WeaponFactory(offect){ _frameName = "weapon/eraser.png"; }

	virtual Weapon* getWeapon()override;
};