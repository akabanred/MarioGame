#ifndef __ItemBullet_H__
#define __ItemBullet_H__
#include "Common.h"
#include "Item.h"
class ItemBullet :public Item
{
public:

	static ItemBullet* create();
	bool init();


};
#endif
