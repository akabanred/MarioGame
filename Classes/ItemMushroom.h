#ifndef __ItemMushroom_H__
#define __ItemMushroom_H__
#include "Common.h"
#include "ItemCanMove.h"
class ItemMushroom :public ItemCanMove
{
public:
	enum State
	{
		SLEEP,
		GROW,
		MOVE
	};
	static ItemMushroom* create(ValueMap& map);
	bool init(ValueMap& map);
	virtual void wakeup();
	void update(float dt)override;

	
	void updateStatus();
	void afterGrowCallback();

	void collisionCheck(float dt);
public:
	State _state;
	
};
#endif
