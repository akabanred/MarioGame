#ifndef __ItemBoss_H__
#define __ItemBoss_H__
#include "Common.h"
#include "ItemCanMove.h"
class ItemBoss :public ItemCanMove
{
public:
	enum State
	{
		NORMAL,
		DROPPING
	} m_status;
	static ItemBoss* create(ValueMap& map);
	bool init(ValueMap& map);
	virtual void moveCheck(float dt);
	virtual void collisionCheck(float dt);
	virtual void update(float dt);
	void updateStatus();
	void jumpCallback(float dt);

	float m_left;
	float m_right;
};
#endif
