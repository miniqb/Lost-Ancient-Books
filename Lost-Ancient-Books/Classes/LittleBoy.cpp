#include "LittleBoy.h"
#include "GlobalWorld.h"
#include "GameConfig.h"
#include "Package.h"
#include "TempScene.h"
#include "AudioEngine.h"
USING_NS_CC;

StandState LittleBoy::standState;
JumpState LittleBoy::jumpState;
RunState LittleBoy::runState;
AttackState LittleBoy::attackState;
BoyDeadState LittleBoy::deadState;
SkillState LittleBoy::skillState;
HardState LittleBoy::hardState;

LittleBoy* LittleBoy::boy = nullptr;
const float LittleBoy::speedLen = 400.f;
const float LittleBoy::jumpPower = 220.f;

BoyState* BoyState::handleInput(LittleBoy* boy, GameCommand* command)
{
    GameCommand::Command name = command->getCommandName();
    if ((int)name &
        ((int)GameCommand::Command::ATTACK |
            (int)GameCommand::Command::ATTACK_RUSH))
    {
        if (command->execute(boy))
            return &LittleBoy::attackState;
    }
    else if (name == GameCommand::Command::SKILL_POWER)
    {
        if (command->execute(boy))
            return &LittleBoy::skillState;
    }
    return nullptr;
}

void BoyState::outState(LittleBoy* boy)
{
    boy->stopAnimate();
}

BoyState* BoyState::update(LittleBoy* boy, float dt)
{
    if (boy->isDead())
        return &LittleBoy::deadState;
    return nullptr;
}

StandState::StandState() :
    _autoJumpTime(0.2f),
    _nowJumpTime(0.f)
{
    _name = "Stand";
}

///////////////////////////////////////////////////Õ¾Á¢×´Ì¬//////////////////////////////////////////
void StandState::enterState(LittleBoy* boy)
{
    GroundState::enterState(boy);
    boy->runAnimateForever(Animate::create(boy->getAnimations()._anStand));
    boy->setSpeed(0, 0);
}

BoyState* StandState::handleInput(LittleBoy* boy, GameCommand* command)
{
    GameCommand::Command name = command->getCommandName();
    if ((int)name &
        ((int)GameCommand::Command::MOVE_LEFT |
            (int)GameCommand::Command::MOVE_RIGHT))
    {
        command->execute(boy);
        return &LittleBoy::runState;
    }
    return GroundState::handleInput(boy, command);
}

BoyState* StandState::update(LittleBoy* boy, float dt)
{
    return GroundState::update(boy, dt);
}

///////////////////////////////////////////////////ÅÜ²½×´Ì¬//////////////////////////////////////////
RunState::RunState()
{
    _name = "Run";
}

void RunState::enterState(LittleBoy* boy)
{
    GroundState::enterState(boy);
    boy->runAnimateForever(Animate::create(boy->getAnimations()._anRun));
    boy->setSpeed(boy->getSpeedDir());
    boy->setFlippedX(!boy->getDir());
}

BoyState* RunState::handleInput(LittleBoy* boy, GameCommand* command)
{
    GameCommand::Command name = command->getCommandName();
    if ((int)name &
        ((int)GameCommand::Command::MOVE_LEFT |
            (int)GameCommand::Command::MOVE_RIGHT))
    {
        command->execute(boy);
    }
    else if ((int)name &
        ((int)GameCommand::Command::STOP_MOVE_LEFT |
            (int)GameCommand::Command::STOP_MOVE_RIGHT))
    {
        if (command->execute(boy))
            return &LittleBoy::standState;
    }
    return GroundState::handleInput(boy, command);
}

BoyState* RunState::update(LittleBoy* boy, float dt)
{
    boy->setSpeed(boy->getSpeedDir());
    BoyState* res = GroundState::update(boy, dt);
    if (res)
        boy->setSpeedY(0.f);
    return res;
}


///////////////////////////////////////////////////ÌøÔ¾×´Ì¬//////////////////////////////////////////
JumpState::JumpState()
{
    _name = "Jump";
}

void JumpState::enterState(LittleBoy* boy)
{
    boy->runAnimate(Animate::create(boy->getAnimations()._anJump));
    boy->setPhysicsSpeedX(0);
}

BoyState* JumpState::handleInput(LittleBoy* boy, GameCommand* command)
{
    GameCommand::Command name = command->getCommandName();
    if ((int)name &
        ((int)GameCommand::Command::MOVE_LEFT |
            (int)GameCommand::Command::MOVE_RIGHT |
            (int)GameCommand::Command::STOP_MOVE_LEFT |
            (int)GameCommand::Command::STOP_MOVE_RIGHT |
            (int)GameCommand::Command::JUMP))
    {
        if (name == GameCommand::Command::JUMP)
        {
            if (boy->canJump())
            {
                command->execute(boy);
                boy->reduceJumpTimes();
            }
        }
        else
            command->execute(boy);
    }
    else
        return BoyState::handleInput(boy, command);
    return nullptr;
}

BoyState* JumpState::update(LittleBoy* boy, float dt)
{
    BoyState* res = BoyState::update(boy, dt);
    if (res)
        return res;
    if (boy->isOnDown())
    {
        if (boy->getSpeedX())
        {
            res = &LittleBoy::runState;
        }
        else
        {
            res = &LittleBoy::standState;
        }
    }
    return res;
}

/********************************************************µØÃæ×´Ì¬*******************************************************************************/
BoyState* GroundState::handleInput(LittleBoy* boy, GameCommand* command)
{
    GameCommand::Command name = command->getCommandName();
    if (name == GameCommand::Command::JUMP)
    {
        command->execute(boy);
        return &LittleBoy::jumpState;
    }
    else if (name == GameCommand::Command::FALL_DOWN)
    {
        command->execute(boy);
    }
    else
        return BoyState::handleInput(boy, command);
    return nullptr;
}

void GroundState::enterState(LittleBoy* boy)
{
    _nowTime = 0;
    _time = 0.01f;
    boy->resetResJumpTimes();
}

BoyState* GroundState::update(LittleBoy* boy, float dt)
{
    auto res = BoyState::update(boy, dt);
    if (res)
        return res;
    if (!boy->isOnDown())
    {
        _nowTime += dt;
        if (_nowTime >= _time)
            return &LittleBoy::jumpState;
    }
    else
        _nowTime = 0;
    return nullptr;
}

/********************************************************¼¼ÄÜ×´Ì¬*******************************************************************************/
SkillState::SkillState()
{
    _name = "skill";
}

BoyState* SkillState::handleInput(LittleBoy* boy, GameCommand* command)
{
    GameCommand::Command name = command->getCommandName();
    if ((int)name &
        ((int)GameCommand::Command::MOVE_LEFT |
            (int)GameCommand::Command::MOVE_RIGHT |
            (int)GameCommand::Command::STOP_MOVE_LEFT |
            (int)GameCommand::Command::STOP_MOVE_RIGHT))
    {
        command->execute(boy);
    }
    return nullptr;
}

BoyState* SkillState::update(LittleBoy* boy, float dt)
{
    auto res = BoyState::update(boy, dt);
    if (res)
        return res;

    if (!boy->isSkill())
    {
        if (boy->isOnDown())
        {
            if (boy->getSpeedX())
                return &LittleBoy::runState;
            else
                return &LittleBoy::standState;
        }
        else
        {
            return &LittleBoy::jumpState;
        }
    }
    return nullptr;
}

void SkillState::enterState(LittleBoy* boy)
{
    BoyState::enterState(boy);
    boy->runSkillAnimate(Animate::create(boy->getSkillPower()->getAnimation()));
    boy->pauseMove(true);
}

void SkillState::outState(LittleBoy* boy)
{
    BoyState::outState(boy);
    boy->pauseMove(false);
}

/********************************************************¹¥»÷×´Ì¬*******************************************************************************/
AttackState::AttackState()
{
    _name = "attack";
}

BoyState* AttackState::handleInput(LittleBoy* boy, GameCommand* command)
{
    GameCommand::Command name = command->getCommandName();
    if ((int)name &
        ((int)GameCommand::Command::MOVE_LEFT |
            (int)GameCommand::Command::MOVE_RIGHT |
            (int)GameCommand::Command::STOP_MOVE_LEFT |
            (int)GameCommand::Command::STOP_MOVE_RIGHT))
    {
        command->execute(boy);
    }
    else if ((int)name & (int)GameCommand::Command::SKILL_POWER)
    {
        if (command->execute(boy))
            return &LittleBoy::skillState;
    }
    return nullptr;
}

void AttackState::enterState(LittleBoy* boy)
{
    boy->runAtkAnimate(Animate::create(boy->getAnimations()._anAttackClose));
    boy->pauseMove(true);
    auto& sound = boy->getWeapon()->getSound();
    AudioEngine::play2d(sound, false);
}

void AttackState::outState(LittleBoy* boy)
{
    BoyState::outState(boy);
    boy->pauseMove(false);
    boy->setAtk(false);
}

BoyState* AttackState::update(LittleBoy* boy, float dt)
{
    auto res = BoyState::update(boy, dt);
    if (res)
        return res;

    if (!boy->isAtk())
    {
        if (boy->isOnDown())
        {
            if (boy->getSpeedX())
                return &LittleBoy::runState;
            else
                return &LittleBoy::standState;
        }
        else
        {
            return &LittleBoy::jumpState;
        }
    }
    return nullptr;
}

/********************************************************ËÀÍö×´Ì¬*******************************************************************************/
void BoyDeadState::enterState(LittleBoy* boy)
{
    boy->stopAllActions();
    Sprite* sp = Sprite::createWithSpriteFrame(boy->getAnimations()._anDead->getFrames().at(0)->getSpriteFrame());
    sp->setScale(boy->getScale());
    sp->setPosition(boy->getPosition());
    sp->setFlippedX(boy->isFlippedX());
    sp->runAction(Animate::create(boy->getAnimations()._anDead));
    boy->getParent()->addChild(sp, boy->getLocalZOrder());
    boy->setVisible(false);
    boy->runAction(Sequence::create(DelayTime::create(2), CallFunc::create([]() {
        auto pack = Package::getInstance();
        pack->onSceneEnd();
        pack->setLastSoundName("");
        AudioEngine::stopAll();
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, TempScene::create(0.5, false, true), Color3B(234, 224, 201)));
        }), nullptr));
}

BoyState* BoyDeadState::update(LittleBoy* boy, float dt)
{
    //CCLOG("dead :(%.2f)", boy->getSpeedX());
    return nullptr;
}
///////////////////////////////////////////////////////////Ó²Ö±×´Ì¬////////////////////////////////////////////////////////////////////////////////
HardState::HardState()
{
    _name = "hurt";
}

BoyState* HardState::handleInput(LittleBoy* boy, GameCommand* command)
{
    GameCommand::Command name = command->getCommandName();
    if ((int)name &
        ((int)GameCommand::Command::MOVE_LEFT |
            (int)GameCommand::Command::MOVE_RIGHT |
            (int)GameCommand::Command::STOP_MOVE_LEFT |
            (int)GameCommand::Command::STOP_MOVE_RIGHT))
    {
        command->execute(boy);
    }
    return nullptr;
}

void HardState::enterState(LittleBoy* boy)
{
    boy->runHardAnimate(Animate::create(boy->getAnimations()._anHurt));
    boy->getPhysicsBody()->setVelocity(Vec2(boy->getBeAtkDir() ? 30 : -30, 30));
    boy->pauseMove(true);
}

void HardState::outState(LittleBoy* boy)
{
    BoyState::outState(boy);
    boy->pauseMove(false);
    boy->setHard(false);
}

BoyState* HardState::update(LittleBoy* boy, float dt)
{
    auto res = BoyState::update(boy, dt);
    if (res)
        return res;

    if (!boy->isHard())
    {
        if (boy->isOnDown())
        {
            if (boy->getSpeedX())
                return &LittleBoy::runState;
            else
                return &LittleBoy::standState;
        }
        else
        {
            return &LittleBoy::jumpState;
        }
    }
    return nullptr;
}


///////////////////////////////////////////////////////////Ð¡ÄÐº¢////////////////////////////////////////////////////////////////////////////////
LittleBoy::LittleBoy() :
    GameSprite(),
    _boyState(&standState),
    _moveAction(nullptr),
    _nowAnimate(nullptr),
    _nowAnimateRepeat(nullptr),
    _beAtkDir(false),
    _animations()
{
    _speedLen = speedLen;
    _jumpPower = jumpPower;
}

LittleBoy* LittleBoy::create(SpriteFrame* frame)
{
    LittleBoy* boy = new (std::nothrow) LittleBoy();
    if (boy && boy->initWithSpriteFrame(frame))
    {
        boy->autorelease();
        boy->Init();
        return boy;
    }
    CC_SAFE_DELETE(boy);
    return nullptr;
}

void LittleBoy::handleInput(GameCommand* command)
{
    if (command && _life > 0)
    {
        BoyState* new_state = _boyState->handleInput(this, command);
        if (new_state)
        {
            this->changeState(new_state);
        }
    }
}

bool LittleBoy::Init()
{
    _moveAction = MoveWithTarget::create(true);
    this->runAction(_moveAction);
    return true;
}

void LittleBoy::setAnimations(const BoyAnimation& an)
{
    _animations = an;
    _animationVec.reserve(9);
    _animationVec.pushBack(_animations._anAttackClose);
    _animationVec.pushBack(_animations._anAttackFar);
    _animationVec.pushBack(_animations._anDead);
    _animationVec.pushBack(_animations._anHurt);
    _animationVec.pushBack(_animations._anJump);
    _animationVec.pushBack(_animations._anRun);
    _animationVec.pushBack(_animations._anRush);
    _animationVec.pushBack(_animations._anSkill);
    _animationVec.pushBack(_animations._anStand);
}

void LittleBoy::runAnimate(cocos2d::Animate* an)
{
    this->runAction(an);
    this->setNowAnimate(an);
}

void LittleBoy::runAnimateForever(cocos2d::Animate* an)
{
    auto animate = RepeatForever::create(an);
    this->runAction(animate);
    this->setNowAnimate(an);
    this->setNowAnimateRepeat(animate);
}

void LittleBoy::stopAnimate()
{
    this->stopAction(_nowAnimate);
    _nowAnimate = nullptr;
    this->stopAction(_nowAnimateRepeat);
    _nowAnimateRepeat = nullptr;
}

void LittleBoy::runAtkAnimate(cocos2d::Animate* an)
{
    this->runAction(Sequence::create(an, CallFunc::create([this]() {
        this->setAtk(false);
        }), nullptr));
    this->setNowAnimate(an);
}

void LittleBoy::runSkillAnimate(cocos2d::Animate* an)
{
    this->runAction(Sequence::create(an, CallFunc::create([this]() {
        this->setIsSkill(false);
        }), nullptr));
    this->setNowAnimate(an);
}

void LittleBoy::runHardAnimate(cocos2d::Animate* an)
{
    this->runAction(Sequence::create(an, CallFunc::create([this]() {
        this->setHard(false);
        }), nullptr));
    this->setNowAnimate(an);
}

void LittleBoy::beAttack(int h, bool isRight)
{
    if (!_isBlock)
    {
        this->getHurt(h);
        this->setFlippedX(isRight);
        if (_life <= 0)
        {
            this->setSpeed(0, 0);
        }
        else
        {
            this->setHard(true);
            this->setBeAtkDir(isRight);
            this->changeState(&hardState);
        }
    }
    else
    {
        //CCLOG("Block!");
        ++_blockSucceed;
    }
}

void LittleBoy::insideUpdate(float dt)
{
    BoyState* new_state = _boyState->update(this, dt);
    if (new_state)
    {
        this->changeState(new_state);
    }
}

void LittleBoy::changeState(BoyState* state)
{
    _boyState->outState(this);
    _boyState = state;
    _boyState->enterState(this);
}

void LittleBoy::pauseMove(bool p)
{
    _moveAction->pause(p);
}

