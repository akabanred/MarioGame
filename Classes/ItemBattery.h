#ifndef __ItemBattery_H__
#define __ItemBattery_H__
#include "Common.h"
#include "ItemCanMove.h"
class ItemBattery :public ItemCanMove
{
public:

	static ItemBattery* create(ValueMap& map);
	bool init(ValueMap& map);


};
#endif
