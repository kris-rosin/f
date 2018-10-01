//
//  AppManager.cpp
//  ZombieSmasher
//
//  Created by papo on 10/31/13.
//
//

#include "AppManager.h"
#include "mConstants.h"
#include "AdmobChartboost.h"
#include "PlayerProfile.h"
#include "BaseScene.h"

using namespace cocos2d;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOSGameCenterManager.h"
#include "Bridge.h"

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C" {
#include "mConstants.h"
#include "jni/JniHelper.h"
#include "jni.h"
    
    void jniAndroidVibration() {
        cocos2d::JniMethodInfo method;
        bool b = cocos2d::JniHelper::getStaticMethodInfo(method,"com/app/manager/AndroidManager","vibration","()V");
        if(b) {
            method.env->CallStaticVoidMethod(method.classID,method.methodID);
            
            method.env->DeleteLocalRef(method.classID);
        }
    }
    
    void jniAndroidRateApp() {
        cocos2d::JniMethodInfo method;
        bool b = cocos2d::JniHelper::getStaticMethodInfo(method,"com/app/manager/AndroidManager","rateApp","()V");
        if(b) {
            method.env->CallStaticVoidMethod(method.classID,method.methodID);
            
            method.env->DeleteLocalRef(method.classID);
        }
    }
    
    void jniAndroidHandleQuitFromMenu() {
        cocos2d::JniMethodInfo method;
        bool b = cocos2d::JniHelper::getStaticMethodInfo(method,"com/app/manager/AndroidManager","handleQuitFromMenu","()V");
        if(b) {
            method.env->CallStaticVoidMethod(method.classID,method.methodID);
            
            method.env->DeleteLocalRef(method.classID);
        }
    }
    
    void jniAndroidGameServicesSignIn() {
        cocos2d::JniMethodInfo method;
        bool b = cocos2d::JniHelper::getStaticMethodInfo(method,"com/app/manager/AndroidManager","gameServicesSignIn","()V");
        if(b) {
            method.env->CallStaticVoidMethod(method.classID,method.methodID);
            
            method.env->DeleteLocalRef(method.classID);
        }
    }
    
    void jniAndroidUpdateTopScoreLeaderboard(const char* leaderboardId, int score) {
        
        cocos2d::JniMethodInfo method;
        bool b = cocos2d::JniHelper::getStaticMethodInfo(method,"com/app/manager/AndroidManager","updateTopScoreLeaderboard","(Ljava/lang/String;I)V");
        if(b) {
            jstring s = method.env->NewStringUTF(leaderboardId);
            
            method.env->CallStaticVoidMethod(method.classID,method.methodID, s, (jint)score);
            
            method.env->DeleteLocalRef(method.classID);
        }
    }
    
    void jniAndroidShowLeaderboards() {
        cocos2d::JniMethodInfo method;
        bool b = cocos2d::JniHelper::getStaticMethodInfo(method,"com/app/manager/AndroidManager","showLeaderboards","()V");
        if(b) {
            method.env->CallStaticVoidMethod(method.classID,method.methodID);
            
            method.env->DeleteLocalRef(method.classID);
        }
    }
    
    void jniSubmitAchievement(const char*itemId)
    {
        cocos2d::JniMethodInfo method;
        bool b = cocos2d::JniHelper::getStaticMethodInfo(method,"com/app/manager/AndroidManager", "submitAchievement", "(Ljava/lang/String;)V");
        if (b) {
            jstring s = method.env->NewStringUTF(itemId);
            
            method.env->CallStaticVoidMethod(method.classID,method.methodID, s);
            
            method.env->DeleteLocalRef(method.classID);
        }
    }
    
    void jniAndroidOpenUrl(const char* url) {
        cocos2d::JniMethodInfo method;
        bool b = cocos2d::JniHelper::getStaticMethodInfo(method,"com/app/manager/AndroidManager","openUrl","(Ljava/lang/String;)V");
        
        if(b) {
            
            jstring s = method.env->NewStringUTF(url);
            
            method.env->CallStaticVoidMethod(method.classID,method.methodID, s);
            
            method.env->DeleteLocalRef(method.classID);
        }
    }
    
    void jniAndroidShareTheGame(const char* text, const char* subject, int shareCode) {
        cocos2d::JniMethodInfo method;
        bool b = cocos2d::JniHelper::getStaticMethodInfo(method,"com/app/manager/AndroidManager","shareTheGame","(Ljava/lang/String;Ljava/lang/String;I)V");
        if(b) {
            jstring mText = method.env->NewStringUTF(text);
            jstring mSubject = method.env->NewStringUTF(subject);
            
            method.env->CallStaticVoidMethod(method.classID,method.methodID, mText, mSubject, (jint)shareCode);
            
            method.env->DeleteLocalRef(method.classID);
        }
    }
    
    void Java_com_app_manager_AndroidManager_giveVideoReward(JNIEnv* env, jobject thiz, jstring value) {
        
        const char* tmpValue = env->GetStringUTFChars(value, 0);
        
        int intVal = atoi(tmpValue);
        
        AppManager::shared()->giveVideoReward(intVal);
    }

    void Java_org_cocos2dx_cpp_AppActivity_shareFbReward(JNIEnv* env, jobject thiz) {
        //vn.sunnet.cocos2dx.libs
        //hackyhack_publish - share fb
        CCLOG("hackyhack: shareFbReward cocos2dx, FB Reward Value - %d", SHARE_FACEBOOK_REWARD_COIN);
        auto scene = BaseScene::findBaseScene();
        if (scene)
        {
            scene->increasePlayerValues(SHARE_FACEBOOK_REWARD_COIN, 0, false);
        }

    }
    
}
#endif

static AppManager _shareAppManager;
static bool _isInitApp = true;


AppManager* AppManager::shared() {
    if (_isInitApp) {
        _isInitApp = false;
        _shareAppManager.init();
    }
    
    return &_shareAppManager;
}

void AppManager::init() {
    
}

void AppManager::vibration() {
#if  (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //    IOSGameCenterManager::shared()->vibration();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //    jniAndroidVibration();
#endif
}

void AppManager::loginGameCenter() {
#if  (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOSGameCenterManager::shared()->loginGameCenter();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    jniAndroidGameServicesSignIn();
#endif
}

void AppManager::showLeaderBoard() {
    CCLOG("hackyhack: showLeaderboard()");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOSGameCenterManager::shared()->showLeaderBoard();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    jniAndroidShowLeaderboards();
#endif
}

void AppManager::sendScore(int score, const char *category) {
    CCLOG("hackyhack: sendScore()");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOSGameCenterManager::shared()->sendScore(score, category);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    jniAndroidUpdateTopScoreLeaderboard(category, score);
#endif
}

void AppManager::submitAchievement(const char* identifier) {
    CCLOG("hackyhack: submitAchievement()");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOSGameCenterManager::shared()->submitAchievement(identifier);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    jniSubmitAchievement(identifier);
#endif
}


void AppManager::moreGames() {
    CCLOG("hackyhack: moreGames()");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    AppManager::openUrl(MORE_GAMES_ANDROID);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    AppManager::openUrl(MORE_GAMES_IOS);
#endif
}

void AppManager::openUrl(const std::string &url) {
    CCLOG("hackyhack: openUrl()");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    jniAndroidOpenUrl(url.data());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    Application::getInstance()->openURL(url.data());
#endif
}

void AppManager::followTwitter() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //    AppManager::openUrl(TWITTER_PAGE);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //    AppManager::openUrl(TWITTER_PAGE);
#endif
}

void AppManager::likeFacebookPage() {
    CCLOG("hackyhack: likeFacebookPage()");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    AppManager::openUrl(LIKE_FACEBOOK_PAGE);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    AppManager::openUrl(LIKE_FACEBOOK_PAGE);
#endif
}

void AppManager::likeCommunityPage() {
    CCLOG("hackyhack: likeCommunityPage()");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    AppManager::openUrl(LIKE_COMMUNITY_PAGE);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    AppManager::openUrl(LIKE_COMMUNITY_PAGE);
#endif
}

void AppManager::rateMe() {
    CCLOG("hackyhack: rateMe()");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    jniAndroidRateApp();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    AppManager::shared()->openUrl(RATE_ME_IOS);
#endif
}

void AppManager::shareTheGame(int shareType) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    jniAndroidShareTheGame(MCONSTANTS_TEXT_SHARE_GAME, MCONSTANTS_TEXT_SUBJECT, shareType);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    Bridge::shared()->shareTheGame(shareType);
#endif
}

void AppManager::handleQuitFromMenu() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //hackyhack_android
    jniAndroidHandleQuitFromMenu();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void AppManager::giveVideoReward(int value) {
    CCLOG("hackyhack: reward from video: %i", value);
    AdmobChartboost::shared()->giveVideoReward(value);
}

//hackyhack_setting_push_notification
void AppManager::setIOSDeviceToken(const char* deviceToken) {
    cocos2d::UserDefault::getInstance()->setStringForKey("device_token", std::string(deviceToken));
}

//hackyhack_setting_push_notification
std::string AppManager::getIOSDeviceToken() {
    std::string deviceToken = cocos2d::UserDefault::getInstance()->getStringForKey("device_token", "null");
    return deviceToken;
}



