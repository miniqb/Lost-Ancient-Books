#include "CollisionHelper.h"
USING_NS_CC;
#define ABS(x) (x>0?x:-x)
#define SAMESY(x,y) (x>0&&y>0||x<0&&y<0)
CollisionHelper::CollisionHelper():
	_min(0.01f)
{
}

Vec2 CollisionHelper::getDirAccept(const cocos2d::PhysicsContactData* contact, cocos2d::Node* aim)
{
	auto points = contact->points;
	Vec2 normal = contact->normal;
	
	//Size size = aim->getContentSize();
	Vec2 centerPos = aim->getPhysicsBody()->getFirstShape()->getCenter();
	Size size = aim->getBoundingBox().size;
	//centerPos += size * 0.5f;
	centerPos = aim->convertToWorldSpaceAR(centerPos);
	Vec2 res=normal;
	Vec2 aimPos;
	for (size_t i = 0; i < contact->count; i++)
		aimPos += points[i];
	aimPos = aimPos / contact->count;
	
	Vec2 ppos = aim->convertToNodeSpace(points[0]);
	
	//法向量垂直
	if (ABS(normal.x) <= _min)
	{
		//反了
		if (aimPos.y > centerPos.y && normal.y > 0 ||//接触点在上面而方向向量向上
			aimPos.y < centerPos.y && normal.y < 0)//接触点在下面而方向向量向下
		{
			res = -normal;
		}
	}
	//法向量水平
	else if (ABS(normal.y) <= _min)
	{

		if (aimPos.x > centerPos.x && normal.x > 0 ||
			aimPos.x < centerPos.x && normal.x < 0)
		{
			res = -normal;
		}
	}
	//碰到了某个角
	else
	{
		float a = size.height / size.width;
		if (SAMESY(normal.x, normal.y))
		{
			a = -a;
		}
		Vec2 adjPos = aimPos - centerPos;
		float midy = a * adjPos.x;
		if (midy > adjPos.y && normal.y < 0 ||
			midy < adjPos.y && normal.y>0)
		{
			res = -normal;
		}
		//if (aimPos.x > centerPos.x && normal.x > 0 ||
		//	aimPos.x < centerPos.x && normal.x < 0)
		//{
		//	res = -normal;
		//}
		//else if (aimPos.y > centerPos.y && normal.y > 0 ||
		//	aimPos.y < centerPos.y && normal.y < 0)
		//{
		//	res = -normal;
		//}
	}
	return res;

	//switch (contact->count)
	//{
	//case 1:
	//{
	//	Vec2 pos = aim->convertToNodeSpace(points[0]);
	//	//法向量垂直
	//	if (ABS(normal.x) <= _min)
	//	{
	//		//反了
	//		if (pos.y > centerPos.y&&normal.y>0||//接触点在上面而方向向量向上
	//			pos.y<centerPos.y&&normal.y<0)//接触点在下面而方向向量向下
	//		{
	//			res = -normal;
	//		}
	//	}
	//	//法向量水平
	//	else if (ABS(normal.y) <= _min)
	//	{
	//		if (pos.x > centerPos.x && normal.x > 0 ||
	//			pos.x < centerPos.x && normal.x < 0)
	//		{
	//			res = -normal;
	//		}
	//	}
	//	//碰到了某个角
	//	else
	//	{
	//		if (SAMESY(normal.x, normal.y))
	//		{

	//		}
	//		if (pos.x > centerPos.x && normal.x > 0 ||
	//			pos.x < centerPos.x && normal.x < 0)
	//		{
	//			res = -normal;
	//		}
	//		////右边
	//		//if (pos.x > centerPos.x)
	//		//{
	//		//	//上边
	//		//	if (pos.y > centerPos.y)
	//		//	{
	//		//		res.x = -ABS(normal.x);
	//		//		res.y = -ABS(normal.y);
	//		//	}
	//		//	//下边
	//		//	else
	//		//	{
	//		//		res.x = -ABS(normal.x);
	//		//		res.y = ABS(normal.y);
	//		//	}
	//		//}
	//		////左边
	//		//else
	//		//{
	//		//	//上边
	//		//	if (pos.y > centerPos.y)
	//		//	{
	//		//		res.x = ABS(normal.x);
	//		//		res.y = -ABS(normal.y);
	//		//	}
	//		//	//下边
	//		//	else
	//		//	{
	//		//		res.x = ABS(normal.x);
	//		//		res.y = ABS(normal.y);
	//		//	}
	//		//}
	//	}
	//}
	//break;
	//case 2:
	//{
	//	Vec2 pos1 = aim->convertToNodeSpace(points[0]);
	//	Vec2 pos2 = aim->convertToNodeSpace(points[1]);
	//	//法向量垂直
	//	if (ABS(normal.x) <= _min)
	//	{
	//		if (pos1.y > centerPos.y && normal.y > 0 || 
	//			pos1.y < centerPos.y && normal.y < 0)
	//		{
	//			res = -normal;
	//		}
	//	}
	//	//法向量水平
	//	else if (ABS(normal.y) <= _min)
	//	{
	//		if (pos1.x > centerPos.x && normal.x>0||
	//			pos1.x < centerPos.x && normal.x < 0)
	//		{
	//			res = -normal;
	//		}
	//	}
	//	//碰到了某个角
	//	else
	//	{
	//		float ceny = (pos1.y + pos2.y) * 0.5f;
	//		if (SAMESY(normal.x, normal.y))
	//		{
	//			if (ceny > centerPos.y)
	//			{
	//				res.x = -ABS(normal.x);
	//				res.y = -ABS(normal.y);
	//			}
	//			else
	//			{
	//				res.x = ABS(normal.x);
	//				res.y = ABS(normal.y);
	//			}
	//		}
	//		else
	//		{
	//			if (ceny > centerPos.y)
	//			{
	//				res.x = ABS(normal.x);
	//				res.y = -ABS(normal.y);
	//			}
	//			else
	//			{
	//				res.x = -ABS(normal.x);
	//				res.y = ABS(normal.y);
	//			}
	//		}
	//	}
	//}
	//break;
	//default:
	//	break;
	//}

}

CollisionHelper::DirNormal CollisionHelper::getColDir(const cocos2d::PhysicsContactData* contact, cocos2d::Node* aim, cocos2d::Node* other, cocos2d::Node* who)
{
	DirNormal res;
	if (who == aim)
		res._normal = getDirAccept(contact, aim);
	else if (who == other)
		res._normal = -getDirAccept(contact, other);



	Vec2 normal = res._normal;

	if (ABS(normal.x) <= _min)//垂直
	{
		res._dir = normal.y > 0 ? ColDir::DOWN : ColDir::TOP;
	}
	else if (ABS(normal.y) <= _min)//水平
	{
		res._dir = normal.x > 0 ? ColDir::LEFT : ColDir::RIGHT;
	}
	else
	{
		if (normal.x > 0)
		{
			res._dir = normal.y > 0 ? ColDir::LEFT_DOWN : ColDir::LEFT_TOP;
		}
		else if (normal.x < 0)
		{
			res._dir = normal.y > 0 ? ColDir::RIGHT_DOWN : ColDir::RIGHT_TOP;
		}
	}
	return res;
	//	if (ABS(normal.x) <= _min)
	//	{
	//		if (pos.y > centerPos.y)
	//		{
	//			res._dir = ColDir::TOP;
	//			res._normal.y = -ABS(normal.y);
	//		}
	//		else
	//		{
	//			res._dir = ColDir::DOWN;
	//			res._normal.y = ABS(normal.y);
	//		}
	//	}
	//	//法向量水平
	//	else if (ABS(normal.y) <= _min)
	//	{
	//		if (pos.x > centerPos.x)
	//		{
	//			res._dir = ColDir::RIGHT;
	//			res._normal.x = -ABS(normal.x);
	//		}
	//		else
	//		{
	//			res._dir = ColDir::LEFT;
	//			res._normal.x = ABS(normal.x);
	//		}
	//	}
	//	//碰到了某个角
	//	else
	//	{
	//		//右边
	//		if (pos.x > centerPos.x)
	//		{
	//			//上边
	//			if (pos.y > centerPos.y)
	//			{
	//				res._dir = ColDir::RIGHT_TOP;
	//				res._normal.x = -ABS(normal.x);
	//				res._normal.y = -ABS(normal.y);
	//			}
	//			//下边
	//			else
	//			{
	//				res._dir = ColDir::RIGHT_DOWN;
	//				res._normal.x = -ABS(normal.x);
	//				res._normal.y = ABS(normal.y);
	//			}
	//		}
	//		//左边
	//		else
	//		{
	//			//上边
	//			if (pos.y > centerPos.y)
	//			{
	//				res._dir = ColDir::LEFT_TOP;
	//				res._normal.x = ABS(normal.x);
	//				res._normal.y = -ABS(normal.y);
	//			}
	//			//下边
	//			else
	//			{
	//				res._dir = ColDir::LEFT_DOWN;
	//				res._normal.x = ABS(normal.x);
	//				res._normal.y = ABS(normal.y);
	//			}
	//		}
	//	}
	//}
	//{
	//	Vec2 pos1 = aim->convertToNodeSpace(points[0]);
	//	Vec2 pos2 = aim->convertToNodeSpace(points[1]);
	//	//法向量垂直
	//	if (ABS(normal.x) <= _min)
	//	{
	//		if (pos1.y > centerPos.y)
	//		{
	//			res._dir = ColDir::TOP;
	//			res._normal.y = -ABS(normal.y);
	//		}
	//		else
	//		{
	//			res._dir = ColDir::DOWN;
	//			res._normal.y = ABS(normal.y);
	//		}
	//	}
	//	//法向量水平
	//	else if (ABS(normal.y) <= _min)
	//	{
	//		if (pos1.x > centerPos.x)
	//		{
	//			res._dir = ColDir::RIGHT;
	//			res._normal.x = -ABS(normal.x);
	//		}
	//		else
	//		{
	//			res._dir = ColDir::LEFT;
	//			res._normal.x = ABS(normal.x);
	//		}
	//	}
	//	//碰到了某个角
	//	else
	//	{
	//		float ceny = (pos1.y + pos2.y) * 0.5f;
	//		if (SAMESY(normal.x, normal.y))
	//		{
	//			if (ceny > centerPos.y)
	//			{
	//				res._dir = ColDir::RIGHT_TOP;
	//				res._normal.x = -ABS(normal.x);
	//				res._normal.y = -ABS(normal.y);
	//			}
	//			else
	//			{
	//				res._dir = ColDir::LEFT_DOWN;
	//				res._normal.x = ABS(normal.x);
	//				res._normal.y = ABS(normal.y);
	//			}
	//		}
	//		else
	//		{
	//			if (ceny > centerPos.y)
	//			{
	//				res._dir = ColDir::LEFT_TOP;
	//				res._normal.x = ABS(normal.x);
	//				res._normal.y = -ABS(normal.y);
	//			}
	//			else
	//			{
	//				res._dir = ColDir::RIGHT_DOWN;
	//				res._normal.x = -ABS(normal.x);
	//				res._normal.y = ABS(normal.y);
	//			}
	//		}
	//	}
	//}
	//}
}

cocos2d::Node* CollisionHelper::getNodeWithTag(cocos2d::Node* a, cocos2d::Node* b, SpriteTag tag)
{
	if (a->getTag() == tag)
		return a;
	if (b->getTag() == tag)
		return b;
	return nullptr;
}

void CollisionHelper::getNodeABWithTag(cocos2d::Node*& a, cocos2d::Node*& b, SpriteTag tag)
{
	if (b->getTag() == tag)
	{
		Node* tmp = a;
		a = b;
		b = tmp;
	}
}
