#include "GameConfig.h"

GameConfig* GameConfig::gameConfig = nullptr;

GameConfig* GameConfig::getInstance()
{
    if (gameConfig)
        return gameConfig;

    gameConfig = new GameConfig();
    if (gameConfig && gameConfig->init("config/config.plist"))
        return gameConfig;

    CC_SAFE_DELETE(gameConfig);
    return nullptr;
}

void GameConfig::release()
{
    CC_SAFE_DELETE(gameConfig);
}

bool GameConfig::init(const std::string& fileName)
{
    auto fileUtils = cocos2d::FileUtils::getInstance();
    if (fileUtils == nullptr)
        return false;

    auto configMap = fileUtils->getValueMapFromFile(fileName);
    
    auto qfs = configMap["question files"].asValueVector();
    size_t size = qfs.size();
    _questionFiles.reserve(size);
    for (size_t i = 0; i < size; i++)
    {
        _questionFiles.push_back(qfs[i].asString());
    }

    _allHanziFile = configMap["all Hanzi file"].asString();
    _gameInfoFile = configMap["game info files"].asString();
    _gameInfoFont = configMap["game info font"].asString();
    _rightFont = configMap["queston font right"].asString();
    _wrongFont = configMap["question font wrong"].asString();

    _mapObjectsWall = configMap["map objects wall"].asString();
   _mapObjectsPlatform = configMap["map objects platform"].asString();
    _mapObjectsSeesaw = configMap["map objects seesaw"].asString();
    _mapObjectsTrampoline = configMap["map objects trampoline"].asString();
    _mapObjectsMonster = configMap["map objects monster"].asString();
    _mapObjectsDoor = configMap["map objects door"].asString();
    _mapObjectsBoy = configMap["map objects boy"].asString();
    _mapObjectsWeaponTable= configMap["weapon table"].asString();
    _mapObjectsSkillTable=configMap["skill table"].asString();

    auto maps = configMap["maps"].asValueVector();
    size_t mapSize = maps.size();
    _maps.reserve(mapSize);
    for (size_t i = 0; i < mapSize; i++)
    {
        _maps.push_back(std::vector<std::string>());
        auto imap = maps[i].asValueVector();
        size_t size = imap.size();
        _maps[i].reserve(size);
        for (size_t j = 0; j < size; j++)
        {
            _maps[i].push_back(imap[j].asString());
        }
    }

    auto backgrounds= configMap["backgrounds"].asValueVector();
    size_t backSize = backgrounds.size();
    _backgrounds.reserve(backSize);
    for (size_t i = 0; i < backSize; i++)
    {
        _backgrounds.push_back(backgrounds[i].asString());
    }

    auto backSounds = configMap["back sounds"].asValueVector();
    size_t soundSize = backSounds.size();
    _backSounds.reserve(soundSize);
    for (size_t i = 0; i < soundSize; i++)
    {
        _backSounds.push_back(backSounds[i].asString());
    }

    auto names = configMap["cut names"].asValueVector();
    size_t nameSize = names.size();
    _cutNames.reserve(nameSize);
    for (size_t i = 0; i < nameSize; i++)
    {
        _cutNames.push_back(names[i].asString());
    }

    return true;
}
