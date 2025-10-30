#include "AppDelegate.h"
#include "SceneLoadResource.h"
#include "Common.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#define USE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
#endif

USING_NS_CC;

// MERGED: Resolution change from the new code is included here.
static cocos2d::Size designResolutionSize = cocos2d::Size(1024, 768);
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
#endif
}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
    GLView::setGLContextAttrs(glContextAttrs);
}

static int register_all_packages()
{
    return 0;
}

bool AppDelegate::applicationDidFinishLaunching()
{
    CCLOG("==> applicationDidFinishLaunching: START");

    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    // AllocConsole();
    // freopen("CONOUT$", "w", stdout);
    // freopen("CONOUT$", "w", stderr);
#endif

    if (!glview)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || \
    (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) ||   \
    (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        CCLOG("Create GLView with rect %dx%d", (int)designResolutionSize.width, (int)designResolutionSize.height);
        glview = GLViewImpl::createWithRect("Mario",
                                            cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        CCLOG("Create GLView default");
        glview = GLViewImpl::create("Mario");
#endif
        director->setOpenGLView(glview);
    }
    else
    {
        CCLOG("GLView already exists");
    }

    director->setAnimationInterval(1.0f / 60);

    glview->setDesignResolutionSize(
        designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);

    const auto frameSize = glview->getFrameSize();
    CCLOG("frameSize: %.0fx%.0f", frameSize.width, frameSize.height);

    if (frameSize.height > mediumResolutionSize.height)
    {
        float sf = MIN(largeResolutionSize.height / designResolutionSize.height,
                       largeResolutionSize.width / designResolutionSize.width);
        director->setContentScaleFactor(sf);
    }
    else if (frameSize.height > smallResolutionSize.height)
    {
        float sf = MIN(mediumResolutionSize.height / designResolutionSize.height,
                       mediumResolutionSize.width / designResolutionSize.width);
        director->setContentScaleFactor(sf);
    }
    else
    {
        float sf = MIN(smallResolutionSize.height / designResolutionSize.height,
                       smallResolutionSize.width / designResolutionSize.width);
        director->setContentScaleFactor(sf);
    }

    register_all_packages();

    auto scene = SceneLoadResource::create();
    if (!scene)
    {
        CCLOGERROR("SceneLoadResource::create() returned nullptr! App will exit.");
        return false;
    }

    director->runWithScene(scene);

    CCLOG("==> applicationDidFinishLaunching: END (return true)");
    return true;
}

void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();
#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#endif
}

void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();
#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#endif
}
