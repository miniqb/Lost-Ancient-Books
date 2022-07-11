#pragma once
#include "GameCommand.h"
#include "CallbackThing.h"
typedef cocos2d::EventKeyboard::KeyCode Input;
class GameCommand;

enum class KeyState
{
	Key_Down=0,
	Key_Up=1
};
class KeyController
{
public:
	static KeyController* getInstance() { return controller; }
	static void release();
	bool handleCallBackThingsInput(Input input, KeyState state);
	GameCommand* acceptInput(Input input, KeyState state);
	bool isKeyDown(Input key) { return keys[int(key)]; }
	void addCallbackThing(CallbackThing* t)
	{
		_callBackThings.push_back(t);
	}
	void removeCallbackThing(CallbackThing* t);
	void releaseCallbackList() { _callBackThings.clear(); }
	CallbackThing* getNowCallbackThings();
	~KeyController();
private:
	static KeyController* controller;
	bool keys[300]{ false };
	GameCommand* _jumpCommand;
	GameCommand* _moveLeftCommand;
	GameCommand* _moveRightCommand;
	GameCommand* _stopMoveLeft;
	GameCommand* _stopMoveRight;
	GameCommand* _attackCommand;
	GameCommand* _attackRushRightCommand;
	GameCommand* _attackRushLeftCommand;
	GameCommand* _fallPlatFormCommand;
	GameCommand* _skillPowerCommand;

	std::list<CallbackThing*> _callBackThings;
private:
	KeyController();
};
