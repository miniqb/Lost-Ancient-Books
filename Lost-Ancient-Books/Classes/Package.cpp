#include "Package.h"
#include "cocos2d.h"
#include "GameConfig.h"
USING_NS_CC;
Package* Package::package = nullptr;
void Package::onSceneEnd()
{
    if (!_boy)
        return;
    _fullLife = _boy->getAllLife();
    _nowLife = _boy->getLife();

    auto gameConfig = GameConfig::getInstance();
    int  maxBack = gameConfig->getBacks().size();
    int maxCut = gameConfig->getMaps()[_nowBack].size();

    if (_nowCut >= maxCut - 1)
    {
        _nowCut = 0;
        _nowBack++;
        if (_nowBack >= maxBack)
        {
            _nowBack = 0;
            _nowLife = _fullLife;
            _endTimes++;
            _valueMap["end times"] = Value(_endTimes);
            _isEnd = true;
            _weaponIndex = 0;
            _shell = 0;
        }
    }
    else
        _nowCut++;

    _valueMap["skill"] = Value(_skillIndex);
    _valueMap["piece"] = Value(_knowledgePieces);

    auto fileUtiles = FileUtils::getInstance();
    auto fullName = fileUtiles->fullPathForFilename(_flieName);
    fileUtiles->writeValueMapToFile(_valueMap, fullName);

    if (_boy->isDead())
    {
        resetBoy();
        _nowCut = 0;
        _nowBack = 0;
        _isEnd = false;
    }
}
void Package::onHanziSceneEnd()
{
    _valueMap["piece"] = Value(_knowledgePieces);
    _valueMap["right times"] = Value(_allTimes.rightTimes);
    _valueMap["all times"] = Value(_allTimes.allTimes);

    auto fileUtiles = FileUtils::getInstance();
    auto fullName = fileUtiles->fullPathForFilename(_flieName);
    fileUtiles->writeValueMapToFile(_valueMap, fullName);

}
void Package::resetBoy()
{
    _nowLife = _fullLife;
    _shell = 0;
}
void Package::release()
{
    CC_SAFE_DELETE(package);
}
void Package::releaseButtons()
{
    _hanziButtons.clear();
}
void Package::addCutInfo(bool isRight)
{
    _nowTimes.allTimes++;
    _allTimes.allTimes++;
    if (isRight)
    {
        _nowTimes.rightTimes++;
        _allTimes.rightTimes++;
    }
}
bool Package::isEnd()
{
    return _isEnd;
}
void Package::setBoy(LittleBoy* b)
{
    _boy = b;
}
void Package::addHanziButton(const std::string& str)
{
    this->_hanziButtons.pushBack(HanziButton::create("n.png", str, GameConfig::getInstance()->getRightFont(), 40, "nf.png"));
}
Package* Package::getInstance()
{
    if (!package)
        package = new Package();

    return package;
}

Package::~Package()
{
}

Package::Package()
    :_knowledgePieces(0)
    , _shell(0)
    , _fullLife(100)
    , _nowLife(100)
    , _skillIndex(-1)
    , _weaponIndex(0)
    , _boy(nullptr)
    , _flieName("gamedata/gameData.plist")
    , _nowBack(0)
    , _nowCut(0)
    , _endTimes(0)
    , _isEnd(false)
{
    auto fileUtiles = FileUtils::getInstance();
    auto vmap = fileUtiles->getValueMapFromFile(_flieName);
    _valueMap = vmap;
    _fullLife = vmap["life"].asInt();
    _nowLife = _fullLife;
    _knowledgePieces = vmap["piece"].asInt();
    _skillIndex = vmap["skill"].asInt();
    _endTimes = vmap["end times"].asInt();
    _allTimes.allTimes = vmap["all times"].asInt();
    _allTimes.rightTimes = vmap["right times"].asInt();
}
