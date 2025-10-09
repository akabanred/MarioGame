#ifndef __ItemFireString_H__
#define __ItemFireString_H__
#include "Common.h"
#include "Item.h"
class ItemFireString :public Item
{
public:

	static ItemFireString* create(ValueMap& map);
	bool init(ValueMap& map);
	virtual void collisionCheck(float dt);

};
#endif
