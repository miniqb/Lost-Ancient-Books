#include "MyTransitionScene.h"
#include "GameScene.h"
#include "InfoHelper.h"
#include "GameConfig.h"
#include "MainMenuScene.h"
#include "AudioEngine.h"
USING_NS_CC;

MyTransitionScene* MyTransitionScene::create(cocos2d::RenderTexture* texture)
{
    auto scene = new(std::nothrow) MyTransitionScene();
    if (scene && scene->initPause(texture))
    {
        scene->autorelease();
        return scene;
    }
    CC_SAFE_DELETE(scene);
    return nullptr;
}

bool MyTransitionScene::initPause(cocos2d::RenderTexture* texture)
{
    if (!Scene::init())
        return false;

    Sprite* background = Sprite::createWithTexture(texture->getSprite()->getTexture());
    Size winSize = _director->getWinSize();
    background->setPosition(winSize * 0.5f);
    background->setFlippedY(true);
    background->setColor(Color3B::GRAY);
    addChild(background);

    auto& infoMap = InfoHelper::getInstance()->getInfoMap();
    auto font = GameConfig::getInstance()->getGameInfoFont();
    auto menu = Menu::create();

    auto label = Label::createWithBMFont(font, infoMap["has pause"].asString());
    Vec2 lPos = winSize * 0.5f;
    lPos.y += 200;
    label->setPosition(lPos);
    this->addChild(label);

    auto continueItem = MenuItemFont::create(infoMap["continue"].asString(), 
        [](Ref*) {
            AudioEngine::resumeAll();
            Director::getInstance()->popScene();
        });
    auto returnItem= MenuItemFont::create(infoMap["return main"].asString(),
        [](Ref*) {
            AudioEngine::stopAll();
            Director::getInstance()->popToRootScene();
        });
    //continueItem->setFontName(font);
    
    menu->addChild(continueItem);
    menu->addChild(returnItem);
    menu->setPosition(winSize * 0.5f);
    menu->alignItemsVerticallyWithPadding(20);
    menu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(menu);
    auto keyListener = EventListenerKeyboard::create();
    keyListener->onKeyPressed = CC_CALLBACK_2(MyTransitionScene::handleInputKeyDown, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, this);

    continueItem->setOpacity(20);
    continueItem->setScale(2);
    returnItem->setOpacity(20);
    returnItem->setScale(2);
    label->setOpacity(0);
    label->setScale(2);
    auto act = Spawn::create(FadeIn::create(0.2f), ScaleTo::create(0.2f, 1.f), nullptr);
    continueItem->runAction(act);
    label->runAction(act->clone());
    returnItem->runAction(act->clone());

    return true;
}

void MyTransitionScene::handleInputKeyDown(Input keyCode, cocos2d::Event* event)
{
    if (keyCode == Input::KEY_ESCAPE)
    {
        AudioEngine::resumeAll();
        _director->popScene();

    }
}
