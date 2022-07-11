#include "GameHelper.h"
#include "GameConfig.h"
USING_NS_CC;

void GameHelper::throwAway(std::vector<cocos2d::Node*>& nodes, float angleMin, float angleMax, float speedLen, bool isPhy)
{
	for (auto node:nodes)
	{
		float angle = RandomHelper::random_real(angleMin, angleMax);
		Vec2 speed = Vec2::forAngle(angle) * speedLen;
        if (isPhy)
            node->getPhysicsBody()->setVelocity(speed);
        else
            node->runAction(MoveBy::create(0.6, speed * 0.6));
	}
}

Vector<SpriteFrame*> GameHelper::getAnimation(const char* format, int count)
{
    auto spritecache = SpriteFrameCache::getInstance();
    Vector<SpriteFrame*> animFrames;
    char str[50];
    for (int i = 1; i <= count; i++)
    {
        sprintf(str, format, i);
        animFrames.pushBack(spritecache->getSpriteFrameByName(str));
    }
    return animFrames;
}

Label* GameHelper::getHanziLabel(bool isRight, const std::string str)
{
    auto config = GameConfig::getInstance();
    std::string font = isRight ? config->getRightFont() : config->getWrongFont();
    auto label = Label::createWithBMFont(font, str, TextHAlignment::CENTER);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    return label;
}
