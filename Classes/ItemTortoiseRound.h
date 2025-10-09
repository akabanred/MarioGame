#ifndef __ItemTortoiseRound_H__
#define __ItemTortoiseRound_H__
#include "Common.h"
#include "ItemCanMove.h"
class ItemTortoiseRound :public ItemCanMove
{
public:
	enum State
	{
		NORMAL,
		SLEEP,
		CRAZY
	};
	static ItemTortoiseRound* create(ValueMap& map);
	bool init(ValueMap& map);

	virtual void moveCheck(float dt);
	virtual void collisionCheck(float dt);
	void updateStatus();

	void autoReLiveCallback(float dt);
	void cancelGodModeCallback(float dt);
	int m_initXV;
	int m_roundDis;
	
	int m_state;
	bool _bIsGodMode;
};
#endif
