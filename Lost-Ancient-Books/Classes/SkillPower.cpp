#include "SkillPower.h"
#include "GlobalWorld.h"
#include "AtkAreaFactory.h"

USING_NS_CC;
SkillPower* SkillPower::create()
{
    SkillPower* ret = new (std::nothrow) SkillPower();

    if (ret && ret->init())
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

void SkillPower::useSkillPower()
{
    if (isCoolingOk())
    {
        insideUseSkill();
        resetNowCoolingTime();
        _gameOwner->setIsSkill(true);
    }
}

void SkillPower::setOwner(cocos2d::Node* owner)
{
    CoolingComponent::setOwner(owner);
    _gameOwner = static_cast<GameSprite*>(owner);
}

void SkillPower::setCellInfo(const std::string& name, const std::string& info, int price)
{
    _skillName = name;
    _skillInfo = info;
    _price = price;
}

SkillPower::SkillPower()
    :CoolingComponent()
    , _gameOwner(nullptr)
    , _kind(0)
{
    _name = "SkillPower";
}

SkillPower::~SkillPower()
{
}

RushSP* RushSP::create()
{
    RushSP* ret = new (std::nothrow) RushSP();

    if (ret && ret->init())
        ret->autorelease();
    else
        CC_SAFE_DELETE(ret);

    return ret;
}

void RushSP::insideUseSkill()
{
    if (_gameOwner)
    {
        Vec2 dir(1.f, 0.f);
        if (_gameOwner->isOnDown())
            dir = _gameOwner->getGroundDirPrep().getRPerp();
        if (!_gameOwner->getDir())
            dir = -dir;
        Vec2 len = dir * _rushLen;
        _gameOwner->runAction(MoveByWithWall::create(_rushTime, len, true));

        auto body = _gameOwner->getPhysicsBody();
        int omsk = body->getContactTestBitmask();

        if (_gameOwner->getTag() == T_BOY)
            body->setContactTestBitmask(CON_INV_BOY);
        else if (_gameOwner->getTag() == T_MONSTER)
            body->setContactTestBitmask(CON_INV_MONSTER);

        _gameOwner->runAction(Sequence::create(DelayTime::create(_rushTime*1.1), CallFunc::create([omsk, body]() {
            body->setContactTestBitmask(omsk);
            }), nullptr));

        Size box = _gameOwner->getBoundingBox().size;
        auto mot = MotionStreak::create(0.2, 10, box.height*0.5f, Color3B::BLUE, "png/brush.png");
        mot->setBlendFunc(BlendFunc::ADDITIVE);
        mot->setPosition(_gameOwner->getPosition());
        mot->runAction(Sequence::create(DelayTime::create(_rushTime), RemoveSelf::create(), nullptr));
        mot->runAction(FllowNode::create(_gameOwner));

        _gameOwner->getParent()->addChild(mot, 899);
    }
}

RushSP::RushSP()
    :SkillPower()
    , _rushLen(0)
    , _rushTime(0)
{
    _name = "rush";
    _kind = 1;
}

BoomSP* BoomSP::create()
{
    BoomSP* ret = new (std::nothrow) BoomSP();

    if (ret && ret->init())
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

void BoomSP::insideUseSkill()
{
    if (_gameOwner)
    {
        AtkArea* area = BoomSkillBoomFactory(true,_boomR).getAtkArea();
        area->setPosition(_gameOwner->getPosition());
        area->setHarm(60);
        area->setBt(0.3f, 200);
        _gameOwner->getParent()->addChild(area);
    }
}

BoomSP::BoomSP()
    :SkillPower()
    , _boomR(10)
{
    _name = "boom";
    _kind = 2;
}

BlockSP* BlockSP::create()
{
    BlockSP* ret = new (std::nothrow) BlockSP();

    if (ret && ret->init())
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

void BlockSP::initBlokSP(float justTime, float proTime, float reduce)
{
    _justTime = justTime;
    _proTime = proTime;
    _reduceHarm = reduce;
}

cocos2d::PhysicsBody* BlockSP::getBody()
{
    auto body = PhysicsBody::createCircle(_gameOwner->getBoundingBox().size.height * 1.1f);
    body->setDynamic(false);
    if (_owner->getTag() == T_BOY)
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

void BlockSP::insideUseSkill()
{
    if (_gameOwner)
    {
        auto O = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("block/01.png"));
        _owner->addChild(O);
        O->setTag(123);
        O->setScale(1.5f);
        O->setOpacity(0);
        float ut = _justTime;
        auto rate = RotateBy::create(ut, 60);
        auto sqe = Sequence::create(ScaleTo::create(ut, 1.f), nullptr);
        auto fadein = FadeIn::create(ut);
        auto spawn = Spawn::create(sqe, fadein, rate, nullptr);

        float nt = 0.2;
        auto fadeh = FadeTo::create(0, 100);
        auto fadeout = FadeOut::create(nt);
        auto ffout = Sequence::create(fadeh, fadeout, RemoveSelf::create(), nullptr);
        auto biger = ScaleTo::create(nt, 1.8f);
        auto spawn2 = Spawn::create(ffout, biger, nullptr);
        O->runAction(Sequence::create(spawn, spawn2, nullptr));
        O->runAction(FadeTo::create(0.4f, 255));
        O->setPosition(_owner->getContentSize() * 0.5f);

        _gameOwner->setBlock(true);
        _gameOwner->scheduleOnce([this](float) {
            _gameOwner->setBlock(false);
            }, _justTime, "BlockSP");
    }
}

void BlockSP::insideUpdate(float delta)
{
    if (_gameOwner)
    {
        if (_gameOwner->getBlockSucceed())
        {
            _owner->removeChildByTag(123);
            auto area = BoomAtkArea::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("block/02.png"));
            area->setScale(1.2f);
            auto body = this->getBody();
            area->setPhysicsBody(body);
            area->setTag(_gameOwner->getTag() == T_BOY ? SpriteTag::T_ATKAREA_B : SpriteTag::T_ATKAREA_M);
            area->setPosition(_gameOwner->getContentSize()*0.5f);
            area->setHarm(40);
            float existTime = 0.4f;
            area->setExistTime(existTime);
            area->runAction(Sequence::create(ScaleTo::create(existTime, 2), RemoveSelf::create(), nullptr));
            area->runAction(FadeOut::create(existTime));
            _owner->addChild(area);

            //成功格挡返还冷却
            this->setNowCoolingTime(0);
        }
    }
}

BlockSP::BlockSP()
    :SkillPower()
    ,_justTime(0)
    ,_proTime(0)
    , _reduceHarm(0)
{
    _name = "block";
    _kind = 3;
}
