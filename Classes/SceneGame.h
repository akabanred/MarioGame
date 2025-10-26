#ifndef __SceneGame_H__
#define __SceneGame_H__

#include "Common.h"
#include <set>
#include <list>

class Item;
class Mario;
class ProgressBar;
class ItemMushroom;

class SceneGame : public Scene
{
public:
    static SceneGame* create(int level);

    void moveRightCallback(Ref*);
    void moveLeftCallback(Ref*);
    void jumpKeyCallback(Ref*);
    void fireKeyCallback(Ref*);
    void checkMarioTouchPole(float dt);
    void checkMarioTouchEndPointCallback(float dt);
    void gameOver();

private:
    bool init(int level);
    void addMap();
    void addCtrlButton();
    void addMapObjectGroup();
    void addAnimationToCache();

    /* override */
    void onEnter() override;
    void onExit() override;
    void update(float delta) override;

    void moveMarioCheck(float delta);
    void marioEatCoinCheck(float delta);
    void marioEatHideMushroomCheck();
    void marioHitSomethingCheck(float dt);
    void marioHitBlockHandle(Sprite* block, const Vec2& tileCoordiate, common::BlockType type);
    void marioHitQuestionAnimationEndCallback(Node* block, ItemMushroom* mushroom);
    void marioHitQuestionHandle(Sprite* block, const Vec2& tileCoordiate);
    void destroyBlock(Sprite* block);

public:
    TMXTiledMap*    _map = nullptr;
    ProgressBar*    _bar = nullptr;
    int             _level = 1;

    Mario*          _mario = nullptr;
    Item*           _itemFlagpoint = nullptr;
    Item*           _finalPoint = nullptr;

    Texture2D*      _textureDirNone = nullptr;
    Texture2D*      _textureDirRight = nullptr;
    Texture2D*      _textureDirLeft = nullptr;

    Sprite*                 _menuShow = nullptr;
    common::Direction       _menuDir = common::NONE;
    std::set<Item*>         _items;
    std::list<Item*>        _mushrooms;

    // Cờ để tránh spawn boss trùng
    bool _bossSpawned;
};

#endif // __SceneGame_H__
