//
//  AdmobChartboost.cpp
//  IslandDefense
//
//  Created by Hammy Rahardja on 7/11/15.
//
//

#include "AdmobChartboost.h"
#include "mConstants.h"
#include "PlayerProfile.h"
#include "BaseScene.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "Bridge.h"
#endif

using namespace cocos2d;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C" {
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include "cocos2d.h"
    
    void jniAndroidShowInterstitialChartboost() {
        cocos2d::JniMethodInfo method;
        bool b = cocos2d::JniHelper::getStaticMethodInfo(method,"org.cocos2dx.cpp.AppActivity","showInterstitialChartboost","()V");
        if(b) {
            method.env->CallStaticVoidMethod(method.classID,method.methodID);
            
            method.env->DeleteLocalRef(method.classID);
        }
    }
    
    void jniAndroidShowInterstitialAdmob() {
        cocos2d::JniMethodInfo method;
        bool b = cocos2d::JniHelper::getStaticMethodInfo(method,"org.cocos2dx.cpp.AppActivity","showInterstitialAdmob","()V");
        if(b) {
            method.env->CallStaticVoidMethod(method.classID,method.methodID);
            
            method.env->DeleteLocalRef(method.classID);
        }
    }

    void jniAndroidPlayRewardedVideo() {
        cocos2d::JniMethodInfo method;
        bool b = cocos2d::JniHelper::getStaticMethodInfo(method,"org.cocos2dx.cpp.AppActivity","playRewardedVideo","()V");
        if(b) {
            method.env->CallStaticVoidMethod(method.classID,method.methodID);
            
            method.env->DeleteLocalRef(method.classID);
        }
    }
    
    void Java_org_cocos2dx_cpp_AppActivity_videoReward(JNIEnv* env, jobject thiz, jstring value) {
        //vn.sunnet.cocos2dx.libs
        const char* tmpValue = env->GetStringUTFChars(value, 0);
        //hackyhack
        CCLOG("hackyhack video reward");
        int intVal = atoi(tmpValue);
        CCLOG("hackyhack video reward value: %i", intVal);

//        if (CLICKED_VIDEO_FROM == AdmobChartboost::REWARD_VIDEO_FUEL) {
//            ScoreCounter::shared().addMoney( kScoreFuel, intVal*1, true );
//        } else if (CLICKED_VIDEO_FROM == AdmobChartboost::REWARD_VIDEO_COINS) {
//            ScoreCounter::shared().addMoney( kScoreCrystals, intVal*5, true ); //coins
//        } else if (CLICKED_VIDEO_FROM == AdmobChartboost::REWARD_VIDEO_CHIPS) {
//            ScoreCounter::shared().addMoney( kScoreTicket, intVal*1, true ); //chips
//        } else if (CLICKED_VIDEO_FROM == AdmobChartboost::REWARD_VIDEO_HERO) {
//            ScoreCounter::shared().addMoney( kScoreCrystals, intVal*5, true ); //coins
//        } else if (CLICKED_VIDEO_FROM == AdmobChartboost::REWARD_VIDEO_GEAR) {
//            
//        }
//        UserData::shared().save();

    }
    
}
#endif


static AdmobChartboost _shareAppManager;
static bool _isInitApp = true;

AdmobChartboost* AdmobChartboost::shared() {
    if (_isInitApp) {
        _isInitApp = false;
        _shareAppManager.init();
    }
    
    return &_shareAppManager;
}

void AdmobChartboost::init() {
}

void AdmobChartboost::showInterstitialChartboost() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    Bridge::shared()->showInterstitialChartboost();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    jniAndroidShowInterstitialChartboost();
#endif
}

void AdmobChartboost::showInterstitialAdmob() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    Bridge::shared()->showInterstitialAdmob();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    jniAndroidShowInterstitialAdmob();
#endif
}

void AdmobChartboost::showBannerAdmob() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    Bridge::shared()->showBannerAdmob();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    jniAndroidShowInterstitialChartboost();
#endif
}

void AdmobChartboost::playRewardedVideo(int clickedFrom) {
//    CCLOG("hackyhack: playRewardedVideo - clickedFrom id: %d", CLICKED_VIDEO_FROM);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    Bridge::shared()->playRewardedVideo();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    jniAndroidPlayRewardedVideo();
#endif
}

void AdmobChartboost::giveVideoReward(int value) {
    CCLOG("hackyhack: giveVideoReward: %d", value);
    
    auto scene = BaseScene::findBaseScene();
    if (scene)
    {
        scene->increasePlayerValues(value, 0, false);
    }
}
