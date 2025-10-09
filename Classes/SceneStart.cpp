#include "SceneStart.h"
#include "SceneSelectLevel.h"

bool SceneStart::init(){
	Scene::init();

	Sprite* bg = Sprite::create(IMAGE_background);
	addChild(bg);
	bg->setPosition(center);

	addCtrlMenu();


	return true;
}
void SceneStart::addCtrlMenu(){
	Menu* menu = Menu::create();
	addChild(menu);

	MenuItemImage* start = MenuItemImage::create(IMAGE_startgame_normal,
												 IMAGE_startgame_select,
												 std::bind(
												 &SceneStart::startCallback, this, std::placeholders::_1)

												 );
	MenuItemImage* setup = MenuItemImage::create(IMAGE_Setting_n,
												 IMAGE_setting_s,
												 std::bind(
												 &SceneStart::setupCallback, this, std::placeholders::_1)

												 );
	MenuItemImage* quit = MenuItemImage::create(IMAGE_quitgame_normal,
												IMAGE_quitgame_select,
												std::bind(
												&SceneStart::quitCallback, this, std::placeholders::_1)

												);
	MenuItemImage* about = MenuItemImage::create(IMAGE_about_normal,
												 IMAGE_about_select,
												 std::bind(
												 &SceneStart::aboutCallback, this, std::placeholders::_1)

												 );

	menu->addChild(start);
	menu->addChild(setup);
	menu->addChild(quit);
	menu->addChild(about);

	//设置四个按钮的位置

	setup->setPosition(start->getPosition() - Vec2(0, 50));
	quit->setPosition(start->getPosition() - Vec2(0, 100));
	about->setPosition(start->getPosition() - Vec2(-195, 150));

}
void SceneStart::startCallback(Ref*){
	Director::getInstance()->replaceScene(SceneSelectLevel::create());
}
void SceneStart::setupCallback(Ref*){

}
void SceneStart::quitCallback(Ref*){
	Director::getInstance()->popScene();
}
void SceneStart::aboutCallback(Ref*){
	
}