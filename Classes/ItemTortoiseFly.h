#ifndef __ItemTortoiseFly_H__
#define __ItemTortoiseFly_H__
#include "Common.h"
#include "ItemCanMove.h"
class ItemTortoiseFly :public ItemCanMove
{
public:
	//״̬
	enum 
	{
		NORMAL,
		DROPPING,
		ONLAND,
		SLEEP,
		CRAZY
	} m_status;
	static ItemTortoiseFly* create(ValueMap& map);
	bool init(ValueMap& map);

	void updateStatus();

	virtual void moveCheck(float dt);
	virtual void collisionCheck(float dt);
	void reLiveCallback(float);
	
	int m_flyDis;
	
};
#endif
