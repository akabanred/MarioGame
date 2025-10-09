#include "Mario.h"
#include "Item.h"
#include "SceneGame.h"
#include "ItemMushroom.h"
#include <unordered_set>
Mario* Mario::sm_mario = NULL;
Mario* Mario::getInstance(){
	if (sm_mario){
		return sm_mario;
	}
	else{
		sm_mario = new Mario();
		if (sm_mario&&sm_mario->init()){

		}
		else{
			delete sm_mario;
			sm_mario = nullptr; 
		}
		return sm_mario;
	}
}

bool Mario::init(){

	Texture2D* texture = Director::getInstance()->getTextureCache()->addImage("smallWalkRight.png");
	SpriteFrame* frame = SpriteFrame::createWithTexture(texture, Rect(0, 0, texture->getContentSize().width / 11, texture->getContentSize().height));
	Sprite::initWithSpriteFrame(frame);

	_faceDir = common::RIGHT;
	_speed = Vec2(0, 0);
	
	_isFly = false;
	_life = 3;
	_state = State::Small;
	_isGodMode = false;
	_isDead = false;
	_isAutoRunning = false;
	_isOnLadder = false;

	initResourceCache();

	return true;
}

void Mario::initResourceCache(){
	{

		//加载到缓存中
		AnimationCache::getInstance()->addAnimation(myutil::createAnimation("smallWalkLeft.png",
			0, 10, 14, 0.05f), "smallMoveLeftAnimation");
		AnimationCache::getInstance()->addAnimation(myutil::createAnimation("smallWalkRight.png",
			0, 10, 14, 0.05f), "smallMoveRightAnimation");

		//马里奥跳的时候要用的帧
		SpriteFrameCache::getInstance()
			->addSpriteFrame(myutil::getSpriteFrame("smallWalkLeft.png", 10, 14),
			"smallJumpLeft");
		SpriteFrameCache::getInstance()->
			addSpriteFrame(myutil::getSpriteFrame("smallWalkRight.png", 10, 14)
			, "smallJumpRight");
	}
	{

		//加载到缓存中
		AnimationCache::getInstance()
			->addAnimation(myutil::createAnimation("walkRight.png",
			0, 10, 18, 0.05f), "bigMoveRightAnimation");

		AnimationCache::getInstance()
			->addAnimation(myutil::createAnimation("walkLeft.png",
			0, 10, 18, 0.05f)
			, "bigMoveLeftAnimation");

		//马里奥跳的时候要用的帧


		SpriteFrameCache::getInstance()
			->addSpriteFrame(myutil::getSpriteFrame("walkLeft.png", 10, 18),
			"bigJumpLeft");



		SpriteFrameCache::getInstance()->addSpriteFrame(
			myutil::getSpriteFrame("walkRight.png", 10, 18),
			"bigJumpRight");


	}
	{
		AnimationCache::getInstance()
			->addAnimation(myutil::createAnimation("small_die.png",
			0, 7, 16, 0.05f)
			, "smallDieAnimation");

	}
}

//更新状态
void Mario::updateStatus(){
	if (_isDead){
		return;
	}

	this->stopAllActions();
	if (_isAutoRunning){
		const char *name = nullptr;
		switch (_state)
		{
		case Mario::Small:
			name = "smallMoveRightAnimation";
			break;
		case Mario::Big:
			name = "bigMoveRightAnimation";
			break;
		case Mario::CanFire:
			name = "bigMoveRightAnimation";
			break;
		default:
			name = nullptr;
			break;
		}
		this->setDisplayFrameWithAnimationName( name , 0);
		return;
	}
	
	if (_isFly){
		if (_state!=State::Small){
			this->setSpriteFrame(SpriteFrameCache::getInstance()
								  ->getSpriteFrameByName(_faceDir == common::LEFT ?
								  "bigJumpLeft" : "bigJumpRight"));
		}
		else{
			this->setSpriteFrame(SpriteFrameCache::getInstance()
								  ->getSpriteFrameByName(_faceDir == common::LEFT ?
								  "smallJumpLeft" : "smallJumpRight"));
		}

		return;
	}

	if (_speed.x < 0){

		runAction(RepeatForever::create(
			Animate::create(AnimationCache::getInstance()
			->getAnimation(_state != State::Small ? "bigMoveLeftAnimation" : "smallMoveLeftAnimation"))));
	}
	else if (_speed.x>0){

		runAction(RepeatForever::create(
			Animate::create(AnimationCache::getInstance()
			->getAnimation(_state != State::Small ? "bigMoveRightAnimation" : "smallMoveRightAnimation"))));
	}
	else{

		if (_faceDir == common::LEFT){

			this->setDisplayFrameWithAnimationName(_state != State::Small ? "bigMoveLeftAnimation" : "smallMoveLeftAnimation", 0);
		}
		else {

			this->setDisplayFrameWithAnimationName(_state != State::Small ? "bigMoveRightAnimation" : "smallMoveRightAnimation", 0);
		}

	}

}

void Mario::stop(){
	
	if (_speed.x){
		_speed.x = 0;
		updateStatus();
	}
}

bool Mario::canMoveDown(float dt){
	//if (m_bIsOnLadder)
	//	CCLOG("incanMoveDown m_bISonLadder=%d", m_bIsOnLadder);
	if (_isDead || _isOnLadder)
		return false;


	Rect rcMario(getBoundingBox().origin.x, getBoundingBox().origin.y,
				   getBoundingBox().size.width - 1, getBoundingBox().size.height - 1);
	Vec2 ptMario = getPosition();

	TMXTiledMap* map = getMap();
	Vec2 ptMarioWorld = map->convertToWorldSpace(ptMario);
	Vec2 pts[3];

	float minY = rcMario.getMinY() + _speed.y*dt;
	if (rcMario.getMinY() >= map->getContentSize().height){
		return true;
	}
	//判断是否出界
	if (minY < 0){
		
		return true;
	}
	//向下
	pts[0] = Vec2(rcMario.getMinX(), minY);
	pts[1] = Vec2(rcMario.getMidX(), minY);
	pts[2] = Vec2(rcMario.getMaxX(), minY);

	for (int i = 0; i < 3; ++i){
		Vec2 ptTile = myutil::bLGLPointToTile(pts[i], map);

		//墙,水管,地板
		static const char *layerName[] = {
			"block",
			"pipe",
			"land"
		};
		for (int j = 0; j < sizeof(layerName) / sizeof(*layerName); ++j){
			TMXLayer* layer = map->getLayer(layerName[j]);
			int gid = layer->getTileGIDAt(ptTile);
			if (gid){
				//有东西挡住了
				return false;
			}
		}
	}

	return true;
}

bool Mario::canMoveUp(float dt){
	if (_isDead || _isAutoRunning)
		return false;
	Rect rcMario(getBoundingBox().origin.x, getBoundingBox().origin.y,
				   getBoundingBox().size.width - 1, getBoundingBox().size.height - 1);
	Vec2 ptMario = getPosition();

	TMXTiledMap* map = getMap();
	Vec2 ptMarioWorld = map->convertToWorldSpace(ptMario);
	Vec2 pts[3];

	//向上
	//判断是否出界
	float maxY = rcMario.getMaxY() + _speed.y*dt;
	if (maxY > map->getContentSize().height)
		return true;

	pts[0] = Vec2(rcMario.getMinX(), maxY);
	pts[1] = Vec2(rcMario.getMidX(), maxY);
	pts[2] = Vec2(rcMario.getMaxX(), maxY);

	for (int i = 0; i < 3; ++i){
		Vec2 ptTile = myutil::bLGLPointToTile(pts[i],map);

		//墙,水管,地板
		static const char *layerName[] = {
			"block",
			"pipe",
			"land"
		};
		for (int j = 0; j < sizeof(layerName) / sizeof(*layerName); ++j){
			TMXLayer* layer = map->getLayer(layerName[j]);
			int gid = layer->getTileGIDAt(ptTile);
			if (gid){
				//有东西挡住了				
				return false;
			}
		}
	}

	return true;
}


bool Mario::canMoveHorizontally(float dt){
	if (_isDead || _isAutoRunning)
		return false;
	Rect rcMario(getBoundingBox().origin.x, getBoundingBox().origin.y,
				   getBoundingBox().size.width - 1, getBoundingBox().size.height - 1);
	Vec2 ptMario = getPosition();

	TMXTiledMap* map = getMap();
	Vec2 ptMarioWorld = map->convertToWorldSpace(ptMario);
	Vec2 pts[3];

	if (ptMario.y >= map->getContentSize().height){
		return true;
	}
	if (getPositionY() < 0)
		return true;
	//判断是否出界
	if (ptMarioWorld.x + _speed.x*dt <= 0){
		return false;
	}

	float midY = rcMario.getMidY() > map->getContentSize().height - 1 ? map->getContentSize().height - 1 : rcMario.getMidY();
	float maxY = rcMario.getMaxY() > map->getContentSize().height - 1 ? map->getContentSize().height - 1 : rcMario.getMaxY();
	//CCLOG("midY=%g,maxY=%g", midY, maxY);
	if (_speed.x < 0){
		//向左走
		float minX = rcMario.getMinX() + _speed.x*dt;
		pts[0] = Vec2(minX, midY);
		pts[1] = Vec2(minX, maxY);
		pts[2] = Vec2(minX, rcMario.getMinY());

	}
	else{
		//向右走
		float maxX = rcMario.getMaxX() + _speed.x*dt;
		pts[0] = Vec2(maxX, midY);
		pts[1] = Vec2(maxX, maxY);
		pts[2] = Vec2(maxX, rcMario.getMinY());
	}

	for (int i = 0; i < 3; ++i){
		Vec2 ptTile = myutil::bLGLPointToTile(pts[i], map);

		//墙,水管,地板
		static const char *layerName[] = {
			"block",
			"pipe",
			"land"
		};
		for (int j = 0; j < sizeof(layerName) / sizeof(*layerName); ++j){
			TMXLayer* layer = map->getLayer(layerName[j]);
			int gid = layer->getTileGIDAt(ptTile);
			if (gid){
				//有东西挡住了
				return false;
			}
		}
	}

	return true;
}

TMXTiledMap* Mario::getMap(){
	return (TMXTiledMap*)getParent();
}


void Mario::setHorizontalSpeed(common::Direction dir){
	
	if (_faceDir!=dir||!_speed.x){

		if (dir == common::LEFT){
			_speed.x = -Mario_speed;
		}
		else{
			_speed.x = Mario_speed;
		}

		_faceDir = dir;

		updateStatus();
	}

}


void Mario::jump(int initV){
	if (_isDead || _isFly || _isAutoRunning)
		return;

	_speed.y = initV;
	_isFly = true;

	updateStatus();
}

void Mario::moveVerticalCheck(float dt){
	if (getPositionY() < -100){
		//mario 死亡
		this->die(true);
		return;
	}

	//判断是否可以自由落体
	if (!_isFly){
		_speed.y -= ARG_GRAVITY;
		if (canMoveDown(dt)){
			//没有东西挡住,自由下落
			this->setPositionY(getPositionY() + dt*_speed.y);
			_isFly = true;

		}
		else{
			_isFly = false;
			_speed.y = 0;
		}

	}

	if (_speed.y > 0){
		if (canMoveUp(dt)){
			setPositionY(getPositionY() + dt*_speed.y);
			_speed.y -= ARG_GRAVITY;
		}
		else{
			//速度反弹
			_speed.y = -_speed.y;
		}
	}
	else if (_speed.y < 0){
		if (canMoveDown(dt)){
			setPositionY(getPositionY() + dt*_speed.y);
			_speed.y -= ARG_GRAVITY;
		}
		else{
			_isFly = false;
			_speed.y = 0;
			updateStatus();
		}
	}
	else{
		_speed.y -= ARG_GRAVITY;

	}


}

void Mario::moveHorizontalCheck(float dt){
	if (!_speed.x)
		return;
	if (!canMoveHorizontally(dt))
		return;
	this->setPositionX(getPositionX() + _speed.x*dt);
	if (_speed.x > 0){
		Node* node = getParent();
		Vec2 ptWorld = node->convertToWorldSpace(getPosition());

		if (!_isAutoRunning&&ptWorld.x > winSize.width / 2){
			node->setPositionX(node->getPositionX() - dt*abs(_speed.x));
		}
	}

	
}

void Mario::setDead(bool isDead){
	_isDead = isDead;
}


void Mario::die(bool realDead){
	
	if (isDead())
		return;

	if (realDead){
		//真正死亡了
		_isDead = true;
		--_life;

		this->stopAllActions();
		
		_speed.x = 0;

		//进入死亡动画
		Animate* animate = Animate::create(AnimationCache::getInstance()
										   ->getAnimation("smallDieAnimation"));
		MoveBy* moveBy = MoveBy::create(1,
										Vec2(0, -winSize.height));
		CallFunc* callfunc = CallFunc::create([&](){
			//死亡动画结束了

			SceneGame* game = dynamic_cast<SceneGame*> (getMap()->getParent());
			if (game){
				if (_life > 0){
					int level = game->_level;
					this->removeFromParent();
					Director::getInstance()->replaceScene(SceneGame::create(level));
				}
				else{
					game->gameOver();
				}
			}
			else{
				CCLOG("%s:%d dynamic_cast err", __FILE__, __LINE__);
			}



		});

		runAction(Sequence::create(animate, moveBy, callfunc, nullptr));


	}
	else{
		//碰到怪先变小

		if (_state!=State::Small){
			_state = State::Small;
			this->beginGodMode();
			this->updateStatus();
		}
		else{
			die(true);
		}

	}
}

bool Mario::isFly(){
	return _isFly;
}
bool Mario::isDead(){
	return _isDead;
}





void Mario::eatMushroom(Item::ItemType type){
	if (type == Item::IT_MUSHROOMREWARD)
		_state=State::Big;

	updateStatus();
}

void Mario::beginGodMode(float dt){
	if (_isGodMode)
		return;
	_isGodMode = true;
	scheduleOnce(CC_SCHEDULE_SELECTOR(Mario::cancelGodModeCallback), dt);
}
void Mario::cancelGodModeCallback(float dt){

	_isGodMode = false;
}

bool Mario::isGodMode(){
	return _isGodMode;
}

void Mario::autoRun(){
	_isAutoRunning = true;
	updateStatus();
	_speed.y = 0;


	//自动向前走
	scheduleOnce(CC_SCHEDULE_SELECTOR(Mario::beginAutoMoveRightCallback), 1.5f);

}

void Mario::beginAutoMoveRightCallback(float dt){
	//自动向前走
	schedule(CC_SCHEDULE_SELECTOR(Mario::autoMoveRightCallback));

}
void Mario::autoMoveRightCallback(float dt){
	setPositionX(getPositionX() + dt * 100);
}

void Mario::setIsOnLadder(bool isOnLadder){
	//if (isOnLadder)
	//CCLOG("setIsOnLadder() invoke isOnLadder=%d", isOnLadder);
	_isOnLadder = isOnLadder;
}
bool Mario::isOnLadder(){
	return _isOnLadder;
}

int Mario::getSpeedY(){
	return _speed.y;
}
int Mario::getSpeedX(){
	return _speed.x;
}

void Mario::setSpeedY(int v_y){
	_speed.y = v_y;
}
void Mario::setSpeedX(int v_x){
	_speed.x = v_x;
}

void Mario::reverseSpeedY(){
	_speed.y = -_speed.y;
}

void  Mario::setIsFly(bool isFly){
	_isFly = isFly;
}

void Mario::endAutoRun() {

	_isAutoRunning = false;
}