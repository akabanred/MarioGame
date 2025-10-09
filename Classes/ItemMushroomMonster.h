#ifndef __ITEMMUSHROOM_H__
#define __ITEMMUSHROOM_H__
#include "Common.h"
#include "ItemCanMove.h"
class ItemMushroomMonster :public ItemCanMove
{
public:
	
	static ItemMushroomMonster* create(ValueMap& map);
	bool init(ValueMap& map);
	virtual void collisionCheck(float dt);

protected:
	
	
	bool _isDead;
};
#endif
