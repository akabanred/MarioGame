#include "ItemCanMove.h"

bool ItemCanMove::init(){
	Item::init();
	_isGodMode = false;
	return true;
}
void ItemCanMove::onEnter(){
	Item::onEnter();
	
}
void ItemCanMove::onExit(){
	Item::onExit();
	
}
void ItemCanMove::update(float dt){
	moveCheck(dt);
	collisionCheck(dt);
}
void ItemCanMove::moveCheck(float dt){

	if (canMoveHorizontal(dt)){
		moveHorizontal(dt);
	}
	else{
		_speedX = -_speedX;
	}

	if (canMoveDown(dt)){
		moveDown(dt);
	}
	else{
		_speedY = 0;
	}

}

bool ItemCanMove::canMoveHorizontal(float dt){


	Rect rc(getBoundingBox().origin, getBoundingBox().size - Size(1, 1));


	TMXTiledMap* map = getMap();
	Vec2 ptInWorld = map->convertToWorldSpace(getPosition());
	Vec2 pts[3];

	//判断水平是否出界
	if (ptInWorld.x + _speedX*dt <= 0){
		return false;
	}

	//竖直方向如果出界 ,则可以移动
	if (getPositionY() < 0 || getPositionY() >= map->getContentSize().height){
		return true;
	}



	float midY = rc.getMidY() > map->getContentSize().height - 1 ? map->getContentSize().height - 1 : rc.getMidY();
	float maxY = rc.getMaxY() > map->getContentSize().height - 1 ? map->getContentSize().height - 1 : rc.getMaxY();

	if (_speedX < 0){
		//向左走
		float minX = rc.getMinX() + _speedX*dt;
		pts[0] = Vec2(minX, midY);
		pts[1] = Vec2(minX, maxY);
		pts[2] = Vec2(minX, rc.getMinY());

	}
	else{
		//向右走
		float maxX = rc.getMaxX() + _speedX*dt;
		pts[0] = Vec2(maxX, midY);
		pts[1] = Vec2(maxX, maxY);
		pts[2] = Vec2(maxX, rc.getMinY());
	}

	for (int i = 0; i < 3; ++i){
		Vec2 ptTile = myutil::bLGLPointToTile(pts[i],map);

		//墙,水管,地板
		static const char *layerName[] = {
			"block",
			"pipe",
			"land"
		};
		for (int j = 0; j < sizeof(layerName) / sizeof(*layerName); ++j){
			TMXLayer* layer = map->getLayer(layerName[j]);
			int gid = layer->getTileGIDAt(ptTile);
			if (gid){
				//有东西挡住了
				return false;
			}
		}
	}


	return true;
}
bool ItemCanMove::canMoveDown(float dt){
	if (_speedY > 0){
	
		return true;
	}
	Rect rcItem(getBoundingBox().origin,
				  getBoundingBox().size-Size(1,1));
	Vec2 ptItem = getPosition();

	TMXTiledMap* map = getMap();
	Vec2 ptItemInWorld = map->convertToWorldSpace(ptItem);
	Vec2 pts[3];

	if (!_speedY)
		_speedY -= ARG_GRAVITY;

	float minY = rcItem.getMinY() + _speedY*dt;
	if (rcItem.getMinY() >= map->getContentSize().height){
		return true;
	}

	//判断是否出界
	if (minY < 0){
		return true;
	}

	//向下
	pts[0] = Vec2(rcItem.getMinX(), minY);
	pts[1] = Vec2(rcItem.getMidX(), minY);
	pts[2] = Vec2(rcItem.getMaxX(), minY);

	for (int i = 0; i < 3; ++i){
		Vec2 ptTile = myutil::bLGLPointToTile(pts[i], map);

		//墙,水管,地板
		static const char *layerName[] = {
			"block",
			"pipe",
			"land",

		};
		for (int j = 0; j < sizeof(layerName) / sizeof(*layerName); ++j){
			TMXLayer* layer = map->getLayer(layerName[j]);
			int gid = layer->getTileGIDAt(ptTile);
			if (gid){
				//有东西挡住了
				return false;
			}
		}
	}


	return true;
}
void ItemCanMove::moveHorizontal(float dt){
	setPositionX(getPositionX() + dt*_speedX);
}
void ItemCanMove::moveDown(float dt){
	setPositionY(getPositionY() + dt*_speedY);
	_speedY -= ARG_GRAVITY;
}
void ItemCanMove::beginGodMode(float dt){
	_isGodMode = true;
	scheduleOnce(CC_SCHEDULE_SELECTOR(ItemCanMove::cancelGodModeCallback),dt);
}
void ItemCanMove::cancelGodModeCallback(float dt){
	_isGodMode = false;
}