#include "WeaponFactory.h"
#include "InfoHelper.h"
USING_NS_CC;
Weapon* PenFactory::getWeapon()
{
    AtkArea* area = PenAtkAreaFactory(true).getAtkArea();
    CloseWeapon* weapon = CloseWeapon::createWeapon(0.06f, 100.f, 15, 0.3f, area, _offect);
    weapon->setAtkTime(_time);
    auto& infoMap = InfoHelper::getInstance()->getInfoMap();
    weapon->setWeaponName(infoMap["pen"].asString());
    weapon->setWeaponInfo(infoMap["pen info"].asString());
    weapon->setPrice(60);
    return weapon;
}

Weapon* PencilFactory::getWeapon()
{
    AtkArea* area = PencilAtkAreaFactory(true).getAtkArea();
    CloseWeapon* weapon = CloseWeapon::createWeapon(0.06f, 100.f, 15, 0.3f, area, _offect);
    weapon->setAtkTime(_time);
    auto& infoMap = InfoHelper::getInstance()->getInfoMap();
    weapon->setWeaponName(infoMap["pencil"].asString());
    weapon->setWeaponInfo(infoMap["pencil info"].asString());
    weapon->setPrice(40);
    return weapon;
}

Weapon* InkWeaponFactory::getWeapon()
{
    auto weapon= ParabolaWeapon::createWeapon(15, 1.f, 7, new InkBottleFactory(true), _offect);
    auto& infoMap = InfoHelper::getInstance()->getInfoMap();
    weapon->setWeaponName(infoMap["ink"].asString());
    weapon->setWeaponInfo(infoMap["ink info"].asString());
    weapon->setPrice(70);
    return weapon;
}

Weapon* EraserWeaponFactory::getWeapon()
{
    auto weapon = ParabolaWeapon::createWeapon(8, 0.4, 0, new EraserFactory(true), _offect);
    auto& infoMap = InfoHelper::getInstance()->getInfoMap();
    weapon->setWeaponName(infoMap["eraser"].asString());
    weapon->setWeaponInfo(infoMap["eraser info"].asString());
    weapon->setPrice(60);
    return weapon;
}

cocos2d::Sprite* WeaponFactory::getWeaponSp()
{
    auto sp = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(_frameName));
    return sp;
}
