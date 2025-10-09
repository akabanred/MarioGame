#ifndef __SceneStart_H__
#define __SceneStart_H__
#include "Common.h"
class SceneStart:public Scene
{
public:
	CREATE_FUNC(SceneStart);

private:
	bool init();
	void addCtrlMenu();
	void startCallback(Ref*);
	void setupCallback(Ref*);
	void quitCallback(Ref*);
	void aboutCallback(Ref*);
};

#endif