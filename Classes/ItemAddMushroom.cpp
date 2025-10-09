#include "ItemAddMushroom.h"

ItemAddMushroom* ItemAddMushroom::create(ValueMap& map)
{
	ItemAddMushroom * pRet = new ItemAddMushroom();
	if (pRet&&pRet->init(map)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = NULL;
	}
	return pRet;

}


bool ItemAddMushroom::init(ValueMap& map)
{
	ItemCanMove::init();
	_type = Item::IT_MushroomMonster;
	setPositionByProperty(map);
	setDisplayFrameWithAnimationName("mushroomMoving", 0);
	return true;
}
