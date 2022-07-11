#pragma once
#include "Crash.h"
#include <map>
class CallbackThing :public Crash
{
public:
	static CallbackThing* creat(cocos2d::EventKeyboard::KeyCode key, const std::string& str, const std::function<void()>& func);
	static CallbackThing* create();
	static CallbackThing* createWithSpriteFrame(cocos2d::SpriteFrame* frame);
	void initWithFunc(cocos2d::EventKeyboard::KeyCode key, const std::string str, const std::function<void()>& func);

	void setActive(bool a) { _isActive = a; }
	bool isActive() { return _isActive; }

	virtual bool crash(cocos2d::Node* other)override;
	virtual void outCrash(cocos2d::Node* other)override;

	void setShowNode(cocos2d::Node* node);

	void activeTip();
	void sleepTip();

	bool excuteFunc(cocos2d::EventKeyboard::KeyCode key);

	void addKey(cocos2d::EventKeyboard::KeyCode key, const std::function<void()>& func); //{ _key = key, _keyStr = str; }
	void setKeyStr(const std::string& str);
	//const std::vector<cocos2d::EventKeyboard::KeyCode>& getKeys() { return _keys; }
	int foundKey(cocos2d::EventKeyboard::KeyCode key);

protected:
	void addTip();
	void removeTip();

public:
	CallbackThing()
		: _isActive(false)
		, _tip(nullptr)
		, _keyAndFunc()
		, _keyStr()
		, _showNode(nullptr)
	{
	}
	virtual ~CallbackThing();
protected:
	bool _isActive;
	cocos2d::Node* _tip;
	std::map<cocos2d::EventKeyboard::KeyCode, std::function<void()>> _keyAndFunc;
	std::string _keyStr;
	cocos2d::Node* _showNode;
};

