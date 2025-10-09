 
#include "SceneGame.h"
#include "SimpleAudioEngine.h"
#include "MenuCtrl.h"
#include "SceneStart.h"
#include "Mario.h"
#include "Item.h"
#include "ItemMushroom.h"
SceneGame* SceneGame::create(int level){
	 SceneGame* pRet = new SceneGame();
	 if (pRet&&pRet->init(level)){
		 pRet->autorelease();
	 }
	 else{
		 delete pRet;
		 pRet = nullptr;
	 }

	 return pRet;
}
bool SceneGame::init(int level){
	Scene::init();
	_menuDir = common::NONE;
	_level = level;
	_finalPoint = nullptr;
	
	addMap();
	addCtrlButton();
	addAnimationToCache();
	addMapObjectGroup();
	
	return true;
}

void SceneGame::addMap(){
	char mapName[32];
	myutil::format(mapName, "MarioMap", _level, ".tmx");
	_map = TMXTiledMap::create(mapName);
	addChild(_map);
	_map->setPosition(_map->getPosition() + Vec2(0, winSize.height - _map->getContentSize().height));

}
void SceneGame::addCtrlButton(){
	//增加控制面板背景
	Sprite* crtlBG = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("controlUI.png"));
	crtlBG->setPosition(Vec2(0, 0));
	crtlBG->setAnchorPoint(Vec2(0, 0));
	addChild(crtlBG);

	//自定义菜单，实现连续点击功能
	MenuCtrl * dirMenu = MenuCtrl::create();
	addChild(dirMenu);

	_textureDirNone = Director::getInstance()->getTextureCache()->addImage("backKeyImage.png");
	_textureDirLeft = Director::getInstance()->getTextureCache()->addImage("backKeyLeft.png");
	_textureDirRight = Director::getInstance()->getTextureCache()->addImage("backKeyRight.png");


	//方向控制键效果
	_menuShow = Sprite::createWithTexture(_textureDirNone);
	Vec2 menuBGPoint(80, 50);
	_menuShow->setPosition(menuBGPoint);
	_menuShow->setScaleX(2.0);
	addChild(_menuShow);



	auto left1 = Sprite::create();
	auto left2 = Sprite::create();

	left1->setContentSize(Size(_textureDirNone->getContentSize().width / 2, _textureDirNone->getContentSize().height));
	left2->setContentSize(Size(_textureDirNone->getContentSize().width / 2, _textureDirNone->getContentSize().height));
	MenuItemSprite* menuItemLeft = MenuItemSprite::create(left1, left2, std::bind(&SceneGame::moveLeftCallback, this, std::placeholders::_1));




	auto right1 = Sprite::create();
	auto right2 = Sprite::create();
	right1->setContentSize(Size(_textureDirNone->getContentSize().width / 2, _textureDirNone->getContentSize().height));
	right2->setContentSize(Size(_textureDirNone->getContentSize().width / 2, _textureDirNone->getContentSize().height));
	MenuItemSprite* menuItemRight = MenuItemSprite::create(right1, right2, std::bind(&SceneGame::moveRightCallback, this, std::placeholders::_1));

	menuItemLeft->setScaleX(2.0f);
	menuItemRight->setScaleX(2.0f);

	Vec2  menuItemLeftPoint(menuBGPoint.x - winSize.width / 2 - menuItemRight->getBoundingBox().size.width / 2, menuBGPoint.y - winSize.height / 2);
	Vec2  menuItemRightPoint(menuBGPoint.x - winSize.width / 2 + menuItemRight->getBoundingBox().size.width / 2, menuBGPoint.y - winSize.height / 2);

	menuItemLeft->setPosition(menuItemLeftPoint);
	menuItemRight->setPosition(menuItemRightPoint);
	
	dirMenu->addChild(menuItemLeft);
	dirMenu->addChild(menuItemRight);


	/*跳跃键*/
	Menu* jumpMenu = Menu::create();
	addChild(jumpMenu);

	Texture2D* textureAB_normal = Director::getInstance()->getTextureCache()->addImage("AB_normal.png");
	Texture2D* textureAB_select = Director::getInstance()->getTextureCache()->addImage("AB_select.png");
	auto jumpNormal = Sprite::createWithTexture(textureAB_normal);
	
	
	auto jumpSelect = Sprite::createWithTexture(textureAB_select);
	
	MenuItemSprite* jumpItem = MenuItemSprite::create(jumpNormal,
													  jumpSelect,
													  std::bind(&SceneGame::jumpKeyCallback, this, std::placeholders::_1));

	jumpItem->setScale(2.0f);
	jumpItem->setPosition(Vec2(130, -110));
	jumpMenu->addChild(jumpItem);

}
void SceneGame::addMapObjectGroup(){
	//加载地图对象元素
	TMXObjectGroup* objGroup = _map->getObjectGroup("objects");
	ValueVector& values = objGroup->getObjects();

	for (auto& value : values){

		ValueMap &map = value.asValueMap();
		const Value& type = map.at("type");
		if (type.asString() == "BirthPoint"){
			const Value& x = map.at("x");
			const Value& y = map.at("y");

			//创建mario
			_mario = Mario::getInstance();
			_mario->setDead(false);
			_mario->setPosition(Vec2(x.asInt(), y.asInt() - _map->getTileSize().height));
			_mario->setIgnoreAnchorPointForPosition(true);
			_mario->setLocalZOrder(common::ZO_MARIO);
			_mario->updateStatus();
	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  			_map->addChild(_mario);
		}
		else{
			
			Item* item = Item::create(map);
			
			if (item){
				
				switch (item->getType())
				{
				case Item::ItemType::IT_FLAGPOINT:
					_itemFlagpoint = item;
					break;
				case Item::ItemType::IT_FINALPOINT:
					_finalPoint = item;
					break;
				case Item::ItemType::IT_MUSHROOADDLIFE:
					_mushrooms.push_back(item);
				case Item::ItemType::IT_MUSHROOMREWARD:
					_mushrooms.push_back(item);
				default:
					break;
				}

				_map->addChild(item);
			}
		}
	}
}
void SceneGame::addAnimationToCache(){
	//加载蘑菇怪资源
	//加载动画资源
	{
		Animation* animation = myutil::createAnimation("Mushroom0.png", 0, 2,
															  16, 0.1f);
		AnimationCache::getInstance()->addAnimation(animation, "mushroomMoving");

		SpriteFrame* dead1 = myutil::getSpriteFrame("Mushroom0.png", 2, 16);
		SpriteFrame* dead2 = myutil::getSpriteFrame("Mushroom0.png", 2, 16);

		SpriteFrameCache::getInstance()->addSpriteFrame(dead1, "mushroomDead1");
		SpriteFrameCache::getInstance()->addSpriteFrame(dead2, "mushroomDead2");
	}
	{
		//加载乌龟的动画资源
		Animation* animation1 = myutil::createAnimation("tortoise0.png", 2, 2,
															  18, 0.4f);
		AnimationCache::getInstance()->addAnimation(animation1, "tortoiseLeftMoving");

		Animation* animation2 = myutil::createAnimation("tortoise0.png", 4, 2,
														 18, 0.4f);
		AnimationCache::getInstance()->addAnimation(animation2, "tortoiseRightMoving");

		Animation* animation3 = myutil::createAnimation("tortoise0.png", 8, 2,
														  18, 0.4f);
		AnimationCache::getInstance()->addAnimation(animation3, "tortoiseDead");

		SpriteFrame* dead1 = myutil::getSpriteFrame("tortoise0.png", 2, 16);
		SpriteFrame* dead2 = myutil::getSpriteFrame("tortoise0.png", 2, 16);

		
	}

	{
		//加载飞天乌龟的动画资源
		Animation* animation1 = myutil::createAnimation("tortoise0.png", 0, 2,
														  18, 0.4f);
		AnimationCache::getInstance()->addAnimation(animation1, "tortoiseFlyLeft");

		Animation* animation2 = myutil::createAnimation("tortoise0.png", 6, 2,
														  18, 0.4f);
		AnimationCache::getInstance()->addAnimation(animation2, "tortoiseFlyRight");



	}
	{
		//加载花资源
		Animation* animation = myutil::createAnimation("flower0.png", 0, 2,
														 16, 0.5f);
		AnimationCache::getInstance()->addAnimation(animation, "flowerShow");

	}
	{
		SpriteFrame* frame=	myutil::getSpriteFrame("rewardMushroomSet.png", 0, 16);
		SpriteFrameCache::getInstance()->addSpriteFrame(frame, "rewardMushroom");
	}
	{
		
		Animation* animation1 = myutil::createAnimation("flyFishLeft.png", 0, 6,
														  16, 0.4f);
		AnimationCache::getInstance()->addAnimation(animation1, "flyFishLeft");

		Animation* animation2 = myutil::createAnimation("flyFishRight.png", 0, 6,
														  16, 0.4f);
		AnimationCache::getInstance()->addAnimation(animation2, "flyFishRight");


	}
	{
		//boss
		Animation* animation1 = myutil::createAnimation("boss.png", 0, 4,
														  32, 0.4f);
		AnimationCache::getInstance()->addAnimation(animation1, "bossLeft");

		Animation* animation2 = myutil::createAnimation("boss.png", 4, 4,
														  32, 0.4f);
		AnimationCache::getInstance()->addAnimation(animation2, "bossRight");


	}
}
void SceneGame::onEnter(){
	Scene::onEnter();
	//cocos2d-x
	//播放背景音乐
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("OnLand.wma",true);
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5f);
	scheduleUpdate();
	
}
void SceneGame::onExit(){

	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	Scene::onExit();
}
void SceneGame::update(float delta){
	moveMarioCheck(delta);
	marioEatCoinCheck(delta);
	checkMarioTouchPole(delta);
	marioHitSomethingCheck(delta);

	

}
void SceneGame::marioEatCoinCheck(float delta){
	TMXLayer* coinLayer = _map->getLayer("coin");
	if (!coinLayer)
		return;
	Vec2 pts[4];
	Rect rc = _mario->getBoundingBox();
	pts[0] = Vec2(rc.getMinX(), rc.getMinY());
	pts[1] = Vec2(rc.getMinX(), rc.getMaxY());
	pts[2] = Vec2(rc.getMaxX(), rc.getMinY());
	pts[3] = Vec2(rc.getMaxX(), rc.getMaxY());
	for (int i = 0; i < 4; ++i){
		
		Vec2 ptTile = myutil::bLGLPointToTile(pts[i], _map);
		if (ptTile.x < 0 || ptTile.y < 0 || ptTile.y >= _map->getMapSize().height)
			continue;
		int gid = coinLayer->getTileGIDAt(ptTile);
		if (gid){
			//播放吃金币的声音
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("EatCoin.wma");
			coinLayer->setTileGID(0, ptTile);
		}
	}
}
void SceneGame::marioEatHideMushroomCheck(){
	if (_mario->getSpeedY() > 5){

		for (auto mushroom : _mushrooms){
			Rect rect = mushroom->getBoundingBox();
			if (rect.intersectsRect(this->getBoundingBox())){
				//顶到隐藏的蘑菇了
				
			}
		}
	}

	

}
void SceneGame::moveRightCallback(Ref*){
	_menuShow->setTexture(_textureDirRight);
	_menuDir = common::RIGHT;
}
void SceneGame::moveLeftCallback(Ref*){
	_menuShow->setTexture(_textureDirLeft);
	_menuDir = common::LEFT;
}
void SceneGame::moveMarioCheck(float delta){
	
#ifdef WIN32
	short key;
	key = GetKeyState('A');
	if (key < 0)_menuDir = common::LEFT;
	key = GetKeyState('D');
	if (key < 0)_menuDir = common::RIGHT;
	key = GetKeyState('J');
	if (key < 0)jumpKeyCallback(nullptr);
#endif

	common::Direction dir = _menuDir;
	switch (dir)
	{
	
	case common::RIGHT:
	case common::LEFT:
		_mario->setHorizontalSpeed(dir);
		_menuDir = common::NONE;
		break;
	
	case common::NONE:
		_menuShow->setTexture(_textureDirNone);
		_mario->stop();
		break;
	default:
		break;
	}
	//每个帧循环都去往上移动,竖直方向有速度则移动,没速度则静止
	_mario->moveVerticalCheck(delta);

	//每个帧循环都去左右移动,左右方向有速度则移动,没速度则静止
	_mario->moveHorizontalCheck(delta);
	

}
void SceneGame::jumpKeyCallback(Ref*){
	_mario->jump();
}
void SceneGame::fireKeyCallback(Ref*){}
void SceneGame::gameOver(){
	
	Director::getInstance()->replaceScene(SceneStart::create());
}
void SceneGame::checkMarioTouchPole(float dt){
	
	if (_mario->getPositionY() >= _map->getContentSize().height|| _mario->getPositionY()<=0)
		return;

	auto rcMario = _mario->getBoundingBox();
	Vec2 pt = Vec2(rcMario.getMidX(), rcMario.getMaxY());
	Vec2 ptTile = myutil::bLGLPointToTile(pt, _map);
	Rect mapSizeRect(Vec2(0,0),_map->getMapSize()-Size(1,1));
	
	
	if (!mapSizeRect.containsPoint(ptTile))
		return;

	TMXLayer* layer = _map->getLayer("flagpole");
	
	if (layer->getTileGIDAt(ptTile)){
		//碰到旗杆了
		_mario->autoRun();
		unschedule(CC_SCHEDULE_SELECTOR(SceneGame::checkMarioTouchPole));

		//开启是否碰到终点碰撞检测
		schedule(CC_SCHEDULE_SELECTOR(SceneGame::checkMarioTouchEndPointCallback));
	}
}
void SceneGame::checkMarioTouchEndPointCallback(float dt){

	if (_mario->getPositionX() >= _finalPoint->getPositionX()){
		unschedule(CC_SCHEDULE_SELECTOR(SceneGame::checkMarioTouchEndPointCallback));
		
		if (_level < ALL_LEVEL_NUM){
						
			_mario->endAutoRun();
			
			//因为下一个场景需要用到，所以让_mario存储的父亲节点指针置空
			_mario->removeFromParent();
			Director::getInstance()->
				replaceScene(SceneGame::create(_level + 1));
		}
		else{
			
			//全部过关
			Director::getInstance()->
				replaceScene(SceneStart::create());
		}
		
	}
}
void SceneGame::marioHitSomethingCheck(float dt)
{

	if (_mario->getSpeedY() > 0) {
		
		//Mario正在上升
		
		//得到下一帧Mario头顶中间的点
		Vec2 marioPointOfHeadMid(_mario->getBoundingBox().getMidX(),_mario->getBoundingBox().getMaxY()+ _mario->getSpeedY()*dt);
		if (marioPointOfHeadMid.y >= _map->getContentSize().height)
			return;

		Vec2 tileCoordiate;

		//得到该点的地图精灵
		Sprite* spriteInMarioHead = _map->getLayer("block")->getTileAt(tileCoordiate=myutil::bLGLPointToTile(marioPointOfHeadMid,_map));
		
		Sprite* marioHitTarget = nullptr;
		int blockGid=0;
		if (spriteInMarioHead){
			marioHitTarget = spriteInMarioHead;
			blockGid = _map->getLayer("block")->getTileGIDAt(tileCoordiate);
		}
		else {
			//头顶中间的点没顶到，看看头顶左边的点有没有撞到

			//得到下一帧Mario头顶左边的点
			Vec2 marioPointOfHeadLeft(_mario->getBoundingBox().getMinX(), _mario->getBoundingBox().getMaxY() + _mario->getSpeedY()*dt);

			//得到该点的地图精灵
			Sprite* spriteInMarioHeadLeft = _map->getLayer("block")->getTileAt(tileCoordiate=myutil::bLGLPointToTile(marioPointOfHeadMid, _map));
			if (spriteInMarioHeadLeft) {
				//左边撞到了
				blockGid = _map->getLayer("block")->getTileGIDAt(tileCoordiate);
				marioHitTarget = spriteInMarioHeadLeft;
			}
			else
			{
				//左边也没有，那么再看看右边
				//得到下一帧Mario头顶右边的点
				Vec2 marioPointOfHeadLRight(_mario->getBoundingBox().getMaxX(), _mario->getBoundingBox().getMaxY() + _mario->getSpeedY()*dt);

				//得到该点的地图精灵
				Sprite* spriteInMarioHeadRight = _map->getLayer("block")->getTileAt(tileCoordiate=myutil::bLGLPointToTile(marioPointOfHeadMid, _map));
				if (spriteInMarioHeadRight) {
					//右边撞到了
					 blockGid = _map->getLayer("block")->getTileGIDAt(tileCoordiate);
					 marioHitTarget = spriteInMarioHeadRight;
				}
			}
		}
		if (marioHitTarget) {
			//mario顶到砖头了
			common::BlockType blockType = common::getBlockTypeByGid(blockGid);
			marioHitBlockHandle(marioHitTarget, tileCoordiate, blockType);
		}

	}
	
	
}
void SceneGame::marioHitBlockHandle(Sprite* block,const Vec2& tileCoordiate, common::BlockType blockType) {
	//判断砖头的类型
	switch (blockType)
	{
	case common::common:
	{
		
		if(_mario->getState()==Mario::Small){
			//单纯的让砖跳一下
			JumpBy*  by = JumpBy::create(0.15f, Vec2(0, 0), 8, 1);
			block->runAction(Sequence::create(by, nullptr));
		}
		else {
			//砖块破碎
			destroyBlock(block);
		}
		break;
	}
	case common::question:
	{
		marioHitQuestionHandle(block, tileCoordiate);
		break;
	}
	case common::commonGetHard:

		break;
	case common::commonHarfOnRight:

		break;
	case common::commonHarfOnRiLeft:

		break;
	case common::hardHarfOnRight:

		break;
	case common::hardHarfOnRiLeft:

		break;
	case common::Hard:

		break;
	case common::none:

		break;
	default:
		break;
	}

	

}

void SceneGame::marioHitQuestionHandle(Sprite* block, const Vec2& tileCoordiate) {
	//顶到question,，然后在变成普通硬转
	
	_map->getLayer("block")->setTileGID(31, tileCoordiate);

	Rect rcNode = block->getBoundingBox();
	rcNode.size = rcNode.size - Size(1, 1);

	ItemMushroom* mushroom = nullptr;
	for (auto ib = _mushrooms.begin(); ib != _mushrooms.end(); ++ib) {
		Item* item = *ib;
		Rect rcItem = item->getBoundingBox();
		rcItem.size = rcItem.size - Size(1, 1);

		//撞到蘑菇了
		if (rcItem.intersectsRect(rcNode)) {

			mushroom = dynamic_cast<ItemMushroom*>(item);
			if (!mushroom) {
				CCLOG_DYNAMIC_ERR;
				break;
			}
			_mushrooms.erase(ib);
			break;
		}
	}

	if (!mushroom) {
		//没有撞到蘑菇，弹出一个钢镚
		TMXLayer* layer = _map->getLayer("coin");
		Vec2 tileCoordiateCoin = tileCoordiate - Vec2(0, 1);
		layer->setTileGID(GID_COIN, tileCoordiateCoin);
		Sprite* coin = layer->getTileAt(tileCoordiateCoin);
		coin->runAction(Sequence::create(JumpBy::create(.5f,Vec2(0,0),30,1),CallFunc::create(std::bind(&Sprite::removeFromParent, coin)),nullptr));
	}
	

	auto func = std::bind(&SceneGame::marioHitQuestionAnimationEndCallback, this, std::placeholders::_1, mushroom);
	CallFuncN *callfunc = CallFuncN::create(func);
	JumpBy*  by = JumpBy::create(0.15f, Vec2(0, 0), 8, 1);
	block->runAction(Sequence::create(by, callfunc, nullptr));
	
}

void SceneGame::marioHitQuestionAnimationEndCallback(Node* block,ItemMushroom* mushroom) {
	
	if (mushroom)
		mushroom->wakeup();
	
}
void SceneGame::destroyBlock(Sprite* block) {
	SpriteFrame* spriteFrame=block->getSpriteFrame();

	const Rect& rc=spriteFrame->getRect();
	
	Rect rcc (rc);
	rcc.size = rcc.size / 2;

	Texture2D* texture2D = spriteFrame->getTexture();
	SpriteFrame* smallSpriteFrame = SpriteFrame::createWithTexture(texture2D, rcc);
	
	Sprite* sps[4];
	for (size_t i = 0; i < 4; i++)
	{
		_map->addChild(sps[i] = Sprite::createWithSpriteFrame(smallSpriteFrame));
	}
	

	sps[0]->setPosition(block->getPosition() + Vec2(-4,4));

	sps[1]->setPosition(block->getPosition() + Vec2(4,4));
	sps[2]->setPosition(block->getPosition() + Vec2(-4, -4));
	sps[3]->setPosition(block->getPosition() + Vec2(4, -4));
	sps[0]->runAction(Sequence::create(JumpBy::create(0.5f, Vec2(-50, -60), 40, 1), CallFunc::create(std::bind(&Sprite::removeFromParent, sps[0])), nullptr));
	sps[1]->runAction(Sequence::create(JumpBy::create(0.5f, Vec2(50, -60), 40, 1), CallFunc::create(std::bind(&Sprite::removeFromParent, sps[1])), nullptr));
	sps[2]->runAction(Sequence::create(JumpBy::create(0.5f, Vec2(-50, -60), 20, 1), CallFunc::create(std::bind(&Sprite::removeFromParent, sps[2])), nullptr));
	sps[3]->runAction(Sequence::create(JumpBy::create(0.5f, Vec2(50, -60), 20, 1), CallFunc::create(std::bind(&Sprite::removeFromParent, sps[3])), nullptr));

	block->removeFromParent();

}


