#include "ItemFlagpoint.h"

ItemFlagpoint* ItemFlagpoint::create(ValueMap& map)
{
	ItemFlagpoint * pRet = new ItemFlagpoint();
	if (pRet&&pRet->init(map)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = NULL;
	}
	return pRet;

}


bool ItemFlagpoint::init(ValueMap& map)
{
	Item::init();
	_type = Item::IT_FLAGPOINT;
	setPositionByProperty(map);

	Texture2D* texture = textureCache->addImage("flag.png");
	setTexture(texture);
	setTextureRect(Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
	setPositionX(getPositionX() - getContentSize().width/2);
	return true;
}

void ItemFlagpoint::autoDropFlag(){
	
	this->runAction(MoveBy::create (1, Vec2(0, -150)));
}
