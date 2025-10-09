#include "ItemBullet.h"

ItemBullet* ItemBullet::create()
{
	ItemBullet * pRet = new ItemBullet();
	if (pRet&&pRet->init()){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = NULL;
	}
	return pRet;

}


bool ItemBullet::init()
{
	Item::init();
	_type = Item::IT_bullet;
	//setPositionByProperty(dict);
	


	return true;
}
