#ifndef __ItemBullet_H__
#define __ItemBullet_H__

#include "Item.h"
#include "cocos2d.h"

class ItemBullet : public Item
{
public:
    // Tạo đối tượng: sẽ gọi init() do CREATE_FUNC sinh ra
    CREATE_FUNC(ItemBullet);

    // Item overrides
    bool init() override;
    void update(float dt) override;

    // API để boss điều khiển
    void setVelocity(const cocos2d::Vec2& v);
    void setOwnerCategory(uint32_t catMask);
	virtual void onEnter() override;
	uint32_t getOwnerCategory() const { return _ownerCat; }

private:
    cocos2d::Vec2 _vel{0.f, 0.f};
    uint32_t      _ownerCat{0};     // để loại trừ va chạm với “chủ” của viên đạn
};

#endif // __ItemBullet_H__
