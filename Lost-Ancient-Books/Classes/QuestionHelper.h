#pragma once
#include "cocos2d.h"
#include "HanziButton.h"
class QuestionHelper
{
public:
	static QuestionHelper* getInstance();
	static void release();
	cocos2d::Value& getRandomPom();
	cocos2d::Value& getNowChoosePom();
	std::vector<int>& getRandomIndexs(int num, int allSum, const std::vector<int>& cant);
	std::vector<int>& getNullIndexs() { return _nulls; }
	std::string getRandomHanzi();

private:
	bool init(const std::vector<std::string>& fileName, const std::string& randomFileName);
private:
	static QuestionHelper* qHelper;
	cocos2d::ValueVector _pomList;
	cocos2d::ValueVector _random;
	std::vector<int> _nulls;
	int _nowChooseIndex = -1;
};

