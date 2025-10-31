#include "SceneStart.h"
#include "SceneSelectLevel.h"
#include "audio/include/AudioEngine.h"

using namespace cocos2d;

bool SceneStart::init()
{
    Scene::init();

    Sprite *bg = Sprite::create(IMAGE_background);
    addChild(bg);
    bg->setPosition(center);

    addCtrlMenu();

    return true;
}
void SceneStart::addCtrlMenu()
{
    Menu *menu = Menu::create();
    addChild(menu);

    MenuItemImage *start = MenuItemImage::create(IMAGE_startgame_normal,
                                                 IMAGE_startgame_select,
                                                 CC_CALLBACK_1(SceneStart::startCallback, this));

    MenuItemImage *setup = MenuItemImage::create(IMAGE_Setting_n,
                                                 IMAGE_setting_s,
                                                 CC_CALLBACK_1(SceneStart::setupCallback, this));

    MenuItemImage *quit = MenuItemImage::create(IMAGE_quitgame_normal,
                                                IMAGE_quitgame_select,
                                                CC_CALLBACK_1(SceneStart::quitCallback, this));

    MenuItemImage *about = MenuItemImage::create(IMAGE_about_normal,
                                                 IMAGE_about_select,
                                                 CC_CALLBACK_1(SceneStart::aboutCallback, this));

    menu->addChild(start);
    menu->addChild(setup);
    menu->addChild(quit);
    menu->addChild(about);

    setup->setPosition(start->getPosition() - Vec2(0, 50));
    quit->setPosition(start->getPosition() - Vec2(0, 100));
    about->setPosition(start->getPosition() - Vec2(-195, 150));
}

void SceneStart::startCallback(Ref *)
{
    Director::getInstance()->replaceScene(SceneSelectLevel::create());
}

void SceneStart::setupCallback(Ref*)
{
    // Biến lưu trạng thái âm thanh (nên đặt trong SceneStart.h)
    static bool isSoundOn = true;

    if (isSoundOn)
    {
        AudioEngine::pauseAll(); // hoặc stopAll() nếu muốn tắt hẳn
        MessageBoxA(nullptr, "Am thanh da duoc tat.", "Cai dat", MB_OK);
        isSoundOn = false;
    }
    else
    {
        AudioEngine::resumeAll(); // bật lại tất cả âm thanh
        MessageBoxA(nullptr, "Am thanh da duoc bat lai.", "Cai dat", MB_OK);
        isSoundOn = true;
    }
}

void SceneStart::quitCallback(Ref *)
{
    int result = MessageBoxA(
        nullptr,
        "Are you sure to quit the game?",
        "Confirm Exit",
        MB_YESNO | MB_ICONQUESTION);

    if (result == IDYES)
    {
        Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
#endif
    }
}

void SceneStart::aboutCallback(Ref *)
{
    const char *info =
        "Gioi thieu game:\n"
        "Hanh trinh cua Mario bat dau tai vuong quoc Nam Nam bi ke ac Bowser chiem dong.\n"
        "De cuu cong chua Peach, Mario phai vuot qua nhung dia hinh hiem tro, "
        "chay tren vach nui, nhay qua ho sau va chien dau voi quai vat.\n\n"
        "Moi man choi la mot thu thach moi day cam bay va bat ngo. "
        "Chi nhung nguoi nhanh tri va dung cam moi co the di den cuoi hanh trinh!\n\n"
        "Huong dan choi:\n"
        "- Su dung phim W-A-S-D de di chuyen Mario.\n"
        "- Nhan phim J de nhay, tranh chuong ngai vat hoac dap quai vat tu tren xuong.\n"
        "- Thu thap xu va vat pham de tang suc manh va diem so.\n"
        "- Vuot qua tat ca cac man choi de giai cuu cong chua!\n\n"
        "Chuc ban co mot cuoc phieu luu ky thu va chien thang Bowser!";
    MessageBoxA(nullptr, info, "Gioi thieu", MB_OK);
}
