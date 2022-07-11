#include "Seesaw.h"
#include "GlobalWorld.h"

USING_NS_CC;
Seesaw* Seesaw::create(Sprite* sp, const Size& size, int tag, const PhysicsMaterial& pm, float mass)
{
    Seesaw* se = new Seesaw();
    if (se && se->initWithSize(sp, size, tag, pm, mass))
    {
        se->autorelease();
        return se;
    }
    CC_SAFE_DELETE(se);
    return nullptr;
}
void Seesaw::onEnter()
{
    Node::onEnter();
    PhysicsJointPin* joint = PhysicsJointPin::construct(_fix->getPhysicsBody(), _wall->getPhysicsBody(), this->convertToWorldSpace(_fix->getPosition()));
    this->getScene()->getPhysicsWorld()->addJoint(joint);
}

bool Seesaw::initWithSize(Sprite* sp, const Size& size, int tag, const PhysicsMaterial& pm, float mass)
{
    if (!Node::init())
        return false;
    _wall = sp;
    _wall->setTag(tag);
    _wall->setContentSize(size);
    _wall->setAnchorPoint(Vec2(0.5f, 0.5f));
    PhysicsBody* body = PhysicsBody::createBox(size, pm);
    body->setAngularDamping(0.3f);
    //body->setDynamic(false);
    body->setCategoryBitmask(CAT_WALL);
    body->setContactTestBitmask(CON_WALL);
    body->setCollisionBitmask(COL_WALL);
    _wall->setPhysicsBody(body);
    this->addChild(_wall);

    Node* fix = Node::create();
    PhysicsBody* fixBody = PhysicsBody::create(mass, 0);
    fixBody->setDynamic(false);
    fix->setPhysicsBody(fixBody);
    _fix = fix;
    this->addChild(fix);

    //PhysicsJointPin* joint = PhysicsJointPin::construct(fixBody, body, fix->getPosition());
    //_physicsWorld->addJoint(joint);
    return true;
}

void Seesaw::addJoint(cocos2d::PhysicsWorld* world)
{
    PhysicsJointPin* joint = PhysicsJointPin::construct(_fix->getPhysicsBody(), _wall->getPhysicsBody(), this->convertToWorldSpaceAR(_fix->getPosition()));
    world->addJoint(joint);
}
