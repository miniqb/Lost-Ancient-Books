#pragma once
#include "cocos2d.h"
#include "LittleBoy.h"
#include "CollisionHelper.h"
#include "MonsterFactory.h"
#include "WeaponFactory.h"
#include "SkillPowerFactory.h"
#include "StateNode.h"
#include "GemNode.h"
#include "Seesaw.h"
//�������Ϸ������, ���������Ϸ�ڳ���, ��ʱ���ǳ�����
class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    void setSoundName(const std::string& name) { _soundName = name; }

    //overrides
    virtual bool init()override;
    virtual void update(float dt) override;
    virtual void onEnter()override;
    virtual void onExit()override;

    void endScene();

    CREATE_FUNC(GameScene)
protected:
    //������Ӧ����
    void handleInputKeyDown(Input keyCode, cocos2d::Event* event);
    void handleInputKeyUp(Input keyCode, cocos2d::Event* event);

    //��ײ��Ӧ����
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    bool onContactPre(cocos2d::PhysicsContact& contact, cocos2d::PhysicsContactPreSolve& solve);
    void onContactEnd(cocos2d::PhysicsContact& contact);

    void addLittleBoy(const cocos2d::Vec2& postion);
    void addMonster(cocos2d::Node* ground, int sum, MonsterFactory* factory);

    void dealDir(const CollisionHelper::DirNormal& dirNormal,  GameSprite* me, cocos2d::Node* other);

    void handleQuestion(std::vector<Monster*>& monsters, float tipPer);
    void handleWeaponTables(std::vector<CallbackThing*>& tables);
    int getSkillIndex(const std::string& name);
    void handleSkillTables(std::vector<CallbackThing*>& tables);
    void handleHanziDoor(std::vector<CallbackThing*>& doors);
    void handleNormalDoor(std::vector<CallbackThing*>& doors);
    void handleSeesaw(std::vector<Seesaw*>& seesaws);

    cocos2d::RenderTexture* cutScene();
    void pauseFunction();
public:
    ~GameScene();
private:
    cocos2d::TMXTiledMap* _gameMap;
    cocos2d::Node* _allSpNode;
    CollisionHelper _collisionHelper;

    StateNode* _stateNode;
    GemNode* _gemNode;

    //std::vector<cocos2d::Node*> _seesaws;
    //std::vector<Monster*> _monsters;

    WeaponFactory* _weaponFactorys[4];
    int _weaIndex = 1;
    SkillPowerFactory* _skillFactorys[3];
    int _skillIndex = 1;
    int _monsterSum;
    LittleBoy* _littleBoy;
    std::vector<Seesaw*> _seesaws;

    int _backSoundID;
    std::string _soundName;
};

