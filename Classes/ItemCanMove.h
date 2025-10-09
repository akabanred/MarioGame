#ifndef __ItemCanMove_H__
#define __ItemCanMove_H__

#include "Common.h"
#include "Item.h"
class ItemCanMove:public Item
{
public:
	bool init();
	void onEnter()	override;
	void onExit()	override;
	void update(float dt)override;
	

	void moveCheck(float dt);
	bool canMoveHorizontal(float dt);
	bool canMoveDown(float dt);
	void moveHorizontal(float dt);
	void moveDown(float dt);
	void beginGodMode(float dt);
	void cancelGodModeCallback(float dt);

	bool isGodMode();
protected:
	bool _isGodMode;
	CC_SYNTHESIZE(int, _speedX, SpeedX);
	CC_SYNTHESIZE(int, _speedY, SpeedY);

};
#endif // !1
