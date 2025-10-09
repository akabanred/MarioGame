#ifndef __MYUTIL_H__
#define __MYUTIL_H__


#include "cocos2d.h"
USING_NS_CC;

#define  winSize  Director::getInstance()->getWinSize()
#define  center   Vec2(winSize.width/2,winSize.height/2)
#define  CCLOG_DYNAMIC_ERR CCLOG("%s:%d dynamic err",__FILE__,__LINE__)
#define  textureCache (Director::getInstance()->getTextureCache())
#define  spriteFrameCache (SpriteFrameCache::getInstance())
namespace myutil{
	 void format(char* ret, const char * prefix,
		int v, const char* suffix);
	 Vec2 tilePointToBLGL(const Vec2& ptMap, TMXTiledMap* map);
	 Vec2 bLGLPointToTile(const Vec2& pt, TMXTiledMap*map);
	 Animation* createAnimation(const char * finename, int start, int num, int width, float  duration);
	 SpriteFrame* getSpriteFrame(const char* filename, int pos, int width);
}

#endif