#ifndef __ItemBoss_H__
#define __ItemBoss_H__

#include "Item.h"
#include "cocos2d.h"

class Mario;

class ItemBoss : public Item
{
public:
    // Tạo từ TMX (ValueMap có "x","y")
    static ItemBoss* create(cocos2d::ValueMap& map);
    // Tạo theo vị trí (spawn thủ công)
    static ItemBoss* createBoss(const cocos2d::Vec2& spawnPos);

    // Bắt đầu AI (init đã scheduleUpdate; hàm này để tương thích)
    void start();

    // Giới hạn khu giao chiến
    void setBattleArea(const cocos2d::Rect& area) { _battleArea = area; }

    // Máu / trạng thái
    void takeDamage(int dmg);
    bool isDead() const { return _hp <= 0; }

    // override
    void update(float dt) override;

protected:
    bool initFromMap(const cocos2d::ValueMap& map);
    bool initBoss(const cocos2d::Vec2& spawnPos);

    // Hành vi
    void doPatrol(float dt);
    void doAttack(float dt);
    void shootOnce();
    void playMoveAnim();

private:
    // Thuộc tính cơ bản
    int   _hp = 3;                 // 3 mạng
    float _speed = 60.f;           // px/s
    float _attackInterval = 1.75f; // bắn mỗi ~1.75s
    float _attackCooldown = 0.f;
    float _aggroRange = 260.f;     // tầm phát hiện Mario
    bool  _facingRight = true;
	float _hurtCooldown = 0.f;
    cocos2d::Vec2 _spawnPos;
    cocos2d::Rect _battleArea;
    Mario*        _mario = nullptr;
};

#endif // __ItemBoss_H__
