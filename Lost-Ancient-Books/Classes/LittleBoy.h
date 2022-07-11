#pragma once
#include "GameSprite.h"
#include "GameCommand.h"
#include "GameAction.h"
#include "AnimationLoader.h"

class Weapon;
class LittleBoy;
//小男孩状态类接口(概念上)
class BoyState
{
public:
    std::string _name;
    virtual ~BoyState() {}
    virtual BoyState* handleInput(LittleBoy* boy, GameCommand* command);
    virtual void enterState(LittleBoy* boy) {};
    virtual void outState(LittleBoy* boy);
    virtual BoyState* update(LittleBoy* boy, float dt);
};

class GroundState :public BoyState
{
public:
    virtual ~GroundState() {}
    virtual BoyState* handleInput(LittleBoy* boy, GameCommand* command)override;
    virtual void enterState(LittleBoy* boy)override;
    virtual BoyState* update(LittleBoy* boy, float dt)override;
private:
    float _nowTime = 0;
    float _time = 0.2f;
};
//站立状态(完成)
class StandState :public GroundState
{
    const float _autoJumpTime;
    float _nowJumpTime;
public:
    StandState();
    virtual ~StandState() {}
    virtual void enterState(LittleBoy* boy);
    virtual BoyState* handleInput(LittleBoy* boy, GameCommand* command) override;
    virtual BoyState* update(LittleBoy* boy, float dt)override;
};

//跑步状态(完成)
class RunState :public GroundState
{
public:
    RunState();
    virtual ~RunState() {}
    virtual void enterState(LittleBoy* boy)override;
    virtual BoyState* handleInput(LittleBoy* boy, GameCommand* command) override;
    virtual BoyState* update(LittleBoy* boy, float dt)override;
};

//跳跃起来在空中的状态
class JumpState :public BoyState
{
public:
    JumpState();
    virtual ~JumpState() {}
    virtual void enterState(LittleBoy* boy)override;
    virtual BoyState* handleInput(LittleBoy* boy, GameCommand* command) override;
    virtual BoyState* update(LittleBoy* boy, float dt);
};

//使用技能的状态
class SkillState :public BoyState
{
public:
    SkillState();
    virtual ~SkillState() {}
    virtual BoyState* handleInput(LittleBoy* boy, GameCommand* command);
    virtual void enterState(LittleBoy* boy);
    virtual BoyState* update(LittleBoy* boy, float dt);
    virtual void outState(LittleBoy* boy);
};

//攻击状态
class AttackState :public BoyState
{
public:
    AttackState();
    virtual ~AttackState() {}
    virtual BoyState* handleInput(LittleBoy* boy, GameCommand* command);
    virtual void enterState(LittleBoy* boy);
    virtual void outState(LittleBoy* boy);
    virtual BoyState* update(LittleBoy* boy, float dt);
};

//死亡状态
class BoyDeadState :public BoyState
{
public:
    virtual ~BoyDeadState() {}
    virtual BoyState* handleInput(LittleBoy* boy, GameCommand* command) { return nullptr; };
    virtual BoyState* update(LittleBoy* boy, float dt);
    virtual void enterState(LittleBoy* boy);
};

//硬直状态
class HardState :public BoyState
{
public:
    HardState();
    virtual ~HardState() {}
    virtual BoyState* handleInput(LittleBoy* boy, GameCommand* command);
    virtual void enterState(LittleBoy* boy);
    virtual void outState(LittleBoy* boy);
    virtual BoyState* update(LittleBoy* boy, float dt);
};

//主角类
class LittleBoy : public GameSprite
{
public:
    static StandState standState;
    static JumpState jumpState;
    static RunState runState;
    static AttackState attackState;
    static BoyDeadState deadState;
    static SkillState skillState;
    static HardState hardState;
    static const float speedLen;
    static const float jumpPower;
    static LittleBoy* getBoy() { return boy; }
    static void setBoy(LittleBoy* b) { boy = b; }
private:
    static LittleBoy* boy;
public:
    LittleBoy();
    ~LittleBoy() {
    };
    bool Init();
    static LittleBoy* create(cocos2d::SpriteFrame* frame);
    void handleInput(GameCommand* command);
    BoyState* getState() { return _boyState; }
    void changeState(BoyState* state);
    void pauseMove(bool p);
    void setAnimations(const BoyAnimation& an);
    void setNowAnimate(cocos2d::Animate* a) { _nowAnimate = a; }
    void setNowAnimateRepeat(cocos2d::RepeatForever* r) { _nowAnimateRepeat = r; }

    void runAnimate(cocos2d::Animate* an);
    void runAtkAnimate(cocos2d::Animate* an);
    void runSkillAnimate(cocos2d::Animate* an);
    void runHardAnimate(cocos2d::Animate* an);
    void runAnimateForever(cocos2d::Animate* an);
    void stopAnimate();
    void setBeAtkDir(bool d) { _beAtkDir = d; }
    bool getBeAtkDir() { return _beAtkDir; }
    virtual void beAttack(int h, bool isRight)override;

    cocos2d::Animate* getNowAnimate() { return _nowAnimate; }
    const BoyAnimation& getAnimations() { return _animations; }
protected:
    virtual void insideUpdate(float dt) override;

private:
    BoyState* _boyState;
    MoveWithTarget* _moveAction;
    BoyAnimation _animations;
    cocos2d::Vector<cocos2d::Animation*> _animationVec;
    cocos2d::Animate* _nowAnimate;
    cocos2d::RepeatForever* _nowAnimateRepeat;
    bool _beAtkDir;

};


