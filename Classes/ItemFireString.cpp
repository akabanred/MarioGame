#include "ItemFireString.h"
#include "Mario.h"
ItemFireString* ItemFireString::create(ValueMap& map)
{
	ItemFireString * pRet = new ItemFireString();
	if (pRet&&pRet->init(map)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = NULL;
	}
	return pRet;

}


bool ItemFireString::init(ValueMap& map)
{
	Item::init();
	_type = Item::IT_fire_string;
	setPositionByProperty(map);
	
	Texture2D* texture = Director::getInstance()->getTextureCache()->
		addImage("bossBullet.png");
	setTexture(texture);
	setTextureRect(Rect(0, 0, texture->getContentSize().width,
		texture->getContentSize().height));

	setAnchorPoint(Vec2(0,0.5f));
	int begAngle = map.at("begAngle").asInt();
	setRotation(begAngle);

	int time = map.at("time").asInt();
	this->setScale(0.4f);
	runAction(RepeatForever::create(
		RotateBy::create(time, 360)));

	return true;
}

void ItemFireString::collisionCheck(float dt){
	if (!isAppearInWindow()){
		return;
	}

	//获取线段
	struct Line{
		Vec2 p1;
		Vec2 p2;
	};
	Line fireString;

	Rect rcItem = getBoundingBox();

	int angle = getRotation();
	angle %= 360;
	if (angle > 270){
		fireString.p1 = Vec2(rcItem.getMaxX(), rcItem.getMinY());
		fireString.p2 = Vec2(rcItem.getMinX(),rcItem.getMaxY());
	}
	else if (angle > 180){
		fireString.p1 = Vec2(rcItem.getMaxX(), rcItem.getMaxY());
		fireString.p2 = Vec2(rcItem.getMinX(), rcItem.getMinY());

	}
	else if (angle > 90){
		fireString.p1 = Vec2(rcItem.getMinX(), rcItem.getMaxY());
		fireString.p2 = Vec2(rcItem.getMaxX(), rcItem.getMinY());

	}
	else
	{
		fireString.p1 =Vec2(rcItem.getMinX(), rcItem.getMinY());
		fireString.p2 =Vec2(rcItem.getMaxX(), rcItem.getMaxY());

	}
	//CCLOG("len%g", fireString.p1.getDistance(fireString.p2));
	Rect rcMario = Mario::getInstance()->getBoundingBox();
	//mario的线段
	Line lineMario[2];
	lineMario[0].p1 = Vec2(rcMario.getMinX(), rcMario.getMinY());
	lineMario[0].p2 = Vec2(rcMario.getMaxX(), rcMario.getMaxY());
	lineMario[1].p1 = Vec2(rcMario.getMinX(), rcMario.getMaxY());
	lineMario[1].p2 = Vec2(rcMario.getMaxX(), rcMario.getMinY());

	for (int i = 0; i < 2; ++i){
		if (Vec2::isSegmentIntersect(lineMario[i].p1, lineMario[i].p2, fireString.p1, fireString.p2)){
			Mario::getInstance()->die(false);
		}
	}															
}
