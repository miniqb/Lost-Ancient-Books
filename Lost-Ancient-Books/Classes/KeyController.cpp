#include "KeyController.h"

KeyController* KeyController::controller = new KeyController();

void KeyController::release()
{
	if (controller)
	{
		std::fill(controller->keys, controller->keys + 300, false);
		controller->releaseCallbackList();
	}
}

bool KeyController::handleCallBackThingsInput(Input input, KeyState state)
{
	bool res = false;
	if (state == KeyState::Key_Down &&
		!_callBackThings.empty() &&
		_callBackThings.back()->isActive())
	{
		res = _callBackThings.back()->excuteFunc(input);
	}
	return res;
}

GameCommand* KeyController::acceptInput(Input input, KeyState state)
{
	GameCommand* res = nullptr;
	if (state == KeyState::Key_Down)
	{
		keys[int(input)] = true;
		switch (input)
		{
		case Input::KEY_A:
			res = _moveLeftCommand;
			break;
		case Input::KEY_D:
			res = _moveRightCommand;
			break;
		case Input::KEY_SPACE:
			if (keys[int(Input::KEY_S)])
				res = _fallPlatFormCommand;
			else
				res = _jumpCommand;
			break;
		case Input::KEY_K:
			if (keys[int(Input::KEY_D)])
				res = _attackRushRightCommand;
			else if (keys[int(Input::KEY_A)])
				res = _attackRushLeftCommand;
			else
				res = _attackCommand;
			break;
		case Input::KEY_F:
			res = _skillPowerCommand;
			break;
		default:
			break;
		}
	}
	else
	{
		keys[int(input)] = false;
		switch (input)
		{
		case Input::KEY_A:
			res = _stopMoveLeft;
			break;
		case Input::KEY_D:
			res = _stopMoveRight;
			break;
		default:
			break;
		}
	}
	return res;
}

void KeyController::removeCallbackThing(CallbackThing* t)
{
	if (_callBackThings.empty())
		return;
	_callBackThings.remove(t);
}

CallbackThing* KeyController::getNowCallbackThings()
{
	if(_callBackThings.empty())
		return nullptr;
	return _callBackThings.back();
}

KeyController::~KeyController()
{
	CC_SAFE_DELETE(_jumpCommand);
	CC_SAFE_DELETE(_moveLeftCommand);
	CC_SAFE_DELETE(_moveRightCommand);
	CC_SAFE_DELETE(_stopMoveLeft);
	CC_SAFE_DELETE(_stopMoveRight);
	CC_SAFE_DELETE(_attackCommand);
	CC_SAFE_DELETE(_attackRushLeftCommand);
	CC_SAFE_DELETE(_attackRushRightCommand);
	CC_SAFE_DELETE(_fallPlatFormCommand);
	CC_SAFE_DELETE(_skillPowerCommand);
}

KeyController::KeyController():
	_callBackThings()
{
	_jumpCommand = new JumpCommand();
	_moveLeftCommand = new MoveCommand(false);
	_moveRightCommand = new MoveCommand(true);
	_stopMoveLeft = new StopMoveCommand(false);
	_stopMoveRight = new StopMoveCommand(true);
	_attackCommand = new AttackCommand(AttackCommand::NORMAL,false);
	_attackRushRightCommand = new AttackCommand(AttackCommand::RUSH, true);
	_attackRushLeftCommand = new AttackCommand(AttackCommand::RUSH,false);
	_fallPlatFormCommand = new FallDownPlatform();
	_skillPowerCommand = new SkillPowerCommand();
}
