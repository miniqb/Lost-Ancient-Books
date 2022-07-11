#include "TempScene.h"
#include "GameScene.h"
#include "Package.h"
#include "GameConfig.h"
#include "InfoHelper.h"
USING_NS_CC;
TempScene* TempScene::create(float time, bool isEnd, bool isDead)
{
    auto scene = new(std::nothrow) TempScene();
    if (scene && scene->initTemp(time,isEnd, isDead))
    {
        scene->autorelease();
        return scene;
    }
    CC_SAFE_DELETE(scene);
    return nullptr;
}

bool TempScene::initTemp(float time, bool isEnd, bool isDead)
{
    if (!Scene::init())
        return false;

    //Sprite* background = Sprite::createWithTexture(texture->getSprite()->getTexture());
    //Size winSize = _director->getWinSize();
    //background->setPosition(winSize * 0.5f);
    //background->setFlippedY(true);
    //addChild(background);
    if (isEnd)
    {
        Size winSize = _director->getWinSize();

        auto pack = Package::getInstance();
        pack->resetEnd();
        CutInfo now = pack->getNowTime();
        CutInfo all = pack->getAllTime();
        int endTime = pack->getEndTime();
        float nowPer = 0;
        if (now.allTimes != 0)
            nowPer = float(now.rightTimes) / float(now.allTimes) * 100.f;
        float allPer = 0;
        if (now.allTimes != 0)
            allPer = float(all.rightTimes) / float(all.allTimes) * 100.f;



        char strs[20];
        sprintf(strs, "%.2f%%", nowPer);
        std::string nowPerStr = strs;
        sprintf(strs, "%.2f%%", allPer);
        std::string allPerStr = strs;
        sprintf(strs, "%d", endTime);
        std::string endTimeStr = strs;

        auto& infoMap = InfoHelper::getInstance()->getInfoMap();
        nowPerStr = infoMap["now right percent"].asString() + nowPerStr;
        allPerStr = infoMap["all right percent"].asString() + allPerStr;
        endTimeStr= infoMap["end times"].asString() + endTimeStr;

        auto gameConfig = GameConfig::getInstance();
        auto font = gameConfig->getGameInfoFont();
        auto endTitle = infoMap["end title"].asString();
        auto endLabel = Label::createWithBMFont(font, endTitle, TextHAlignment::LEFT);
        endLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        endLabel->setPosition(winSize * 0.5f);
        endLabel->setOpacity(0);
        addChild(endLabel);
        auto act0 = Sequence::create(DelayTime::create(1.5f),FadeIn::create(1.f), DelayTime::create(3.f), FadeOut::create(1.f), RemoveSelf::create(), nullptr);
        endLabel->runAction(act0);

        auto str1 = nowPerStr + '\n' + allPerStr + '\n' + endTimeStr;
        auto label1 = Label::createWithBMFont(font, str1, TextHAlignment::LEFT);
        label1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        label1->setPosition(winSize * 0.5f);
        label1->setOpacity(0);
        addChild(label1);
        auto act = Sequence::create(DelayTime::create(7.8f), FadeIn::create(0.2f), DelayTime::create(2.f), FadeOut::create(0.8f), RemoveSelf::create(), nullptr);
        label1->runAction(act);

        auto strReturn = infoMap["return first"].asString();
        auto label2 = Label::createWithBMFont(font, strReturn, TextHAlignment::CENTER);
        label2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        label2->setOpacity(0);
        label2->setPosition(winSize * 0.5f);
        addChild(label2);
        auto act2 = Sequence::create(DelayTime::create(3.5f+7.8f), FadeIn::create(0.2f), DelayTime::create(2.f), FadeOut::create(0.8f),
            CallFunc::create([this]() {
                runAction(CallFunc::create([]() {Director::getInstance()->replaceScene(GameScene::create()); }));
            }), nullptr);
        label2->runAction(act2);
    }
    else if (isDead)
    {
        Size winSize = _director->getWinSize();
        auto& infoMap = InfoHelper::getInstance()->getInfoMap();

        auto str1 = infoMap["dead"].asString();
        auto gameConfig = GameConfig::getInstance();
        auto label1 = Label::createWithBMFont(gameConfig->getGameInfoFont(), str1, TextHAlignment::LEFT);
        label1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        label1->setColor(Color3B::RED);
        label1->setPosition(winSize * 0.5f);
        label1->setOpacity(0);
        addChild(label1);
        auto act = Sequence::create(FadeIn::create(0.2f), DelayTime::create(2.f), FadeOut::create(0.8f), RemoveSelf::create(), nullptr);
        label1->runAction(act);

        auto strReturn = infoMap["return first"].asString();
        auto label2 = Label::createWithBMFont(gameConfig->getGameInfoFont(), strReturn, TextHAlignment::CENTER);
        label2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        label2->setOpacity(0);
        label2->setPosition(winSize * 0.5f);
        addChild(label2);
        auto act2 = Sequence::create(DelayTime::create(3.5f), FadeIn::create(0.2f), DelayTime::create(2.f), FadeOut::create(0.8f),
            CallFunc::create([this]() {
                runAction(CallFunc::create([]() {Director::getInstance()->replaceScene(GameScene::create()); }));
                }), nullptr);
        label2->runAction(act2);
    }
    else
    {
        runAction(Sequence::create(DelayTime::create(time + 0.05f), CallFunc::create([]() {Director::getInstance()->replaceScene(GameScene::create()); }), nullptr));
    }

    return true;
}

