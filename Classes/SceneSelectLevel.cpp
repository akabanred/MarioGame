
#include "SceneSelectLevel.h"
#include "SceneStart.h"
#include "SceneGame.h"
#include "ui/UIScrollView.h"
#include "SceneGame.h"
bool SceneSelectLevel::init(){
	Scene::init();
	
	//±³¾°Í¼Æ¬
	Sprite* bg = Sprite::create("bg.png");
	addChild(bg);
	bg->setPosition(center);

	_view = ui::ScrollView::create();

	char finename[32] = {0};
	Sprite* preSpr;
	//int largeViewWidth = winSize.width*.6f;
	//int smallViewWidth = winSize.width*.6f;
	int  viewWidth = winSize.width*.6f;
	for (int i = 0; i <ALL_LEVEL_NUM; ++i){

		myutil::format(finename, "select", i+1, ".jpg");

		Sprite* spr = Sprite::create(finename);
		
		if (i == 0){
			spr->setScale(viewWidth / (spr->getContentSize().width));
			spr->setPosition(center);
			preSpr = spr;
		}
		else{
			spr->setScale(viewWidth / (spr->getContentSize().width));
			spr->setPosition(preSpr->getPosition() + Vec2(preSpr->getBoundingBox().size.width/2+spr->getBoundingBox().size.width/2+winSize.width*.01,0));
			preSpr = spr;
		}

		
		spr->setTag(1000 + i);
		_view->addChild(spr);
	}
	
	

	auto listener = EventListenerTouchOneByOne::create();
	_listener = listener;
	listener->onTouchBegan = [&](Touch* touch, Event*){
		return true;
	};
	listener->onTouchEnded = [&](Touch* touch, Event*){
			if (isClick(touch)){
				Vec2 pt = _view->getInnerContainer()->convertTouchToNodeSpace(touch);
				Sprite * clickSpr;
				for (int i = 0; i < ALL_LEVEL_NUM; ++i){
					clickSpr = (Sprite *)_view->getChildByTag(1000 + i);
					if (clickSpr->getBoundingBox().containsPoint(pt)){
						_level = i + 1;
						Director::getInstance()->replaceScene(SceneGame::create(_level));
						break;
					}
					
				}
		
				
		
			}
	
	};
	_eventDispatcher->addEventListenerWithFixedPriority(listener, -120);
	
	//4+4+3
	_view->setContentSize(winSize);
	_view->setInnerContainerSize(Size(preSpr->getBoundingBox().size.width * (ALL_LEVEL_NUM+1), winSize.height));
	_view->setDirection(ui::ScrollView::Direction::HORIZONTAL);
	addChild(_view);
	
	

	//·µ»Ø°´Å¥
	MenuItemImage* backItem = MenuItemImage::create("backA.png",
													"backB.png",
													std::bind(&SceneSelectLevel::backItemCallback,this,std::placeholders::_1));
	backItem->setAnchorPoint(Vec2(1, 0));
	backItem->setPosition(backItem->getPosition()+Vec2(winSize.width / 2, -winSize.height / 2));
	Menu* menu = Menu::create(backItem, NULL);
	addChild(menu);

	

	

	return true;
}

void SceneSelectLevel::onExit(){
	Scene::onExit();

	_eventDispatcher->removeEventListener(_listener);
}
void SceneSelectLevel::backItemCallback(Ref*){
	
	Director::getInstance()->replaceScene(SceneStart::create());
}





bool SceneSelectLevel::isClick(Touch *pTouch){
	Vec2 ptStart = pTouch->getStartLocation();
	Vec2 endLoc = pTouch->getLocation();

	return ptStart.getDistanceSq(endLoc) <= 9;
}


