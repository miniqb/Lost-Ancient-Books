#include "SkillPowerFactory.h"
#include "AnimationLoader.h"
#include "InfoHelper.h"
USING_NS_CC;

RushSPFactory::RushSPFactory() :SkillPowerFactory()
{
    _frameName = "skill/01.png"; 
    auto& infoMap = InfoHelper::getInstance()->getInfoMap();
    _skillName = infoMap["rush"].asString();
    _skillInfo = infoMap["rush info"].asString();
    _price = 50;
}

SkillPower* RushSPFactory::getSkillPower()
{
    RushSP* rush = RushSP::create();
    rush->setCoolingTime(0.7f);
    rush->setRushLen(300.f);
    rush->setRushTime(0.2f);
    rush->setAnimation(AnimationLoader::getInstance()->boy._anRush);

    auto& infoMap = InfoHelper::getInstance()->getInfoMap();
    rush->setCellInfo(infoMap["rush"].asString(), infoMap["rush info"].asString(), 50);
    return rush;
}

BoomSPFactory::BoomSPFactory() 
    :SkillPowerFactory()
{ 
    _frameName = "skill/02.png"; 
    auto& infoMap = InfoHelper::getInstance()->getInfoMap();
    _skillName = infoMap["boom"].asString();
    _skillInfo = infoMap["boom info"].asString();
    _price = 100;
}


SkillPower* BoomSPFactory::getSkillPower()
{
    BoomSP* sp = BoomSP::create();
    sp->setCoolingTime(5.f);
    sp->setBoomR(300);
    sp->setAnimation(AnimationLoader::getInstance()->boy._anSkill);

    auto& infoMap = InfoHelper::getInstance()->getInfoMap();
    sp->setCellInfo(infoMap["boom"].asString(), infoMap["boom info"].asString(), 100);
    return sp;
}

BlockSPFactory::BlockSPFactory()
    :SkillPowerFactory() 
{ 
    _frameName = "skill/03.png"; 
    auto& infoMap = InfoHelper::getInstance()->getInfoMap();
    _skillName = infoMap["block"].asString();
    _skillInfo = infoMap["block info"].asString();
    _price = 200;
}


SkillPower* BlockSPFactory::getSkillPower()
{
    BlockSP* sp = BlockSP::create();
    sp->setCoolingTime(3.f);
    auto anima = AnimationLoader::getInstance()->boy._anSkill;
    float justTime = anima->getDuration();
    sp->initBlokSP(justTime, 0, 0);
    sp->setAnimation(anima);

    auto& infoMap = InfoHelper::getInstance()->getInfoMap();
    sp->setCellInfo(infoMap["block"].asString(), infoMap["block info"].asString(), 200);
    return sp;
}

cocos2d::Sprite* SkillPowerFactory::getSkillSp()
{
    auto sp = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(_frameName));
    return sp;
}

cocos2d::SpriteFrame* SkillPowerFactory::getSkillFrame()
{
    auto sp = SpriteFrameCache::getInstance()->getSpriteFrameByName(_frameName);
    return sp;
}
