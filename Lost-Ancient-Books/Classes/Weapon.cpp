#include "Weapon.h"
#include "GameAction.h"
#include "GlobalWorld.h"
#include "InfoHelper.h"
USING_NS_CC;

void Weapon::attackNormal(bool isRight)
{
	resetNowCoolingTime();
	auto area = createAtkArea(isRight);
	if (area)
	{
		atkAreaAction(area, isRight);
		ownerAction(area, isRight);
	}
}

void Weapon::attackRush(bool _isRight)
{
	attackNormal(_isRight);
}

void Weapon::attackAir(bool _isRight)
{
	attackNormal(_isRight);
}

void Weapon::initWeapon(int atk, float cool, int boom,
	const cocos2d::Vec2& offect)
{
	_atk = atk;
	_coolingTime = cool;
	_boomAtk = boom;
	_offset = offect;
}

void Weapon::setOwner(cocos2d::Node* owner)
{
	CoolingComponent::setOwner(owner);
	_gameOwner = static_cast<GameSprite*>(owner);
}

Weapon::Weapon()
	:CoolingComponent()
	,_offset()
	,_atk(0)
	, _gameOwner(nullptr)
	, _price(0)
{
	_name = "Weapon";
}

Weapon::~Weapon()
{
	//CCLOG("eq");
}

void Weapon::ownerAction(AtkArea* area, bool isRight)
{
	_gameOwner->setAtk(true);
	_gameOwner->setFlippedX(!isRight);
}


CloseWeapon* CloseWeapon::createWeapon(
	float rushTime, 
	float rushLen, 
	int atk, 
	float cool, 
	AtkArea* area,
	const cocos2d::Vec2& offect)
{
	CloseWeapon* weapon = new CloseWeapon();
	weapon->initWeapon(rushTime, rushLen, atk, cool, area, offect);
	return weapon;
}

void CloseWeapon::initWeapon(
	float rushTime, 
	float rushLen, 
	int atk, 
	float cool,
	AtkArea* area, 
	const cocos2d::Vec2& offect)
{
	Weapon::initWeapon(atk, cool, 0, offect);
	_atkArea = area;
	_atkArea->retain();
	_rushTime=rushTime;
	_rushLen = rushLen;
}

void CloseWeapon::attackRush(bool isRight)
{
	attackNormal(isRight);
	Vec2 dir(1.f, 0.f);
	if(_gameOwner->isOnDown())
		dir = _gameOwner->getGroundDirPrep().getRPerp();
	if (isRight != dir.x > 0)
		dir = -dir;
	Vec2 len = dir * _rushLen;
	if (_owner->getTag() == T_BOY)
		_owner->runAction(MoveByWithWall::create(_rushTime, len, true));
	else
		_owner->runAction(MoveByWithWall::create(_rushTime, len, false));
}

CloseWeapon::CloseWeapon():
	Weapon(),
	_rushLen(0.f),
	_rushTime(0.f),
	_atkArea(nullptr)
{
	_name = "CloseWeapon";
	_sound = "sound/wav/close.mp3";
}

CloseWeapon::~CloseWeapon()
{
	CC_SAFE_RELEASE(_atkArea);
}

AtkArea* CloseWeapon::createAtkArea(bool isRight)
{
	return _atkArea;
}

void CloseWeapon::atkAreaAction(AtkArea* area, bool isRight)
{
	//_atkArea->setFlippedX(!isRight);
	_atkArea->setAtkDir(isRight);
	_atkArea->setRotation(isRight ? -27.5 : -152.5);
	_atkArea->runAction(Sequence::create(RotateBy::create(_atkTime, isRight ? 76.8 : -76.8), RemoveSelf::create(), nullptr));

	Size ownerSize = _owner->getContentSize();

	float atkLen = _atkArea->getContentSize().width;
	float handLen = ownerSize.height * 0.2639;//0.2639为手臂站高度的比例，计算得出
	float achor = (handLen - atkLen * 0.2f) / atkLen;//0.2为武器握在手中的那部分长度
	_atkArea->setAnchorPoint(Vec2(-achor, 0.5f));
	_atkArea->setHarm(_atk);

	Vec2 pos = ownerSize * 0.5f;
	pos += isRight ? _offset : -_offset;
	_atkArea->setPosition(pos);

	_owner->addChild(_atkArea);
}


FarWeapon* FarWeapon::createWeapon(
	int atk, 
	float cool,
	int boom,
	AtkAreaFactory* factory,
	const cocos2d::Vec2& offect)
{
	FarWeapon* weapon = new FarWeapon();
	weapon->initWeapon(atk, cool, boom, factory, offect);
	return weapon;
}

void FarWeapon::initWeapon(int atk, float cool, int boom, AtkAreaFactory* factory, const cocos2d::Vec2& offect)
{
	Weapon::initWeapon(atk, cool, boom,offect);
	this->_atkAreaFactory = factory;
}

FarWeapon::FarWeapon():
	Weapon(),
	_atkAreaFactory(nullptr)
{
	_name = "FarWeapon";
}

FarWeapon::~FarWeapon()
{
	CC_SAFE_DELETE(_atkAreaFactory);
}

AtkArea* FarWeapon::createAtkArea(bool isRight)
{
	if (!_atkAreaFactory)
		return nullptr;
	AtkArea* area = _atkAreaFactory->getAtkArea();
	area->setScale(area->getScale() * _owner->getScale());
	area->setHarm(_atk);
	area->setBoomHarm(_boomAtk);
	area->setFlippedX(!isRight);
	_owner->getParent()->addChild(area);
	return area;
}

void FarWeapon::atkAreaAction(AtkArea* area, bool isRight)
{
	float existTime = area->getExistTime();
	auto sqe = Sequence::create(MoveBy::create(existTime, Vec2(isRight ? 1000 : -1000, 0)), RemoveSelf::create(), nullptr);
	auto spa = Spawn::create(sqe, FadeOut::create(existTime), nullptr);
	area->runAction(spa);
	area->setAtkDir(isRight);
	Vec2 pos = _owner->getPosition();
	pos += isRight ? _offset : -_offset;
	area->setPosition(pos);
	area->openParticle();
}

ParabolaWeapon::ParabolaWeapon()
	:FarWeapon()
	, _throwSpeed(150, 120)
	, _torque(500000)
{
	_sound = "sound/wav/far.mp3";
}

ParabolaWeapon* ParabolaWeapon::createWeapon(int atk, float cool, int boom, AtkAreaFactory* factory, const cocos2d::Vec2& offect)
{
	ParabolaWeapon* weapon = new ParabolaWeapon();
	weapon->initWeapon(atk, cool, boom, factory, offect);
	return weapon;
}

void ParabolaWeapon::atkAreaAction(AtkArea* area, bool isRight)
{
	float existTime = area->getExistTime();
	auto act = Sequence::create(DelayTime::create(existTime), RemoveSelf::create(),nullptr);
	area->runAction(act);
	Vec2 pos = _owner->getPosition();
	pos += isRight ? _offset : -_offset;
	area->setPosition(pos);
	area->setAtkDir(isRight);
	area->getPhysicsBody()->applyTorque(isRight ? -_torque : _torque);
	area->getPhysicsBody()->setVelocity(Vec2(isRight ? _throwSpeed.x : -_throwSpeed.x, _throwSpeed.y));
	area->getPhysicsBody()->setDynamic(true);
}
