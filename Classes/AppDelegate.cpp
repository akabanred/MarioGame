#include "AppDelegate.h"
#include "SceneLoadResource.h"
#include "Common.h"

#ifdef _WIN32
#include <Windows.h>  
#endif

// #define USE_AUDIO_ENGINE 1
// #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    CCLOG("==> applicationDidFinishLaunching: START");

    auto director = Director::getInstance();
    auto glview   = director->getOpenGLView();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
// Nếu muốn nhìn log cả khi chạy ngoài Visual Studio, mở console:
    //AllocConsole();
    //freopen("CONOUT$", "w", stdout);
    //freopen("CONOUT$", "w", stderr);
#endif

    if (!glview) {
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || \
        (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)   || \
        (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        CCLOG("Create GLView with rect %dx%d", (int)designResolutionSize.width, (int)designResolutionSize.height);
        glview = GLViewImpl::createWithRect("Mario",
                    cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
    #else
        CCLOG("Create GLView default");
        glview = GLViewImpl::create("Mario");
    #endif
        director->setOpenGLView(glview);
    } else {
        CCLOG("GLView already exists");
    }

    //director->setDisplayStats(true);
    director->setAnimationInterval(1.0f / 60);

    glview->setDesignResolutionSize(
        designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);

    const auto frameSize = glview->getFrameSize();
    CCLOG("frameSize: %.0fx%.0f", frameSize.width, frameSize.height);

    if (frameSize.height > mediumResolutionSize.height) {
        float sf = MIN(largeResolutionSize.height/designResolutionSize.height,
                       largeResolutionSize.width/designResolutionSize.width);
        CCLOG("use LARGE scale factor = %.3f", sf);
        director->setContentScaleFactor(sf);
    } else if (frameSize.height > smallResolutionSize.height) {
        float sf = MIN(mediumResolutionSize.height/designResolutionSize.height,
                       mediumResolutionSize.width/designResolutionSize.width);
        CCLOG("use MEDIUM scale factor = %.3f", sf);
        director->setContentScaleFactor(sf);
    } else {
        float sf = MIN(smallResolutionSize.height/designResolutionSize.height,
                       smallResolutionSize.width/designResolutionSize.width);
        CCLOG("use SMALL scale factor = %.3f", sf);
        director->setContentScaleFactor(sf);
    }

    CCLOG("register_all_packages()");
    register_all_packages();

    CCLOG("Create first scene: SceneLoadResource::create()");
        auto scene = SceneLoadResource::create();
        if (!scene) {
            CCLOGERROR("SceneLoadResource::create() returned nullptr! App will exit.");
        #ifdef _WIN32
            MessageBoxA(nullptr,
                        "SceneLoadResource::create() failed (nullptr).",
                        "ERROR",
                        MB_OK | MB_ICONERROR);
        #endif
            return false;   // nhớ return false để thoát đúng
        }



    CCLOG("runWithScene()");
    director->runWithScene(scene);

    CCLOG("==> applicationDidFinishLaunching: END (return true)");
    return true; // nhớ return true, nếu false cũng sẽ thoát
}


// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}
