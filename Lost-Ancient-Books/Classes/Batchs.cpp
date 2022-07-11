#include "Batchs.h"
USING_NS_CC;
Batchs* Batchs::_batchs = nullptr;

Batchs* Batchs::getInstance()
{
    if(_batchs)
        return _batchs;
    _batchs = new Batchs();
    if (_batchs && _batchs->initBatchs())
    {
        return _batchs;
    }
    return nullptr;
}

void Batchs::release()
{
    CC_SAFE_DELETE(_batchs);
}

bool Batchs::initBatchs()
{
    _particleNode = Node::create();
    _particleNode->retain();
    _hanziNode = Node::create();
    _hanziNode->retain();
    return true;
}

Batchs::~Batchs()
{
    CC_SAFE_RELEASE_NULL(_particleNode);
    CC_SAFE_RELEASE_NULL(_hanziNode);
}
