#include "myutil.h"
Animation* myutil::createAnimation(const char * finename, int start, int num, int width, float  duration){

	auto texture = Director::getInstance()->getTextureCache()->addImage(finename);
	Vector<SpriteFrame*> vec;
	for (int i = start; i < start + num; ++i){
		SpriteFrame* frame = SpriteFrame::createWithTexture(texture, Rect(width*i, 0, width, texture->getContentSize().height));
		vec.pushBack(frame);
	}

	return Animation::createWithSpriteFrames(vec, duration);


}
SpriteFrame* myutil::getSpriteFrame(const char* filename, int pos, int width){
	auto texture = Director::getInstance()->getTextureCache()->addImage(filename);
	return  SpriteFrame::createWithTexture(texture, Rect(width*pos, 0, width, texture->getContentSize().height));

}
void myutil::format(char* ret, const char * prefix,
			int v, const char* suffix)

{
	sprintf(ret, "%s%d%s", prefix, v, suffix);
}
Vec2 myutil::tilePointToBLGL(const Vec2& ptMap, TMXTiledMap* map){

	return Vec2(ptMap.x*(map->getContentSize().width / map->getMapSize().width),
				(map->getMapSize().height - ptMap.y - 1)*((map->getContentSize().height / map->getMapSize().height)));
}

Vec2 myutil::bLGLPointToTile(const Vec2& pt, TMXTiledMap*map){

	return Vec2((int)(pt.x / map->getTileSize().width),
				map->getMapSize().height - 1 - (int)(pt.y / map->getTileSize().height));

}