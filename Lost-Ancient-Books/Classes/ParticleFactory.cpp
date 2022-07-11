#include "ParticleFactory.h"
USING_NS_CC;

cocos2d::ParticleSystem* FireParticleFactory::getParticleSystem()
{
    //fireStar.plist
    auto part = ParticleSystemQuad::create("spriteSheet/fireStar.plist");
    part->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("particle/t2.png"));
    part->setLocalZOrder(_zorder);
    return part;
}


cocos2d::ParticleSystem* OutFireParticleFactory::getParticleSystem()
{
    auto part = ParticleSystemQuad::create("spriteSheet/outFire.plist");
    part->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("particle/t1.png"));
    part->setLocalZOrder(_zorder);
    return part;
}

cocos2d::ParticleSystem* AngerFireParticleFactory::getParticleSystem()
{
    auto part = ParticleSystemQuad::create("spriteSheet/angerFire.plist");
    part->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("particle/t2.png"));
    part->setLocalZOrder(_zorder);
    return part;
}

cocos2d::ParticleSystem* MushGoustParticleFactory::getParticleSystem()
{
    auto part = ParticleSystemQuad::create("spriteSheet/mushGost.plist");
    part->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("particle/t2.png"));
    part->setLocalZOrder(_zorder);
    return part;
}
