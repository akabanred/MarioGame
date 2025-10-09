#include "ItemMushroom.h"
#include "Mario.h"

ItemMushroom* ItemMushroom::create(ValueMap& map)
{
	ItemMushroom * pRet = new ItemMushroom();
	if (pRet&&pRet->init(map)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = NULL;
	}
	return pRet;

}


bool ItemMushroom::init(ValueMap& map)
{
	ItemCanMove::init();
	_type = Item::IT_MUSHROOMREWARD;
	setPositionByProperty(map);
	
	setVisible(false);
	this->setSpriteFrame(spriteFrameCache->getSpriteFrameByName("rewardMushroom"));
	setLocalZOrder(0);
	_state = SLEEP;
	_speedY = 0;
	_speedX = 40;
	return true;
}
void ItemMushroom::wakeup(){
	_state = GROW;
	this->setVisible(true);
	updateStatus();
}

void ItemMushroom::updateStatus(){
	if (_state == GROW){
		MoveBy* by = MoveBy::create(1.0f, Vec2(0, 16));
		CallFunc* callfunc = CallFunc::create(std::bind(&ItemMushroom::afterGrowCallback,this));
		runAction(Sequence::create(by,callfunc,NULL));
	}
	else if (_state == MOVE){
		
	}
}

void ItemMushroom::afterGrowCallback(){
	_state = MOVE;
	setLocalZOrder(100);
	updateStatus();
}

void ItemMushroom::collisionCheck(float dt){
	if (_state != MOVE)
		return;
	if (Mario::getInstance()->getBoundingBox().intersectsRect(this->getBoundingBox())){
		Mario::getInstance()->eatMushroom(this->_type);
		removeFromParent();		
	}
}

void ItemMushroom::update(float dt){
	if (this->_state != State::MOVE)
		return;

	moveCheck(dt);
	collisionCheck(dt);
}