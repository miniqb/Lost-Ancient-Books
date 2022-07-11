#include "WeaponInfo.h"
#include "GameConfig.h"
#include "GlobalWorld.h"
USING_NS_CC;
WeaponInfo* WeaponInfo::create(const std::string& name, const std::string& info, int price)
{
    auto node = new(std::nothrow) WeaponInfo();
    if (node && node->initStateNode(name, info, price))
    {
        node->autorelease();
        return node;
    }
    CC_SAFE_DELETE(node);
    return nullptr;
}

bool WeaponInfo::initStateNode(const std::string& name, const std::string& info, int price)
{
    if (!init())
        return false;

    auto frameCache = SpriteFrameCache::getInstance();
    _background = Sprite::createWithSpriteFrame(frameCache->getSpriteFrameByName("ui/weapon_info.png"));

    auto config = GameConfig::getInstance();
    auto font = config->getGameInfoFont();
    float sizeRate = GlobalWorld::pxRate;

    _wname = Label::createWithBMFont(font, "", TextHAlignment::CENTER);
    _wname->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _wname->setBMFontSize(30);
    _wname->setColor(Color3B(111, 93, 74));
    Vec2 namePos = Vec2(227, 357) * sizeRate;
    _wname->setPosition(namePos);

    Size infoSize = Size(312, 134) * sizeRate;
    _info = Label::createWithBMFont(font, "", TextHAlignment::LEFT);
    _info->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _info->setBMFontSize(30);
    _info->setColor(Color3B(111, 93, 74));
    _info->setContentSize(infoSize);
    _info->setWidth(infoSize.width);
    Vec2 infoPos = Vec2(76, 281) * sizeRate;
    _info->setPosition(infoPos);

    Vec2 pricePos = Vec2(264, 93) * sizeRate;
    _price= Label::createWithBMFont(font, "", TextHAlignment::LEFT);
    _price->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _price->setBMFontSize(40);
    _price->setColor(Color3B(255, 247, 186));
    _price->setContentSize(infoSize);
    _price->setPosition(pricePos);

    setWName(name);
    setInfo(info);
    setPrice(price);

    _background->addChild(_wname);
    _background->addChild(_info);
    _background->addChild(_price);

    this->addChild(_background);
    this->setContentSize(_background->getContentSize());
    //this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    return true;
}

void WeaponInfo::setWName(const std::string& name)
{
    _wname->setString(name);
}

void WeaponInfo::setInfo(const std::string& info)
{
    _info->setString(info);
}

void WeaponInfo::setPrice(int price)
{
    _price->setString(std::to_string(price));
}

void WeaponInfo::cantPay()
{
    _price->stopActionByTag(99);
    auto act = Repeat::create(Sequence::create(TintTo::create(0.1f, Color3B::RED), TintTo::create(0.1f, Color3B::WHITE), nullptr), 3);
    act->setTag(99);
    _price->runAction(act);
}

WeaponInfo::WeaponInfo()
    :_background(nullptr)
    , _info(nullptr)
    , _wname(nullptr)
    , _price(nullptr)
{
}

WeaponInfo::~WeaponInfo()
{
}
