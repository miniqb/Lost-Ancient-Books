#include "CoolingComponent.h"
USING_NS_CC;

void CoolingComponent::update(float delta)
{
	updateCoolingTime(delta);
	insideUpdate(delta);
}

void CoolingComponent::updateCoolingTime(float delta)
{
	if (_nowCooling > 0)
	{
		_nowCooling -= delta;
		if (_nowCooling < 0)
			_nowCooling = 0;
	}
}

CoolingComponent::CoolingComponent()
	:Component()
	, _coolingTime(0)
	, _nowCooling(0)
{
}

CoolingComponent::~CoolingComponent()
{
}
