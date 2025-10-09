#include "ItemDarkCloud.h"

ItemDarkCloud* ItemDarkCloud::create(ValueMap& map)
{
	ItemDarkCloud * pRet = new ItemDarkCloud();
	if (pRet&&pRet->init(map)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = NULL;
	}
	return pRet;

}


bool ItemDarkCloud::init(ValueMap& map)
{
	Item::init();
	_type = Item::IT_MushroomMonster;
	setPositionByProperty(map);
	setDisplayFrameWithAnimationName("mushroomMoving", 0);
	return true;
}
