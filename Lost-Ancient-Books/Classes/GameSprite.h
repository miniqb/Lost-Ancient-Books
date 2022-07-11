#pragma once
#include "Weapon.h"
#include "KeyController.h"
#include "GameAction.h"
#include "Batchs.h"
#include "SkillPower.h"

class SkillPower;
class Weapon;
class GameSprite :
    public cocos2d::Sprite
{
public:
    float getSpeedLen() { return _speedLen; };
    float getSpeedX() { return _speed.x; }
    const cocos2d::Vec2& getSpeed() { return _speed; };

    void setSpeedX(float x) { _speed.x = x; }
    void setSpeedY(float y) { _speed.y = y; }

    void setSpeed(const cocos2d::Vec2& sp);
    void setSpeed(float x, float y);

    void setJumpPower(float j) { _jumpPower = j; }
    float getJumpPower() { return _jumpPower; }

    bool isOnDown() { return _onDown; }
    void setOnDown(bool b) { _onDown = b; }
    void fallToGround();

    bool isOnTop() { return _onTop; }
    void setOnTop(bool b) { _onTop = b; }

    bool isOnLeft() { return _onLeft; }
    bool isOnOutLeft() { return _outLeft; }
    void setOnLeft(bool b) { _onLeft = b; }

    bool isOnRight() { return _onRight; }
    bool isOnOutRight() { return _outRight; }
    void setOnRight(bool b) { _onRight = b; }

    void setDir(bool d) { _dir = d; this->setFlippedX(!_dir); }
    bool getDir() { return _dir; }

    int getJumpTimes() { return _jumpTimes; }
    void setJumpTimes(int t) { _jumpTimes = t; }
    int getResJumpTimes() { return _resJumpTimes; }
    void setResJumpTimes(int t) { _resJumpTimes = t; }
    void reduceJumpTimes() { _resJumpTimes--; }
    void resetResJumpTimes() { _resJumpTimes = _jumpTimes; }
    bool canJump() { return _resJumpTimes > 0; }

    void setGroundDir(const cocos2d::Vec2& dir) { _groundDir.set(dir); }
    const cocos2d::Vec2& getGroundDirPrep() { return _groundDir; }
    cocos2d::Vec2 getGroundDir();
    cocos2d::Vec2 getSpeedDir();

    void setWeapon(Weapon* w);
    Weapon* getWeapon() { return _weapon; }

    void setAtk(bool a) { _isAtk = a; }
    bool isAtk() { return _isAtk; }
    void setIsSkill(bool s) { _isSkill = s; }
    bool isSkill() { return _isSkill; }
    void setHard(bool h) { _isHard = h; }
    bool isHard() { return _isHard; }

    cocos2d::Vec2 getPhysicsSpeed() { return this->_physicsBody->getVelocity(); }
    void setPhysicsSpeedX(float x);
    void setPhysicsSpeedY(float y);
    void addPhysicsSpeedY(float y);
    void addPhysicsSpeedYPlus(float y);

    void setStandWall(cocos2d::Node* wall) { _standWall = wall; }
    cocos2d::Node* getStandWall() { return _standWall; }

    void finishAtk();

    void addPositionWithWallAndOut(float x, float y);
    void addPositionWithWallAndOut(const cocos2d::Vec2& pos);
    void addPositionWithWall(float x, float y);
    void addPositionWithWall(const cocos2d::Vec2& pos);
    bool canGoXWall(float x);
    bool canGoX(float x);
    bool canGoY(float y);

    void fallDownPlatform();
    /**作为从平台下落的回调专用函数，不要调用**/
    void schedulePlatForm(float);

    virtual void beAttack(int h, bool isRight);
    bool isDead() { return _life <= 0; }
    void setLife(int l) { _life = l; }
    void addLife(int l);
    int getLife() { return _life; }
    void setAllLife(int l) { _allLife = l; }
    int getAllLife() { return _allLife; }

    void setSkillPower(SkillPower* sp);
    void removeSkillPower();
    SkillPower* getSkillPower() { return _skillPower; }
    int getSkillPowerKind();
    void useSkillPower();

    void setBlock(bool b) { _isBlock = b; };
    int getBlockSucceed() { return _blockSucceed; }

    virtual void update(float dt)override;
public:
    void Jump();
    void jump(float spd);
protected:
    virtual void insideUpdate(float dt) {};
    void getHurt(int h) { _life -= h; };
protected:
    int _life;
    int _allLife;
    //人物4个方向是否有阻挡
    bool _onTop;
    bool _onDown;
    bool _onLeft;
    bool _onRight;

    bool _outLeft;
    bool _outRight;

    float _jumpPower;
    int _jumpTimes;
    int _resJumpTimes;
    cocos2d::Vec2 _groundDir;
    cocos2d::Node* _standWall;

    cocos2d::Vec2 _speed;
    float _speedLen;

    //是否正在攻击
    bool _isAtk;
    bool _isSkill;
    bool _isHard;
    bool _dir;

    bool _isBlock;
    int _blockSucceed;

    Weapon* _weapon;
    SkillPower* _skillPower;
    //bool _canFall;
protected:
    
    void reSetCol();
    void countOut();
public:
    GameSprite();
    virtual ~GameSprite();

    void addPosition(const cocos2d::Vec2& pos);
    void addPosition(float x, float y);
};

