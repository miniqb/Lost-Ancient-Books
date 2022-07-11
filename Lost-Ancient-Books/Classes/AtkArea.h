#pragma once
#include "ParticleFactory.h"
#include "AtkAreaFactory.h"
#include "Crash.h"

class AtkAreaFactory;
class AtkArea :
    public Crash
{
public:
    void setFactory(ParticleFactory* par) { _particleFactory = par; }
    ParticleFactory* getFactory() { return _particleFactory; }

    float getExistTime() { return _existTime; }
    void setExistTime(float t) { _existTime = t; }
    int getHarm() { return _harm; }
    void setHarm(int h) { _harm = h; }

    void setBoom(AtkAreaFactory* b) { _boomFactory = b; }
    void setBoomHarm(int h) { _boomHarm = h; }
    AtkAreaFactory* getBoom() { return _boomFactory; }

    void setBt(float btt, float btl) { _btTime = btt, _btLen = btl; }

    cocos2d::ParticleSystem* openParticle();

    void setAtkDir(bool d) { _atkDir = d; }
    bool getAtkDir() { return _atkDir; }
public:
    virtual bool crash(cocos2d::Node* other)=0;
    void onExit()override;
    virtual void boom();
    virtual AtkArea* getBoomAtkArea();
protected:
    virtual void _attack(cocos2d::Node* other);
    virtual void attack(cocos2d::Node* other);
public:
    AtkArea();
    ~AtkArea();
protected:
    int _harm;
    float _existTime;
    ParticleFactory* _particleFactory;
    cocos2d::ParticleSystem* _particle;

    int _boomHarm;
    AtkAreaFactory* _boomFactory;

    float _btTime;
    float _btLen;

    bool _atkDir;
};

class ThroughAtkArea :public AtkArea
{
public:
    virtual bool crash(cocos2d::Node* other)override;
protected:
    void endCrash();
public:
    static ThroughAtkArea* create(const std::string& filename);
    static ThroughAtkArea* createWithSpriteFrame(cocos2d::SpriteFrame* frame);
    static ThroughAtkArea* create();
};

class CantThroughAtkArea :public AtkArea
{
public:
    virtual bool crash(cocos2d::Node* other)override;
public:
    static CantThroughAtkArea* create(const std::string& filename);
    static CantThroughAtkArea* createWithSpriteFrame(cocos2d::SpriteFrame* spriteFrame);

    void setCanCrashTime(int t) { _canCrashTime = t; }
    int getCanCrashTime() { return _canCrashTime; }

    CantThroughAtkArea();
protected:
    void endCrash(cocos2d::Node* other);

protected:
    int _crashTime;
    int _canCrashTime;
};

class BoomAtkArea :public AtkArea
{
public:
    virtual bool crash(cocos2d::Node* other)override;
public:
    static BoomAtkArea* create(const std::string& filename);
    static BoomAtkArea* createWithSpriteFrame(cocos2d::SpriteFrame* spriteFrame);
    BoomAtkArea();

    void update(float)override;
protected:
    virtual void _attack(cocos2d::Node* other)override;
protected:
    int _hasAtk;
};


class EraserAtkArea :public CantThroughAtkArea
{
public:
    virtual bool crash(cocos2d::Node* other)override;
protected:
    virtual void _attack(cocos2d::Node* other)override;
public:
    static EraserAtkArea* create(const std::string& filename);
    static EraserAtkArea* createWithSpriteFrame(cocos2d::SpriteFrame* frame);
    EraserAtkArea();
};

class FirecrackerAtkArea :public AtkArea
{
public:
    virtual bool crash(cocos2d::Node* other)override;
    virtual void outCrash(cocos2d::Node* other)override;
    void setAim(cocos2d::Node* aim, cocos2d::Node* wall) { _aim = aim, _wall = wall; }
    void setThroughTime(float t);
public:
    static FirecrackerAtkArea* createWithSpriteFrame(cocos2d::SpriteFrame* spriteFrame);
    FirecrackerAtkArea();
    virtual void update(float)override;
    void myBoom();
protected:
    bool _hasAtk;
    cocos2d::Node* _aim;
    cocos2d::Node* _wall;
    bool _canThrough;
    int _isIn;
};