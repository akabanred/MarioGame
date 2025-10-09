#ifndef __ItemFlower_H__
#define __ItemFlower_H__
#include "Common.h"
#include "Item.h"
class ItemFlower :public Item
{
public:

	static ItemFlower* create(ValueMap& map);
	bool init(ValueMap& map);
	void update(float dt)override;

	void updateStatus();

	void cancelShowCallback(float);

	virtual void moveCheck(float dt);
	
	virtual void collisionCheck(float dt);
	bool m_bShow;//±íÏÖ×´Ì¬
	int  m_yTop;
	int  m_yButtom;

protected:
	
	CC_SYNTHESIZE(int, _speedY, SpeedY);
	
};
#endif
