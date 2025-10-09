#include "ItemLadderUD.h"

ItemLadderUD* ItemLadderUD::create(ValueMap& map)
{
	ItemLadderUD * pRet = new ItemLadderUD();
	if (pRet&&pRet->init(map)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = NULL;
	}
	return pRet;

}


bool ItemLadderUD::init(ValueMap& map)
{
	Item::init();
	_type = Item::IT_MushroomMonster;
	setPositionByProperty(map);
	setDisplayFrameWithAnimationName("mushroomMoving", 0);
	return true;
}
