#include "ItemBattery.h"

ItemBattery* ItemBattery::create(ValueMap& map)
{
	ItemBattery * pRet = new ItemBattery();
	if (pRet&&pRet->init(map)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = NULL;
	}
	return pRet;

}


bool ItemBattery::init(ValueMap& map)
{
	ItemCanMove::init();
	_type = Item::IT_MushroomMonster;
	setPositionByProperty(map);
	setDisplayFrameWithAnimationName("mushroomMoving", 0);
	return true;
}
