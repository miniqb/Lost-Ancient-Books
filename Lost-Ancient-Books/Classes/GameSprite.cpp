#include "GameSprite.h"
#include <GlobalWorld.h>
USING_NS_CC;

void GameSprite::setSkillPower(SkillPower* sp)
{
    if (sp)
    {
        if (_skillPower != nullptr)
        {
            removeComponent(_skillPower);
        }
        this->addComponent(sp);
        _skillPower = sp;
    }
}

void GameSprite::removeSkillPower()
{
    if (_skillPower)
    {
        removeComponent(_skillPower);
        _skillPower = nullptr;
    }
}

int GameSprite::getSkillPowerKind()
{
    if(_skillPower==nullptr)
        return 0;
    return _skillPower->getKind();
}

void GameSprite::useSkillPower()
{
    if (_skillPower)
    {
        _skillPower->useSkillPower();
    }
}

void GameSprite::Jump()
{
    this->jump(_jumpPower);
}

void GameSprite::jump(float spd)
{
    this->addPosition(0, 2.f);
    this->addPhysicsSpeedYPlus(spd);
}

void GameSprite::setSpeed(const cocos2d::Vec2& sp)
{
    if (sp != _speed)
    {
        _speed = sp;
    }
}

void GameSprite::setSpeed(float x, float y)
{
    if (x != _speed.x || y != _speed.y)
    {
        _speed.set(x, y);
    }
}

void GameSprite::fallToGround()
{
    _onDown = true; 
    resetResJumpTimes();
}

cocos2d::Vec2 GameSprite::getGroundDir()
{
    Vec2 dir = this->getGroundDirPrep().getRPerp();
    if (_dir != dir.x > 0)
        dir = -dir;
    return dir;
}

cocos2d::Vec2 GameSprite::getSpeedDir()
{
    Vec2 res = getGroundDir() * getSpeedLen();
    return res;
}

void GameSprite::setWeapon(Weapon* w)
{
    if (_weapon)
        this->removeComponent(_weapon);
    _weapon = w;
    this->addComponent(w);
}
void GameSprite::setPhysicsSpeedX(float x)
{
    cocos2d::Vec2 spd = _physicsBody->getVelocity();
    spd.x = x;
    _physicsBody->setVelocity(spd);
}

void GameSprite::setPhysicsSpeedY(float y)
{
    cocos2d::Vec2 spd = _physicsBody->getVelocity();
    spd.y = y;
    _physicsBody->setVelocity(spd);
}

void GameSprite::addPhysicsSpeedY(float y)
{
    cocos2d::Vec2 spd = _physicsBody->getVelocity();
    spd.y += y;
    _physicsBody->setVelocity(spd);
}

void GameSprite::addPhysicsSpeedYPlus(float y)
{
    cocos2d::Vec2 spd = _physicsBody->getVelocity();
    if (spd.y < 0 && y>0 || spd.y > 0 && y < 0)
        spd.y = 0;
    spd.y = std::max(y, spd.y);
    _physicsBody->setVelocity(spd);
}

void GameSprite::update(float dt)
{
    Sprite::update(dt);
    insideUpdate(dt);
    reSetCol();
    _blockSucceed = 0;
}

void GameSprite::reSetCol()
{
    _onTop = false;
    _onDown = false;
    _onLeft = false;
    _onRight = false;
    countOut();
    //_standWall = nullptr;
}

void GameSprite::countOut()
{
    if (_standWall)
    {
        Rect sbox = _standWall->getBoundingBox();
        float hwidth = 0;
        auto shape = _physicsBody->getFirstShape();
        if (shape->getType() == PhysicsShape::Type::BOX)
        {
            hwidth = static_cast<PhysicsShapeBox*>(shape)->getSize().width*0.5f;
        }
        else if(shape->getType() == PhysicsShape::Type::CIRCLE)
        {
            hwidth = static_cast<PhysicsShapeCircle*>(shape)->getRadius();
        }
        hwidth *= getScale();

        float offectx = _physicsBody->getPositionOffset().x;
        float minx = getPositionX() - hwidth - offectx;
        float maxx = getPositionX() + hwidth + offectx;
        _outLeft = minx <= sbox.getMinX();
        _outRight = maxx >= sbox.getMaxX();
    }
    else
    {
        _outLeft = false;
        _outRight = false;
    }

}

void GameSprite::finishAtk()
{
    _isAtk = false;
}

void GameSprite::addPositionWithWallAndOut(float x, float y)
{
    if (x != 0.f)
    {
        if ((_onLeft || _outLeft) && x < 0 || (_onRight || _outRight) && x>0)
            x = 0;
    }
    if (y != 0.f)
    {
        if (_onDown && y < 0 || _onTop && y > 0)
            y = 0;
    }
    if (x != 0.f || y != 0.f)
    {
        this->addPosition(x, y);
    }
}

void GameSprite::addPositionWithWallAndOut(const cocos2d::Vec2& pos)
{
    addPositionWithWallAndOut(pos.x, pos.y);
}

void GameSprite::addPositionWithWall(float x, float y)
{
    if (x != 0.f)
    {
        if (_onLeft && x < 0 || _onRight && x>0)
            x = 0;
    }
    if (y != 0.f)
    {
        if (_onDown && y < 0 || _onTop && y > 0)
            y = 0;
    }
    if (x != 0.f || y != 0.f)
    {
        this->addPosition(x, y);
    }
}

void GameSprite::addPositionWithWall(const cocos2d::Vec2& pos)
{
    addPositionWithWall(pos.x, pos.y);
}

bool GameSprite::canGoXWall(float x)
{
    if (_onLeft && x < 0 || _onRight && x>0)
        return false;
    else
        return true;
}

bool GameSprite::canGoX(float x)
{
    if ((_onLeft || _outLeft) && x < 0 || (_onRight || _outRight) && x>0)
        return false;
    else
        return true;
}

bool GameSprite::canGoY(float y)
{
    if (_onDown && y < 0 || _onTop && y > 0)
        return false;
    else
        return true;
}

void GameSprite::fallDownPlatform()
{
    if (_onDown && _standWall && _standWall->getTag() == T_PLATFORM)
    {
        int newMask = _physicsBody->getCollisionBitmask();
        newMask &= ~_standWall->getPhysicsBody()->getCategoryBitmask();
        _physicsBody->setCollisionBitmask(newMask);
        //boy->setPhysicsSpeedY(-GlobalWorld::gravity * 0.1f);
        this->addPosition(0, 2);
        this->scheduleOnce(CC_SCHEDULE_SELECTOR(GameSprite::schedulePlatForm), 0.2f);
    }
}

void GameSprite::schedulePlatForm(float)
{
    _physicsBody->setCollisionBitmask(COL_BOY);
}

void GameSprite::beAttack(int h, bool isRight)
{
    if (!_isBlock)
    {
        this->getHurt(h);
        if (_life <= 0)
        {
            this->setSpeed(0, 0);
            if (_physicsBody)
            {
                _physicsBody->setDynamic(true);
                _physicsBody->applyImpulse(Vec2(isRight ? 200000 : -200000, 220000));
            }
        }
    }
    else
    {
        //CCLOG("Block!");
        ++_blockSucceed;
    }
}

void GameSprite::addLife(int l)
{
    _life += l;
    if (_life > _allLife)
        _life = _allLife;
    if (_life < 0)
        _life = 0;
}

GameSprite::GameSprite() :
    _allLife(0),
    _speedLen(0),
    _speed(),
    _jumpPower(0),
    _onDown(0),
    _onRight(0),
    _onLeft(0),
    _onTop(false),
    _jumpTimes(2),
    _standWall(nullptr),
    _groundDir(),
    _isAtk(false),
    _weapon(nullptr),
    _outLeft(false),
    _outRight(false),
    _resJumpTimes(0),
    _dir(true),
    _life(0),
    _skillPower(nullptr),
    _isBlock(false),
    _blockSucceed(0),
    _isHard(false),
    _isSkill(false)
{
}

GameSprite::~GameSprite()
{
}

void GameSprite::addPosition(const cocos2d::Vec2& pos)
{
    if (pos.x == 0.f && pos.y == 0.f)
        return;

    _position.x += pos.x;
    _position.y += pos.y;

    _transformUpdated = _transformDirty = _inverseDirty = true;
    _usingNormalizedPosition = false;
}

void GameSprite::addPosition(float x, float y)
{
    if (x == 0 && y == 0)
        return;

    _position.x += x;
    _position.y += y;

    _transformUpdated = _transformDirty = _inverseDirty = true;
    _usingNormalizedPosition = false;
}
