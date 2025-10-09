
#include "MenuCtrl.h"


bool MenuCtrl::init(){
	Menu::init();
	scheduleUpdate();
	return  true;
}

void MenuCtrl::update(float){
	if (this->_selectedItem&&this->_state == Menu::State::TRACKING_TOUCH){
			_selectedItem->activate();
	}
}
