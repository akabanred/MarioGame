#ifndef __SceneGame_H__
#define __SceneGame_H__
#include "Common.h"
#include <set>
class Item;
class Mario;
class ProgressBar;
class ItemMushroom;

class SceneGame:public Scene
{
public:
	static SceneGame* create(int level);
	

	void moveRightCallback(Ref*);
	void moveLeftCallback(Ref*);
	

	void jumpKeyCallback(Ref*);
	void fireKeyCallback(Ref*);
	void checkMarioTouchPole(float dt);
	void checkMarioTouchEndPointCallback(float dt);
	void gameOver();

private:
	bool init(int level);
	void addMap();
	void addCtrlButton();
	void addMapObjectGroup();
	void addAnimationToCache();


	/*override*/
	void onEnter()	override;
	void onExit()	override;
	void update(float delta)override;


	void moveMarioCheck(float delta);
	void marioEatCoinCheck(float delta);
	void marioEatHideMushroomCheck();
	void marioHitSomethingCheck(float dt);
	void marioHitBlockHandle(Sprite* block, const Vec2& tileCoordiate,common::BlockType type);
	void marioHitQuestionAnimationEndCallback(Node* block, ItemMushroom* mushroom);
	void marioHitQuestionHandle(Sprite* block, const Vec2& tileCoordiate);

	void destroyBlock(Sprite* block);
public:
	TMXTiledMap*    _map;
	ProgressBar*    _bar;
	int _level;
	

	Mario		          *_mario;
	Item				  *_itemFlagpoint;
	Item			      *_finalPoint;
	Texture2D             *_textureDirNone;
	Texture2D             *_textureDirRight;
	Texture2D             *_textureDirLeft;

	Sprite                *_menuShow;
	common::Direction      _menuDir;
	std::set<Item*>        _items;
	std::list<Item*>       _mushrooms;
};

#endif