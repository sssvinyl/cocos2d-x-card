/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "AppDelegate.h"
#include "HelloWorldScene.h"

 // #define USE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#endif

USING_NS_CC;

static cocos2d::Size kDesignResolutionSize = cocos2d::Size(1080.0f, 2080.0f);
static cocos2d::Size kSmallResolutionSize = cocos2d::Size(480.0f, 320.0f);
static cocos2d::Size kMediumResolutionSize = cocos2d::Size(1024.0f, 768.0f);
static cocos2d::Size kLargeResolutionSize = cocos2d::Size(2048.0f, 1536.0f);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8, 0 };

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching()
{
    cocos2d::Director* ccDirector = cocos2d::Director::getInstance();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    {
        cocos2d::FileUtils* fu = cocos2d::FileUtils::getInstance();
        fu->addSearchPath("Resources", true);
        fu->addSearchPath("../Resources", true);
        fu->addSearchPath("../../Resources", true);
    }
#endif

    cocos2d::GLView* glview = ccDirector->getOpenGLView();
    if (!glview)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = cocos2d::GLViewImpl::createWithRect("Solitaire", cocos2d::Rect(0, 0, 1080, 2080), 0.5f);
#else
        glview = cocos2d::GLViewImpl::create("Solitaire");
#endif
        ccDirector->setOpenGLView(glview);
    }

    glview->setDesignResolutionSize(
        kDesignResolutionSize.width,
        kDesignResolutionSize.height,
        ResolutionPolicy::FIXED_WIDTH);

    ccDirector->setDisplayStats(true);
    ccDirector->setAnimationInterval(1.0f / 60.0f);

    cocos2d::Scene* scene = HelloWorld::createScene();
    ccDirector->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#endif
}