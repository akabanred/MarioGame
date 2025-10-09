#ifndef __COMMON_H__
#define __COMMON_H__
#include "AppMacros.h"
#include "myutil.h"
namespace common
{

	enum Direction
	{
		UP,
		RIGHT,
		DOWN,
		LEFT,
		NONE
	};
	enum ZO
	{
		ZO_MARIO=1000,
		ZO_MUSHROOM=100
	};
	enum BlockType {
		common,
		question,
		commonGetHard,
		commonHarfOnRight,
		commonHarfOnRiLeft,
		hardHarfOnRight,
		hardHarfOnRiLeft,
		Hard,
		none,
	};
	

	BlockType getBlockTypeByGid(int gid);
	extern BlockType gidTable[];
};

#endif


