
#include "Item.h"
#include "ItemMushroomMonster.h"
#include "ItemMushroom.h"
#include "ItemTortoise.h"
#include "ItemFlower.h"
#include "ItemMushroom.h"
#include "ItemMushroom.h"
#include "ItemFlagpoint.h"
#include "ItemFinalpoint.h"
#include "ItemLadderLR.h"
#include "ItemFlyFish.h"
#include "ItemTortoiseRound.h"
#include "ItemTortoiseFly.h"
#include "ItemLadderUD.h"
#include "ItemFireString.h"
#include "ItemBoss.h"
#include "ItemBridgeStartPos.h"
#include "Mario.h"


Item* Item::create( ValueMap&map)
{
	
	const Value& type = map.at("type");
	if (type.asString() == "mushroom"){
		return ItemMushroomMonster::create(map);
	}
	else  if (type.asString() == "tortoise"){
		return ItemTortoise::create(map);
	}
	else if (type.asString() == "flower"){
		return ItemFlower::create(map);
	}
	else if (type.asString() == "MushroomReward"){
		ItemMushroom* mushroomReward = ItemMushroom::create(map);
		return mushroomReward;
	}
	else if (type.asString() == "MushroomAddLife"){
		ItemMushroom* itemMushroomAddLife = ItemMushroom::create(map);


		return itemMushroomAddLife;
	}
	else if (type.asString() == "flagpoint"){
		return	ItemFlagpoint::create(map);
	}
	else if (type.asString() == "finalpoint"){
		return ItemFinalpoint::create(map);
	}
	else if (type.asString() == "ladderLR"){
		return ItemLadderLR::create(map);
	}
	else if (type.asString() == "flyfish"){
		return ItemFlyFish::create(map);
	}
	else if (type.asString() == "tortoise_round"){
		return ItemTortoiseRound::create(map);
	}
	else if (type.asString() == "tortoise_fly"){
		return ItemTortoiseFly::create(map);
	}
	else if (type.asString() == "ladderUD"){
		return ItemLadderUD::create(map);
	}
	else if (type.asString() == "fire_string"){
		return ItemFireString::create(map);
	}
	else if (type.asString() == "boss"){
		return ItemBoss::create(map);
	}
	else if (type.asString() == "bridgestartpos"){
		return ItemBridgeStartPos::create(map);
	}


	return NULL;
}

void Item::setPositionByProperty(const ValueMap& dict){

	const Value& x = dict.at("x");
	const Value& y = dict.at("y");
	setPosition(Vec2(x.asInt(), y.asInt()-16));
	this->setIgnoreAnchorPointForPosition(true);
}
bool Item::init()
{
	Sprite::init();
	setLocalZOrder(common::ZO_MUSHROOM);
	
	return true;
}
void Item::onEnter(){
	Sprite::onEnter();
	scheduleUpdate();
	
}
void Item::onExit(){
	
	unscheduleUpdate();
	Sprite::onExit();
}


void Item::update(float dt){
	collisionCheck(dt);
}

bool Item::isAppearInWindow(){
	Vec2 ptInMap = getPosition();
	TMXTiledMap* map = getMap();
	Vec2 ptInWorld = map->convertToWorldSpace(ptInMap);
	return ( ptInWorld.x < winSize.width);
	
}

TMXTiledMap* Item::getMap(){
	return (TMXTiledMap*)getParent();
}
bool Item::isOutOfWindow(){
	Vec2	ptWorld=getMap()->convertToWorldSpace(getPosition());
	return (ptWorld.x < -winSize.width) || (getPositionY() <= -getBoundingBox().size.height);
}



void Item::autoDropFlag(){

}

 


 
 void Item::runAnimation(const char* name){
	 Animation* animation = AnimationCache::getInstance()->
		 getAnimation(name);
	 runAction(RepeatForever::create(Animate::create(animation)));
 }