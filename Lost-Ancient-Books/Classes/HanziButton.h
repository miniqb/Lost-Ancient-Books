#pragma once
#include "ui/CocosGUI.h"
class HanziButton :public cocos2d::ui::Button
{
public:
	static HanziButton* create(
		const std::string& normalImage,
		const std::string& hanzi = "",
		const std::string& fontName = "",
		int fontSize = 80,
		const std::string& selectedImage = "",
		const std::string& disableImage = "",
		TextureResType texType = TextureResType::PLIST);

	bool initHanziButton(const std::string& hanzi, const std::string& fontName ,int fontSize = 80);

	HanziButton();
	~HanziButton();
	const std::string& getHanzi();

	void setQuestionIndex(int i) { _questionIndex = i; }
	int getQuestionIndex() { return _questionIndex; }

	enum class Mode
	{
		INQ,
		OUTQ
	};
	void changeMode(Mode mode) { _mode = mode; }
	Mode getMode() { return _mode; }
	void setOriPos(const cocos2d::Vec2& p) { _oriPos = p; }
	const cocos2d::Vec2& getOriPos() { return _oriPos; }
	void resetPosition();

	cocos2d::Label* getLabel() { return _label; }

	//override
	virtual void setPosition(const cocos2d::Vec2& pos) override;

public:
	CREATE_FUNC(HanziButton)
private:
	Mode _mode;
	int _questionIndex;
	cocos2d::Label* _label;
	cocos2d::Vec2 _oriPos;
};

