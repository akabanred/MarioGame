#include "ItemFinalpoint.h"

ItemFinalpoint* ItemFinalpoint::create(ValueMap& map)
{
	ItemFinalpoint * pRet = new ItemFinalpoint();
	if (pRet&&pRet->init(map)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = NULL;
	}
	return pRet;

}


bool ItemFinalpoint::init(ValueMap& map)
{
	Item::init();
	_type = Item::IT_FINALPOINT;
	setPositionByProperty(map);
	//setDisplayFrameWithAnimationName("mushroomMoving", 0);
	return true;
}
