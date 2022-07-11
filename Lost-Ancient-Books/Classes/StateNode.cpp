#include "StateNode.h"
#include "GameConfig.h"
#include "GlobalWorld.h"
USING_NS_CC;
StateNode* StateNode::create(int nowSkill, int nowb, int allb)
{
    auto node = new(std::nothrow) StateNode();
    if (node && node->initStateNode(nowSkill, nowb, allb))
    {
        node->autorelease();
        return node;
    }
    CC_SAFE_DELETE(node);
    return nullptr;
}

bool StateNode::initStateNode(int nowSkill, int nowb, int allb)
{
    if(!init())
        return false;

    _nowSkill = nowSkill;
    auto frameCache = SpriteFrameCache::getInstance();
    _state = Sprite::createWithSpriteFrame(frameCache->getSpriteFrameByName("ui/state.png"));
    _loadingBar = ui::LoadingBar::create("ui/loading.png", ui::Widget::TextureResType::PLIST);
    if (nowSkill > 0)
    {
        char skillStr[20];
        sprintf(skillStr, "skill/%02d.png", nowSkill);
        _skill = Sprite::createWithSpriteFrame(frameCache->getSpriteFrameByName(skillStr));
    }
    else
        _skill = Sprite::create();
    auto config = GameConfig::getInstance();
    auto font = config->getGameInfoFont();

    auto nbStr = std::to_string(nowb);
    auto abStr = std::to_string(allb);
    _blodLabel = Label::createWithBMFont(font, nbStr + '/' + abStr, TextHAlignment::CENTER);
    _blodLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _blodLabel->setBMFontSize(20);

    _coolLabel = Label::createWithBMFont(font, "", TextHAlignment::CENTER);
    _coolLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _coolLabel->setBMFontSize(20);

    addChild(_state);
    _state->addChild(_loadingBar);
    _state->addChild(_skill);
    _state->addChild(_blodLabel);
    _state->addChild(_coolLabel);

    float rate = GlobalWorld::pxRate;

    Vec2 skillPos = Vec2(121, 63) * rate;
    _skill->setPosition(skillPos);
    _coolLabel->setPosition(skillPos);

    Vec2 blodPos = Vec2(198,22)*rate;
    Size loadingSize = Size(190,22)*rate;
    _loadingBar->setScale9Enabled(true);
    _loadingBar->setCapInsets(Rect(0, 0, 0, 0));
    _loadingBar->setContentSize(loadingSize);
    _loadingBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _loadingBar->setPercent(100.f * float(nowb) / float(allb));
    _loadingBar->setPosition(blodPos);
    _loadingBar->setDirection(ui::LoadingBar::Direction::LEFT);

    _blodLabel->setPosition(blodPos);

    this->setContentSize(_state->getContentSize());

    return true;
}

void StateNode::setSkill(cocos2d::SpriteFrame* frame)
{
    _skill->setSpriteFrame(frame);
}

void StateNode::setBlod(int nowb, int allb)
{
    nowb = std::max(0, nowb);
    auto nbStr = std::to_string(nowb);
    auto abStr = std::to_string(allb);
    _blodLabel->setString(nbStr + '/' + abStr);
    _loadingBar->setPercent(100.f * float(nowb) / float(allb));
}

void StateNode::setCooling(float time)
{
    if (time <= 0)
        _coolLabel->setString("");
    else
    {
        char coolStr[20];
        sprintf(coolStr, "%.2f", time);
        _coolLabel->setString(coolStr);
    }
}

StateNode::StateNode()
    :Node()
    , _state(nullptr)
    , _loadingBar(nullptr)
    , _blodLabel(nullptr)
    , _skill(nullptr)
    , _nowSkill(0)
    , _coolLabel(nullptr)
{
}

StateNode::~StateNode()
{
}
