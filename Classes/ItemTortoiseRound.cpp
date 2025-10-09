#include "ItemTortoiseRound.h"
#include "Mario.h"
ItemTortoiseRound* ItemTortoiseRound::create(ValueMap& map)
{
	ItemTortoiseRound * pRet = new ItemTortoiseRound();
	if (pRet&&pRet->init(map)){
		pRet->autorelease();
	}
	else{
		delete pRet;
		pRet = nullptr;
	}
	return pRet;

}


bool ItemTortoiseRound::init(ValueMap& map)
{
	ItemCanMove::init();
	_type = Item::IT_tortoise_round;
	setPositionByProperty(map);
	
	m_roundDis = map.at("roundDis").asInt();
	_speedY = 0;
	_speedX = -50;
	m_state  = NORMAL;
	_bIsGodMode = false;
	m_initXV = 50;
	updateStatus();
	return true;
}

void ItemTortoiseRound::moveCheck(float dt){
	if (!isAppearInWindow())
		return;
	if (m_state == NORMAL){

		static float dis = m_roundDis;
		dis += dt*_speedX;
		setPositionX(getPositionX() + dt*_speedX);
		if (dis >= m_roundDis || dis <= 0){
			_speedX = -_speedX;
			updateStatus();
		}
	}
	else if (m_state == CRAZY){
		if (canMoveHorizontal(dt)){
			moveHorizontal(dt);
		}
		if (canMoveDown(dt)){
			moveDown(dt);
			_speedY -= ARG_GRAVITY;
		}
		else{
			_speedY = 0;
		}
	}
}
void ItemTortoiseRound::collisionCheck(float dt){
	
	if (!isAppearInWindow())
		return;
	if (_bIsGodMode)
		return;
	Mario* mario = Mario::getInstance();
	Rect rcMario = mario->getBoundingBox();
	Rect rcItem = this->getBoundingBox();

	if (rcMario.intersectsRect(rcItem)){

		if (m_state == NORMAL){
			if (mario->getSpeedY() <= 0 && rcMario.getMinY() > rcItem.getMaxY() - rcItem.size.height / 2){
				_speedX = 0;
				m_state = SLEEP;
				updateStatus();
				//无敌一段时间
				_bIsGodMode = true;

				//让马里奥弹出去
				mario->jump(100);

				scheduleOnce(CC_SCHEDULE_SELECTOR(ItemTortoiseRound::cancelGodModeCallback), 0.2f);

				//过一段时间复活
				scheduleOnce(CC_SCHEDULE_SELECTOR(ItemTortoiseRound::autoReLiveCallback), 5.0f);

			}
			else{
				//马里奥死亡
				

			}

			return;

		}

		if (m_state == SLEEP){
			_speedX = Mario::getInstance()->getPositionX() < getPositionX() ?
				-m_initXV * 3 : m_initXV * 3;
			m_state = CRAZY;
			//无敌一段时间
			_bIsGodMode = true;
			scheduleOnce(CC_SCHEDULE_SELECTOR(ItemTortoiseRound::cancelGodModeCallback), 0.2f);
			//scheduleOnce(schedule_selector(ItemTortoiseRound::autoReLiveCallback), 5.0f);

			updateStatus();
			return;
		}
		if (m_state == CRAZY){
			if (Mario::getInstance()->getSpeedY() <= 0 && rcMario.getMinY() > rcItem.getMaxY() - rcItem.size.height / 2){
				
				_speedX = 0;
				m_state = SLEEP;
				//无敌一段时间
				_bIsGodMode = true;
				scheduleOnce(CC_SCHEDULE_SELECTOR(ItemTortoiseRound::cancelGodModeCallback), 0.2f);

				//过一段时间复活
				scheduleOnce(CC_SCHEDULE_SELECTOR(ItemTortoiseRound::autoReLiveCallback), 0.2f);

			}
			else{
				if (Mario::getInstance()->isGodMode())
					return;
				Mario::getInstance()->die();

			}


			return;
		}

	}

}
void ItemTortoiseRound::updateStatus(){
	stopAllActions();
	if (m_state == NORMAL){
		
		runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->
														getAnimation(_speedX > 0 ? "tortoiseRightMoving" : "tortoiseLeftMoving"))));
	}
	else if (m_state == SLEEP || m_state==CRAZY){
		
		runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->
														getAnimation("tortoiseDead"))));

	}
}

void ItemTortoiseRound::autoReLiveCallback(float dt){
	m_state = NORMAL;
	_speedX = m_initXV;
	updateStatus();
}
void ItemTortoiseRound::cancelGodModeCallback(float dt){
	_bIsGodMode = false;
}