#pragma once
class GlobalWorld
{
public:
	const static float gravity;
	const static float scale;
	const static float pxRate;
	//static bool keys[300];

};

enum SpriteTag
{
	T_BOY = 0x01,
	T_MONSTER = 0x02,
	T_WALL = 0x04,
	T_OTHER = 0x08,
	T_PLATFORM = 0x10,
	T_SEESAW = 0x20,
	T_TRAMPOLINE = 0x40,
	T_ATKAREA_B = 0x80,
	T_ATKAREA_M = 0x100,
	T_CALLBACK_THINGS = 0x200,
	T_DROP = 0x400
};

//类别掩码（我是谁？）
enum CategoryMASK
{
	CAT_BOY = 0x01,						//0000 0001
	CAT_MONSTER = 0x02,				//0000 0010
	CAT_BIRD = 0x02,
	CAT_WALL = 0x04,					//0000 0100
	CAT_PLATFORM = 0x04,			//0000 0100
	CAT_OTHER = 0x08,					//0000 1000
	CAT_ATKAREA_B = 0x10,			//0001 0000
	CAT_ATKAREA_M = 0x20,			//0010 0000
	CAT_CALLBACK_THINGS = 0x40,	//0100 0000
	CAT_ICON = 0x40,					//0100 0000
	CAT_DROP = 0x40					//0100 0000
};

//通知掩码（可接到通知，要互相才会通知）
enum ContactMASK
{
	CON_BOY = 0x64,						//0110 0100
	CON_INV_BOY = 0x44,				//0100 0100
	CON_ATKAREA_B = 0x07,			//0000 0111
	CON_MONSTER = 0x14,				//0001 0100
	CON_BIRD = 0x10,					//0001 0000
	CON_INV_MONSTER = 0x04,		//0000 0100
	CON_ARKAREA_M = 0x07,			//0000 0111
	CON_WALL = 0x33,					//0011 0011
	CON_PLATFORM = 0x37,			//0011 0111
	CON_OTHER = 0x00,					//0000 0000
	CON_CALLBACK_THINGS = 0x01,	//0000 0001
	CON_DROP = 0x01					//0000 0001
};

//碰撞掩码（谁能撞我）
enum CollisionMASK
{
	COL_BOY = 0x0C,							//0000 1100
	COL_ATKAREA_B = 0x0,
	COL_BONDY_ATKAREA_B = 0x04,
	COL_MONSTER = 0x0C,					//0000 0000
	COL_BIRD=0x00,
	COL_ATKAREA_M = 0x0,
	COL_BONDY_ATKAREA_M = 0x04,
	COL_WALL = 0x73,						//0100 0111
	COL_PLATFORM = 0x73,				//0100 0011
	COL_OTHER = 0x0,						//0000 0101
	COL_CALLBACK_THINGS = 0x0C,		//0000 1100
	COL_DROP = 0x0C						//0000 1100
};

enum BodyGroup
{
	G_BOYS=1
};