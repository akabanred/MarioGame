#ifndef __ItemAddMushroom_H__
#define __ItemAddMushroom_H__
#include "Common.h"
#include "ItemCanMove.h"
class ItemAddMushroom :public ItemCanMove
{
public:

	static ItemAddMushroom* create(ValueMap& map);
	bool init(ValueMap& map);


};
#endif
