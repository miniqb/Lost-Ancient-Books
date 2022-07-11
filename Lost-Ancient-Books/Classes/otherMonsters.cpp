#include "otherMonsters.h"
#include "GameHelper.h"
#include "GlobalWorld.h"
#include "CallbackThingsFactory.h"
#include "Package.h"
#include "InfoHelper.h"
USING_NS_CC;

Mushroom* Mushroom::createWithSpriteFrame(cocos2d::SpriteFrame* frame)
{
	auto monster = new(std::nothrow) Mushroom();
	if (monster && monster->initWithSpriteFrame(frame))
	{
		monster->autorelease();
		return monster;
	}
	CC_SAFE_DELETE(monster);
	return nullptr;
}

Mushroom::Mushroom()
	:Monster()
	,_bulletLen(3000)
	,_bulletTime(10)
{
}

void Mushroom::attack()
{
	auto atkArea = MushroomAreaFactory(false).getAtkArea();
	//atkArea->setScale(0.5f);
	getParent()->addChild(atkArea);

	//修正子弹方向和位置,还有伤害
	suitAtkArea(atkArea);

	Vec2 offect = getContentSize() * getScale();
	//这是嘴巴的位置
	offect.x *= 0.13328;
	offect.y *= -0.30866;
	offect.x = isFlippedX() ? -offect.x: offect.x;
	Vec2 pos = getPosition() + offect;

	atkArea->setPosition(pos);

	//子弹动作
	auto sqe = Sequence::create(MoveBy::create(_bulletTime, Vec2(isFlippedX() ? -_bulletLen : _bulletLen, 0)), RemoveSelf::create(), nullptr);
	atkArea->runAction(sqe);

	auto particle = atkArea->openParticle();
	particle -> runAction(FllowNode::create(atkArea));
	particle->setScale(0.6f * getScale());
	particle->setAngle(isFlippedX() ? 0: 180);
	//particle->setPosition(atkArea->getPosition());
}

void Mushroom::afterAtk()
{
	float dp = getAfterTime() * getSpeedLen()* _vigSpeedRate;
	if (getPositionX() < boy->getPositionX())
		dp = -dp;

	auto moveBy = MoveByWithWall::create(
		this->getFreeMoveTime(),
		Vec2(dp, 0),
		true, false);
	moveBy->setTag(tagMove);
	runAction(moveBy);

	runVigAnimate();
}

void Mushroom::stopAfterAtk()
{
	stopActionByTag(tagMove);
	stopVigAnimate();
}
/*****************************************************************************Bird********************************************************************************************/

Bird* Bird::createWithSpriteFrame(cocos2d::SpriteFrame* frame)
{
	auto monster = new(std::nothrow) Bird();
	if (monster && monster->initWithSpriteFrame(frame))
	{
		monster->autorelease();
		return monster;
	}
	CC_SAFE_DELETE(monster);
	return nullptr;
}

Bird::Bird()
	:Monster()
	, _rushSpeed(200)
	, _bodyRadius(0)
	, _myCallbackThing(nullptr)
	, _atkArea(nullptr)
	, _lastDir(false)
	, _streak(nullptr)
{
	_lastDir = random() % 2;
}

Bird::~Bird()
{
	CC_SAFE_RELEASE(_atkArea);
}

void Bird::initBird(float rushSpd, float radiu)
{
	_rushSpeed = rushSpd;
	_bodyRadius = radiu;
	initAtkArea();
}

void Bird::runAfterAtkAnimate()
{
	runVigAnimate();
}

void Bird::stopAfterAtkAnimate()
{
	stopVigAnimate();
}

bool Bird::foundBoy()
{
	return _foundForce ||
		boy->getPositionX() > this->getPositionX() == _dir &&
		boy->getPosition().getDistanceSq(this->getPosition()) <= _radarLenSq;
}

bool Bird::canAtk()
{
	float lenSq = getPosition().getDistanceSq(boy->getPosition());
	return _atkLen*_atkLen > lenSq;
}

void Bird::beAttack(int h, bool isRight)
{
	this->getHurt(h);
	auto tin = TintTo::create(0.1f, 255, 155, 155);
	auto rtin = TintTo::create(0.1f, _oriColor);
	auto sqe = Sequence::create(tin, rtin, nullptr);
	sqe->setTag(tagTiny);
	runAction(sqe);
	tempFoundForce(5);
	if (_life <= 0)
	{
		this->setSpeed(0, 0);
		this->runAction(CallFunc::create([this, isRight]() {
			_physicsBody->setDynamic(true);
			_physicsBody->setRotationEnable(true);
			_physicsBody->setCollisionBitmask(COL_MONSTER);
			_physicsBody->applyImpulse(Vec2(isRight ? 100000 : -100000, 120000));
			}));
	}
}

void Bird::dead()
{
	if (_sumFS == nullptr)
	{
		auto node = getCallbackThing();
		Rect box = getBoundingBox();
		Vec2 pos = box.origin + Vec2(box.size.width * 0.52778, box.size.height * 0.22947);
		node->setPosition(pos);
		getParent()->addChild(node, 5);
	}
	else
	{
		(*_sumFS)--;
		if (*_sumFS == 0)
		{
			auto board = thowRightHanzi();
			getParent()->addChild(board, this->getLocalZOrder());
			CC_SAFE_DELETE(_sumFS);
		}
		runAction(Sequence::create(DelayTime::create(1.5f), FadeOut::create(1), RemoveSelf::create(), nullptr));
	}
}

void Bird::updateHanziLabel()
{
	if (_myCallbackThing)
	{
		if (_hanziLabel) {
			_hanziLabel->setPosition(_myCallbackThing->getPosition());
			_hanziLabel->setScale(_myCallbackThing->getScale());
		}
	}
	else
		Monster::updateHanziLabel();
}

void Bird::initAtkArea()
{
	_atkArea = ThroughAtkArea::create();
	_atkArea->setTag(T_ATKAREA_M);
	_atkArea->setFactory(new FireParticleFactory());

	auto body = AtkAreaFactory(false).createBody(_bodyRadius, false);
	_atkArea->setPhysicsBody(body);
	_atkArea->retain();
}

CallbackThing* Bird::getCallbackThing()
{
	auto callback = CallbackThing::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("bird/dead/board.png"));
	callback->setTag(T_CALLBACK_THINGS);
	auto body = CallbackThingsFactory::getBody(callback->getContentSize());
	body->setDynamic(true);
	callback->setPhysicsBody(body);

	auto& infoMap = InfoHelper::getInstance()->getInfoMap();
	auto press = infoMap["press"].asString();
	auto erase = infoMap["wipe"].asString();
	auto colloct = infoMap["collect"].asString();
	auto info = press + "Q" + colloct + '\n' + press + "E" + erase;
	callback->initWithFunc(EventKeyboard::KeyCode::KEY_Q, info,
		[this]() {
			this->runAction(RemoveSelf::create());
			_myCallbackThing->runAction(RemoveSelf::create());
			if (_hanzi.size() > 0)
			{
				if (_isGood)//选对了，把汉字加入背包
				{
					Package::getInstance()->addHanziButton(_hanzi);
					this->boy->addLife(4);
					createDrops();
				}
				else//选错了，制造分身
				{
					this->createClons(false);
				}
			}
		});
	callback->addKey(EventKeyboard::KeyCode::KEY_E, 
		[this]() {
			this->runAction(RemoveSelf::create());
			_myCallbackThing->runAction(RemoveSelf::create());
			if (_hanzi.size() > 0)
			{
				if (!_isGood)//选对了，把汉字加入背包
				{
					Package::getInstance()->addHanziButton(_hanzi);
					this->boy->addLife(4);
					createDrops();
				}
				else//选错了，强化
				{
					this->createClons(true);
				}
			}
		});
	callback->setScale(getScale());
	_myCallbackThing = callback;
	return callback;
}

void Bird::attack()
{
	Vec2 pos = getContentSize();
	Vec2 anchPos = this->getAnchorPoint();
	if (isFlippedX())
		anchPos.x = 1.f - anchPos.x;
	pos.x *= anchPos.x;
	pos.y *= anchPos.y;
	_atkArea->setPosition(pos);
	_atkArea->setAtkDir(!isFlippedX());
	_atkArea->setHarm(_atk);

	//粒子
	//auto particle = _atkArea->openParticle();
	//particle->runAction(FllowNode::create(this));
	//particle->setPosition(_atkArea->getPosition());
	addChild(_atkArea);

	_streak= MotionStreak::create(0.3, 10, _bodyRadius*2*getScale(), Color3B::YELLOW, "png/brush.png");
	_streak->setBlendFunc(BlendFunc::ADDITIVE);
	_streak->setPosition(this->getPosition());
	_streak->runAction(FllowNode::create(this));
	getParent()->addChild(_streak, 899);

	float atkT = _animations._anAttack->getTotalDelayUnits();
	Vec2 dp = boy->getPosition() - getPosition();
	dp.normalize();
	dp *= _rushSpeed * atkT;

	auto moveBy = MoveByWithWall::create(atkT, dp, true, false);
	moveBy->setTag(tagAttack);
	runAction(moveBy);
}

void Bird::vig()
{
	auto act = MoveToNode::create(Monster::boy, this->getSpeedLen() * _vigSpeedRate, true, 0.f, true, true);
	act->setTag(tagVig);
	runAction(act);
}

void Bird::stopAttack()
{
	removeChild(_atkArea, true);
	_streak->runAction(Sequence::create(DelayTime::create(_afterTime * 0.3f), RemoveSelf::create(), nullptr));
	_streak = nullptr;
	stopActionByTag(tagAttack);
}

void Bird::afterAtk()
{
	float dp = 220;
	if (boy->getPositionY() < getPositionY())
		dp = -dp;
	auto act = MoveByWithWall::create(_afterTime, Vec2(0, dp), true, false);
	act->setTag(tagAfter);
	runAction(act);
	Monster::afterAtk();
}

void Bird::stopAfterAtk()
{
	Monster::stopAfterAtk();
	stopActionByTag(tagAfter);
}

void Bird::move()
{
	float dp = getFreeMoveTime() * getSpeedLen();
	if (getPositionX() + dp > getParent()->getContentSize().width)
		dp = -dp;
	else if (getPositionX() -dp > 0 && _lastDir)
		dp = -dp;
	_lastDir = !_lastDir;

	auto moveBy = MoveByWithWall::create(
		this->getFreeMoveTime(),
		Vec2(dp, 0),
		true, false);
	auto moveup = MoveBy::create(0.5f, Vec2(0, 20));
	auto moveUD = RepeatForever::create(Sequence::create(moveup, moveup->reverse(), nullptr));
	moveBy->setTag(tagMove);
	moveUD->setTag(tagUpDown);
	runAction(moveBy);
	runAction(moveUD);
}

void Bird::stopMove()
{
	stopActionByTag(tagMove);
	stopActionByTag(tagUpDown);
}

void Bird::stand()
{
	auto moveup = MoveBy::create(0.5f, Vec2(0, 20));
	auto moveUD = RepeatForever::create(Sequence::create(moveup, moveup->reverse(), nullptr));
	moveUD->setTag(tagStand);
	runAction(moveUD);
}

void Bird::stopStand()
{
	stopActionByTag(tagStand);
}

/*****************************************************************************Bottle********************************************************************************************/

Bottle* Bottle::createWithSpriteFrame(cocos2d::SpriteFrame* frame)
{
	auto monster = new(std::nothrow) Bottle();
	if (monster && monster->initWithSpriteFrame(frame))
	{
		monster->autorelease();
		return monster;
	}
	CC_SAFE_DELETE(monster);
	return nullptr;
}

Bottle::Bottle()
	:Monster()
	,_shootDir(0.f)
{
}

bool Bottle::foundBoy()
{
	return _foundForce ||
		boy->getPositionX() > this->getPositionX() == _dir &&
		boy->getPosition().getDistanceSq(this->getPosition()) <= _radarLenSq;
}

bool Bottle::canAtk()
{
	float lenSq = getPosition().getDistanceSq(boy->getPosition());
	return _atkLen * _atkLen > lenSq;
}

void Bottle::attack()
{
	auto atkArea = FirecrackerAreaFactory(false).getAtkArea();
	//保险一点，万一出现啥问题没炸就自己消失吧
	atkArea->runAction(Sequence::create(DelayTime::create(20), RemoveSelf::create(), nullptr));
	getParent()->addChild(atkArea);

	//修正子弹方向和位置,还有伤害
	suitAtkArea(atkArea);

	Vec2 offect = getContentSize() * getScale();
	//这是嘴巴的位置
	offect.x *= 0.07436;
	offect.y *= 0.40774;
	offect.x = isFlippedX() ? -offect.x : offect.x;
	Vec2 pos = getPosition() + offect;

	atkArea->setPosition(pos);

	//子弹动作
	auto rotate = RotateBy::create(2, 360);
	atkArea->runAction(RepeatForever::create(rotate));
	//抛物线
	Vec2 dp = boy->getPosition()-pos;
	float dw = fabsf(dp.x);
	float dh = dp.y;
	float g = _physicsBody->getWorld()->getGravity().y;
	float tan = std::tanf(_shootDir);
	float wsq = g * 0.5f / (dh - dw / tan);
	float lx = dw * sqrtf(wsq < 0 ? -wsq : wsq);
	Vec2 shootSp(lx, 0);
	if (wsq < 0)
	{
		float t = dw / lx;
		shootSp.y = dh / t - 0.5f * g * t;
	}
	else
		shootSp.y = lx / tan;

	if (isFlippedX())
		shootSp.x = -shootSp.x;
	atkArea->getPhysicsBody()->setVelocity(shootSp);

	auto area = static_cast<FirecrackerAtkArea*>(atkArea);
	area->setAim(boy, boy->getStandWall());
	if (fabsf(shootSp.x) != 0.f)
		area->setThroughTime(dw / fabsf(shootSp.x)*0.5f);
	else
		area->setThroughTime(0.5f);
	//打开粒子
	Vec2 poffect = area->getContentSize() * area->getScale();
	poffect.x *= 0.34848;
	poffect.y *= 0.90654;
	auto partic = atkArea->openParticle();
	partic->runAction(FllowNode::create(atkArea, poffect, true));
	//partic->setPosition(area->getPosition());
}

void Bottle::afterAtk()
{
	float dp = getAfterTime() * getSpeedLen()* _vigSpeedRate;
	if (getPositionX() < boy->getPositionX())
		dp = -dp;

	auto moveBy = MoveByWithWall::create(
		this->getFreeMoveTime(),
		Vec2(dp, 0),
		true, false);
	moveBy->setTag(tagMove);
	runAction(moveBy);

	runVigAnimate();
}

void Bottle::stopAfterAtk()
{
	stopActionByTag(tagMove);
	stopVigAnimate();
}

void Bottle::dead()
{
	Monster::dead();
	Size size = this->getContentSize();
	auto newBody = PhysicsBody::createCircle(size.width * 0.5f * 0.8f);
	newBody->setPositionOffset(Vec2(0, -size.height * 0.1f));
	auto oldBody = getPhysicsBody();
	newBody->setCategoryBitmask(oldBody->getCategoryBitmask());
	newBody->setCollisionBitmask(oldBody->getCollisionBitmask());
	newBody->setContactTestBitmask(oldBody->getContactTestBitmask());
	newBody->setVelocity(oldBody->getVelocity());
	setPhysicsBody(newBody);
}

/*****************************************************************************Dinosaur********************************************************************************************/

Dinosaur* Dinosaur::createWithSpriteFrame(cocos2d::SpriteFrame* frame)
{
	auto monster = new(std::nothrow) Dinosaur();
	if (monster && monster->initWithSpriteFrame(frame))
	{
		monster->autorelease();
		return monster;
	}
	CC_SAFE_DELETE(monster);
	return nullptr;
}

Dinosaur::Dinosaur()
	:Monster()
	,_atkArea(nullptr)
{
	_hanziColor = Color3B(40, 30, 30);
}

Dinosaur::~Dinosaur()
{
	CC_SAFE_RELEASE(_atkArea);
}

void Dinosaur::attack()
{
	Size mySize = getContentSize();
	Size mySizeS = mySize * getScale();
	Size areaSize = _atkArea->getContentSize() * getScale();
	Vec2 offect(0.89196, 0.43091);
	Vec2 offectPos(mySize.width * offect.x, mySize.height * offect.y);
	Vec2 offectPosS(mySizeS.width * offect.x, mySizeS.height * offect.y);

	Vec2 pos;
	//pos.y = 0.5f * areaSize.height;
	if (isFlippedX())
	{
		pos.x = mySize.width - offectPos.x;
		_atkArea->setAnchorPoint(Vec2(1, 0));
	}
	else
	{
		//pos.x = mySize.width + areaSize.width * 0.5f - offectPos.x;
		pos.x = offectPos.x;
		_atkArea->setAnchorPoint(Vec2(0, 0));
	}
	_atkArea->setPosition(pos);
	_atkArea->setAtkDir(!isFlippedX());
	addChild(_atkArea);

	_atkArea->setHarm(_atk);
	auto particle = _atkArea->openParticle();
	float partOffectX = offectPosS.x - mySizeS.width * 0.5;
	Vec2 partOffect(isFlippedX() ? -partOffectX : partOffectX, offectPosS.y - mySizeS.height * 0.5f);
	particle->runAction(FllowNode::create(this, partOffect));
	particle->setPosition(getPosition() + partOffect);
	particle->setScale(getScale()*0.8f);
	float angle = particle->getAngle();
	if (isFlippedX()&&angle>-90||!isFlippedX()&&angle<-90)
	{
			angle = -(180 + angle);
	}
	particle->setAngle(angle);
}

void Dinosaur::stopAttack()
{
	removeChild(_atkArea, true);
}

void Dinosaur::afterAtk()
{
	if (std::fabsf(getPositionX() - boy->getPositionX()) < getBoundingBox().size.width * 0.5f)
	{
		float dp = getAfterTime() * getSpeedLen() * _vigSpeedRate * 1.2f;
		if (getPositionX() < boy->getPositionX())
			dp = -dp;

		auto moveBy = MoveByWithWall::create(
			this->getFreeMoveTime(),
			Vec2(dp, 0),
			false, false);
		moveBy->setTag(tagMove);
		runAction(moveBy);

		runVigAnimate();
	}
}

void Dinosaur::stopAfterAtk()
{
	stopActionByTag(tagMove);
	stopVigAnimate();
}


bool Dinosaur::initDinosar()
{
	initAtkArea();
	return true;
}

void Dinosaur::initAtkArea()
{
	_atkArea = ThroughAtkArea::create();
	_atkArea->setTag(T_ATKAREA_M);
	_atkArea->setFactory(new OutFireParticleFactory());
	Size size = getContentSize();
	size.height *= 0.5f;
	size.width *= 1.2f;
	_atkArea->setContentSize(size);

	auto body = AtkAreaFactory(false).createBody(_atkArea->getContentSize(), false);
	_atkArea->setPhysicsBody(body);
	_atkArea->retain();
}
