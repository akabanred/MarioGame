#include "SceneGame.h"
#include "audio/include/AudioEngine.h"
#include "MenuCtrl.h"
#include "SceneStart.h"
#include "Mario.h"
#include "Item.h"
#include "ItemMushroom.h"
#include "ItemBoss.h"
#include "SceneLoadResource.h"

SceneGame *SceneGame::create(int level)
{
	SceneGame *pRet = new SceneGame();
	if (pRet && pRet->init(level))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = nullptr;
	}

	return pRet;
}
bool SceneGame::init(int level)
{
	if (!Scene::initWithPhysics())
		return false;

	auto world = this->getPhysicsWorld();
	CCLOG("SceneGame PhysicsWorld = %p", world);
	world->setGravity(Vec2(0, 0));

	_menuDir = common::NONE;
	_level = level;
	_finalPoint = nullptr;
	_bossSpawned = false;
	_firstUpdate = true;

	addMap();
	addCtrlButton();
	addAnimationToCache();
	addMapObjectGroup();
	return true;
}

void SceneGame::addMap()
{
	char mapName[32];
	myutil::format(mapName, "MarioMap", _level, ".tmx");
	CCLOG("Loading TMX: %s", mapName);
	_map = TMXTiledMap::create(mapName);
	if (!_map)
	{
		CCLOG("ERROR: Cannot load TMX %s. Check Resources path.", mapName);
		return;
	}
	addChild(_map);
	_map->setPosition(_map->getPosition() + Vec2(0, winSize.height - _map->getContentSize().height));
}

void SceneGame::addCtrlButton()
{
	auto labelSetting = Label::createWithTTF("Setting", "fonts/Marker Felt.ttf", 26);
	auto settingItem = MenuItemLabel::create(labelSetting, CC_CALLBACK_1(SceneGame::openSettingMenu, this));
	settingItem->setPosition(Vec2(40, winSize.height - 20));
	labelSetting->setScale(0.8f);

	auto menu = Menu::create(settingItem, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 10);
}

void SceneGame::addMapObjectGroup()
{
	CCLOG("[SceneGame] addMapObjectGroup() begin");

	if (_map->getChildByName("Boss_1") || _map->getChildByName("Boss_Before_Flag"))
	{
		_bossSpawned = true;
	}

	TMXObjectGroup *objGroup = _map->getObjectGroup("objects");
	if (!objGroup)
	{
		CCLOGERROR("[SceneGame] TMX object group 'objects' NOT FOUND!");
		return;
	}

	ValueVector &values = objGroup->getObjects();
	bool foundBossInTMX = false;

	for (auto &v : values)
	{
		ValueMap &dict = v.asValueMap();

		auto itType = dict.find("type");
		if (itType == dict.end())
			continue;

		std::string type = itType->second.asString();
		std::string lowerType = type;
		std::transform(lowerType.begin(), lowerType.end(), lowerType.begin(), ::tolower);

		if (lowerType == "birthpoint")
		{
			int x = dict.count("x") ? dict.at("x").asInt() : 0;
			int y = dict.count("y") ? dict.at("y").asInt() : 0;

			_mario = Mario::getInstance();
			_mario->setDead(false);
			_mario->setPosition(Vec2(static_cast<float>(x), static_cast<float>(y) - _map->getTileSize().height));
			_mario->setIgnoreAnchorPointForPosition(true);
			_mario->setLocalZOrder(common::ZO_MARIO);
			_mario->updateStatus();
			if (_mario->getParent())
				_mario->removeFromParentAndCleanup(true);
			_map->addChild(_mario);
			_mario->setName("Mario");
		}
		else if (lowerType == "boss")
		{
			if (_bossSpawned)
				continue;

			float bx = dict.at("x").asFloat();
			float by = dict.at("y").asFloat();

			if (auto boss = ItemBoss::createBoss({bx, by}))
			{
				boss->setName("Boss_1");
				boss->setLocalZOrder(common::ZO_MARIO - 1);
				boss->setBattleArea(Rect(bx - 200, by - 100, 400, 200));
				_map->addChild(boss);
				boss->start();
				_bossSpawned = true;
				foundBossInTMX = true;
			}
		}
		else
		{
			if (auto item = Item::create(dict))
			{
				switch (item->getType())
				{
				case Item::ItemType::IT_FLAGPOINT:
					_itemFlagpoint = item;
					break;
				case Item::ItemType::IT_FINALPOINT:
					_finalPoint = item;
					break;
				case Item::ItemType::IT_MUSHROOADDLIFE:
				case Item::ItemType::IT_MUSHROOMREWARD:
					_mushrooms.push_back(item);
					break;
				default:
					break;
				}
				_map->addChild(item);
			}
		}
	}

	if (!_bossSpawned && !foundBossInTMX && _finalPoint)
	{
		const float OFFSET_BEFORE_FLAG = 220.f;
		float bx = _finalPoint->getPositionX() - OFFSET_BEFORE_FLAG;
		float by = _finalPoint->getPositionY();

		if (!(_map->getChildByName("Boss_1") || _map->getChildByName("Boss_Before_Flag")))
		{
			if (auto boss = ItemBoss::createBoss({bx, by}))
			{
				boss->setName("Boss_Before_Flag");
				boss->setLocalZOrder(common::ZO_MARIO - 1);
				boss->setBattleArea(Rect(bx - 200, by - 100, 400, 200));
				_map->addChild(boss);
				boss->start();
				_bossSpawned = true;
			}
		}
	}
	CCLOG("[SceneGame] addMapObjectGroup() end");
}

void SceneGame::addAnimationToCache()
{
	AnimationCache *cache = AnimationCache::getInstance();
	cache->addAnimation(myutil::createAnimation("Mushroom0.png", 0, 2, 16, 0.1f), "mushroomMoving");
	SpriteFrameCache::getInstance()->addSpriteFrame(myutil::getSpriteFrame("Mushroom0.png", 2, 16), "mushroomDead1");
	cache->addAnimation(myutil::createAnimation("tortoise0.png", 2, 2, 18, 0.4f), "tortoiseLeftMoving");
	cache->addAnimation(myutil::createAnimation("tortoise0.png", 4, 2, 18, 0.4f), "tortoiseRightMoving");
	cache->addAnimation(myutil::createAnimation("tortoise0.png", 8, 2, 18, 0.4f), "tortoiseDead");
	cache->addAnimation(myutil::createAnimation("tortoise0.png", 0, 2, 18, 0.4f), "tortoiseFlyLeft");
	cache->addAnimation(myutil::createAnimation("tortoise0.png", 6, 2, 18, 0.4f), "tortoiseFlyRight");
	cache->addAnimation(myutil::createAnimation("flower0.png", 0, 2, 16, 0.5f), "flowerShow");
	SpriteFrameCache::getInstance()->addSpriteFrame(myutil::getSpriteFrame("rewardMushroomSet.png", 0, 16), "rewardMushroom");
	cache->addAnimation(myutil::createAnimation("flyFishLeft.png", 0, 6, 16, 0.4f), "flyFishLeft");
	cache->addAnimation(myutil::createAnimation("flyFishRight.png", 0, 6, 16, 0.4f), "flyFishRight");
	cache->addAnimation(myutil::createAnimation("boss.png", 0, 4, 32, 0.4f), "bossLeft");
	cache->addAnimation(myutil::createAnimation("boss.png", 4, 4, 32, 0.4f), "bossRight");
}

void SceneGame::onEnter()
{
	Scene::onEnter();
	AudioEngine::resumeAll();

	CCLOG("Attempting to play background music: %s", SOUND_game_music);
	int audioId = AudioEngine::play2d(SOUND_game_music, true, 0.5f);

	if (audioId == AudioEngine::INVALID_AUDIO_ID)
	{
		CCLOG("ERROR: Failed to play background music.");
	}
	else
	{
		CCLOG("SUCCESS: Background music started with ID: %d", audioId);
	}

	scheduleUpdate();

	auto kb = EventListenerKeyboard::create();
	kb->onKeyPressed = [this](EventKeyboard::KeyCode c, Event *)
	{
		if (c == EventKeyboard::KeyCode::KEY_SPACE)
		{
			if (_mario)
				_mario->fireArrow();
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(kb, this);
}

void SceneGame::onExit()
{
	AudioEngine::stopAll();
	Scene::onExit();
}

void SceneGame::update(float delta)
{
	if (_firstUpdate)
	{
		CCLOG("--- SceneGame::update() is running. ---");
		_firstUpdate = false;
	}
	moveMarioCheck(delta);
	marioEatCoinCheck(delta);
	checkMarioTouchPole(delta);
	marioHitSomethingCheck(delta);
}

void SceneGame::marioEatCoinCheck(float delta)
{
	TMXLayer *coinLayer = _map->getLayer("coin");
	if (!coinLayer)
		return;

	Rect rc = _mario->getBoundingBox();
	Vec2 pts[] = {rc.origin, Vec2(rc.getMinX(), rc.getMaxY()), Vec2(rc.getMaxX(), rc.getMinY()), Vec2(rc.getMaxX(), rc.getMaxY())};

	for (const auto &pt : pts)
	{
		Vec2 ptTile = myutil::bLGLPointToTile(pt, _map);
		if (ptTile.x < 0 || ptTile.y < 0 || ptTile.y >= _map->getMapSize().height)
			continue;

		if (coinLayer->getTileGIDAt(ptTile))
		{
			AudioEngine::play2d(SOUND_eat_coin);
			coinLayer->setTileGID(0, ptTile);
		}
	}
}

void SceneGame::marioEatHideMushroomCheck() {}

void SceneGame::moveRightCallback(Ref *)
{
	_menuDir = common::RIGHT;
}

void SceneGame::moveLeftCallback(Ref *)
{
	_menuDir = common::LEFT;
}

void SceneGame::moveMarioCheck(float delta)
{
#ifdef WIN32
	if (GetKeyState('A') < 0)
		_menuDir = common::LEFT;
	if (GetKeyState('D') < 0)
		_menuDir = common::RIGHT;
	if (GetKeyState('J') < 0)
		jumpKeyCallback(nullptr);
#endif

	switch (_menuDir)
	{
	case common::RIGHT:
	case common::LEFT:
		_mario->setHorizontalSpeed(_menuDir);
		_menuDir = common::NONE;
		break;
	case common::NONE:
		_mario->stop();
		break;
	default:
		break;
	}
	_mario->moveVerticalCheck(delta);
	_mario->moveHorizontalCheck(delta);
}

void SceneGame::jumpKeyCallback(Ref *)
{
	_mario->jump();
}

void SceneGame::fireKeyCallback(Ref *) {}

void SceneGame::gameOver()
{
	Director::getInstance()->replaceScene(SceneStart::create());
}

void SceneGame::checkMarioTouchPole(float dt)
{
	if (!_itemFlagpoint || _mario->getPositionY() >= _map->getContentSize().height || _mario->getPositionY() <= 0)
		return;

	auto rcMario = _mario->getBoundingBox();
	Vec2 pt = Vec2(rcMario.getMidX(), rcMario.getMaxY());
	Vec2 ptTile = myutil::bLGLPointToTile(pt, _map);
	Rect mapSizeRect(Vec2(0, 0), _map->getMapSize() - Size(1, 1));
	TMXLayer* layer = _map->getLayer("flagpole");
	//if (_mario->getBoundingBox().intersectsRect(_itemFlagpoint->getBoundingBox()))
	if (layer->getTileGIDAt(ptTile))
	{
		//_mario->autoRun();
		unschedule(CC_SCHEDULE_SELECTOR(SceneGame::checkMarioTouchPole));
		schedule(CC_SCHEDULE_SELECTOR(SceneGame::checkMarioTouchEndPointCallback));
	}
}

void SceneGame::checkMarioTouchEndPointCallback(float dt)
{
	if (!_finalPoint || _mario->getPositionX() >= _finalPoint->getPositionX())
	{
		unschedule(CC_SCHEDULE_SELECTOR(SceneGame::checkMarioTouchEndPointCallback));
		//_mario->endAutoRun();
		_mario->removeFromParent();

		Scene *nextScene = nullptr;
		int nextLevel = _level + 1;
		if (nextLevel <= ALL_LEVEL_NUM)
		{
			nextScene = SceneGame::create(nextLevel);
		}
		else
		{
			nextScene = SceneStart::create();
		}
		Director::getInstance()->replaceScene(nextScene);
	}
}

void SceneGame::marioHitSomethingCheck(float dt)
{
	if (_mario->getSpeedY() <= 0)
		return;

	Vec2 headMid = Vec2(_mario->getBoundingBox().getMidX(), _mario->getBoundingBox().getMaxY() + _mario->getSpeedY() * dt);
	if (headMid.y >= _map->getContentSize().height)
		return;

	TMXLayer *blockLayer = _map->getLayer("block");
	if (!blockLayer)
		return;

	Vec2 tileCoord = myutil::bLGLPointToTile(headMid, _map);
	Sprite *targetTile = blockLayer->getTileAt(tileCoord);

	if (targetTile)
	{
		int gid = blockLayer->getTileGIDAt(tileCoord);
		marioHitBlockHandle(targetTile, tileCoord, common::getBlockTypeByGid(gid));
	}
}

void SceneGame::marioHitBlockHandle(Sprite *block, const Vec2 &tileCoordiate, common::BlockType blockType)
{
	switch (blockType)
	{
	case common::common:
		if (_mario->getState() == Mario::Small)
		{
			block->runAction(JumpBy::create(0.15f, Vec2::ZERO, 8, 1));
		}
		else
		{
			destroyBlock(block);
		}
		break;
	case common::question:
		marioHitQuestionHandle(block, tileCoordiate);
		break;
	default:
		break;
	}
}

void SceneGame::marioHitQuestionHandle(Sprite *block, const Vec2 &tileCoordiate)
{
	_map->getLayer("block")->setTileGID(31, tileCoordiate);

	Rect rcNode = block->getBoundingBox();
	rcNode.size = rcNode.size - Size(1, 1);

	ItemMushroom *mushroom = nullptr;
	for (auto it = _mushrooms.begin(); it != _mushrooms.end(); ++it)
	{
		if ((*it)->getBoundingBox().intersectsRect(rcNode))
		{
			mushroom = dynamic_cast<ItemMushroom *>(*it);
			if (mushroom)
			{
				_mushrooms.erase(it);
				break;
			}
		}
	}

	if (!mushroom)
	{
		AudioEngine::play2d(SOUND_eat_coin);
		if (TMXLayer *layer = _map->getLayer("coin"))
		{
			Vec2 coinCoord = tileCoordiate - Vec2(0, 1);
			layer->setTileGID(GID_COIN, coinCoord);
			if (Sprite *coin = layer->getTileAt(coinCoord))
			{
				coin->runAction(Sequence::create(JumpBy::create(0.5f, Vec2::ZERO, 30, 1), RemoveSelf::create(), nullptr));
			}
		}
	}

	auto func = std::bind(&SceneGame::marioHitQuestionAnimationEndCallback, this, std::placeholders::_1, mushroom);
	block->runAction(Sequence::create(JumpBy::create(0.15f, Vec2::ZERO, 8, 1), CallFuncN::create(func), nullptr));
}

void SceneGame::marioHitQuestionAnimationEndCallback(Node *block, ItemMushroom *mushroom)
{
	if (mushroom)
	{
		AudioEngine::play2d(SOUND_mushroom_up);
		mushroom->wakeup();
	}
}

void SceneGame::destroyBlock(Sprite *block)
{
	if (!block->getSpriteFrame())
		return;

	const Rect &rc = block->getSpriteFrame()->getRect();
	Rect rcc(rc.origin, rc.size / 2);
	Texture2D *texture = block->getSpriteFrame()->getTexture();

	for (int i = 0; i < 4; ++i)
	{
		rcc.origin = rc.origin + Vec2(static_cast<float>(i % 2) * rcc.size.width, static_cast<float>(i / 2) * rcc.size.height);
		auto smallFrame = SpriteFrame::createWithTexture(texture, rcc);
		auto sp = Sprite::createWithSpriteFrame(smallFrame);
		_map->addChild(sp);
		sp->setPosition(block->getPosition() + Vec2((i % 2 * 2 - 1) * 4.0f, (i / 2 * 2 - 1) * 4.0f));

		float jumpX = (i % 2 * 2 - 1) * 50.0f;
		float jumpY = (i < 2) ? 40.0f : 20.0f;
		sp->runAction(Sequence::create(JumpBy::create(0.5f, Vec2(jumpX, -60), jumpY, 1), RemoveSelf::create(), nullptr));
	}
	AudioEngine::play2d(SOUND_break_brick);
	block->removeFromParent();
}

void SceneGame::openSettingMenu(Ref *sender)
{
	Director::getInstance()->pause();
	auto layerColor = LayerColor::create(Color4B(0, 0, 0, 150));
	layerColor->setName("PauseLayer");
	this->addChild(layerColor, 20);

	auto resumeItem = MenuItemLabel::create(Label::createWithTTF("Resume", "fonts/Marker Felt.ttf", 28), CC_CALLBACK_1(SceneGame::resumeGameCallback, this));
	auto quitItem = MenuItemLabel::create(Label::createWithTTF("Quit", "fonts/Marker Felt.ttf", 28), CC_CALLBACK_1(SceneGame::quitGameCallback, this));
	std::string soundText = isSoundOnn ? "Sound: ON" : "Sound: OFF";
	auto soundItem = MenuItemLabel::create(Label::createWithTTF(soundText, "fonts/Marker Felt.ttf", 28), CC_CALLBACK_1(SceneGame::toggleSoundCallback, this));

	auto menu = Menu::create(resumeItem, soundItem, quitItem, nullptr);
	menu->alignItemsVerticallyWithPadding(15);
	menu->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	layerColor->addChild(menu);
}

void SceneGame::resumeGameCallback(Ref *sender)
{
	this->removeChildByName("PauseLayer");
	Director::getInstance()->resume();
}

void SceneGame::quitGameCallback(Ref *sender)
{
	Director::getInstance()->resume();
	Director::getInstance()->replaceScene(SceneLoadResource::createScene());
}

void SceneGame::toggleSoundCallback(Ref *sender)
{
	isSoundOnn = !isSoundOnn;

	if (isSoundOnn)
	{
		AudioEngine::resumeAll();
	}
	else
	{
		AudioEngine::pauseAll();
	}

	if (auto layer = this->getChildByName("PauseLayer"))
	{
		layer->removeFromParent();
		openSettingMenu(nullptr);
	}
}
