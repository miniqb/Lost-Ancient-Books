#include "SkillInfo.h"
#include "GameConfig.h"
#include "GlobalWorld.h"
USING_NS_CC;

SkillInfo* SkillInfo::create(const std::string& name, const std::string& info, int price, cocos2d::SpriteFrame* frame)
{
    auto node = new(std::nothrow) SkillInfo();
    if (node && node->initSkillNode(name, info, price, frame))
    {
        node->autorelease();
        return node;
    }
    CC_SAFE_DELETE(node);
    return nullptr;
}

bool SkillInfo::initSkillNode(const std::string& name, const std::string& info, int price, cocos2d::SpriteFrame* frame)
{
    if (!init())
        return false;

    auto frameCache = SpriteFrameCache::getInstance();
    _background = Sprite::createWithSpriteFrame(frameCache->getSpriteFrameByName("ui/skill_info.png"));

    auto config = GameConfig::getInstance();
    auto font = config->getGameInfoFont();
    float sizeRate = GlobalWorld::pxRate;

    _sname = Label::createWithBMFont(font, "", TextHAlignment::CENTER);
    _sname->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _sname->setBMFontSize(30);
    _sname->setColor(Color3B(111, 93, 74));
    Vec2 namePos = Vec2(224, 364) * sizeRate;
    _sname->setPosition(namePos);

    Size infoSize = Size(312, 134) * sizeRate;
    _info = Label::createWithBMFont(font, "", TextHAlignment::LEFT);
    _info->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _info->setBMFontSize(30);
    _info->setColor(Color3B(111, 93, 74));
    _info->setContentSize(infoSize);
    _info->setWidth(infoSize.width);
    Vec2 infoPos = Vec2(77, 215) * sizeRate;
    _info->setPosition(infoPos);

    _price = Label::createWithBMFont(font, "", TextHAlignment::LEFT);
    _price->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _price->setBMFontSize(40);
    _price->setColor(Color3B(176, 246, 255));
    _price->setContentSize(infoSize);
    Vec2 pricePos = Vec2(261, 70) * sizeRate;
    _price->setPosition(pricePos);

    Vec2 skillPos = Vec2(228, 285) * sizeRate;
    _skill = Sprite::create();
    _skill->setPosition(skillPos);

    setSName(name);
    setInfo(info);
    setPrice(price);
    setSkill(frame);

    _background->addChild(_skill);
    _background->addChild(_sname);
    _background->addChild(_info);
    _background->addChild(_price);

    this->addChild(_background);
    this->setContentSize(_background->getContentSize());
    //this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    return true;
}

void SkillInfo::setSName(const std::string& name)
{
    _sname->setString(name);
}

void SkillInfo::setInfo(const std::string& info)
{
    _info->setString(info);
}

void SkillInfo::setPrice(int price)
{
    _price->setString(std::to_string(price));
}

void SkillInfo::setSkill(cocos2d::SpriteFrame* frame)
{
    _skill->setSpriteFrame(frame);
    _skill->setScale(2);
}

void SkillInfo::cantPay()
{
    _price->stopActionByTag(99);
    auto act = Repeat::create(Sequence::create(TintTo::create(0.1f, Color3B::RED), TintTo::create(0.1f, Color3B::WHITE), nullptr), 3);
    act->setTag(99);
    _price->runAction(act);
}

SkillInfo::SkillInfo()
    : _background(nullptr)
    , _skill(nullptr)
    , _info(nullptr)
    , _sname(nullptr)
    , _price(nullptr)
{
}

SkillInfo::~SkillInfo()
{
}
