#include "HanziScene.h"
#include "HanziButton.h"
#include "Package.h"
#include "QuestionNode.h"
#include "QuestionHelper.h"
#include "InfoHelper.h"
#include "GameConfig.h"
#include "GameScene.h"
#include "AudioEngine.h"

USING_NS_CC;

bool HanziScene::initHanziScene(cocos2d::RenderTexture* texture)
{
    if (!Scene::init())
    {
        return false;
    }

    _shellSum = 50;
    _pieceSum = 10;

    //加载精灵图集
    auto spritecache = SpriteFrameCache::getInstance();

    Size winSize = _director->getInstance()->getWinSize();

    //先放个背景
    Vec2 backPos = winSize * 0.5f;
    Sprite* background = Sprite::createWithTexture(texture->getSprite()->getTexture());
    background->setPosition(backPos);
    background->setFlippedY(true);
    addChild(background);

    //添加背景卷轴
    _background = Sprite::createWithSpriteFrame(spritecache->getSpriteFrameByName("ask.png"));
    float backHeight = _background->getContentSize().height;
    _background->setScale(winSize.height / backHeight);
    _background->setPosition(Vec2(backPos.x, -backHeight * 0.5f * _background->getScale()));
    addChild(_background);

    //卷轴向上划出来
    _background->runAction(MoveTo::create(0.2f, winSize * 0.5f));

    //获取题目相关数据
    auto questionHelper = QuestionHelper::getInstance();

    ValueMap& pom = questionHelper->getNowChoosePom().asValueMap();

    auto pomstrs = pom["sentence"].asValueVector();
    int length = pomstrs.size();
    std::vector<std::string> strs;
    strs.reserve(length);
    for (size_t i = 0; i < length; i++)
    {
        strs.push_back(pomstrs[i].asString());
    }

    float sizeRate = 800.f / 1024.f;
    /**********************************上方汉字凹槽************************************************/
    float maxWidth = 664 * sizeRate;
    Vec2 quePos = Vec2(493, 347) * sizeRate;
    Size queMargin = Size(4, 6) * sizeRate;
    Size buttonSize = Size(45, 45) * sizeRate;
    {//放置字
        //std::vector<int> nulls = {};
        std::vector<int>& nulls = QuestionHelper::getInstance()->getNullIndexs();
        std::string rightFont = GameConfig::getInstance()->getRightFont();

        _questionNode = QuestionNode::create(buttonSize, rightFont, strs, nulls, 40);
        _questionNode->setMargin(queMargin);
        _questionNode->setMaxWidth(maxWidth);
        _questionNode->setPosition(quePos);
        _background->addChild(_questionNode);
    }


    /**********************************下方汉字石块************************************************/
    Size butMargin = Size(0, 0) * sizeRate;
    Vec2 butPos = Vec2(493, 214) * sizeRate;
    auto pack = Package::getInstance();
    if (pack->getHanziButtons().size() > 0)
    {
        auto bnts = creatHanziPackageNode(maxWidth, butMargin, pack->getHanziButtons(), 40);
        bnts->setPosition(butPos);
        _background->addChild(bnts);
    }

    auto gameConfig = GameConfig::getInstance();
    std::string infoFont = gameConfig->getGameInfoFont();
    auto& strMap = InfoHelper::getInstance()->getInfoMap();


    /**********************************下方提交放弃按钮************************************************/

    Vec2 okPos = Vec2(286, 84) * sizeRate;
    std::string okTitle = strMap["submit"].asString();

    _okButton = ui::Button::create("button2.png", "button2f.png", "", ui::Widget::TextureResType::PLIST);
    Label* labelok = Label::createWithBMFont(infoFont, okTitle);
    labelok->setColor(Color3B(132, 105, 94));
    labelok->setBMFontSize(40);
    _okButton->setTitleLabel(labelok);
    _okButton->setPosition(okPos);
    _okButton->addClickEventListener(CC_CALLBACK_0(HanziScene::answerCallback, this));
    _background->addChild(_okButton);


    Vec2 canclePos = Vec2(732, 89) * sizeRate;
    std::string cancleTitle = strMap["cancle"].asString();
    _cancleButton = ui::Button::create("button1.png", "button1f.png", "", ui::Widget::TextureResType::PLIST);
    Label* labelcan = Label::createWithBMFont(infoFont, cancleTitle);
    labelcan->setColor(Color3B(132, 105, 94));
    labelcan->setBMFontSize(40);
    _cancleButton->setTitleLabel(labelcan);
    _cancleButton->setPosition(canclePos);
    _cancleButton->addClickEventListener(CC_CALLBACK_0(HanziScene::answerCallback, this));
    _background->addChild(_cancleButton);

    /**********************************上方进度条************************************************/
    //Vec2 loadingPos(487, 475);
    Vec2 loadingPos = Vec2(487, 475) * sizeRate;
    Size loadingSize = Size(570, 24) * sizeRate;
    _loadingBar = ui::LoadingBar::create("loading.png", ui::Widget::TextureResType::PLIST);
    _loadingBar->setTag(0);//?
    _loadingBar->setScale9Enabled(true);
    _loadingBar->setCapInsets(Rect(0, 0, 0, 0));
    _loadingBar->setContentSize(loadingSize);
    _loadingBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _loadingBar->setPercent(100);
    _loadingBar->setPosition(loadingPos);
    _loadingBar->setDirection(ui::LoadingBar::Direction::RIGHT);
    _background->addChild(_loadingBar);

    /**********************************下方提示信息************************************************/
    Vec2 infoPos = Vec2(504, 141) * sizeRate;
    _tipLabel = Label::createWithBMFont(infoFont, "", TextHAlignment::CENTER);
    _tipLabel->setBMFontSize(30);
    _tipLabel->setPosition(infoPos);
    _tipLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _background->addChild(_tipLabel);
    this->scheduleUpdate();
    return true;
}

void HanziScene::onMouseDown(EventMouse* event)
{
    auto lable = static_cast<Label*>(event->getCurrentTarget());
    if (!lable)
        return;
    //CCLOG("%s", lable->getString());
}

Node* HanziScene::creatHanziPackageNode(float maxWidth, const Size& margin, const Vector<HanziButton*>& buttons, float fontSize)
{
    Size cellSize = buttons.at(0)->getContentSize();
    Size cell = cellSize;
    cell.height = -cell.height;

    size_t length = buttons.size();//列
    size_t sumY = 1;//行数
    Vec2 startPx = -Vec2((cellSize.width + margin.width) * (length - 1) * 0.5f,
        -(cellSize.height + margin.height) * (sumY - 1) * 0.5f) - cell * 0.5f;

    //startPx.y = this->getContentSize().height * 0.41218;

    float maxW = -maxWidth * 0.5f;
    while (startPx.x <= maxW)//如果问题太长题目装不下
    {
        ++sumY;
        length = size_t(length % sumY > 0) + length / sumY;
        startPx = -Vec2((cellSize.width + margin.width) * (length - 1) * 0.5f,
            -(cellSize.height + margin.height) * (sumY - 1) * 0.5f) - cell * 0.5f;
    }
    Vec2 startPxMid = startPx + cell * 0.5f;

    Node* buttonNode = Node::create();
    buttonNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    Node* labelNode = Node::create();
    labelNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

    size_t vecSize = buttons.size();
    for (size_t i = 0; i < vecSize; i++)
    {
        Vec2 ex(cellSize + margin);
        ex.x *= i % length;
        ex.y *= i / length;
        ex.y = -ex.y;
        Vec2 dpos = startPxMid + ex;

        auto btn = buttons.at(i);
        btn->setPosition(dpos);
        btn->setOriPos(dpos);
        btn->setEnabled(true);
        btn->addClickEventListener([btn, this](Ref*) {
            if (btn->getMode() == HanziButton::Mode::OUTQ)
                _questionNode->putButton(btn);
            else
                _questionNode->returnButton(btn);
            });

        buttonNode->addChild(btn);
        auto lab = btn->getLabel();
        lab->setBMFontSize(fontSize);
        labelNode->addChild(lab);
    }

    Node* resNode = Node::create();
    resNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    resNode->addChild(buttonNode);
    resNode->addChild(labelNode);
    return resNode;
}

Node* HanziScene::createWardNode(const cocos2d::Vec2& pos,const cocos2d::Size& nodeSize, int shellSum, int pieceSum)
{
    Node* res = Node::create();
    res->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    res->setPosition(pos);
    res->setContentSize(nodeSize);
    auto frameCache = SpriteFrameCache::getInstance();
    Sprite* shell = Sprite::createWithSpriteFrame(frameCache->getSpriteFrameByName("shell.png"));
    Sprite* piece = Sprite::createWithSpriteFrame(frameCache->getSpriteFrameByName("fragment.png"));

    auto gameConfig = GameConfig::getInstance();
    char str[20];
    sprintf(str, "+%d", shellSum);
    Label* shellText = Label::createWithBMFont(gameConfig->getGameInfoFont(), str);
    shellText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    shellText->setBMFontSize(30);
    shellText->setColor(Color3B(255, 247, 186));
    sprintf(str, "+%d", pieceSum);
    Label* pieceText = Label::createWithBMFont(gameConfig->getGameInfoFont(), str);
    pieceText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    pieceText->setBMFontSize(30);
    pieceText->setColor(Color3B(176, 246, 255));

    res->addChild(shell);
    res->addChild(piece);
    res->addChild(shellText);
    res->addChild(pieceText);

    float sizeRate = 800.f / 1024.f;
    float posy = nodeSize.height * 0.5f;
    shell->setPosition(Vec2(nodeSize.width * 0.25f - 20, posy));
    shellText->setPosition(Vec2(nodeSize.width * 0.25f + 10, posy));
    piece->setPosition(Vec2(nodeSize.width * 0.75f - 20, posy));
    pieceText->setPosition(Vec2(nodeSize.width * 0.75f + 10, posy));

    return res;
}

void HanziScene::outScene(bool res)
{
    float sizeRate = 800.f / 1024.f;
    int shell = 0, piece = 0;
    if (res)
    {
        shell = _shellSum;
        piece = _pieceSum;
    }
    Vec2 pos = _tipLabel->getPosition();
    _background->removeChild(_tipLabel, true);

    Size size = Size(270, 35) * sizeRate;
    float dy = 30;
    pos.y -= dy;
    auto node = createWardNode(pos, size, shell, piece);
    _background->addChild(node);

    float moveTime = 0.5f;
    float waitTime = 1.8f;
    float allMoveTime = 0.2f;

    node->runAction(MoveBy::create(moveTime, Vec2(0, dy)));

    auto backsqe = Sequence::create(DelayTime::create(moveTime + waitTime),
        MoveBy::create(0.2f, Vec2(0, -0.5f * (_background->getContentSize().height + _director->getWinSize().height))),
        CallFunc::create([this]() {
            _director->popScene();
            }), nullptr);
    _background->runAction(backsqe);
}

bool HanziScene::showResult()
{
    bool res = _questionNode->isRight();
    auto pack = Package::getInstance();
    if (res)
    {
        pack->addKnowledgePieces(_pieceSum);
        pack->addShell(_shellSum);
    }
    pack->addCutInfo(res);

    auto& strMap = InfoHelper::getInstance()->getInfoMap();
    _tipLabel->setColor(res ? Color3B(132, 105, 94) : Color3B(132, 105, 94));
    std::string resStr = strMap[res ? "answer right" : "answer wrong"].asString();
    _tipLabel->setString(resStr);
    return res;
}

void HanziScene::update(float dt)
{
    if (_loadingBar)
    {
        float nper = _loadingBar->getPercent();
        if (nper > 0)
        {
            float cw = (100 / _answerTime) * dt;
            _loadingBar->setPercent(nper - cw);
        }
        else
        {
            _loadingBar = nullptr;
            answerCallback();
        }
    }
}

void HanziScene::onEnter()
{
    Scene::onEnter();
}

void HanziScene::onExit()
{
    Scene::onExit();
    auto pack = Package::getInstance();
    pack->releaseButtons();
    pack->onHanziSceneEnd();
}

HanziScene* HanziScene::create(cocos2d::RenderTexture* texture)
{
    auto scene = new (std::nothrow)HanziScene();
    if (scene && scene->initHanziScene(texture))
    {
        scene->autorelease();
        return scene;
    }
    CC_SAFE_DELETE(scene);
    return nullptr;
}

void HanziScene::answerCallback()
{
    bool res = showResult();
    resetHanziButtons();
    _okButton->setEnabled(false);
    _cancleButton->setEnabled(false);
    this->unscheduleUpdate();
    this->scheduleOnce([this,res](float) {outScene(res); }, 2.f, "outScene");
}

void HanziScene::resetHanziButtons()
{
    auto btns = Package::getInstance()->getHanziButtons();
    for (auto btn : btns)
    {
        btn->setEnabled(false);
    }
}
