#include "QuestionNode.h"
constexpr auto NANA = 0x7fffffff;
USING_NS_CC;
QuestionNode* QuestionNode::create(
	const Size& size, 
	const std::string& path, 
	const std::vector<std::string>& que,
	const std::vector<int>& nulls, 
	int fsize)
{
	QuestionNode* widget = new (std::nothrow) QuestionNode();
	if (widget && widget->init() && widget->initQuestion(size,path, que, nulls, fsize))
	{
		widget->autorelease();
		return widget;
	}
	CC_SAFE_DELETE(widget);
	return nullptr;
}


bool QuestionNode::initQuestion(
	const Size& size, 
	const std::string& path, 
	const std::vector<std::string>& que, 
	const std::vector<int>& nulls, 
	int fsize)
{
	this->_cellSize = size;
	this->_fontPath = path;
	this->_question = que;
	this->_nulls = nulls;
	this->_fontSize = fsize;
	if (_nulls.size() != 0)
	{
		_nowNullsIndex = 0;
		_hanziRabbet.assign(_nulls.size(), nullptr);
	}
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	return true;
}


void QuestionNode::onEnter()
{
	Node::onEnter();

	Size cell = _cellSize;
	cell.height = -cell.height;

	size_t length = _question.size();//列数
	Vec2 startPx = countStart(length);//该方法会改变length
	Vec2 startPxMid = startPx + cell * 0.5f;

	Node* drect = Node::create();
	//DrawNode* drect = DrawNode::create();
	drect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(drect);

	Vec2 nPos = startPx;
	Vec2 nPosMid = startPxMid;
	size_t nullsize = _nulls.size();
	size_t vecSize = _question.size();
	for (size_t i = 0, j=0; i < vecSize; i++)
	{
		Vec2 ex = getEx(i, length);
		nPos = startPx + ex;
		nPosMid = startPxMid + ex;

		if (j< nullsize && _nulls[j] == i)//为空, 画方框
		{
			++j;
			//drect->drawRect(nPos, nPos + cell, Color4F::YELLOW);
			auto u = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("u.png"));
			u->setPosition(nPosMid);
			drect->addChild(u);
		}
		else//不为空
		{
			Label* label = Label::createWithBMFont(_fontPath, _question[i],TextHAlignment::CENTER);
			label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			label->setBMFontSize(_fontSize);
			label->setPosition(nPosMid);
			this->addChild(label);
		}
	}
}

void QuestionNode::putButton(HanziButton* button)
{
	if (_nowNullsIndex == NANA || !button->getParent())
		return;

	Size cell = _cellSize;
	cell.height = -cell.height;

	size_t length = _question.size();//列数
	Vec2 startPx = countStart(length);//该方法会改变length
	Vec2 startPxMid = startPx + cell * 0.5f;

	Vec2 pos(startPxMid + getEx(_nulls[_nowNullsIndex], length));
	pos = this->convertToWorldSpaceAR(pos);
	pos = button->getParent()->convertToNodeSpaceAR(pos);

	auto move = MoveTo::create(0.1f, pos);
	move->setTag(1);
	button->stopActionsByFlags(2);
	button->runAction(move);
	button->getLabel()->runAction(move->clone());
	//button->setPosition(pos);
	button->setQuestionIndex(_nowNullsIndex);
	button->changeMode(HanziButton::Mode::INQ);
	this->_hanziRabbet[_nowNullsIndex] = button;

	while (_nowNullsIndex != NANA && _hanziRabbet[_nowNullsIndex])
	{
		++_nowNullsIndex;
		if (_nowNullsIndex >= _hanziRabbet.size())
			_nowNullsIndex = NANA;
	}
}

void QuestionNode::returnButton(HanziButton* button)
{
	int index = button->getQuestionIndex();
	CCASSERT(_hanziRabbet[index] == button, "parameter button is not same as the expected button !");

	_hanziRabbet[index] = nullptr;
	_nowNullsIndex = std::min(_nowNullsIndex, index);
	button->setQuestionIndex(NANA);
	button->changeMode(HanziButton::Mode::OUTQ);
	auto move = MoveTo::create(0.1f, button->getOriPos());
	move->setTag(2);
	button->stopActionsByFlags(1);
	button->runAction(move);
	button->getLabel()->runAction(move->clone());
	//button->resetPosition();
}

bool QuestionNode::isRight()
{
	size_t length = _hanziRabbet.size();
	for (size_t i = 0; i < length; i++)
	{
		if (_hanziRabbet[i] == nullptr ||
			_hanziRabbet[i]->getHanzi() != _question[_nulls[i]])
			return false;

		//if (_hanziRabbet[i] == nullptr ||
		//	_hanziRabbet[i]->getRightIndex() != _nulls[i])
		//	return false;
	}
	return true;
}

Vec2 QuestionNode::getStartpx(size_t length, size_t height)
{
	if (!length)
		return Vec2::ZERO;
	return -Vec2((_cellSize.width + _margin.width) * (length - 1) * 0.5f,
		-(_cellSize.height + _margin.height) * (height - 1) * 0.5f);
}

cocos2d::Vec2 QuestionNode::countStart(size_t& length)
{
	Size cell = _cellSize;
	cell.height = -cell.height;

	size_t sumY = 0;//行数
	Vec2 startPx;
	float maxW = -_maxWidth * 0.5f;

	size_t len = 0;
	do 
	{
		++sumY;
		len = size_t(length % sumY > 0) + length / sumY;
		startPx = getStartpx(len, sumY) - cell * 0.5f;
	} 	while (startPx.x <= maxW);//如果问题太长题目装不下

	length = len;
	return startPx;
}

cocos2d::Vec2 QuestionNode::getEx(size_t i, size_t length)
{
	Vec2 ex(_cellSize + _margin);
	ex.x *= i % length;
	ex.y *= i / length;
	ex.y = -ex.y;
	return ex;
}

QuestionNode::QuestionNode():
	_fontSize(80),
	_nowNullsIndex(NANA),
	_hanziRabbet(),
	_maxWidth(1600)
{
}

QuestionNode::~QuestionNode()
{
}
