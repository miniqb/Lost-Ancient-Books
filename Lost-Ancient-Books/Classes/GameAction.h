#pragma once
#include "GameSprite.h"
/// <summary>
/// MoveToNode
/// </summary>
class MoveToNode :public cocos2d::Action
{
public:
    static MoveToNode* create(cocos2d::Node* aim, float speedLen, bool autoFinish = false, float stopLen = 0.f, bool autoFlix=true, bool isFly = false);
    bool initAction(cocos2d::Node* aim, float speedLen, bool autoFinish = false, float stopLen = 0.f, bool autoFlix = true, bool isFly = false);
    //
    // Overrides
    //
    virtual void step(float dt) override;
    virtual MoveToNode* clone() const override;
    virtual MoveToNode* reverse() const  override;
    virtual bool isDone() const;
    virtual void startWithTarget(cocos2d::Node* target);

CC_CONSTRUCTOR_ACCESS:
    MoveToNode()
        : _isFly(false)
        , _autoFinish(false)
        , _autoFlix(true)
        , _aim(nullptr)
        , _gameTarget(nullptr)
        , _isDone(false)
        , _stopLen(0.f)
        , _speedLen(0.f){}
    virtual ~MoveToNode();

protected:
    bool _isFly;
    bool _autoFinish;
    bool _autoFlix;
    bool _isDone;
    float _stopLen;
    float _speedLen;
    cocos2d::Node* _aim;
    GameSprite* _gameTarget;

private:
    CC_DISALLOW_COPY_AND_ASSIGN(MoveToNode)
};

/// <summary>
/// MoveByWithWall
/// </summary>
class MoveByWithWall : public cocos2d::ActionInterval
{
public:
    static MoveByWithWall* create(float duration, const cocos2d::Vec2& deltaPosition, bool autoFlix=true, bool air=true);

    //
    // Overrides
    //
    virtual MoveByWithWall* clone() const override;
    virtual MoveByWithWall* reverse() const  override;
    virtual void startWithTarget(cocos2d::Node* target) override;
    /**
     * @param time in seconds
     */
    virtual void update(float time) override;

CC_CONSTRUCTOR_ACCESS:
    MoveByWithWall(){}
    virtual ~MoveByWithWall() {}


    /** initializes the action */

    bool initWithDuration(float duration, const cocos2d::Vec2& deltaPosition, bool autoFlix, bool air = true);
protected:
    cocos2d::Vec2 _positionDelta;
    cocos2d::Vec2 _startPosition;
    cocos2d::Vec2 _previousPosition;
    GameSprite* _gameTarget;
    bool _goAir;
    bool _autoFlix;
private:
    CC_DISALLOW_COPY_AND_ASSIGN(MoveByWithWall)
};

/// <summary>
/// GameMoveTo
/// </summary>
class MoveToWithWall : public MoveByWithWall
{
public:
    /**
     * Creates the action.
     * @param duration Duration time, in seconds.
     * @param position The destination position in 2d.
     * @return An autoreleased MoveTo object.
     */
    static MoveToWithWall* create(float duration, const cocos2d::Vec2& position);

    //
    // Overrides
    //
    virtual MoveToWithWall* clone() const override;
    virtual MoveToWithWall* reverse() const  override;
    virtual void startWithTarget(cocos2d::Node* target) override;

CC_CONSTRUCTOR_ACCESS:
    MoveToWithWall() {}
    virtual ~MoveToWithWall() {}

    /**
     * initializes the action
     * @param duration in seconds
     */
    bool initWithDuration(float duration, const cocos2d::Vec2& position);

protected:
    cocos2d::Vec2 _endPosition;

private:
    CC_DISALLOW_COPY_AND_ASSIGN(MoveToWithWall)
};

/// <summary>
/// MoveWithTarget
/// </summary>
class MoveWithTarget :public cocos2d::Action
{
public:
    static MoveWithTarget* create(bool jump=false);
    bool initAction(bool jump=false);
    //
    // Overrides
    //
    virtual void step(float dt) override;
    virtual MoveWithTarget* clone() const override;
    virtual MoveWithTarget* reverse() const  override;
    virtual bool isDone() const;
    void setDone(bool d) { _isDone = d; }
    virtual void startWithTarget(cocos2d::Node* target);
    void pause(bool p) { _pause = p; }
CC_CONSTRUCTOR_ACCESS:
    MoveWithTarget():_isDone(false),_pause(false){}
    virtual ~MoveWithTarget() {}

protected:
    bool _isDone;
    bool _jump;
    bool _pause;
    GameSprite* _gameTarget;
private:
    CC_DISALLOW_COPY_AND_ASSIGN(MoveWithTarget)
};


class FllowNode :public cocos2d::Action
{
public:
    static FllowNode* create(cocos2d::Node* aim, const cocos2d::Vec2& offect = cocos2d::Vec2::ZERO, bool anc = false);
    bool initAction(cocos2d::Node* aim, const cocos2d::Vec2& offect, bool anc);

    //
    // Override
    //
    virtual FllowNode* clone() const override;
    virtual FllowNode* reverse() const override;
    /**
     * @param dt in seconds.
     * @js NA
     */
    virtual void step(float dt) override;
    virtual bool isDone() const override;
    virtual void stop() override;


CC_CONSTRUCTOR_ACCESS:
    FllowNode():Action(),_aim(nullptr),_anc(false) {}
    virtual ~FllowNode();

protected:
    cocos2d::Node* _aim;
    cocos2d::Vec2 _offect;
    bool _anc;
private:
    CC_DISALLOW_COPY_AND_ASSIGN(FllowNode)
};