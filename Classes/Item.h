#ifndef __ITEM_H__
#define __ITEM_H__
#include "Common.h"
#include <set>
#include <string>
class ItemFlagpoint;
class ItemMushroom;
class Mario;
class ItemTortoiseRound;

class Item:public Sprite
{
public:
	enum ItemType
	{
		IT_MushroomMonster,
		IT_TORTOISE,
		IT_FLOWER,
		IT_MUSHROOMREWARD,
		IT_MUSHROOADDLIFE,
		IT_FLAGPOINT,
		IT_FINALPOINT,
		IT_ladderLR,
		IT_flyfish,
		IT_tortoise_round,
		IT_tortoise_fly,
		IT_fire_string,
		IT_boss,
		IT_bullet,
		IT_BridgeStartPos,
		IT_BOSS, 
	};
	
	static Item* create(ValueMap &map);
	bool init();
	void onEnter()override;
	void onExit()override;

	
	virtual void collisionCheck(float dt){};
	
	void update(float dt);

	void setPositionByProperty(const ValueMap&);
	bool isAppearInWindow();
	bool isOutOfWindow();
	TMXTiledMap* getMap();
	virtual void autoDropFlag();
	
	void runAnimation(const char* name);


	CC_SYNTHESIZE(ItemType, _type, Type);
	
};

#endif