#pragma once
#include "CoolingComponent.h"
#include "AtkArea.h"
#include "GameSprite.h"

class GameSprite;
class SkillPower :public CoolingComponent
{
public:
	static SkillPower* create();

	virtual void useSkillPower();
	virtual void setOwner(cocos2d::Node* owner)override;

	void setAnimation(cocos2d::Animation* animation) { _animation = animation; }
	cocos2d::Animation* getAnimation() { return _animation; }

	int getKind() { return _kind; }
	const std::string& getInfo() { return _skillInfo; }
	const std::string& getSkillName() { return _skillName; }
	void setInfo(const std::string& str) { _skillInfo = str; }
	void setSkillName(const std::string& str) { _skillName = str; }
	void setPrice(int p) { _price = p; }
	int getPrice() { return _price; }
	void setCellInfo(const std::string& name, const std::string& info, int price);
protected:
	//overrides
	virtual void insideUseSkill() {/*override me*/ };
public:
	SkillPower();
	~SkillPower();
protected:
	GameSprite* _gameOwner;
	cocos2d::Animation* _animation;
	int _kind;

	std::string _skillName;
	std::string _skillInfo;
	int _price;
};

/*RushSP 冲刺技能*/
class RushSP : public SkillPower
{
public:
	static RushSP* create();

	void setRushLen(float l) { _rushLen = l; }
	float getRushLen() { return _rushLen; }

	void setRushTime(float l) { _rushTime = l; }
	float getRushTime() { return _rushTime; }
protected:
	//overrides
	virtual void insideUseSkill()override;
private:
	float _rushLen;
	float _rushTime;
public:
	RushSP();
};

/*BoomSP 爆炸技能*/
class BoomSP :public SkillPower
{
public:
	static BoomSP* create();

	void setBoomR(float r) { _boomR = r; }
	float getRoomR() { return _boomR; }
protected:
	//overrides
	virtual void insideUseSkill()override;
public:
	BoomSP();
private:
	float _boomR;
};

class BlockSP :public SkillPower
{
public:
	static BlockSP* create();

	void initBlokSP(float justTime, float proTime, float reduce);
protected:
	cocos2d::PhysicsBody* getBody();
	//overrides
	virtual void insideUseSkill()override;
	virtual void insideUpdate(float delta)override;
private:
	float _justTime;
	float _proTime;
	float _reduceHarm;
public:
	BlockSP();
};
