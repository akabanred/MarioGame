#ifndef __SCENELOADRESOURCE_H__
#define __SCENELOADRESOURCE_H__
#include "Common.h"
USING_NS_CC;
#include <string>
class SceneLoadResource:public Scene
{
public:
	CREATE_FUNC(SceneLoadResource);
	bool init();
	virtual void onEnter() override;

private:
	void copyResourceFileNameToClassMember();


	////////////////////////////////////////////////////
private:
	ProgressTimer	   *_bar;

	int					_loadFileCount = 0;
	int					_allFineNum;
	std::vector<const char*>	_picfiles;
	std::vector<const char*>  _musicfiles;
};

#endif