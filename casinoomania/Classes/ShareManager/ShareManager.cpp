//
//  ShareManager.cpp
//
//  Created by Stanislav Zheronkin on 23.08.17.
//

#include "ShareManager.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "ShareManager-iOS.h"
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/jni/JniHelper.h"

static std::map<unsigned int, std::function<void(bool)>> boolCallbacks;

#endif

USING_NS_CC;

ShareManager::ShareManager()
{
    isScreenshotTaken = false;
}

ShareManager::ShareManager(const ShareManager & other)
{
}

ShareManager & ShareManager::operator=(const ShareManager & other)
{
    return *this;
}

ShareManager::~ShareManager()
{
}

ShareManager & ShareManager::getInstance()
{
    static ShareManager instance;
    
    return instance;
}

void ShareManager::sharePhotoFacebook(cocos2d::Image * image, std::string optionalURL /*= ""*/)
{
    if (!image)
        return;
    
    std::string path = FileUtils::getInstance()->getWritablePath() + ".photo.png";
    image->saveToFile(path);
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    ShareManager_iOS::sharePhotoFacebook(path, optionalURL);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       "com/codeexotics/tools/ShareManager",
                                       "sharePhotoFacebook",
                                       "(Ljava/lang/String;Ljava/lang/String;)V"))
    {
        jstring imagePath = t.env->NewStringUTF(path.c_str());
        jstring imageURL  = t.env->NewStringUTF(optionalURL.c_str());
        
        t.env->CallStaticVoidMethod(t.classID, t.methodID, imagePath, imageURL);
        
        t.env->DeleteLocalRef(imagePath);
        t.env->DeleteLocalRef(imageURL);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void ShareManager::shareURLFacebook(std::string url, std::function<void(bool)> callback, std::string optionalQuote /*= ""*/)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    ShareManager_iOS::shareURLFacebook(url, callback, optionalQuote);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       "com/codeexotics/tools/ShareManager",
                                       "shareURLFacebook",
                                       "(Ljava/lang/String;Ljava/lang/String;)V"))
    {
        jstring url_string   = t.env->NewStringUTF(url.c_str());
        jstring quote_string = t.env->NewStringUTF(optionalQuote.c_str());
        
        t.env->CallStaticVoidMethod(t.classID, t.methodID, url_string, quote_string);
        
        t.env->DeleteLocalRef(url_string);
        t.env->DeleteLocalRef(quote_string);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

cocos2d::RenderTexture * ShareManager::takeScreenshot()
{
    auto  wndSize = Director::getInstance()->getVisibleSize();
    
    mScreenshot = RenderTexture::create((int)wndSize.width, (int)wndSize.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
    //screenshot->setContentSize(wndSize);
    mScreenshot->beginWithClear(0, 0, 0, 0, 1.0f);
    
    Director::getInstance()->getRunningScene()->setPosition(- Director::getInstance()->getVisibleOrigin());
    Director::getInstance()->getRunningScene()->visit();
    
    mScreenshot->end();
    
    Director::getInstance()->getRenderer()->render();
    
    Director::getInstance()->getRunningScene()->setPosition(Vec2::ZERO);
    
    isScreenshotTaken = true;
    
    return mScreenshot;
}

cocos2d::RenderTexture * ShareManager::getScreenshot()
{
    return mScreenshot;
}

void ShareManager::releaseRetainScreenshot() {
    if (isScreenshotTaken) {
        mScreenshot->release();
        isScreenshotTaken = false;
    }
}

