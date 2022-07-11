#include "FirstScene.h"
#include "Package.h"
#include "GameConfig.h"
#include "InfoHelper.h"
#include "MainMenuScene.h"
#include "AudioEngine.h"
USING_NS_CC;
bool FirstScene::init()
{
    if (!Scene::init())
        return false;


    Size winSize = _director->getWinSize();

    //Ô¤¼ÓÔØÒôÀÖ
    auto gameConfig = GameConfig::getInstance();
    auto sounds = gameConfig->getBackSounds();
    for (auto& sound:sounds)
    {
        AudioEngine::preload(sound);
    }
    AudioEngine::preload("sound/wav/close.mp3");
    AudioEngine::preload("sound/wav/far.mp3");

    auto& infoMap = InfoHelper::getInstance()->getInfoMap();

    auto str1 = infoMap["first title"].asString();
    auto label1 = Label::createWithBMFont(gameConfig->getGameInfoFont(), str1, TextHAlignment::LEFT);
    label1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    label1->setPosition(winSize * 0.5f);
    label1->setOpacity(0);
    addChild(label1);
    auto act = Sequence::create(DelayTime::create(2.f), FadeIn::create(1.8f), DelayTime::create(2.f), FadeOut::create(1.8f),DelayTime::create(1.f), CallFunc::create([this]() {
        runAction(CallFunc::create([]() {
            Director::getInstance()->replaceScene(TransitionFade::create(0.9, MainMenuScene::createScene(), Color3B(234, 224, 201))); }));
        }), nullptr);
    label1->runAction(act);
    return true;
}
