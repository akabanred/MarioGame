#include "SceneGame.h"
#include "SimpleAudioEngine.h"
#include "MenuCtrl.h"
#include "SceneStart.h"
#include "Mario.h"
#include "Item.h"
#include "ItemMushroom.h"
#include "ItemBoss.h"
#include "SceneLoadResource.h"
//USING_NS_CC;
SceneGame* SceneGame::create(int level) {
	SceneGame* pRet = new SceneGame();
	if (pRet && pRet->init(level)) {
		pRet->autorelease();
	}
	else {
		delete pRet;
		pRet = nullptr;
	}

	return pRet;
}
bool SceneGame::init(int level)
{
	// PHẢI dùng initWithPhysics
	if (!Scene::initWithPhysics()) return false;

	auto world = this->getPhysicsWorld();
	CCLOG("SceneGame PhysicsWorld = %p", world);
	world->setGravity(Vec2(0, 0));                // hoặc (0,-980) tuỳ bạn
	// world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	_menuDir = common::NONE;
	_level = level;
	_finalPoint = nullptr;
	_bossSpawned = false;
	_isAutoRunning = false;
	_hasTouchedPole = false;
	addMap();
	addCtrlButton();
	addAnimationToCache();
	addMapObjectGroup();
	return true;
}





void SceneGame::addMap() {
	char mapName[32];
	myutil::format(mapName, "MarioMap", _level, ".tmx");
	CCLOG("Loading TMX: %s", mapName);
	_map = TMXTiledMap::create(mapName);
	if (!_map) {
		CCLOG("ERROR: Cannot load TMX %s. Check Resources path.", mapName);
		return; // để thấy fallback ở AppDelegate
	}
	addChild(_map);
	_map->setPosition(_map->getPosition() + Vec2(0, winSize.height - _map->getContentSize().height));

}
void SceneGame::addCtrlButton() {
	////���ӿ�����屳��
	//Sprite* crtlBG = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("controlUI.png"));
	//crtlBG->setPosition(Vec2(0, 0));
	//crtlBG->setAnchorPoint(Vec2(0, 0));
	//addChild(crtlBG);

	////�Զ���˵���ʵ�������������
	//MenuCtrl* dirMenu = MenuCtrl::create();
	//addChild(dirMenu);

	//_textureDirNone = Director::getInstance()->getTextureCache()->addImage("backKeyImage.png");
	//_textureDirLeft = Director::getInstance()->getTextureCache()->addImage("backKeyLeft.png");
	//_textureDirRight = Director::getInstance()->getTextureCache()->addImage("backKeyRight.png");


	////������Ƽ�Ч��
	//_menuShow = Sprite::createWithTexture(_textureDirNone);
	//Vec2 menuBGPoint(80, 50);
	//_menuShow->setPosition(menuBGPoint);
	//_menuShow->setScaleX(2.0);
	//addChild(_menuShow);



	//auto left1 = Sprite::create();
	//auto left2 = Sprite::create();

	//left1->setContentSize(Size(_textureDirNone->getContentSize().width / 2, _textureDirNone->getContentSize().height));
	//left2->setContentSize(Size(_textureDirNone->getContentSize().width / 2, _textureDirNone->getContentSize().height));
	//MenuItemSprite* menuItemLeft = MenuItemSprite::create(left1, left2, std::bind(&SceneGame::moveLeftCallback, this, std::placeholders::_1));




	//auto right1 = Sprite::create();
	//auto right2 = Sprite::create();
	//right1->setContentSize(Size(_textureDirNone->getContentSize().width / 2, _textureDirNone->getContentSize().height));
	//right2->setContentSize(Size(_textureDirNone->getContentSize().width / 2, _textureDirNone->getContentSize().height));
	//MenuItemSprite* menuItemRight = MenuItemSprite::create(right1, right2, std::bind(&SceneGame::moveRightCallback, this, std::placeholders::_1));

	//menuItemLeft->setScaleX(2.0f);
	//menuItemRight->setScaleX(2.0f);

	//Vec2  menuItemLeftPoint(menuBGPoint.x - winSize.width / 2 - menuItemRight->getBoundingBox().size.width / 2, menuBGPoint.y - winSize.height / 2);
	//Vec2  menuItemRightPoint(menuBGPoint.x - winSize.width / 2 + menuItemRight->getBoundingBox().size.width / 2, menuBGPoint.y - winSize.height / 2);

	//menuItemLeft->setPosition(menuItemLeftPoint);
	//menuItemRight->setPosition(menuItemRightPoint);

	////dirMenu->addChild(menuItemLeft);
	////dirMenu->addChild(menuItemRight);


	///*��Ծ��*/
	//Menu* jumpMenu = Menu::create();
	////addChild(jumpMenu);

	//Texture2D* textureAB_normal = Director::getInstance()->getTextureCache()->addImage("AB_normal.png");
	//Texture2D* textureAB_select = Director::getInstance()->getTextureCache()->addImage("AB_select.png");
	//auto jumpNormal = Sprite::createWithTexture(textureAB_normal);


	//auto jumpSelect = Sprite::createWithTexture(textureAB_select);

	//MenuItemSprite* jumpItem = MenuItemSprite::create(jumpNormal,
	//	jumpSelect,
	//	std::bind(&SceneGame::jumpKeyCallback, this, std::placeholders::_1));

	//jumpItem->setScale(2.0f);
	//jumpItem->setPosition(Vec2(130, -110));
	//jumpMenu->addChild(jumpItem);

	// === Nút Setting ===
	auto labelSetting = Label::createWithTTF("Setting", "fonts/Marker Felt.ttf", 26);
	auto settingItem = MenuItemLabel::create(labelSetting, CC_CALLBACK_1(SceneGame::openSettingMenu, this));
	settingItem->setPosition(Vec2(40, winSize.height - 20)); // góc trái trên
	labelSetting->setScale(0.8f);

	auto menu = Menu::create(settingItem, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 10);

}
void SceneGame::addMapObjectGroup() {
	CCLOG("[SceneGame] addMapObjectGroup() begin");

	// Guard cấp 0: nếu boss node đã tồn tại
	if (_map->getChildByName("Boss_1") || _map->getChildByName("Boss_Before_Flag")) {
		CCLOG("[SceneGame] Boss node already exists -> _bossSpawned=true (pre-guard)");
		_bossSpawned = true;
	}

	TMXObjectGroup* objGroup = _map->getObjectGroup("objects");
	if (!objGroup) {
		CCLOGERROR("[SceneGame] TMX object group 'objects' NOT FOUND!");
		return;
	}

	ValueVector& values = objGroup->getObjects();
	CCLOG("[SceneGame] objects count = %d", (int)values.size());

	bool foundBossInTMX = false;

	for (auto& v : values) {
		ValueMap& dict = v.asValueMap();

		auto itType = dict.find("type");
		if (itType == dict.end()) {
			CCLOGERROR("[SceneGame] object missing 'type' -> skip");
			continue;
		}
		std::string type = itType->second.asString();

		// Chuẩn hoá: dùng chữ thường để so dễ
		std::string lowerType = type;
		std::transform(lowerType.begin(), lowerType.end(), lowerType.begin(), ::tolower);

		if (lowerType == "birthpoint") {
			int x = dict.count("x") ? dict.at("x").asInt() : 0;
			int y = dict.count("y") ? dict.at("y").asInt() : 0;

			_mario = Mario::getInstance();
			_mario->setDead(false);
			_mario->setPosition(Vec2(static_cast<float>(x), static_cast<float>(y) - _map->getTileSize().height)); // căn chân như cũ
			_mario->setIgnoreAnchorPointForPosition(true);	
			_mario->setLocalZOrder(common::ZO_MARIO);
			_mario->updateStatus();
			if (_mario->getParent()) _mario->removeFromParentAndCleanup(true);
			_map->addChild(_mario);
			_mario->setName("Mario");
			CCLOG("[SceneGame] Mario spawned at (%.1f, %.1f)", _mario->getPositionX(), _mario->getPositionY());
		}
		else if (lowerType == "boss") {
			// Guard cấp 1: nếu đã spawn rồi -> bỏ qua mọi boss khác trong TMX
			if (_bossSpawned) {
				CCLOG("[SceneGame] Skip extra TMX boss (already spawned)");
				continue;
			}

			float bx = dict.at("x").asFloat();
			float by = dict.at("y").asFloat(); // căn chân như Mario

			if (auto boss = ItemBoss::createBoss({ bx, by })) {
				boss->setName("Boss_1");
				boss->setLocalZOrder(common::ZO_MARIO - 1);
				boss->setBattleArea(Rect(bx - 200, by - 100, 400, 200));
				_map->addChild(boss);
				boss->start();
				_bossSpawned = true;
				foundBossInTMX = true;
				CCLOG("[SceneGame] Boss spawned from TMX at (%.1f, %.1f)", bx, by);
			}
			else {
				CCLOGERROR("[SceneGame] createBoss failed for TMX boss");
			}
		}
		else {
			// Các item thường
			if (auto item = Item::create(dict)) {
				switch (item->getType()) {
				case Item::ItemType::IT_FLAGPOINT:  _itemFlagpoint = item; break;
				case Item::ItemType::IT_FINALPOINT: _finalPoint = item; break;
				case Item::ItemType::IT_MUSHROOADDLIFE:
				case Item::ItemType::IT_MUSHROOMREWARD:
					_mushrooms.push_back(item); break;
				default: break;
				}
				_map->addChild(item);
			}
		}
	}

	// Fallback: nếu TMX KHÔNG có boss, spawn 1 con trước cột cờ
	if (!_bossSpawned && !foundBossInTMX && _finalPoint) {
		const float OFFSET_BEFORE_FLAG = 220.f;
		float bx = _finalPoint->getPositionX() - OFFSET_BEFORE_FLAG;
		float by = _finalPoint->getPositionY();

		// Guard cấp 2: check lại theo tên node (phòng khi ở đâu đó đã add)
		if (_map->getChildByName("Boss_1") || _map->getChildByName("Boss_Before_Flag")) {
			CCLOG("[SceneGame] Skip fallback boss: boss node already exists");
			_bossSpawned = true;
		}
		else {
			CCLOG("[SceneGame] Try spawn BOSS before flag at (%.1f, %.1f)", bx, by);
			if (auto boss = ItemBoss::createBoss({ bx, by })) {
				boss->setName("Boss_Before_Flag");
				boss->setLocalZOrder(common::ZO_MARIO - 1);
				boss->setBattleArea(Rect(bx - 200, by - 100, 400, 200));
				_map->addChild(boss);
				boss->start();
				_bossSpawned = true;
				CCLOG("[SceneGame] Boss-before-flag spawned OK");
			}
			else {
				CCLOGERROR("[SceneGame] Boss-before-flag createBoss FAILED");
			}
		}
	}
	else {
		CCLOG("[SceneGame] No fallback spawn. _bossSpawned=%s, foundBossInTMX=%s, _finalPoint=%s",
			_bossSpawned ? "true" : "false",
			foundBossInTMX ? "true" : "false",
			_finalPoint ? "valid" : "null");
	}

	CCLOG("[SceneGame] addMapObjectGroup() end");
}







void SceneGame::addAnimationToCache() {
	//����Ģ������Դ
	//���ض�����Դ
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
		//�����ڹ�Ķ�����Դ
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
		//���ط����ڹ�Ķ�����Դ
		Animation* animation1 = myutil::createAnimation("tortoise0.png", 0, 2,
			18, 0.4f);
		AnimationCache::getInstance()->addAnimation(animation1, "tortoiseFlyLeft");

		Animation* animation2 = myutil::createAnimation("tortoise0.png", 6, 2,
			18, 0.4f);
		AnimationCache::getInstance()->addAnimation(animation2, "tortoiseFlyRight");



	}
	{
		//���ػ���Դ
		Animation* animation = myutil::createAnimation("flower0.png", 0, 2,
			16, 0.5f);
		AnimationCache::getInstance()->addAnimation(animation, "flowerShow");

	}
	{
		SpriteFrame* frame = myutil::getSpriteFrame("rewardMushroomSet.png", 0, 16);
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
void SceneGame::onEnter() {
	Scene::onEnter();
	//cocos2d-x
	//���ű�������
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("OnLand.wma", true);
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5f);
	scheduleUpdate();

	auto kb = EventListenerKeyboard::create();
	kb->onKeyPressed = [this](EventKeyboard::KeyCode c, Event*) {
		if (c == EventKeyboard::KeyCode::KEY_SPACE) {
			if (_mario) _mario->fireArrow();
		}
		};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(kb, this);


}
void SceneGame::onExit() {

	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	Scene::onExit();
}
void SceneGame::update(float delta) {
	moveMarioCheck(delta);
	marioEatCoinCheck(delta);
	checkMarioTouchPole(delta);
	marioHitSomethingCheck(delta);



}
void SceneGame::marioEatCoinCheck(float delta) {
	TMXLayer* coinLayer = _map->getLayer("coin");
	if (!coinLayer)
		return;
	Vec2 pts[4];
	Rect rc = _mario->getBoundingBox();
	pts[0] = Vec2(rc.getMinX(), rc.getMinY());
	pts[1] = Vec2(rc.getMinX(), rc.getMaxY());
	pts[2] = Vec2(rc.getMaxX(), rc.getMinY());
	pts[3] = Vec2(rc.getMaxX(), rc.getMaxY());
	for (int i = 0; i < 4; ++i) {

		Vec2 ptTile = myutil::bLGLPointToTile(pts[i], _map);
		if (ptTile.x < 0 || ptTile.y < 0 || ptTile.y >= _map->getMapSize().height)
			continue;
		int gid = coinLayer->getTileGIDAt(ptTile);
		if (gid) {
			//���ųԽ�ҵ�����
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("EatCoin.wma");
			coinLayer->setTileGID(0, ptTile);
		}
	}
}
void SceneGame::marioEatHideMushroomCheck() {
	if (_mario->getSpeedY() > 5) {

		for (auto mushroom : _mushrooms) {
			Rect rect = mushroom->getBoundingBox();
			if (rect.intersectsRect(this->getBoundingBox())) {
				//�������ص�Ģ����

			}
		}
	}



}
void SceneGame::moveRightCallback(Ref*) {
	_menuShow->setTexture(_textureDirRight);
	_menuDir = common::RIGHT;
}
void SceneGame::moveLeftCallback(Ref*) {
	_menuShow->setTexture(_textureDirLeft);
	_menuDir = common::LEFT;
}
void SceneGame::moveMarioCheck(float delta) {
	if (_isAutoRunning) {
		return;
	}

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
		//_menuShow->setTexture(_textureDirNone);
		_mario->stop();
		break;
	default:
		break;
	}
	//ÿ��֡ѭ����ȥ�����ƶ�,��ֱ�������ٶ����ƶ�,û�ٶ���ֹ
	_mario->moveVerticalCheck(delta);

	//ÿ��֡ѭ����ȥ�����ƶ�,���ҷ������ٶ����ƶ�,û�ٶ���ֹ
	_mario->moveHorizontalCheck(delta);


}
void SceneGame::jumpKeyCallback(Ref*) {
	_mario->jump();
}
void SceneGame::fireKeyCallback(Ref*) {}
void SceneGame::gameOver() {

	Director::getInstance()->replaceScene(SceneStart::create());
}
void SceneGame::checkMarioTouchPole(float dt) {

	if (_mario->getPositionY() >= _map->getContentSize().height || _mario->getPositionY() <= 0)
		return;

	auto rcMario = _mario->getBoundingBox();
	Vec2 pt = Vec2(rcMario.getMidX(), rcMario.getMaxY());
	Vec2 ptTile = myutil::bLGLPointToTile(pt, _map);
	Rect mapSizeRect(Vec2(0, 0), _map->getMapSize() - Size(1, 1));


	if (!mapSizeRect.containsPoint(ptTile))
		return;

	TMXLayer* layer = _map->getLayer("flagpole");

	if (layer->getTileGIDAt(ptTile)) {
		//���������
		//_isAutoRunning = true;
		//_mario->autoRun();
		unschedule(CC_SCHEDULE_SELECTOR(SceneGame::checkMarioTouchPole));

		//�����Ƿ������յ���ײ���
		schedule(CC_SCHEDULE_SELECTOR(SceneGame::checkMarioTouchEndPointCallback));
	}
}
void SceneGame::checkMarioTouchEndPointCallback(float dt) {

	if (_mario->getPositionX() >= _finalPoint->getPositionX()) {
		unschedule(CC_SCHEDULE_SELECTOR(SceneGame::checkMarioTouchEndPointCallback));

		if (_level < ALL_LEVEL_NUM) {

			/*_mario->endAutoRun();
			_isAutoRunning = false;*/

			//��Ϊ��һ��������Ҫ�õ���������_mario�洢�ĸ��׽ڵ�ָ���ÿ�
			_mario->removeFromParent();
			Director::getInstance()->replaceScene(SceneGame::create(_level + 1));
		}
		else {

			//ȫ������
			Director::getInstance()->replaceScene(SceneStart::create());
		}

	}
}
void SceneGame::marioHitSomethingCheck(float dt)
{

	if (_mario->getSpeedY() > 0) {

		//Mario��������

		//�õ���һ֡Marioͷ���м�ĵ�
		Vec2 marioPointOfHeadMid(_mario->getBoundingBox().getMidX(), _mario->getBoundingBox().getMaxY() + _mario->getSpeedY() * dt);
		if (marioPointOfHeadMid.y >= _map->getContentSize().height)
			return;

		Vec2 tileCoordiate;

		//�õ��õ�ĵ�ͼ����
		Sprite* spriteInMarioHead = _map->getLayer("block")->getTileAt(tileCoordiate = myutil::bLGLPointToTile(marioPointOfHeadMid, _map));

		Sprite* marioHitTarget = nullptr;
		int blockGid = 0;
		if (spriteInMarioHead) {
			marioHitTarget = spriteInMarioHead;
			blockGid = _map->getLayer("block")->getTileGIDAt(tileCoordiate);
		}
		else {
			//ͷ���м�ĵ�û����������ͷ����ߵĵ���û��ײ��

			//�õ���һ֡Marioͷ����ߵĵ�
			Vec2 marioPointOfHeadLeft(_mario->getBoundingBox().getMinX(), _mario->getBoundingBox().getMaxY() + _mario->getSpeedY() * dt);

			//�õ��õ�ĵ�ͼ����
			Sprite* spriteInMarioHeadLeft = _map->getLayer("block")->getTileAt(tileCoordiate = myutil::bLGLPointToTile(marioPointOfHeadMid, _map));
			if (spriteInMarioHeadLeft) {
				//���ײ����
				blockGid = _map->getLayer("block")->getTileGIDAt(tileCoordiate);
				marioHitTarget = spriteInMarioHeadLeft;
			}
			else
			{
				//���Ҳû�У���ô�ٿ����ұ�
				//�õ���һ֡Marioͷ���ұߵĵ�
				Vec2 marioPointOfHeadLRight(_mario->getBoundingBox().getMaxX(), _mario->getBoundingBox().getMaxY() + _mario->getSpeedY() * dt);

				//�õ��õ�ĵ�ͼ����
				Sprite* spriteInMarioHeadRight = _map->getLayer("block")->getTileAt(tileCoordiate = myutil::bLGLPointToTile(marioPointOfHeadMid, _map));
				if (spriteInMarioHeadRight) {
					//�ұ�ײ����
					blockGid = _map->getLayer("block")->getTileGIDAt(tileCoordiate);
					marioHitTarget = spriteInMarioHeadRight;
				}
			}
		}
		if (marioHitTarget) {
			//mario����שͷ��
			common::BlockType blockType = common::getBlockTypeByGid(blockGid);
			marioHitBlockHandle(marioHitTarget, tileCoordiate, blockType);
		}

	}


}
void SceneGame::marioHitBlockHandle(Sprite* block, const Vec2& tileCoordiate, common::BlockType blockType) {
	//�ж�שͷ������
	switch (blockType)
	{
	case common::common:
	{

		if (_mario->getState() == Mario::Small) {
			//��������ש��һ��
			JumpBy* by = JumpBy::create(0.15f, Vec2(0, 0), 8, 1);
			block->runAction(Sequence::create(by, nullptr));
		}
		else {
			//ש������
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
	//����question,��Ȼ���ڱ����ͨӲت

	_map->getLayer("block")->setTileGID(31, tileCoordiate);

	Rect rcNode = block->getBoundingBox();
	rcNode.size = rcNode.size - Size(1, 1);

	ItemMushroom* mushroom = nullptr;
	for (auto ib = _mushrooms.begin(); ib != _mushrooms.end(); ++ib) {
		Item* item = *ib;
		Rect rcItem = item->getBoundingBox();
		rcItem.size = rcItem.size - Size(1, 1);

		//ײ��Ģ����
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
		//û��ײ��Ģ��������һ�����G
		TMXLayer* layer = _map->getLayer("coin");
		Vec2 tileCoordiateCoin = tileCoordiate - Vec2(0, 1);
		layer->setTileGID(GID_COIN, tileCoordiateCoin);
		Sprite* coin = layer->getTileAt(tileCoordiateCoin);
		coin->runAction(Sequence::create(JumpBy::create(.5f, Vec2(0, 0), 30, 1), CallFunc::create(std::bind(&Sprite::removeFromParent, coin)), nullptr));
	}


	auto func = std::bind(&SceneGame::marioHitQuestionAnimationEndCallback, this, std::placeholders::_1, mushroom);
	CallFuncN* callfunc = CallFuncN::create(func);
	JumpBy* by = JumpBy::create(0.15f, Vec2(0, 0), 8, 1);
	block->runAction(Sequence::create(by, callfunc, nullptr));

}

void SceneGame::marioHitQuestionAnimationEndCallback(Node* block, ItemMushroom* mushroom) {

	if (mushroom)
		mushroom->wakeup();

}
void SceneGame::destroyBlock(Sprite* block) {
	SpriteFrame* spriteFrame = block->getSpriteFrame();

	const Rect& rc = spriteFrame->getRect();

	Rect rcc(rc);
	rcc.size = rcc.size / 2;

	Texture2D* texture2D = spriteFrame->getTexture();
	SpriteFrame* smallSpriteFrame = SpriteFrame::createWithTexture(texture2D, rcc);

	Sprite* sps[4];
	for (size_t i = 0; i < 4; i++)
	{
		_map->addChild(sps[i] = Sprite::createWithSpriteFrame(smallSpriteFrame));
	}


	sps[0]->setPosition(block->getPosition() + Vec2(-4, 4));

	sps[1]->setPosition(block->getPosition() + Vec2(4, 4));
	sps[2]->setPosition(block->getPosition() + Vec2(-4, -4));
	sps[3]->setPosition(block->getPosition() + Vec2(4, -4));
	sps[0]->runAction(Sequence::create(JumpBy::create(0.5f, Vec2(-50, -60), 40, 1), CallFunc::create(std::bind(&Sprite::removeFromParent, sps[0])), nullptr));
	sps[1]->runAction(Sequence::create(JumpBy::create(0.5f, Vec2(50, -60), 40, 1), CallFunc::create(std::bind(&Sprite::removeFromParent, sps[1])), nullptr));
	sps[2]->runAction(Sequence::create(JumpBy::create(0.5f, Vec2(-50, -60), 20, 1), CallFunc::create(std::bind(&Sprite::removeFromParent, sps[2])), nullptr));
	sps[3]->runAction(Sequence::create(JumpBy::create(0.5f, Vec2(50, -60), 20, 1), CallFunc::create(std::bind(&Sprite::removeFromParent, sps[3])), nullptr));

	block->removeFromParent();

}
void SceneGame::openSettingMenu(Ref* sender) {
	// Tạm dừng game
	Director::getInstance()->pause();

	// Tạo lớp nền mờ
	auto layerColor = LayerColor::create(Color4B(0, 0, 0, 150));
	layerColor->setName("PauseLayer"); // để dễ xóa sau
	this->addChild(layerColor, 20);

	// === Nút Resume ===
	auto labelResume = Label::createWithTTF("Resume", "fonts/Marker Felt.ttf", 28);
	auto resumeItem = MenuItemLabel::create(labelResume, CC_CALLBACK_1(SceneGame::resumeGameCallback, this));

	// === Nút Quit ===
	auto labelQuit = Label::createWithTTF("Quit", "fonts/Marker Felt.ttf", 28);
	auto quitItem = MenuItemLabel::create(labelQuit, CC_CALLBACK_1(SceneGame::quitGameCallback, this));

	// === Nút Bật/Tắt âm thanh ===
	std::string soundText = isSoundOnn ? "Sound: ON" : "Sound: OFF";
	auto labelSound = Label::createWithTTF(soundText, "fonts/Marker Felt.ttf", 28);
	auto soundItem = MenuItemLabel::create(labelSound, CC_CALLBACK_1(SceneGame::toggleSoundCallback, this));

	// Tạo menu
	auto menu = Menu::create(resumeItem, soundItem, quitItem, nullptr);
	menu->alignItemsVerticallyWithPadding(15);
	menu->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	layerColor->addChild(menu);
}

void SceneGame::resumeGameCallback(Ref* sender) {
	this->removeChildByName("PauseLayer");
	Director::getInstance()->resume();
}

void SceneGame::quitGameCallback(Ref* sender) {
	Director::getInstance()->resume(); // tránh bị pause ở scene mới
	Director::getInstance()->replaceScene(SceneLoadResource::createScene());
}

void SceneGame::toggleSoundCallback(Ref* sender) {
	isSoundOnn = !isSoundOnn;
	// Cập nhật lại chữ
	auto layer = this->getChildByName("PauseLayer");
	if (layer) {
		layer->removeFromParent();
		openSettingMenu(nullptr); // mở lại menu với chữ cập nhật
	}

	// Ở đây bạn có thể thêm code bật/tắt âm thật, ví dụ:
	// if (isSoundOnn) SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	// else SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}