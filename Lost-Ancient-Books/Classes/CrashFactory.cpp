#include "CrashFactory.h"
#include "GlobalWorld.h"
USING_NS_CC;
Crash* ShellFactory::getCrash()
{
	auto crash = Shell::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("gem/shell.png"));
	crash->setPhysicsBody(getBody(crash->getContentSize() * 0.9f));
	crash->setTag(T_DROP);
    return crash;
}



Crash* KonwPieceFactory::getCrash()
{
	auto crash = KnowPiece::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("gem/fragment.png"));
	crash->setPhysicsBody(getBody(crash->getContentSize() * 0.9f));
	crash->setTag(T_DROP);
	return crash;
}



cocos2d::PhysicsBody* CrashFactory::getBody(float r)
{
	auto body = PhysicsBody::createCircle(r);
	body->setDynamic(true);
	body->setCategoryBitmask(CAT_DROP);
	body->setContactTestBitmask(CON_DROP);
	body->setCollisionBitmask(COL_DROP);

	return body;
}

cocos2d::PhysicsBody* CrashFactory::getBody(const cocos2d::Size& size)
{
	auto body = PhysicsBody::createBox(size);
	body->setDynamic(true);
	body->setCategoryBitmask(CAT_DROP);
	body->setContactTestBitmask(CON_DROP);
	body->setCollisionBitmask(COL_DROP);

	return body;
}
