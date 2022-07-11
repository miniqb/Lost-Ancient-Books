#include "GameCommand.h"

USING_NS_CC;
bool MoveCommand::execute(GameSprite* sprite)
{
	sprite->setDir(_dir);
	sprite->setSpeedX(_dir?sprite->getSpeedLen():-sprite->getSpeedLen());
	return true;
}

bool StopMoveCommand::execute(GameSprite* sprite)
{
	if (_dir == sprite->getDir())
	{
		sprite->setSpeedX(0);
		return true;
	}
	return false;
}

bool JumpCommand::execute(GameSprite* sprite)
{
	sprite->Jump();
	return true;
}

const int AttackCommand::NORMAL = 0;
const int AttackCommand::RUSH = 1;

bool AttackCommand::execute(GameSprite* sprite)
{
	auto weapon = sprite->getWeapon();
	if (weapon->isCoolingOk())
	{
		if (_commandName == Command::ATTACK)
		{
			sprite->getWeapon()->attackNormal(sprite->getDir());
		}
		else if (_commandName == Command::ATTACK_RUSH)
		{
			sprite->getWeapon()->attackRush(_dir);
		}
		return true;
	}

	return false;
}

bool FallDownPlatform::execute(GameSprite* sprite)
{
	sprite->fallDownPlatform();
	return true;
}

bool SkillPowerCommand::execute(GameSprite* sprite)
{
	if (sprite->getSkillPower()&&sprite->getSkillPower()->isCoolingOk())
	{
		sprite->useSkillPower();
		return true;
	}
	return false;
}
