#include "MainMenuScene.h"
#include "GameScene.h"
#include "HanziScene.h"
#include "InfoHelper.h"
#include "Package.h"
#include <QuestionHelper.h>
#include "InfoHelper.h"
#include "GameConfig.h"
#include "GlobalWorld.h"
#include "AudioEngine.h"
#include "TempScene.h"

USING_NS_CC;

Scene* MainMenuScene::createScene()
{
    return MainMenuScene::create();
}

bool MainMenuScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    Size winSize = _director->getWinSize();

    float sizeRate = GlobalWorld::scale;
    auto backStr = "mainScene/mainMenu.png";
    auto startStr = "mainScene/start.png";
    auto helpStr = "mainScene/help.png";
    auto aboutStr = "mainScene/about.png";
    auto startStrc = "mainScene/startc.png";
    auto helpStrc = "mainScene/helpc.png";
    auto aboutStrc = "mainScene/aboutc.png";
    //±³¾°
    auto background = Sprite::create("mainScene/mainMenu.png");
    background->setPosition(winSize * 0.5f);
    background->setScale(winSize.width / background->getContentSize().width);
    addChild(background);
  
    //°´Å¥ÃÇ
    bstart = ui::Button::create(startStr, startStrc);
    bhelp = ui::Button::create(helpStr, helpStrc);
    babout = ui::Button::create(aboutStr, aboutStrc);
    
    background->addChild(bstart);
    background->addChild(bhelp);
    background->addChild(babout);
    Size bsize = background->getContentSize();

    Vec2 startPos = Vec2(1406.87 / 1600 * bsize.width, (900 - 413.82) / 900 * bsize.height);
    Vec2 helpPos = Vec2(1450.50 / 1600 * bsize.width, (900 - 596.0) / 900 * bsize.height);
    Vec2 aboutPos = Vec2(1451.50 / 1600 * bsize.width, (900 - 775.0) / 900 * bsize.height);
    bstart->setPosition(startPos);
    bhelp->setPosition(helpPos);
    babout->setPosition(aboutPos);
    bstart->setRotation(-15.2);

    bstart->addClickEventListener([this](Ref*) {
        _director->pushScene(TransitionFade::create(0.5, TempScene::create(0.5f), Color3B(0, 0, 0)));
        });

    auto& infoMap = InfoHelper::getInstance()->getInfoMap();
    auto helpInfoStr = infoMap["help info"].asString();
    auto aboutInfoStr = infoMap["about info"].asString();
    bhelp->addClickEventListener([this, helpInfoStr](Ref*) {
        if (_drect->isVisible() == false)
        {
            _drect->setVisible(true);
            //_infoLabel->setBMFontSize(30);
            _infoLabel->setString(helpInfoStr);
            babout->setVisible(false);
            bstart->setVisible(false);
        }
        else
        {
            _drect->setVisible(false);
            babout->setVisible(true);
            bstart->setVisible(true);
        }
        });

    babout->addClickEventListener([this, aboutInfoStr](Ref*) {
        if (_drect->isVisible() == false)
        {
            _drect->setVisible(true);
            //_infoLabel->setBMFontSize(25);
            _infoLabel->setString(aboutInfoStr);
            bhelp->setVisible(false);
            bstart->setVisible(false);
        }
        else
        {
            _drect->setVisible(false);
            bhelp->setVisible(true);
            bstart->setVisible(true);
        }
        });
    auto font = GameConfig::getInstance()->getGameInfoFont();


    _drect = DrawNode::create();
    Size dsize = Size(800, 600);
    _drect->drawSolidRect(-Vec2(dsize * 0.5f), dsize * 0.5f, Color4F(0, 0, 0, 100));
    _infoLabel = Label::createWithBMFont(font, "");
    _infoLabel->setBMFontSize(30);
    _infoLabel->setWidth(dsize.width * 0.9f);
    _drect->addChild(_infoLabel);
    _drect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _infoLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _drect->setPosition(bsize.width * 0.4, bsize.height * 0.5f);
    background->addChild(_drect);
    _drect->setVisible(false);

    return true;
}

void MainMenuScene::onExit()
{
    Scene::onExit();
    InfoHelper::release();
    Package::release();
    Batchs::release();
    KeyController::release();
    QuestionHelper::release();
    AnimationLoader::release();
    GameConfig::release();
    InfoHelper::release();
}

MainMenuScene::~MainMenuScene()
{
    AudioEngine::uncacheAll();
}
