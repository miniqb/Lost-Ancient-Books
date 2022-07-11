#pragma once
#include "GameSprite.h"
#include "MonsterFactory.h"
#include "CallbackThing.h"
#include "AnimationLoader.h"
#include "BlodStrand.h"
class MonsterFactory;
class Monster;

class MonsterState
{
public:
     enum class State
    {
         NONE,          //ÎÞ×´Ì¬
         TEAR,           //ÏÐ¹ä×´Ì¬
         VIGILANCE,   //¾¯¾õ×´Ì¬
         BEFORE,
         ATTACK,       //¹¥»÷×´Ì¬
         AFTER,
         DEAD,          //ËÀÍö×´Ì¬
    };
protected:
    State _myTag;
    Monster* _monster;
public:
    MonsterState(Monster* m):_myTag(State::NONE),_monster(m){}
    virtual ~MonsterState() {}
    virtual void enterState() {};
    virtual MonsterState* update(float dt);
    virtual void outState();
    virtual State getTag() { return _myTag; }
protected:
    virtual MonsterState* insideUpdate(float dt) { return nullptr; }
};

//ÒÆ¶¯×´Ì¬
class MonsterMove :public MonsterState
{
public:
    MonsterMove(Monster* m) :MonsterState(m) { _myTag = MonsterState::State::TEAR; }
    virtual ~MonsterMove() {}
    virtual void enterState();
    virtual void outState();
protected:
    virtual MonsterState* insideUpdate(float dt)override;
};

//Í£Ö¹×´Ì¬
class StopState :public MonsterState
{
public:
    StopState(Monster* m) :MonsterState(m),_nowTime(0) { _myTag = MonsterState::State::TEAR; }
    virtual ~StopState() {}
    virtual void enterState();
    virtual void outState();
protected:
    virtual MonsterState* insideUpdate(float dt)override;
    float _nowTime;
};

//·¢ÏÖÖ÷½Ç×´Ì¬,×·Öð×´Ì¬
class VigState:public MonsterState
{
public:
    VigState(Monster* m):MonsterState(m) { _myTag = MonsterState::State::VIGILANCE; }
    virtual ~VigState() {};
    virtual void enterState();
    virtual void outState();
protected:
    virtual MonsterState* insideUpdate(float dt)override;
};

//Ç°Ò¡
class BeforeAtkState :public MonsterState
{
public:
    BeforeAtkState(Monster* m) :MonsterState(m),_nowTime(0) { _myTag = MonsterState::State::BEFORE; }
    virtual ~BeforeAtkState() {};
    virtual void enterState();
    virtual void outState();
protected:
    virtual MonsterState* insideUpdate(float dt)override;
private:
    float _nowTime;
};

//¹¥»÷×´Ì¬
class AtkState :public MonsterState
{
public:
    AtkState(Monster* m) :MonsterState(m) { _myTag = MonsterState::State::ATTACK; }
    static const int myTag = 4;
    virtual ~AtkState() {};
    virtual void enterState()override;
    virtual void outState()override;
protected:
    virtual MonsterState* insideUpdate(float dt)override;
};

//ºóÒ¡
class AfterAtkState :public MonsterState
{
public:
    AfterAtkState(Monster* m) :MonsterState(m),_nowTime(0) { _myTag = MonsterState::State::AFTER; }
    virtual ~AfterAtkState() {};
    virtual void enterState();
    virtual void outState();
protected:
    virtual MonsterState* insideUpdate(float dt)override;
private:
    float _nowTime;
};

//ËÀÍö×´Ì¬
class DeadState :public MonsterState
{
public:
    DeadState(Monster* m) :MonsterState(m) { _myTag = MonsterState::State::DEAD; }
    virtual ~DeadState() {}
    virtual void enterState();
    virtual MonsterState* update(float dt) { return nullptr; }
};

struct MonsterStation
{
    MonsterMove* _move;
    StopState* _stop;
    VigState* _vig;
    BeforeAtkState* _before;
    AtkState* _atk;
    AfterAtkState* _after;
    DeadState* _dead;
    MonsterStation()
        :_move(nullptr)
        ,_stop(nullptr)
        , _vig(nullptr)
        , _before(nullptr)
        , _atk(nullptr)
        , _after(nullptr)
        , _dead(nullptr)
    {}
    ~MonsterStation()
    {
        CC_SAFE_DELETE(_vig);
        CC_SAFE_DELETE(_before);
        CC_SAFE_DELETE(_atk);
        CC_SAFE_DELETE(_after);
        CC_SAFE_DELETE(_dead);
    }
};

class Monster :
    public GameSprite
{
//public:
//    static const int tagLabel = 99;
protected:
    enum
    {
        tagMove = 1,
        tagUpDown,
        tagStand,
        tagVig,
        tagAttack,
        tagBefore,
        tagAfter,
        tagRunAnimate,
        tagVigAnimate,
        tagAtkAnimate,
        tagStandAnimate,
        tagDeadAnimate,
        tagTiny,
        tagOther
    };
public:
    static Monster* create(cocos2d::SpriteFrame* frame, float fmt, float fwt, int atkLen, int life, float splen);
    static GameSprite* boy;

    MonsterStation& getStation() { return _station; }
public:
    virtual bool foundBoy();
    virtual bool canAtk();

    //ËæÒâ×ß¶¯
    void setFreeMoveTime(float t) { _freeMoveTime = t; }
    float getFreeMoveTime() { return _freeMoveTime; }
    void setFreeWaitTime(float t) { _freeWaitTime = t; }
    float getFreeWaitTime() { return _freeWaitTime; }

    //À×´ï·¶Î§
    void setRadarLen(float l) { _radarLen = l; }
    float getRadarLen() { return _radarLen; }
    void setAtkLen(float l) { _atkLen = l; }
    float getAtkLen() { return _atkLen; }

    //¶¯»­
    void runRunAnimate(bool isForver = true);
    void stopRunAnimate();
    void runVigAnimate(bool isForver = true);
    void stopVigAnimate();
    void runAttackAnimate(bool autoAtk, bool isForver = false);
    void stopAttackAnimate();
    void runStandAnimate(bool isForver = true);
    void stopStandAnimate();
    void runDeadAnimate(bool isForver = true);
    void stopDeadAnimate();

    virtual void runAfterAtkAnimate();
    virtual void stopAfterAtkAnimate();


    void setClonSum(int s) { _clonSum = s; }
    int getClonSum() { return _clonSum; }
    void setClonFactory(MonsterFactory* f) { _clonFactory = f; }

    void changeState(MonsterState* state);
    CallbackThing* addCallbackThing();

    void setHanzi(const std::string& str) { _hanzi = str; }
    const std::string& getHanzi() { return _hanzi; }
    void setHanzi(const std::string& str, bool g) { _hanzi = str, _isGood = g; }
    void setHanziLabel(cocos2d::Label* label) { _hanziLabel = label; }
    void setHanziOffect(const cocos2d::Vec2& offect) { _hanziOffect = offect; }
    void setGoodHanzi(bool g) { _isGood = g; }
    bool isGoodHanzi() { return _isGood; }
    const cocos2d::Color3B& getHanziColor() { return _hanziColor; }

    std::vector<Monster*> getClons();
    void createClons(bool s);
    void createDrops();

    void setShell(int s) { _shell = s; }
    int getShell() { return _shell; }
    void setKnowPiece(int p) { _knowPiece = p; }
    int getKnowPiece() { return _knowPiece; }

    void setBeforeTime(float t) { _beforeTime = t; }
    float getBeforeTime() { return _beforeTime; }
    void setAfterTime(float t) { _afterTime = t; }
    float getAfterTime() { return _afterTime; }

    void setAnimations(const MonsterAnimation& an) { _animations = an; };
    void setNowAnimate(cocos2d::Animate* a) { _nowAnimate = a; }
    void setNowAnimateRepeat(cocos2d::RepeatForever* r) { _nowAnimateRepeat = r; }

    void randomSelf(float min, float max);

    //ÐÐÎª¶¯×÷
    virtual void attack();
    virtual void stopAttack();
    virtual void move();
    virtual void stopMove();
    virtual void stand();
    virtual void stopStand();
    virtual void vig();
    virtual void stopVig();
    virtual void beforeAtk();
    virtual void stopBeforeAtk();
    virtual void afterAtk();
    virtual void stopAfterAtk();
    virtual void dead();
    bool isMoveing();

    virtual void beAttack(int h, bool isRight)override;
    void onExit()override;
    void setFSSum(int* s) { _sumFS = s; }
protected:
    void insideUpdate(float dt)override;
    void suitAtkArea(AtkArea* area);
    virtual void initState();
    void tempFoundForce(float time);
    virtual void updateHanziLabel();
    cocos2d::Node* thowRightHanzi();
    virtual void strengthSelf();
protected:
    /**×´Ì¬Ïà¹Ø**/
    MonsterState* _state;
    MonsterStation _station;

    /**ÐÐÎªÏà¹Ø**/
    float _freeMoveTime;
    float _freeWaitTime;
    float _radarLen;
    float _radarLenSq;
    float _atkLen;
    int _foundForce;
    int _atk;
    float _vigSpeedRate;

    /**ºº×ÖÏà¹Ø**/
    std::string _hanzi;
    bool _isGood;
    int _clonSum;
    MonsterFactory* _clonFactory;
    cocos2d::Label* _hanziLabel;
    cocos2d::Vec2 _hanziOffect;
    cocos2d::Color3B _hanziColor;

    /**µôÂäÎï**/
    int _shell;
    int _knowPiece;

    /**¶¯»­Ïà¹Ø**/
    MonsterAnimation _animations;
    cocos2d::Animate* _nowAnimate;
    cocos2d::RepeatForever* _nowAnimateRepeat;

    float _beforeTime;
    float _afterTime;

    //·ÖÉíÊý
    int* _sumFS;
    //Á£×Ó
    cocos2d::ParticleSystem* _particle;

    BlodStrand* _blodStrand;

    cocos2d::Color3B _oriColor;
public:
    Monster();
    virtual ~Monster();
    bool Init(float fmt, float fwt, int atkLen, int life, float splen, int atk, int ranLen, float beforeT, float afterT);
};