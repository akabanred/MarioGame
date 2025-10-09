#ifndef __ItemFinalpoint_H__
#define __ItemFinalpoint_H__
#include "Common.h"
#include "Item.h"
class ItemFinalpoint :public Item
{
public:

	static ItemFinalpoint* create(ValueMap& map);
	bool init(ValueMap& map);


};
#endif
