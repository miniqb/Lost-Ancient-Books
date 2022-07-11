#pragma once
#include "GameSprite.h"
class GameSprite;
class GameCommand
{
public:
	enum class Command
	{
		NONE = 0,
		MOVE_LEFT = 0x01,
		MOVE_RIGHT = 0x02,
		STOP_MOVE_LEFT = 0x04,
		STOP_MOVE_RIGHT = 0x08,
		JUMP = 0x10,
		ATTACK = 0x20,
		ATTACK_RUSH = 0x40,
		FALL_DOWN = 0x80,
		SKILL_POWER = 0x100
	};
protected:
	Command _commandName = Command::NONE;
public:
	Command getCommandName() { return _commandName; }
	virtual ~GameCommand() {}
	virtual bool execute(GameSprite* sprite) = 0;
};

class MoveCommand :public GameCommand
{
	bool _dir;
public:
	MoveCommand(bool d) :_dir(d) 
	{
		if (d)
			_commandName = Command::MOVE_RIGHT;
		else
			_commandName = Command::MOVE_LEFT;
	}
	virtual ~MoveCommand() {}
	virtual bool execute(GameSprite* sprite);
	void setDir(bool d) { _dir = d; }
};

class StopMoveCommand :public GameCommand
{
	bool _dir;
public:
	StopMoveCommand(bool d) :_dir(d)
	{
		if (d)
			_commandName = Command::STOP_MOVE_RIGHT;
		else
			_commandName = Command::STOP_MOVE_LEFT;
	}
	virtual ~StopMoveCommand() {}
	virtual bool execute(GameSprite* sprite);
	void setDir(bool d) { _dir = d; }

};

class JumpCommand :public GameCommand
{
public:
	JumpCommand()
	{
		_commandName = Command::JUMP;
	}
	virtual ~JumpCommand() {}
	virtual bool execute(GameSprite* sprite);
};

class AttackCommand :public GameCommand
{
public:
	static const int NORMAL;
	static const int RUSH;
	AttackCommand(int kind, bool d):
		_dir(d)
	{
		if (kind == NORMAL)
			_commandName = Command::ATTACK;
		else if (kind == RUSH)
			_commandName = Command::ATTACK_RUSH;
	}
	virtual ~AttackCommand() {}
	virtual bool execute(GameSprite* sprite);
private:
	bool _dir;
};

class FallDownPlatform:public GameCommand
{
public:
	Command getCommandName() { return _commandName; }
	FallDownPlatform()
	{
		_commandName = Command::FALL_DOWN;
	}
	virtual ~FallDownPlatform() {}
	virtual bool execute(GameSprite* sprite);
};

class SkillPowerCommand :public GameCommand
{
public:
	Command getCommandName() { return _commandName; }
	SkillPowerCommand()
	{
		_commandName = Command::SKILL_POWER;
	}
	virtual ~SkillPowerCommand() {}
	virtual bool execute(GameSprite* sprite);
};