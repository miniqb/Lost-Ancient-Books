#include "CallbackThing.h"
#include "GlobalWorld.h"
#include "GameConfig.h"
USING_NS_CC;
void CallbackThing::initWithFunc(cocos2d::EventKeyboard::KeyCode key, const std::string str, const std::function<void()>& func)
{
    _keyStr = str;
    this->addKey(key, func);
}
bool CallbackThing::crash(cocos2d::Node* other)
{
    if (other->getTag() == T_BOY)
    {
        this->setActive(true);
        this->activeTip();
        return true;
    }
    return false;
}

void CallbackThing::outCrash(cocos2d::Node* other)
{
    if (other->getTag() == T_BOY)
    {
        this->setActive(false);
        this->sleepTip();
    }
}

void CallbackThing::setShowNode(cocos2d::Node* node)
{
    node->retain();
    _showNode = node;
}

void CallbackThing::activeTip()
{
    if (!_tip)
        addTip();
    if (_tip->getParent() == nullptr)
        _director->getRunningScene()->addChild(_tip, 999);
    if (_showNode&& _showNode->getParent()==nullptr)
        this->getParent()->addChild(_showNode);
}

void CallbackThing::sleepTip()
{
    if (_tip && _tip->getParent() != nullptr)
        _tip->runAction(RemoveSelf::create(false));
    if (_showNode && _showNode->getParent() != nullptr)
        _showNode->runAction(RemoveSelf::create(false));
}

bool CallbackThing::excuteFunc(cocos2d::EventKeyboard::KeyCode key)
{
    auto it = _keyAndFunc.find(key);
    
    if (it != _keyAndFunc.end())
    {
        (it->second)();
        return true;
    }

    return false;
}

void CallbackThing::addTip()
{
    auto label = Label::createWithBMFont(GameConfig::getInstance()->getGameInfoFont(), _keyStr, TextHAlignment::CENTER);
    //label->setColor(Color3B::BLACK);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _tip = label;
    //_tip->setAnchorPoint(Vec2(0.5f, 0));
    Vec2 pos = _director->getRunningScene()->getContentSize();
    pos.x *= 0.5f;
    pos.y *= 0.08f;
    _tip->setPosition(pos);
    auto move = ScaleBy::create(1.f, 1.2f);
    //ÉÏÏÂÒÆ¶¯
    _tip->runAction(RepeatForever::create(Sequence::create(move, move->reverse(), nullptr)));
    _tip->retain();
}

void CallbackThing::removeTip()
{
    if (_tip)
    {
        this->removeChild(_tip, true);
        CC_SAFE_RELEASE_NULL(_tip);
    }
}

CallbackThing::~CallbackThing()
{
    //if (_tip && _tip->getParent())
    //    _tip->removeFromParent();
    //if (_showNode && _showNode->getParent())
    //    _showNode->removeFromParent();
    CC_SAFE_RELEASE(_tip);
    CC_SAFE_RELEASE(_showNode);
}

CallbackThing* CallbackThing::creat(cocos2d::EventKeyboard::KeyCode key, const std::string& str, const std::function<void()>& func)
{
    auto node = new (std::nothrow) CallbackThing();
    if (node && node->init())
    {
        node->autorelease();
        node->initWithFunc(key, str, func);
        return node;
    }
    CC_SAFE_DELETE(node);
    return nullptr;
}

CallbackThing* CallbackThing::create()
{
    auto node = new (std::nothrow) CallbackThing();
    if (node && node->init())
    {
        node->autorelease();
        return node;
    }
    CC_SAFE_DELETE(node);
    return nullptr;
}

CallbackThing* CallbackThing::createWithSpriteFrame(cocos2d::SpriteFrame* frame)
{
    auto node = new (std::nothrow) CallbackThing();
    if (node && node->initWithSpriteFrame(frame))
    {
        node->autorelease();
        return node;
    }
    CC_SAFE_DELETE(node);
    return nullptr;
}

void CallbackThing::addKey(cocos2d::EventKeyboard::KeyCode key, const std::function<void()>& func)
{
    _keyAndFunc[key] = func;
}

void CallbackThing::setKeyStr(const std::string& str)
{
    _keyStr = str;
}

int CallbackThing::foundKey(cocos2d::EventKeyboard::KeyCode key)
{
    //int len = _keys.size();
    //for (size_t i = 0; i < len; i++)
    //{
    //    if (_keys[i] == key)
    //        return i;
    //}
    return -1;
}
