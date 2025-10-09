#include "Common.h"
common::BlockType common::getBlockTypeByGid(int gid) {
	return gidTable[gid];
}



namespace common{

	BlockType gidTable[] = {
		none,common,common,common,none,none,none ,Hard,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,
		commonGetHard,commonGetHard,commonGetHard,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,
		commonGetHard,commonGetHard,commonGetHard,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,
		common,common,common,none,none,none ,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,
		none,none,none ,none,none,none ,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,
		none,none,none ,none,none,none ,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,
		none,none,none ,none,none,none ,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,
		none,none,none ,none,none,none ,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,
		none,none,none ,none,none,none ,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,
		commonHarfOnRight,commonHarfOnRiLeft,none ,none,none,none ,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,
		commonHarfOnRight,commonHarfOnRiLeft,none ,none,none,none ,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,
		commonHarfOnRight,commonHarfOnRiLeft,none ,none,none,Hard ,Hard,Hard,Hard,Hard,Hard,Hard,Hard,Hard,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,
		commonHarfOnRight,commonHarfOnRiLeft,none ,none,none,none ,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,
		commonHarfOnRight,commonHarfOnRiLeft,none ,none,none,none ,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,
		commonHarfOnRight,commonHarfOnRiLeft,none ,none,none,none ,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,
		hardHarfOnRight,hardHarfOnRiLeft,none ,none,none,none ,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,
		none,none,none ,none,none,none ,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,
		none,none,none ,none,none,none ,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,
		none,none,none ,none,none,none ,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,
		none,none,none ,none,none,none ,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,
		question,question,question ,none,none,none ,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,
		question,question,question ,none,none,none ,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,
		none,none,none ,none,none,none ,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,
		none,none,none ,none,none,none ,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,
		none,none,none ,none,none,none ,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,
		none,none,none ,none,none,none ,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,
		none,none,none ,none,none,none ,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,
		none,none,none ,none,none,none ,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,
		none,none,none ,none,none,none ,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,
		none,none,none ,none,none,none ,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none,none
	};

}