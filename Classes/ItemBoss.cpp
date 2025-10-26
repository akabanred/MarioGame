#include "ItemBoss.h"
#include "ItemBullet.h"
#include "Mario.h"
#include "Common.h"

USING_NS_CC;

/* ---------- Factory ---------- */
ItemBoss* ItemBoss::create(ValueMap& map)
{
    auto p = new (std::nothrow) ItemBoss();
    if (p && p->initFromMap(map)) { p->autorelease(); return p; }
    CC_SAFE_DELETE(p); return nullptr;
}

ItemBoss* ItemBoss::createBoss(const Vec2& spawnPos)
{
    auto p = new (std::nothrow) ItemBoss();
    if (p && p->initBoss(spawnPos)) { p->autorelease(); return p; }
    CC_SAFE_DELETE(p); return nullptr;
}

/* ---------- Init ---------- */
static bool initBossSpriteFromSheet(Sprite* spr)
{
    // Nạp texture của sprite-sheet
    auto tex = Director::getInstance()->getTextureCache()->addImage("boss.png");
    if (!tex) return false;

    // Số khung trong sheet (đếm đúng theo ảnh của bạn)
    const int COLS = 8;   // 8 con xếp ngang -> 8 frame
    const int ROWS = 1;

    const float fw = tex->getContentSize().width  / COLS;
    const float fh = tex->getContentSize().height / ROWS;

    // Cắt frame đầu tiên (cột 0, hàng 0)
    return spr->initWithTexture(tex, Rect(0, 0, fw, fh));
}

bool ItemBoss::initFromMap(const ValueMap& map)
{
    if (!Item::init()) return false;
    if (!initBossSpriteFromSheet(this)) return false;

    float x = 0.f, y = 0.f;
    auto itx = map.find("x");
    auto ity = map.find("y");
    if (itx != map.end()) x = itx->second.asFloat();
    if (ity != map.end()) y = ity->second.asFloat();

    _spawnPos = Vec2(x, y);
    setPosition(_spawnPos);
    setIgnoreAnchorPointForPosition(true);
    setLocalZOrder(common::ZO_MUSHROOM);

    if (_battleArea.size.equals(Size::ZERO))
        _battleArea = Rect(_spawnPos.x - 320, _spawnPos.y - 160, 640, 320);

    scheduleUpdate();
    return true;
}

bool ItemBoss::initBoss(const Vec2& spawnPos)
{
    if (!Item::init()) return false;
    if (!initBossSpriteFromSheet(this)) return false;

    _spawnPos = spawnPos;
    setPosition(_spawnPos);
    setIgnoreAnchorPointForPosition(true);
    setLocalZOrder(common::ZO_MUSHROOM);

    if (_battleArea.size.equals(Size::ZERO))
        _battleArea = Rect(_spawnPos.x - 320, _spawnPos.y - 160, 640, 320);

    scheduleUpdate();
    return true;
}

void ItemBoss::start() {
    if (!isScheduled(CC_SCHEDULE_SELECTOR(ItemBoss::update)))
        scheduleUpdate();
}

/* ---------- Update ---------- */
void ItemBoss::update(float dt)
{
    Item::update(dt);

    // Giảm thời gian invincibility frame (i-frame)
    if (_hurtCooldown > 0.f)
        _hurtCooldown -= dt;

    // Kiểm tra nếu boss đã chết
    if (isDead())
    {
        stopAllActions();
        runAction(Sequence::create(
            FadeOut::create(0.35f),
            RemoveSelf::create(),
            nullptr
        ));
        unscheduleUpdate();
        return;
    }

    // Lấy con trỏ tới Mario nếu chưa có
    if (!_mario && getParent())
        _mario = dynamic_cast<Mario*>(getParent()->getChildByName("Mario"));

    // Giữ boss trong phạm vi battle area
    auto p = getPosition();
    p.x = clampf(p.x, _battleArea.getMinX(), _battleArea.getMaxX());
    p.y = clampf(p.y, _battleArea.getMinY(), _battleArea.getMaxY());
    setPosition(p);

    // Logic tấn công hoặc tuần tra
    if (_mario && getPosition().distance(_mario->getPosition()) <= _aggroRange)
        doAttack(dt);
    else
        doPatrol(dt);

    // Kiểm tra va chạm giữa Mario và Boss
    if (_mario)
    {
        const Rect rcM = _mario->getBoundingBox();
        const Rect rcB = this->getBoundingBox();

        if (rcM.intersectsRect(rcB))
        {
            const bool marioOnTop =
                (_mario->getPositionY() > getPositionY() + rcB.size.height * 0.3f);

            // Mario nhảy trúng đầu boss (và hết i-frame)
            if (marioOnTop && _mario->getSpeedY() <= 0 && _hurtCooldown <= 0.f)
            {
                takeDamage(1);
                _hurtCooldown = 0.35f;  // Khoá trừ máu liên tục
                _mario->setPositionY(getPositionY() + rcB.size.height);
                _mario->jump();
            }
            else
            {
                // Mario bị boss chạm vào
                if (!_mario->isGodMode())
                    _mario->die();
            }
        }
    }
}

/* ---------- Behaviors ---------- */
void ItemBoss::doPatrol(float dt)
{
    const float leftX  = _spawnPos.x - 120.f;
    const float rightX = _spawnPos.x + 120.f;

    auto pos = getPosition();
    const float dir = _facingRight ? +1.f : -1.f;
    pos.x += dir * _speed * dt;

    if (pos.x < leftX)  { pos.x = leftX;  _facingRight = true;  setFlippedX(false); }
    if (pos.x > rightX) { pos.x = rightX; _facingRight = false; setFlippedX(true);  }

    setPosition(pos);
    playMoveAnim();
}

void ItemBoss::doAttack(float dt)
{
    _attackCooldown -= dt;
    if (_attackCooldown <= 0.f)
    {
        shootOnce();
        _attackCooldown = _attackInterval;
    }
    doPatrol(dt * 0.2f);
}

void ItemBoss::shootOnce()
{
    auto parent = getParent();
    if (!parent) return;

    const Vec2 muzzle = getPosition() + Vec2(0, getContentSize().height * 0.25f);
    Vec2 dir = _facingRight ? Vec2(1, 0) : Vec2(-1, 0);
    if (_mario) dir = (_mario->getPosition() - muzzle).getNormalized();

    auto bullet = ItemBullet::create();
    bullet->setPosition(muzzle);
    bullet->setVelocity(dir * 220.f);
    bullet->setOwnerCategory(0x1u << 4); // CAT_BOSS
    parent->addChild(bullet, this->getLocalZOrder());
}

void ItemBoss::playMoveAnim()
{
    // nếu có AnimationCache "bossLeft"/"bossRight" thì kích hoạt ở đây
}

void ItemBoss::takeDamage(int dmg)
{
    if (isDead()) return;
    _hp -= dmg;
    if (_hp < 0) _hp = 0;

    runAction(Sequence::create(
        TintTo::create(0, 255, 80, 80),
        DelayTime::create(0.06f),
        TintTo::create(0, 255, 255, 255),
        nullptr));
}
