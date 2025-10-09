
#ifndef __SceneSelectLevel_H__
#define __SceneSelectLevel_H__
#include "Common.h"

#include "ui/UIScrollView.h"

class SceneSelectLevel :public Scene
{
public:
	CREATE_FUNC(SceneSelectLevel);

	virtual void onExit()override;
private:
	bool init();
	void backItemCallback(Ref*);
	bool isClick(Touch *pTouch);
	

	CC_SYNTHESIZE(ui::ScrollView*,_view,View);
	CC_SYNTHESIZE(int, _level, Level);

private:
	EventListenerTouchOneByOne* _listener;
	
};

#endif

