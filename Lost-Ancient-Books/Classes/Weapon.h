#pragma once
#include "GameSprite.h"
#include "AtkAreaFactory.h"
#include "CoolingComponent.h"

class GameSprite;

class Weapon :public CoolingComponent
{
public:
	//正常站在地面上的攻击
	virtual void attackNormal(bool _isRight);
	//站在地面上的冲刺攻击（攻击的同时按下左右移动键）
	virtual void attackRush(bool _isRight);
	//在空中的攻击
	virtual void attackAir(bool _isRight);

	void initWeapon(int atk, float cool, int boom=0, const cocos2d::Vec2& offect=cocos2d::Vec2::ZERO);

	//virtual std::string getWeaponInfo();
	//overrides
	virtual void setOwner(cocos2d::Node* owner)override;
public:
	void setAtk(int atk) { _atk = atk; }
	int getAtk() { return _atk; }
	void setBoomAtk(int a) { _boomAtk = a; }
	int getBoomAtk() { return _boomAtk; }

	void setWeaponName(const std::string& str) { _weapenName = str; }
	const std::string& getWeaponName() { return _weapenName; }
	void setWeaponInfo(const std::string& str) { _info = str; }
	const std::string& getWeaponInfo() { return _info; }
	void setPrice(int p) { _price = p; }
	int getPrice() { return _price; }

	const std::string& getSound() { return _sound; }
public:
	Weapon();
	virtual ~Weapon();
protected:
	virtual AtkArea* createAtkArea(bool isRight) = 0;
	virtual void atkAreaAction(AtkArea* area, bool isRight) = 0;
	virtual void ownerAction(AtkArea* area, bool isRight);
protected:
	//伤害
	int _atk;
	int _boomAtk;
	//生成攻击波偏移
	cocos2d::Vec2 _offset;
	//持有者
	GameSprite* _gameOwner;

	std::string _weapenName;
	std::string _info;
	std::string _sound;
	int _price;
};

//近战武器
class CloseWeapon :public Weapon
{
public:
	static CloseWeapon* createWeapon(float rushTime, float rushLen, int atk, float cool, AtkArea* area, const cocos2d::Vec2& offect = cocos2d::Vec2::ZERO);
	void initWeapon(float rushTime, float rushLen, int atk, float cool, AtkArea* area, const cocos2d::Vec2& offect = cocos2d::Vec2::ZERO);
public:
	//站在地面上的冲刺攻击（攻击的同时按下左右移动键）
	virtual void attackRush(bool _isRight);
public:
	float getRushTime() { return _rushTime; }
	float getRushLen() { return _rushLen; }
	void setRushTime(float t) { _rushTime = t; }
	void setRushLen(float l) { _rushLen = l; }
	void setAtkArea(AtkArea* a) { _atkArea = a; _atkArea->retain(); }
	AtkArea* getAtkArea() { return _atkArea; }
	void setAtkTime(float t) { _atkTime = t; }
public:
	CloseWeapon();
	virtual ~CloseWeapon();

protected:
	virtual AtkArea* createAtkArea(bool isRight)override;
	virtual void atkAreaAction(AtkArea* area, bool isRight)override;
	//virtual void ownerAction(AtkArea* area, bool isRight)override;
protected:
	//攻击波（子弹）
	AtkArea* _atkArea;
	float _rushTime;
	float _rushLen;
	float _atkTime;
};

//远程武器
class FarWeapon :public Weapon
{
public:
	static FarWeapon* createWeapon(int atk, float cool, int boom, AtkAreaFactory* factory, const cocos2d::Vec2& offect);
public:
	void setAtkAreaFactory(AtkAreaFactory* f) { _atkAreaFactory = f; }
	AtkAreaFactory* getAtkAreaFactory() { return _atkAreaFactory; }
	void initWeapon(int atk, float cool, int boom, AtkAreaFactory* factory, const cocos2d::Vec2& offect = cocos2d::Vec2::ZERO);
public:
	FarWeapon();
	virtual ~FarWeapon();
protected:
	virtual AtkArea* createAtkArea(bool isRight)override;
	virtual void atkAreaAction(AtkArea* area, bool isRight)override;
	//virtual void ownerAction(AtkArea* area, bool isRight)override;
protected:
	AtkAreaFactory* _atkAreaFactory;
};

//抛物线武器
class ParabolaWeapon :public FarWeapon
{
public:
	ParabolaWeapon();
	static ParabolaWeapon* createWeapon(int atk, float cool, int boom, AtkAreaFactory* factory, const cocos2d::Vec2& offect);
	void setThrowSpeed(const cocos2d::Vec2& sp) { _throwSpeed = sp; }
	void setTorque(float t) { _torque = t; }
protected:
	virtual void atkAreaAction(AtkArea* area, bool isRight)override;
	cocos2d::Vec2 _throwSpeed;
	float _torque;
};