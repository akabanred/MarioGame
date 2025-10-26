#include "ItemBullet.h"
#include "Common.h"
#include "cocos2d.h"

// Các entity để va chạm
#include "Mario.h"
#include "ItemBoss.h"
#include "ItemMushroomMonster.h"

USING_NS_CC;

// Bitmask “chủ sở hữu”
static const uint32_t CAT_MARIO  = 0x1u << 1;
static const uint32_t CAT_BOSS   = 0x1u << 4;
// (Không cần CAT_BULLET nếu không dùng physics)

static inline bool aabbIntersects(Node* a, Node* b) {
    if (!a || !b) return false;
    return a->getBoundingBox().intersectsRect(b->getBoundingBox());
}

bool ItemBullet::init()
{
    if (!Item::init()) return false;

    // Tạm 1 texture (sẽ đổi khi setOwnerCategory)
    if (!Sprite::initWithFile("bossBullet.png"))
        return false;

    setLocalZOrder(common::ZO_MUSHROOM);
    scheduleUpdate();
    return true;
}

void ItemBullet::setVelocity(const Vec2& v)
{
    _vel = v;
}

void ItemBullet::setOwnerCategory(uint32_t cat) {
    _ownerCat = cat;
    if (auto body = getPhysicsBody()) {
        body->setCollisionBitmask(0xFFFF & ~cat);
        body->setContactTestBitmask(0xFFFF);
    }
    const uint32_t CAT_MARIO = 0x1u << 1;
    if (cat == CAT_MARIO) {
        // đổi qua ảnh mũi tên
        this->setTexture("arrowBullet.png");
        this->setTextureRect(Rect(0,0,this->getTexture()->getContentSize().width,
                                      this->getTexture()->getContentSize().height));
    }
}


void ItemBullet::update(float dt)
{
    // Bay theo vận tốc
    setPosition(getPosition() + _vel * dt);

    // Ra khỏi màn thì xoá
    const Size win = Director::getInstance()->getWinSize();
    const Vec2 wp  = this->convertToWorldSpaceAR(Vec2::ZERO);
    if (wp.x < -64 || wp.x > win.width + 64 || wp.y < -64 || wp.y > win.height + 64) {
        removeFromParent();
        return;
    }

    // Không parent thì thôi
    Node* parent = getParent();
    if (!parent) return;

    // Lấy Mario nếu có
    Mario* mario = dynamic_cast<Mario*>(parent->getChildByName("Mario"));

    // === Logic va chạm ===
    // Duyệt các node cùng parent để check:
    const Vector<Node*>& children = parent->getChildren();
    for (ssize_t i = 0; i < children.size(); ++i)
    {
        Node* n = children.at(i);
        if (n == this) continue;

        // 1) Đạn-đạn triệt tiêu (Mario vs Boss)
        if (ItemBullet* other = dynamic_cast<ItemBullet*>(n)) {
            if (_ownerCat != 0 && other->_ownerCat != 0) {
                const bool opposite =
                    (_ownerCat == CAT_MARIO && other->_ownerCat == CAT_BOSS) ||
                    (_ownerCat == CAT_BOSS  && other->_ownerCat == CAT_MARIO);
                if (opposite && aabbIntersects(this, other)) {
                    other->removeFromParent();
                    this->removeFromParent();
                    return;
                }
            }
            continue;
        }

        // 2) Đạn Boss trúng Mario
        if (_ownerCat == CAT_BOSS && mario && n == (Node*)mario) {
            if (aabbIntersects(this, mario)) {
                if (!mario->isGodMode()) {
                    mario->die();
                }
                this->removeFromParent();
                return;
            }
            continue;
        }

        // 3) Đạn Mario
        if (_ownerCat == CAT_MARIO) {
            // a) Trúng Boss -> -1 HP
            if (ItemBoss* boss = dynamic_cast<ItemBoss*>(n)) {
                if (aabbIntersects(this, boss)) {
                    boss->takeDamage(1);      // Boss 3 máu -> cần 3 hit
                    this->removeFromParent();
                    return;
                }
            }

            // b) Trúng nấm quái -> xoá nấm
            if (ItemMushroomMonster* mush = dynamic_cast<ItemMushroomMonster*>(n)) {
                if (aabbIntersects(this, mush)) {
                    mush->removeFromParent();
                    this->removeFromParent();
                    return;
                }
            }
        }
    }
}
void ItemBullet::onEnter() {
    Item::onEnter();   // giữ schedule/update mặc định, KHÔNG tạo PhysicsBody lần nữa
}
