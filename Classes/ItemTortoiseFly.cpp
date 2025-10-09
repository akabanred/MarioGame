#include "ItemTortoiseFly.h"
#include "Mario.h"
ItemTortoiseFly* ItemTortoiseFly::create(ValueMap& map)
{
	ItemTortoiseFly * pRet = new ItemTortoiseFly();
	if (pRet&&pRet->init(map)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = nullptr;
	}
	return pRet;

}


bool ItemTortoiseFly::init(ValueMap& map)
{
	ItemCanMove::init();
	_type = Item::IT_tortoise_fly;
	setPositionByProperty(map);
	
	m_flyDis = map.at("flyDis").asInt();
	m_status = NORMAL;
	_speedX = -50;
	_isGodMode = false;
	return true;
}

void ItemTortoiseFly::updateStatus(){
	
	if (m_status == NORMAL){
		stopAllActions();
		runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->
														getAnimation(_speedX > 0 ? "tortoiseFlyRight" : "tortoiseFlyLeft"))));

	}

}

void ItemTortoiseFly::moveCheck(float dt){
	if (m_status == NORMAL){
		if (canMoveHorizontal(dt)){
			moveHorizontal(dt);
		}

		
	}
	else if (m_status == DROPPING){
		if (canMoveDown(dt)){
			moveDown(dt);
		}
		else{
			_speedY = 0;
		}
	}
	else if (m_status == ONLAND||m_status==CRAZY){
		if (canMoveHorizontal(dt)){
			moveHorizontal(dt);
		}

		if (canMoveDown(dt)){
			moveDown(dt);
		}
		else{
			_speedY = 0;
		}
	
	}
	
}
void ItemTortoiseFly::collisionCheck(float dt){

	Rect rcItem = getBoundingBox();
	Rect rcMario = Mario::getInstance()->getBoundingBox();
	if (rcItem.intersectsRect(rcMario)){
		if (m_status == NORMAL){
				if (Mario::getInstance()->getSpeedY() <= 0 && rcMario.getMinY() > rcItem.getMaxY() - rcItem.size.height / 2){
					
					_speedX = 0;
					m_status = DROPPING;
					updateStatus();
					//无敌一段时间
					beginGodMode(0.2f);

					//让马里奥弹出去
					Mario::getInstance()->jump(100);
				}
				else{
					//马里奥死亡
					Mario::getInstance()->die(false);
				}
			}
			else if (m_status == DROPPING){
				//设计马里奥的下降的速度小于   乌龟的下降速度
			}
			else if (m_status == ONLAND){
				if (Mario::getInstance()->getSpeedY() <= 0 && rcMario.getMinY() > rcItem.getMaxY() - rcItem.size.height / 2){
					
					_speedX = 0;
					m_status = SLEEP;
					updateStatus();
					//无敌一段时间
					beginGodMode(0.2f);

					//过一会复活
					//过一段时间复活
					scheduleOnce(CC_SCHEDULE_SELECTOR(ItemTortoiseFly::reLiveCallback), 5.0f);

					//让马里奥弹出去
					Mario::getInstance()->jump(100);

				}
				else{
					//马里奥死亡
					Mario::getInstance()->die(false);
				}
			}

			else if (m_status == SLEEP){
				_speedX = Mario::getInstance()->getPositionX() < getPositionX() ?
					150 :- 150;
				m_status= CRAZY;


			}
			else if (m_status == CRAZY){

			}
	
	}
}

void ItemTortoiseFly::reLiveCallback(float){

	m_status = NORMAL;
	_speedX = 50;
	updateStatus();
}