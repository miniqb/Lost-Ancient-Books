#include "HanziButton.h"
USING_NS_CC;
HanziButton* HanziButton::create(
    const std::string& normalImage,
    const std::string& hanzi, 
    const std::string& fontName,
    int fontSize,
    const std::string& selectedImage, 
    const std::string& disableImage, 

    TextureResType texType)
{
    HanziButton* btn = new (std::nothrow) HanziButton();
    if (btn &&
        btn->init(normalImage, selectedImage, disableImage, texType) &&
        btn->initHanziButton(hanzi, fontName, fontSize))
    {
        btn->autorelease();
        return btn;
    }
    CC_SAFE_DELETE(btn);
    return nullptr;
}

bool HanziButton::initHanziButton(const std::string& hanzi, const std::string& fontName ,int fontSize)
{
    _label = Label::createWithBMFont(fontName, hanzi, TextHAlignment::CENTER);
    if (_label)
    {
        _label->retain();
        _label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _label->setBMFontSize(fontSize);
        return true;
    }
    CC_SAFE_DELETE(_label);
    return false;
}

HanziButton::HanziButton():
	Button(),
    _mode(Mode::OUTQ),
    _questionIndex(-1)
{
}

HanziButton::~HanziButton()
{
    CC_SAFE_RELEASE(_label);
}

const std::string& HanziButton::getHanzi()
{
    CCASSERT(_label != nullptr, "The label of HanziButton is null for unkonw resaon!");
    return _label->getString();
}

void HanziButton::resetPosition()
{
    setPosition(_oriPos);
}

void HanziButton::setPosition(const Vec2& pos)
{
    Button::setPosition(pos);
    if (_label)
        _label->setPosition(pos);
}
