#include "GameAction.h"
USING_NS_CC;

//
///////////////////////////////////////////  MoveToNode  ///////////////////////////////////////////////////////////////////////////////////////////
//
MoveToNode* MoveToNode::create(Node* aim, float speedLen, bool autoFinish, float stopLen, bool autoFlix, bool isFly)
{
    MoveToNode* action = new(std::nothrow) MoveToNode();
    if (action && action->initAction(aim, speedLen, autoFinish, stopLen, autoFlix, isFly))
    {
        action->autorelease();
        return action;
    }
    CC_SAFE_DELETE(action);
    return nullptr;
}

bool MoveToNode::initAction(Node* aim, float speedLen, bool autoFinish, float stopLen, bool autoFlix, bool isFly)
{
    aim->retain();
    _aim = aim;
    _speedLen = speedLen;
    _isFly = isFly;
    _autoFinish = autoFinish;
    _stopLen = stopLen;
    _autoFlix = autoFlix;
    return true;
}

void MoveToNode::step(float dt)
{
    if (_aim)
    {
        Vec2 aimPos = _aim->getPosition();
        Vec2 myPos = _gameTarget->getPosition();
        Vec2 offect = aimPos - myPos;

        if ((_isFly && offect.getLengthSq() <= _stopLen * _stopLen) ||
            (!_isFly && std::fabsf(offect.x) <= _stopLen))
        {
            if (_autoFinish)
                _isDone = true;
            return;
        }

        if (_isFly)
        {
            Vec2 spd = offect.getNormalized() * _speedLen;
            _gameTarget->addPosition(spd * dt);
        }
        else if(_gameTarget->isOnDown())
        {
            _gameTarget->addPositionWithWallAndOut((offect.x > 0 ? _speedLen : -_speedLen) * dt, 0.f);
        }
        else
        {
            _gameTarget->addPositionWithWall((offect.x > 0 ? _speedLen : -_speedLen) * dt, 0.f);
        }

        if (_autoFlix&&offect.x)
            _gameTarget->setDir(offect.x > 0);
    }
}

MoveToNode* MoveToNode::clone() const
{
    return MoveToNode::create(_aim,_speedLen,_autoFinish,_stopLen,_autoFlix, _isFly);
}

MoveToNode* MoveToNode::reverse() const
{
    return clone();
}

bool MoveToNode::isDone() const
{
    return _isDone || !(_aim->isRunning());
}

void MoveToNode::startWithTarget(Node* target)
{
    Action::startWithTarget(target);
    _gameTarget = static_cast<GameSprite*>(target);
}

MoveToNode::~MoveToNode()
{
    CC_SAFE_RELEASE(_aim);
}

//
///////////////////////////////////////////  MoveByWithWall  ///////////////////////////////////////////////////////////////////////////////////////////
//
MoveByWithWall* MoveByWithWall::create(float duration, const cocos2d::Vec2& deltaPosition, bool autoFlix, bool air)
{
    MoveByWithWall* ret = new (std::nothrow) MoveByWithWall();

    if (ret && ret->initWithDuration(duration, deltaPosition, autoFlix, air))
    {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

MoveByWithWall* MoveByWithWall::clone() const
{
    return MoveByWithWall::create(_duration, _positionDelta, _autoFlix, _goAir);
}

MoveByWithWall* MoveByWithWall::reverse() const
{
    return MoveByWithWall::create(_duration, -_positionDelta, _autoFlix, _goAir);
}

void MoveByWithWall::startWithTarget(cocos2d::Node* target)
{
    ActionInterval::startWithTarget(target);
    _gameTarget = static_cast<GameSprite*>(target);
    _previousPosition = _startPosition = _gameTarget->getPosition();
    if(_autoFlix && _positionDelta.x)
        _gameTarget->setDir(_positionDelta.x > 0);
}

void MoveByWithWall::update(float t)
{
    if (_gameTarget)
    {
        bool cantGoX = true;
        if (_goAir||!_gameTarget->isOnDown())
            cantGoX = !_gameTarget->canGoXWall(_positionDelta.x) || _positionDelta.x == 0.f;
        else
            cantGoX = !_gameTarget->canGoX(_positionDelta.x) || _positionDelta.x == 0.f;
        bool cantGoY = !_gameTarget->canGoY(_positionDelta.y) || _positionDelta.y == 0.f;
        if (cantGoX && cantGoY)
            return;
        Vec2 currentPos = _gameTarget->getPosition();
        Vec2 diff = currentPos - _previousPosition;
        _startPosition = _startPosition + diff;
        Vec2 newPos = _startPosition + (_positionDelta * t);
        if (cantGoX)
            newPos.x = currentPos.x;
        if (cantGoY)
            newPos.y = currentPos.y;
        _gameTarget->setPosition(newPos);
        _previousPosition = newPos;
    }
}

bool MoveByWithWall::initWithDuration(float duration, const cocos2d::Vec2& deltaPosition, bool autoFlix, bool air)
{
    bool ret = false;

    if (ActionInterval::initWithDuration(duration))
    {
        _positionDelta = deltaPosition;
        _goAir = air;
        _autoFlix = autoFlix;
        ret = true;
    }

    return ret;
}

//
///////////////////////////////////////////  MoveToWithWall  ///////////////////////////////////////////////////////////////////////////////////////////
//
MoveToWithWall* MoveToWithWall::create(float duration, const Vec2& position)
{
    MoveToWithWall* ret = new (std::nothrow) MoveToWithWall();

    if (ret && ret->initWithDuration(duration, position))
    {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool MoveToWithWall::initWithDuration(float duration, const cocos2d::Vec2& position)
{
    bool ret = false;

    if (ActionInterval::initWithDuration(duration))
    {
        _endPosition = position;
        ret = true;
    }

    return ret;
}

MoveToWithWall* MoveToWithWall::clone() const
{
    return MoveToWithWall::create(_duration, _endPosition);
}

MoveToWithWall* MoveToWithWall::reverse() const
{
    CCASSERT(false, "reverse() not supported in MoveToWithWall");
    return nullptr;
}

void MoveToWithWall::startWithTarget(Node* target)
{
    MoveByWithWall::startWithTarget(target);
    _positionDelta = _endPosition - target->getPosition();
}

MoveWithTarget* MoveWithTarget::create(bool jump)
{
    MoveWithTarget* action = new(std::nothrow) MoveWithTarget();
    if (action && action->initAction(jump))
    {
        action->autorelease();
        return action;
    }
    CC_SAFE_DELETE(action);
    return nullptr;
}

bool MoveWithTarget::initAction(bool jump)
{
    _jump = jump;
    return true;
}

void MoveWithTarget::step(float dt)
{
    if (_pause)
        return;
    Vec2 speed = _gameTarget->getSpeed();
    if (_jump)
        _gameTarget->addPositionWithWall(speed * dt);
    else
        _gameTarget->addPositionWithWallAndOut(speed * dt);
}

MoveWithTarget* MoveWithTarget::clone() const
{
    return MoveWithTarget::create();
}

MoveWithTarget* MoveWithTarget::reverse() const
{
    return MoveWithTarget::create();
}

bool MoveWithTarget::isDone() const
{
    return _isDone;
}

void MoveWithTarget::startWithTarget(cocos2d::Node* target)
{
    Action::startWithTarget(target);
    _gameTarget = static_cast<GameSprite*>(target);
}

FllowNode* FllowNode::create(cocos2d::Node* aim, const cocos2d::Vec2& offect, bool anc)
{
    FllowNode* action = new(std::nothrow) FllowNode();
    if (action && action->initAction(aim,offect,anc))
    {
        action->autorelease();
        return action;
    }
    CC_SAFE_DELETE(action);
    return nullptr;
}

bool FllowNode::initAction(cocos2d::Node* aim, const cocos2d::Vec2& offect, bool anc)
{
    CCASSERT(aim != nullptr, "FollowedNode can't be NULL");
    if (aim == nullptr)
    {
        CCLOG("Follow::initWithTarget error: followedNode is nullptr!");
        return false;
    }
    aim->retain();
    _aim = aim;
    _offect = offect;
    _anc = anc;
    return true;
}

FllowNode* FllowNode::clone() const
{
    return create(_aim, _offect);
}

FllowNode* FllowNode::reverse() const
{
    return clone();
}

void FllowNode::step(float dt)
{
    if (_anc)
    {
        Vec2 pos = _aim->convertToWorldSpace(_offect);
        pos = _aim->getParent()->convertToNodeSpace(pos);
        _target->setPosition(pos);
    }
    else
    {
        Vec2 aimPos = _aim->getPosition();

        //if(_aim->getParent())
        //    aimPos = _aim->getParent()->convertToWorldSpaceAR(aimPos);
        //if (_target->getParent())
        //    aimPos = _target->getParent()->convertToNodeSpaceAR(aimPos);
        _target->setPosition(aimPos + _offect);
    }
}

bool FllowNode::isDone() const
{
    return (!_aim->isRunning());
}

void FllowNode::stop()
{
    Action::stop();
}

FllowNode::~FllowNode()
{
    CC_SAFE_RELEASE(_aim);
}
