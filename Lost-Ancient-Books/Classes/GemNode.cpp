#include "GemNode.h"
#include "GameConfig.h"
USING_NS_CC;
GemNode* GemNode::create(int shell, int piece)
{
    auto node = new(std::nothrow) GemNode();
    if (node && node->initGemNode(shell, piece))
    {
        node->autorelease();
        return node;
    }
    CC_SAFE_DELETE(node);
    return nullptr;
}

bool GemNode::initGemNode(int shell, int piece)
{
    if (!init())
        return false;

    auto frameCache = SpriteFrameCache::getInstance();
    _shell = Sprite::createWithSpriteFrame(frameCache->getSpriteFrameByName("gem/shell.png"));
    _piece = Sprite::createWithSpriteFrame(frameCache->getSpriteFrameByName("gem/fragment.png"));

    auto gameConfig = GameConfig::getInstance();
    _shellLabel = Label::createWithBMFont(gameConfig->getGameInfoFont(), "");
    _shellLabel->setColor(Color3B(255, 247, 186));
    _shellLabel->setBMFontSize(30);
    _shellLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _pieceLabel = Label::createWithBMFont(gameConfig->getGameInfoFont(), "");
    _pieceLabel->setColor(Color3B(176, 246, 255));
    _pieceLabel->setBMFontSize(30);
    _pieceLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);


    Size shellSize = _shell->getContentSize();
    Size pieceSize = _piece->getContentSize();
    float posY = std::max(shellSize.height, pieceSize.height) * 0.5f;
    float posX = shellSize.width * 0.5f;
    _shell->setPosition(Vec2(posX, posY));
    posX += 20;
    _shellLabel->setPosition(Vec2(posX, posY));

    posX += 200;

    _piece->setPosition(Vec2(posX, posY));
    posX += 20;
    _pieceLabel->setPosition(Vec2(posX, posY));

    addChild(_shell);
    addChild(_piece);
    addChild(_shellLabel);
    addChild(_pieceLabel);

    setShell(shell);
    setPiece(piece);
    return true;
}

void GemNode::setShell(int num)
{
    char numStr[10];
    sprintf(numStr, ": %d", num);
    _shellLabel->setString(numStr);
}

void GemNode::setPiece(int num)
{
    char numStr[10];
    sprintf(numStr, ": %d", num);
    _pieceLabel->setString(numStr);
}

GemNode::GemNode()
    :_shell(nullptr)
    , _shellLabel(nullptr)
    , _piece(nullptr)
    , _pieceLabel(nullptr)
{
}

GemNode::~GemNode()
{
}
