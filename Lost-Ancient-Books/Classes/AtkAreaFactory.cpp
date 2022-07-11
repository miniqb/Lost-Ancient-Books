#include "AtkAreaFactory.h"
#include "GlobalWorld.h"
#include "GameHelper.h"
#include "AnimationLoader.h"
USING_NS_CC;

AtkArea* FireAtkAreaFactory::getAtkArea()
{
	AtkArea* area = CantThroughAtkArea::create("attack.png");
	auto body = this->createBody(area->getContentSize(), false);
	area->setPhysicsBody(body);
	area->setTag(_isBoy ? SpriteTag::T_ATKAREA_B : SpriteTag::T_ATKAREA_M);
	area->setFactory(new FireParticleFactory());
	area->setExistTime(1.6f);
	return area;
}

cocos2d::PhysicsBody* AtkAreaFactory::createBody(const Size& size, bool dynamic)
{
	auto body = PhysicsBody::createBox(size);
	body->setDynamic(dynamic);
	if (_isBoy)
	{
		body->setCategoryBitmask(CAT_ATKAREA_B);
		body->setContactTestBitmask(CON_ATKAREA_B);
		body->setCollisionBitmask(COL_ATKAREA_B);
	}
	else
	{
		body->setCategoryBitmask(CAT_ATKAREA_M);
		body->setContactTestBitmask(CON_ARKAREA_M);
		body->setCollisionBitmask(COL_ATKAREA_M);
	}

	return body;
}

cocos2d::PhysicsBody* AtkAreaFactory::createBody(float r, bool dynamic)
{
	auto body = PhysicsBody::createCircle(r);
	body->setDynamic(dynamic);
	if (_isBoy)
	{
		body->setCategoryBitmask(CAT_ATKAREA_B);
		body->setContactTestBitmask(CON_ATKAREA_B);
		body->setCollisionBitmask(COL_ATKAREA_B);
	}
	else
	{
		body->setCategoryBitmask(CAT_ATKAREA_M);
		body->setContactTestBitmask(CON_ARKAREA_M);
		body->setCollisionBitmask(COL_ATKAREA_M);
	}

	return body;
}

AtkArea* CloseAtkAreaFactory::getAtkArea()
{
	AtkArea* area = BoomAtkArea::create("attack.png");
	auto body = this->createBody(area->getContentSize(), false);
	area->setPhysicsBody(body);
	area->setTag(_isBoy ? SpriteTag::T_ATKAREA_B : SpriteTag::T_ATKAREA_M);
	area->setFactory(new FireParticleFactory());
	area->setExistTime(1.6f);
	return area;
}

AtkArea* InkBottleFactory::getAtkArea()
{
	auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("weapon/ink.png");
	AtkArea* area = CantThroughAtkArea::createWithSpriteFrame(frame);
	auto body = this->createBody(area->getContentSize(), true);
	area->setPhysicsBody(body);
	area->setTag(_isBoy ? SpriteTag::T_ATKAREA_B : SpriteTag::T_ATKAREA_M);
	area->setExistTime(3.f);
	area->setBoom(new lnkBoomFactory(_isBoy));
	return area;
}

AtkArea* lnkBoomFactory::getAtkArea()
{
	auto animation = AnimationLoader::getInstance()->boom._boom7;
	auto area = BoomAtkArea::createWithSpriteFrame(animation->getFrames().front()->getSpriteFrame());
	area->runAction(Sequence::create(Animate::create(animation), RemoveSelf::create(), nullptr));
	area->setColor(Color3B::BLUE);
	area->setScale(4);
	auto body = createBody(area->getContentSize().width * 0.5f * 0.8f, false);
	area->setPhysicsBody(body);
	area->setTag(_isBoy ? SpriteTag::T_ATKAREA_B : SpriteTag::T_ATKAREA_M);
	area->setFactory(nullptr);
	return area;
}

AtkArea* EraserFactory::getAtkArea()
{
	EraserAtkArea* area = EraserAtkArea::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("weapon/eraser.png"));

	auto body = PhysicsBody::createBox(area->getContentSize(), PhysicsMaterial(0.1, 2, 0.4));
	body->setDynamic(true);
	if (_isBoy)
	{
		body->setCategoryBitmask(CAT_ATKAREA_B);
		body->setContactTestBitmask(CON_ATKAREA_B);
		body->setCollisionBitmask(COL_BONDY_ATKAREA_B);
	}
	else
	{
		body->setCategoryBitmask(CAT_ATKAREA_M);
		body->setContactTestBitmask(CON_ARKAREA_M);
		body->setCollisionBitmask(COL_BONDY_ATKAREA_M);
	}

	area->setPhysicsBody(body);
	area->setTag(_isBoy ? SpriteTag::T_ATKAREA_B : SpriteTag::T_ATKAREA_M);
	area->setExistTime(5.f);
	area->setCanCrashTime(1);
	area->setBoom(nullptr);
	area->setBt(0.1f, 40.f);

	return area;
}

AtkArea* PenAtkAreaFactory::getAtkArea()
{
	AtkArea* area = ThroughAtkArea::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("weapon/pen.png"));
	area->setBt(0.2f, 80.f);
	auto body = this->createBody(area->getContentSize(), false);
	area->setPhysicsBody(body);
	area->setTag(_isBoy ? SpriteTag::T_ATKAREA_B : SpriteTag::T_ATKAREA_M);
	area->setFactory(nullptr);
	return area;
}

AtkArea* FirecrackerAreaFactory::getAtkArea()
{
	auto animation = AnimationLoader::getInstance()->bullet._firecracker;
	auto area = FirecrackerAtkArea::createWithSpriteFrame(animation->getFrames().front()->getSpriteFrame());
	area->runAction(RepeatForever::create(Animate::create(animation)));
	auto body = createBody(area->getContentSize(), true);
	area->setPhysicsBody(body);
	area->setTag(_isBoy ? SpriteTag::T_ATKAREA_B : SpriteTag::T_ATKAREA_M);
	area->setFactory(new FireParticleFactory());
	area->setBoom(new FirecrackerBoomFactory(_isBoy));
	return area;
}

AtkArea* FirecrackerBoomFactory::getAtkArea()
{
	auto animation = AnimationLoader::getInstance()->boom._boom15;
	auto area = BoomAtkArea::createWithSpriteFrame(animation->getFrames().front()->getSpriteFrame());
	area->runAction(Sequence::create(Animate::create(animation), RemoveSelf::create(), nullptr));
	auto body = createBody(area->getContentSize().width * 0.5f * 0.8f, false);
	area->setPhysicsBody(body);
	area->setTag(_isBoy ? SpriteTag::T_ATKAREA_B : SpriteTag::T_ATKAREA_M);
	area->setFactory(nullptr);
	return area;
}

AtkArea* MushroomAreaFactory::getAtkArea()
{
	AtkArea* area = CantThroughAtkArea::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("bullet/mush.png"));
	auto body = this->createBody(area->getContentSize().width * 0.5f, false);
	area->setPhysicsBody(body);
	area->setTag(_isBoy ? SpriteTag::T_ATKAREA_B : SpriteTag::T_ATKAREA_M);
	area->setFactory(new MushGoustParticleFactory());
	area->setExistTime(10.f);
	return area;
}

AtkArea* PencilAtkAreaFactory::getAtkArea()
{
	AtkArea* area = ThroughAtkArea::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("weapon/pencil.png"));
	auto body = this->createBody(area->getContentSize(), false);
	area->setPhysicsBody(body);
	area->setTag(_isBoy ? SpriteTag::T_ATKAREA_B : SpriteTag::T_ATKAREA_M);
	return area;
}

AtkArea* BoomSkillBoomFactory::getAtkArea()
{
	auto animation = AnimationLoader::getInstance()->boom._boom15;
	auto area = BoomAtkArea::createWithSpriteFrame(animation->getFrames().front()->getSpriteFrame());
	area->setColor(Color3B::WHITE);
	area->runAction(Sequence::create(Animate::create(animation), RemoveSelf::create(), nullptr));
	Size size = area->getContentSize();
	area->setScale(_boomR / (size.width * 0.5f *0.8f));
	auto body = createBody(size.width * 0.5f * 0.8f, false);
	area->setPhysicsBody(body);
	area->setTag(_isBoy ? SpriteTag::T_ATKAREA_B : SpriteTag::T_ATKAREA_M);
	area->setFactory(nullptr);
	return area;
}

AtkArea* BlockSkillBoomFactory::getAtkArea()
{
	auto animation = AnimationLoader::getInstance()->boom._boom24;
	auto area = BoomAtkArea::createWithSpriteFrame(animation->getFrames().front()->getSpriteFrame());
	area->setBt(0.2f, 300.f);
	area->setColor(Color3B::BLUE);
	area->runAction(Sequence::create(Animate::create(animation), RemoveSelf::create(), nullptr));
	auto body = createBody(area->getContentSize().width * 0.5f * 0.8f, false);
	area->setPhysicsBody(body);
	area->setTag(_isBoy ? SpriteTag::T_ATKAREA_B : SpriteTag::T_ATKAREA_M);
	area->setFactory(nullptr);
	return area;
}
