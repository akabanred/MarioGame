#ifndef __ItemFlagpoint_H__
#define __ItemFlagpoint_H__
#include "Common.h"
#include "Item.h"
class ItemFlagpoint :public Item
{
public:

	static ItemFlagpoint* create(ValueMap& map);
	bool init(ValueMap& map);
	virtual void autoDropFlag();

};
#endif
