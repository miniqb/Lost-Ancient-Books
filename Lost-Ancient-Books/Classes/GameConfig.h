#pragma once
#include "cocos2d.h"
class GameConfig
{
public:
	static GameConfig* getInstance();
	static void release();
public:
	const std::vector<std::string>& getQuestionFiles() { return _questionFiles; }
	const std::string& getAllHanziFile() { return _allHanziFile; }
	const std::string& getGameInfoFile() { return _gameInfoFile; }
	const std::string& getGameInfoFont() { return _gameInfoFont; }
	const std::string& getRightFont() { return _rightFont; }
	const std::string& getWrongFont() { return _wrongFont; }

	const std::string& getMapObjectsWall() { return _mapObjectsWall; }
	const std::string& getMapObjectsPlatform() { return _mapObjectsPlatform; }	
	const std::string& getMapObjectsSeesaw() { return _mapObjectsSeesaw; }
	const std::string& getMapObjectsTrampoline() { return _mapObjectsTrampoline; }
	const std::string& getMapObjectsMonster() { return _mapObjectsMonster; }
	const std::string& getMapObjectsDoor() { return _mapObjectsDoor; }
	const std::string& getMapObjectsBoy() { return _mapObjectsBoy; }
	const std::string& getMapObjectsWeaponTable() { return _mapObjectsWeaponTable; }
	const std::string& getMapObjectsSkillTable() { return _mapObjectsSkillTable; }
	const std::vector< std::vector<std::string>>& getMaps() { return _maps; }
	const std::vector<std::string>& getBacks() { return _backgrounds; }
	const std::vector<std::string>& getBackSounds() { return _backSounds; }
	const std::vector<std::string>& getNames() { return _cutNames; }

private:
	bool init(const std::string& fileName);
private:
	std::vector<std::string> _questionFiles;
	std::string _allHanziFile;
	std::string _gameInfoFile;
	std::string _gameInfoFont;
	std::string _rightFont;
	std::string _wrongFont;

	std::string _mapObjectsWall;
	std::string _mapObjectsPlatform;
	std::string _mapObjectsSeesaw;
	std::string _mapObjectsTrampoline;
	std::string _mapObjectsMonster;
	std::string _mapObjectsDoor;
	std::string _mapObjectsBoy;
	std::string _mapObjectsWeaponTable;
	std::string _mapObjectsSkillTable;

	std::vector< std::vector<std::string>> _maps;
	std::vector<std::string> _backgrounds;
	std::vector<std::string> _backSounds;
	std::vector<std::string> _cutNames;
private:
	static GameConfig* gameConfig;
};

