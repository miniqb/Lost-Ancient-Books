#pragma once
#include "cocos2d.h"
#include "Monster.h"
#include "CallbackThing.h"
#include "Seesaw.h"
class MapBuilder
{
public:
    void handleWall();
    void handlePlatform();
    void handleSeesaw(std::vector<Seesaw*>& seesaws, const cocos2d::PhysicsMaterial& pm, float mass);
    void handleTrampoline(const cocos2d::PhysicsMaterial& pm);
    void handleMap(const cocos2d::PhysicsMaterial& seesawPm, float mass, const cocos2d::PhysicsMaterial& TramPm);
    void handleMonster(/*out*/std::vector<Monster*>& monsters, int clonSum);
    void handleDoor(/*out*/std::vector<CallbackThing*>& hanziDoors,/*out*/std::vector<CallbackThing*>& normalDoors);
    void handleWeaponTable(/*out*/std::vector<CallbackThing*>& tables);
    void handleSkillTable(/*out*/std::vector<CallbackThing*>& tables);
    cocos2d::Vec2 getBoyPosition();

    cocos2d::Sprite* getSpriteByGid(int gid);
private:
    cocos2d::Node* getWall(const cocos2d::Vec2& pos, const cocos2d::Size& size);
    cocos2d::Node* getPlatform(const cocos2d::Vec2& pos, const cocos2d::Size& size);
    Seesaw* getSeesaw(cocos2d::Sprite* sp, const cocos2d::Vec2& pos, const cocos2d::Size& size, const cocos2d::PhysicsMaterial& pm, float mass);
    cocos2d::Node* getTrampoline(const cocos2d::Vec2& pos, const cocos2d::Size& size, const cocos2d::PhysicsMaterial& pm);
    cocos2d::Node* getDoor(const cocos2d::Vec2& pos, const cocos2d::Size& size);


    Monster* getMonster(const cocos2d::Vec2& pos, MonsterFactory* factory, int clonSum);
private:
    cocos2d::Vec2 getStanderPosition(float x, float y, const cocos2d::Size& tiledSize);
    cocos2d::Rect getStanderRect(float x, float y, float width, float height, const cocos2d::Size& tiledSize);
private:
    cocos2d::TMXTiledMap* _map;
    cocos2d::Size _fixTileSize;
public:
    MapBuilder(cocos2d::TMXTiledMap* map);
};

