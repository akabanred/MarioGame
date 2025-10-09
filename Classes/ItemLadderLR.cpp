#include "ItemLadderLR.h"
#include "Mario.h"
ItemLadderLR* ItemLadderLR::create(ValueMap& map)
{
	ItemLadderLR * pRet = new ItemLadderLR();
	if (pRet&&pRet->init(map)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = NULL;
	}
	return pRet;

}


bool ItemLadderLR::init(ValueMap& map)
{
	Item::init();
	_type = Item::IT_ladderLR;
	m_dist = 0;
	setPositionX(getPositionX() - 30);
	setPositionByProperty(map);
	Texture2D* texture =textureCache->addImage("ladder.png");
	setTexture(texture);
	setTextureRect(Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));

	//一开始还是向右
	m_lorR=map.at("LorR").asInt();

	//规定向右速度为正
	_speedX = m_lorR? 20:-20;
	

	//摆动幅度
	m_ladderDis = map.at("ladderDis").asInt();
	m_dist = m_lorR ? 0 : m_ladderDis;
	m_bIsMarioOnThis = false;
	return true;
}
void ItemLadderLR::update(float dt){
	collisionCheck( dt);
	moveCheck(dt);
}
void ItemLadderLR::moveCheck(float dt){
	m_dist += dt*_speedX;
	setPositionX(getPositionX() + dt*_speedX);
	
	if (m_dist >= m_ladderDis || m_dist<=0){
		_speedX = -_speedX;
	}
}
void ItemLadderLR::collisionCheck(float dt){
	Rect rcMario = Mario::getInstance()->getBoundingBox();
	Rect rcLadder = getBoundingBox();
	if (rcMario.intersectsRect(rcLadder)){
		m_bIsMarioOnThis = true;
		if (Mario::getInstance()->isOnLadder()){
			//跟着梯子一起动
			Mario::getInstance()->setPositionX(Mario::getInstance()->getPositionX() + dt*_speedX);
		}
		else{
			if (Mario::getInstance()->isFly()){
				if (Mario::getInstance()->getSpeedY() > 0){
					//脑袋撞上了

					//反弹回去
					Mario::getInstance()->reverseSpeedY();
				}
				else if (Mario::getInstance()->getSpeedY() < 0){
					//落在梯子上
					Mario::getInstance()->setIsOnLadder(true);
					Mario::getInstance()->setSpeedY(0);
					Mario::getInstance()->setIsFly(false);
				}
				else{

				}
			}
			else{
				//落在梯子上
				Mario::getInstance()->setIsOnLadder(true);
			}
		
		}
	}
	else{
		if (m_bIsMarioOnThis){
			Mario::getInstance()->setIsOnLadder(false);
			m_bIsMarioOnThis = false;
		}
		
			
	}
	
}