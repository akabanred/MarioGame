#ifndef __ItemTortoise_H__
#define __ItemTortoise_H__
#include "Common.h"
#include "ItemCanMove.h"
class ItemTortoise :public ItemCanMove
{
public:
	enum State
	{
		NORMAL,
		CRAZY,
		SLEEP,
	};
	static ItemTortoise* create(ValueMap& map);
	bool init(ValueMap& map);

	
	
	virtual void collisionCheck(float dt);
	
	
	virtual void moveCheck(float dt);
	void cancelGodModeCallback(float dt);
	void reLiveCallback(float dt);

	void updateStatus();
protected:
	
	int m_initXV;
	bool m_bIsDead;
	State m_state;

};
#endif
