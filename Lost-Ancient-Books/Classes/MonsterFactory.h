#pragma once
#include "Monster.h"
class Monster;
class MonsterFactory
{
public:
	virtual Monster* getMonster(int clonSum) { return nullptr; }
};

class CloseMonsterFactory :public MonsterFactory
{
public:
	virtual Monster* getMonster(int clonSum)override;
};

class MushroomFactory :public MonsterFactory
{
public:
	virtual Monster* getMonster(int clonSum)override;
};

class BirdFactory :public MonsterFactory
{
public:
	virtual Monster* getMonster(int clonSum)override;
};

class BottleFactory :public MonsterFactory
{
public:
	virtual Monster* getMonster(int clonSum)override;
};

class DinosaurFactory :public MonsterFactory
{
public:
	virtual Monster* getMonster(int clonSum)override;
};