#include "MapHelper.h"
USING_NS_CC;

cocos2d::Vec2 MapHelper::getStanderPosition(float x, float y, const cocos2d::Size& tiledSize)
{
    return Vec2(roundf(x / tiledSize.width) * tiledSize.width, roundf(y / tiledSize.height) * tiledSize.height);
}

cocos2d::Rect MapHelper::getStanderRect(float x, float y, float width, float height, const cocos2d::Size& tiledSize)
{
    Vec2 standerPos = getStanderPosition(x, y, tiledSize);
    Vec2 standerMPos = getStanderPosition(x + width, y + height, tiledSize);
    return Rect(standerPos.x, standerPos.y, standerMPos.x - standerPos.x, standerMPos.y - standerPos.y);
}
