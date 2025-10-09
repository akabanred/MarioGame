#ifndef __MARIO_H__
#define __MARIO_H__
#include "Common.h"
#include "ItemCanMove.h"

//将mario做成单例
class Mario :public Sprite
{
public:

	enum State
	{
		Small,
		Big,
		CanFire
	};
	
	static Mario* getInstance();
	

	TMXTiledMap*   getMap();

	//给马里奥竖直方向的速度
	void jump(int initV=300);

	//给马里奥水平方向的速度
	void setHorizontalSpeed(common::Direction dir);

	//在帧循环中速度检测,如果有速度,则移动
	void moveVerticalCheck(float dt);
	void moveHorizontalCheck(float dt);


	
	//更新马里奥实际状态
	void updateStatus();

	
	void stop();

	bool canMoveDown(float dt);
	bool canMoveUp(float dt);
	bool canMoveHorizontally(float dt);


	
	

	
	bool isFly();
	bool isDead();
	void die(bool realDead = false);
	
	void eatMushroom(Item::ItemType type);
	

	void beginGodMode(float dt=2.0f);
	
	void cancelGodModeCallback(float dt);

	void autoRun();
	void beginAutoMoveRightCallback(float);
	void autoMoveRightCallback(float);
	void setIsOnLadder(bool isOnLadder);
	bool isOnLadder();

	int  getSpeedY();
	int  getSpeedX();
	void setSpeedY(int v_y);
	void setSpeedX(int v_x);
	void reverseSpeedY();
	void setIsFly(bool isFly);
	void endAutoRun();
	void setDead(bool isDead);
	bool isGodMode();

private:
	Mario(){}
	bool init();
	void initResourceCache();


protected:
	//status
	static Mario * sm_mario;
	common::Direction  _faceDir;	//静止时脸的朝向
				
	Vec2	_speed;					//速度向上为正	
				
	
	bool    _isFly;
	bool    _isGodMode;			//被敌人撞击后得到短暂的无敌模式
	int		_life;
	CC_SYNTHESIZE(State, _state, State);
	
	bool	_isDead;
	bool	_isAutoRunning;
	bool    _isOnLadder;
};

#endif