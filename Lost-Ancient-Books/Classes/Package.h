#pragma once
#include "cocos2d.h"
#include "HanziButton.h"
#include "LittleBoy.h"

struct CutInfo
{
	int rightTimes;
	int allTimes;
	CutInfo() :rightTimes(0), allTimes(0) {}
};

class Package
{
private:
	static Package* package;
public:
	int getKnowledgePieces() { return _knowledgePieces; }
	void setKnowledgePieces(int k) { _knowledgePieces = k; }
	void addKnowledgePieces(int k) { _knowledgePieces += k; }
	void releaseKnowledgePieces(int k) { _knowledgePieces -= k; }
	bool isKnowledgeEnough(int k) { return _knowledgePieces >= k; }

	int getShell() { return _shell; }
	void setShell(int k) { _shell = k; }
	void addShell(int k) { _shell += k; }
	void releaseShell(int k) { _shell -= k; }
	bool isShellEnough(int k) { return _shell >= k; }

	CutInfo getAllTime() { return _allTimes; }
	CutInfo getNowTime() { return _nowTimes; }
	int getEndTime() { return _endTimes; }
	void addCutInfo(bool isRight);

	bool isEnd();
	void resetEnd() { _isEnd = false; }

	void setBoy(LittleBoy* b);
	LittleBoy* getBoy() { return _boy; }

	void addHanziButton(const std::string& str);
	const cocos2d::Vector<HanziButton*>& getHanziButtons() { return _hanziButtons; }

	int getNowLife() { return _nowLife; };
	int getFullLife() { return _fullLife; }
	int getSkill() { return _skillIndex; }
	void setSkill(int s) { _skillIndex = s; }
	void setWeapon(int s) { _weaponIndex = s; }
	int getWeapon() { return _weaponIndex; }
	void onSceneEnd();
	void onHanziSceneEnd();
	void resetBoy();
	int getNowCut() { return _nowCut; };//获取当前小关
	int getNowBack() { return _nowBack; }

	void setLastSoundName(const std::string& name) { _lastSoundName = name; }
	const std::string& getLastSoundName() { return _lastSoundName; }
public:
	static void release();
	void releaseButtons();
	static Package* getInstance();
	~Package();
private:
	Package();
private:
	int _knowledgePieces;
	int _shell;
	int _fullLife;
	int _nowLife;
	int _skillIndex;
	int _weaponIndex;
	int _nowBack;//当前大关
	int _nowCut;//当前小关
	bool _isEnd;
	int _endTimes;
	CutInfo _allTimes;
	CutInfo _nowTimes;
	std::string _flieName;
	LittleBoy* _boy;

	cocos2d::Vector<HanziButton*> _hanziButtons;

	cocos2d::ValueMap _valueMap;

	std::string _lastSoundName;
};

