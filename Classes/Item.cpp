#include "Item.h"
#include "cocos2d.h"

#include "ItemMushroomMonster.h"
#include "ItemMushroom.h"
#include "ItemTortoise.h"
#include "ItemFlower.h"
#include "ItemFlagpoint.h"
#include "ItemFinalpoint.h"
#include "ItemLadderLR.h"
#include "ItemFlyFish.h"
#include "ItemTortoiseRound.h"
#include "ItemTortoiseFly.h"
#include "ItemLadderUD.h"
#include "ItemFireString.h"
#include "ItemBridgeStartPos.h"
#include "Mario.h"
#include "Common.h"
#include "ItemBoss.h"


USING_NS_CC;

Item* Item::create(ValueMap& map)
{
    const auto it = map.find("type");
    if (it == map.end()) return nullptr;
    const std::string type = it->second.asString();

    if (type == "mushroom")            return ItemMushroomMonster::create(map);
    else if (type == "tortoise")       return ItemTortoise::create(map);
    else if (type == "flower")         return ItemFlower::create(map);
    else if (type == "MushroomReward") return ItemMushroom::create(map);
    else if (type == "MushroomAddLife")return ItemMushroom::create(map);
    else if (type == "flagpoint")      return ItemFlagpoint::create(map);
    else if (type == "finalpoint")     return ItemFinalpoint::create(map);
    else if (type == "ladderLR")       return ItemLadderLR::create(map);
    else if (type == "flyfish")        return ItemFlyFish::create(map);
    else if (type == "tortoise_round") return ItemTortoiseRound::create(map);
    else if (type == "tortoise_fly")   return ItemTortoiseFly::create(map);
    else if (type == "ladderUD")       return ItemLadderUD::create(map);
    else if (type == "fire_string")    return ItemFireString::create(map);
    else if (type == "bridgestartpos") return ItemBridgeStartPos::create(map);
	


    return nullptr;
}

void Item::setPositionByProperty(const ValueMap& dict)
{
    
    float x = 0.f, y = 0.f;
    auto itx = dict.find("x");
    auto ity = dict.find("y");
    if (itx != dict.end()) x = itx->second.asFloat();
    if (ity != dict.end()) y = ity->second.asFloat();

    setPosition(Vec2(x, y - 16.0f));
    setIgnoreAnchorPointForPosition(true);
}

bool Item::init()
{
    Sprite::init();
    setLocalZOrder(common::ZO_MUSHROOM);
    return true;
}

void Item::onEnter()
{
    Sprite::onEnter();
    scheduleUpdate();
}

void Item::onExit()
{
    unscheduleUpdate();
    Sprite::onExit();
}

void Item::update(float dt)
{
    collisionCheck(dt);
}

bool Item::isAppearInWindow()
{
    auto* map = getMap();
    if (!map) return false;
    Vec2 ptWorld = map->convertToWorldSpace(getPosition());
    return (ptWorld.x < winSize.width);
}

TMXTiledMap* Item::getMap()
{
    return dynamic_cast<TMXTiledMap*>(getParent());
}

bool Item::isOutOfWindow()
{
    auto* map = getMap();
    if (!map) return true;
    Vec2 ptWorld = map->convertToWorldSpace(getPosition());
    return (ptWorld.x < -winSize.width) || (getPositionY() <= -getBoundingBox().size.height);
}

void Item::autoDropFlag() {}

void Item::runAnimation(const char* name)
{
    auto anim = AnimationCache::getInstance()->getAnimation(name);
    if (anim)
        runAction(RepeatForever::create(Animate::create(anim)));
}
