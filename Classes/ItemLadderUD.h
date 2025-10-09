#ifndef __ItemLadderUD_H__
#define __ItemLadderUD_H__
#include "Common.h"
#include "Item.h"
class ItemLadderUD :public Item
{
public:

	static ItemLadderUD* create(ValueMap& map);
	bool init(ValueMap& map);


};
#endif
