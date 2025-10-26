#ifndef __MARIO_H__
#define __MARIO_H__
#include "Common.h"
#include "ItemCanMove.h"

//��mario���ɵ���
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

	//���������ֱ������ٶ�
	void jump(int initV=300);

	//�������ˮƽ������ٶ�
	void setHorizontalSpeed(common::Direction dir);

	//��֡ѭ�����ٶȼ��,������ٶ�,���ƶ�
	void moveVerticalCheck(float dt);
	void moveHorizontalCheck(float dt);


	
	//���������ʵ��״̬
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
	void tryShoot();    
	void fireArrow();
private:
	Mario(){}
	bool init();
	void initResourceCache();
	int   _arrowCount = 0;
    int   _arrowLimit = 2;
    float _shootCooldown = 0.f;


protected:
	//status
	static Mario * sm_mario;
	common::Direction  _faceDir;	//��ֹʱ���ĳ���
				
	Vec2	_speed;					//�ٶ�����Ϊ��	
				
	
	bool    _isFly;
	bool    _isGodMode;			//������ײ����õ����ݵ��޵�ģʽ
	int		_life;
	CC_SYNTHESIZE(State, _state, State);
	
	bool	_isDead;
	bool	_isAutoRunning;
	bool    _isOnLadder;
};

#endif