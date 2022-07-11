#include "BlodStrand.h"
#include "GameConfig.h"
#include "GlobalWorld.h"
USING_NS_CC;
BlodStrand* BlodStrand::create(int nowb, int allb)
{
    auto node = new(std::nothrow) BlodStrand();
    if (node && node->initStateNode(nowb, allb))
    {
        node->autorelease();
        return node;
    }
    CC_SAFE_DELETE(node);
    return nullptr;
}

bool BlodStrand::initStateNode(int nowb, int allb)
{
    if (!init())
        return false;

    auto frameCache = SpriteFrameCache::getInstance();
    _background = Sprite::createWithSpriteFrame(frameCache->getSpriteFrameByName("ui/blodBack.png"));
    _blod = ui::LoadingBar::create("ui/loading.png", ui::Widget::TextureResType::PLIST);

    float rate = GlobalWorld::pxRate;
    Size blodSize = Size(250, 20) * rate;
    _blod->setScale9Enabled(true);
    _blod->setCapInsets(Rect(0, 0, 0, 0));
    _blod->setContentSize(blodSize);
    _blod->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _blod->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _blod->setDirection(ui::LoadingBar::Direction::LEFT);

    _background->addChild(_blod);
    addChild(_background);

    //this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->setContentSize(_background->getContentSize());

    setBlod(nowb, allb);
    return true;

    //auto config = GameConfig::getInstance();
//auto font = config->getGameInfoFont();

//char blodStr[20];
//sprintf(blodStr, "%d/%d", nowb, allb);

//_blodLabel = Label::createWithBMFont(font, blodStr, TextHAlignment::CENTER);
//_blodLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//_blodLabel->setBMFontSize(20);

//_background->addChild(_blodLabel);
}

void BlodStrand::setBlod(int nowb, int allb)
{
    _blod->setPercent(100.f * float(nowb) / float(allb));
}


BlodStrand::BlodStrand()
    :_background(nullptr)
    , _blod(nullptr)
    , _blodLabel(nullptr)
{
}

BlodStrand::~BlodStrand()
{
}
