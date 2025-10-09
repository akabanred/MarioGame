#ifndef __ItemBridgeStartPos_H__
#define __ItemBridgeStartPos_H__
#include "Common.h"
#include "Item.h"
class ItemBridgeStartPos :public Item
{
public:

	static ItemBridgeStartPos* create(ValueMap& map);
	bool init(ValueMap& map);
	virtual void collisionCheck(float dt);

};
#endif
