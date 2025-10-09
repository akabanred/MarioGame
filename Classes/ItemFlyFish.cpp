#include "ItemFlyFish.h"
#include "Mario.h"
ItemFlyFish* ItemFlyFish::create(ValueMap& map)
{
	ItemFlyFish * pRet = new ItemFlyFish();
	if (pRet&&pRet->init(map)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = NULL;
	}
	return pRet;

}


bool ItemFlyFish::init(ValueMap& map)
{
	Item::init();
	_type = Item::IT_flyfish;
	setPositionByProperty(map);

	_duration = map.at("duration").asInt();
	_offsetH  = map.at("offsetH").asInt();
	_offsetV  = map.at("offsetV").asInt();
	
	_speedX      = _offsetH;
	_alreadlyFly = false;
	
	return true;
}
void ItemFlyFish::update(float dt){
	collisionCheck(dt);
	moveCheck(dt);
}
void ItemFlyFish::moveCheck(float dt){
	if (Mario::getInstance()->getPositionX() < getPositionX())
		return;
	if (!this->isAppearInWindow()||_alreadlyFly){
		return;
	}
	updateStatus();
	//±´Èû¶ûÇúÏßÔË¶¯
	ccBezierConfig config;
	config.controlPoint_1.x = CCRANDOM_0_1()*_offsetH;
	config.controlPoint_1.y = CCRANDOM_0_1()*_offsetV;
	config.controlPoint_2.x = CCRANDOM_0_1()*_offsetH;
	config.controlPoint_2.y = CCRANDOM_0_1()*_offsetV;
	config.endPosition = Vec2(_offsetH, _offsetV);
	BezierBy* bezier = BezierBy::create(_duration, config);

	ccBezierConfig config1;
	config1.controlPoint_1.x = CCRANDOM_0_1()*_offsetH;
	config1.controlPoint_1.y = CCRANDOM_0_1()*_offsetV;
	config1.controlPoint_2.x = CCRANDOM_0_1()*_offsetH;
	config1.controlPoint_2.y = CCRANDOM_0_1()*_offsetV;
	config1.endPosition = Vec2(_offsetH, -_offsetV);
	BezierBy* bezier1 = BezierBy::create(_duration, config1);

	auto callfunc = CallFunc::create([this]() {
		this->removeFromParent();
	});
	runAction(Sequence::create(bezier, bezier1, callfunc,NULL));
	_alreadlyFly = true;
	
}
void ItemFlyFish::collisionCheck(float dt){
	if (!_alreadlyFly)
		return;
	Mario* mario = Mario::getInstance();
	if (mario->getBoundingBox().intersectsRect(getBoundingBox())){
		mario->die(false);
	}
}

void ItemFlyFish::updateStatus(){
	Animation* ani = AnimationCache::getInstance()
		->getAnimation(_speedX>0 ? "flyFishRight" : "flyFishLeft");
	this->runAction(RepeatForever::create(Animate::create(ani)));
}