#pragma once
#include "cocos2d.h"
#include "GlobalWorld.h"
class CollisionHelper
{
public:
	CollisionHelper();
	~CollisionHelper() {};
	enum class  ColDir
	{
		NONE = 0,
		LEFT = 0x01,
		RIGHT = 0x02,
		TOP = 0x04,
		DOWN = 0x08,
		LEFT_TOP = 0x10,
		LEFT_DOWN = 0x20,
		RIGHT_TOP = 0x40,
		RIGHT_DOWN = 0x80
	};
	struct DirNormal
	{
		ColDir _dir;
		cocos2d::Vec2 _normal;
		DirNormal() :_dir(ColDir::NONE), _normal() {}
	};
	/// <summary>
	/// 
	/// </summary>
	/// <param name="contact">��ײ����</param>
	/// <param name="aim">Ҫ���������Ŀ��</param>
	/// <param name="other">��������Ŀ��</param>
	/// <param name="who">��ײ����˭���ϣ��ò���Ҫ��aim��other�е�����һ��������������</param>
	/// <returns></returns>
	DirNormal getColDir(const cocos2d::PhysicsContactData* contact, cocos2d::Node* aim, cocos2d::Node* other, cocos2d::Node* who);

	cocos2d::Vec2 getDirAccept(const cocos2d::PhysicsContactData* contact, cocos2d::Node* aim);

	struct NodeAB
	{
		cocos2d::Node* a, *b;
		NodeAB() :a(nullptr), b(nullptr) {}
	};
	cocos2d::Node* getNodeWithTag(cocos2d::Node* a, cocos2d::Node* b, SpriteTag tag);
	void getNodeABWithTag(cocos2d::Node*& a, cocos2d::Node*& b, SpriteTag tag);
	void setMin(float min) { _min = min; }
private:
	float _min;
};