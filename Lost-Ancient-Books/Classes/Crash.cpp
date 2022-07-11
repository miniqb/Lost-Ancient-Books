#include "Crash.h"
#include "Package.h"
USING_NS_CC;
Shell* Shell::createWithSpriteFrame(SpriteFrame* frame)
{
	auto crash = new(std::nothrow) Shell();
	if (crash && crash->initWithSpriteFrame(frame))
	{
		crash->autorelease();
		return crash;
	}
	CC_SAFE_DELETE(crash);
	return nullptr;
}
void Shell::take()
{
	auto pack = Package::getInstance();
	pack->addShell(1);
	this->runAction(RemoveSelf::create());
}

KnowPiece* KnowPiece::createWithSpriteFrame(cocos2d::SpriteFrame* frame)
{
	auto crash = new(std::nothrow) KnowPiece();
	if (crash && crash->initWithSpriteFrame(frame))
	{
		crash->autorelease();
		return crash;
	}
	CC_SAFE_DELETE(crash);
	return nullptr;
}

void KnowPiece::take()
{
	auto pack = Package::getInstance();
	pack->addKnowledgePieces(1);
	this->runAction(RemoveSelf::create());
}

Drop::Drop()
	:Crash()
	,_nowTime(0)
	,_takeTime(1.f)
	, _sumIn(0)
{
}

void Drop::update(float dt)
{
	Crash::update(dt);
	if (_nowTime < _takeTime)
		_nowTime += dt;

	if (canTake()&&_sumIn>0)
		take();
}
