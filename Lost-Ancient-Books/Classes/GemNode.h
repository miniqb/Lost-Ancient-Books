#pragma once
#include "cocos2d.h"
class GemNode:public cocos2d::Node
{
public:
	static GemNode* create(int shell, int piece);
	bool initGemNode(int shell, int piece);

	void setShell(int num);
	void setPiece(int num);
private:
	cocos2d::Sprite* _shell;
	cocos2d::Label* _shellLabel;
	cocos2d::Sprite* _piece;
	cocos2d::Label* _pieceLabel;
public:
	GemNode();
	~GemNode();
};

