//
//  PaymentManager.cpp
//  ZombieSmasher
//
//  Created by papo on 10/31/13.
//
//

#include "PaymentManager.h"
#include "cocos2d.h"
#include "PaymentManager.h"
#include "mConstants.h"
#include "PlayerProfile.h"
#include "BaseScene.h"

static PaymentManager     _sharedPaymentManager;

extern void changeCurrentCoin( int );

bool isJustBuyCoin = false;
bool hasUpdatedCoinMainMenu = false;
bool hasUpdatedCoinShop = false;

static std::function<void(int, std::string)> __BuyCoinsCallback = nullptr;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C"{
#include "jni/JniHelper.h"
#include "jni.h"
    
    void jniBuyProduct(const char* purchaseId){
        cocos2d::JniMethodInfo method;
        bool b = cocos2d::JniHelper::getStaticMethodInfo(method,"com/app/manager/AndroidManager", "buyProduct", "(Ljava/lang/String;)V");
        if (b) {
            jstring s = method.env->NewStringUTF(purchaseId);
            
            method.env->CallStaticVoidMethod(method.classID,method.methodID, s);
            
            method.env->DeleteLocalRef(method.classID);
        }

    }
    
    
    void Java_com_app_manager_AndroidManager_jniOnComplete(JNIEnv* env, jobject thiz, jstring featureAId) {
        //vn.sunnet.cocos2dx.libs
        const char* proID = env->GetStringUTFChars(featureAId, 0);
        CCLOG("hackyhack: jni call onComplete: %s", proID);
        PaymentManager::shared()->onComplete(proID);
        
        env->ReleaseStringUTFChars(featureAId, proID);
    }
    
    void Java_com_app_manager_AndroidManager_jniOnCancel(JNIEnv* env, jobject thiz) {
        //vn.sunnet.cocos2dx.libs
        CCLOG("jni call onCancel");
        PaymentManager::shared()->onCancel();
    }
}
#endif /* (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) */



PaymentManager* PaymentManager::shared()
{
    return &_sharedPaymentManager;
}

PaymentManager::PaymentManager()
{
    _delegate = NULL;
}

PaymentManager::~PaymentManager()
{
    
}


bool PaymentManager::isFeaturePurchased(const char *kFeatureID)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return IOSPayment::shared()->isFeaturePurchased(kFeatureID);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return false;
#endif
}

bool PaymentManager::isSubscriptionActive(const char *kFeatureID)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return IOSPayment::shared()->isSubscriptionActive(kFeatureID);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return false;
#endif
}

void PaymentManager::buyFeature(const char *kFeatureAId)
{
    CCLOG("hackyhack: buyFeature!");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    jniBuyProduct(kFeatureAId);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOSPayment::shared()->buyFeature(kFeatureAId);
#endif
}

void PaymentManager::buyFeatureCallback(const char *kFeatureAId, std::function<void(int, std::string)> callback) {
    __BuyCoinsCallback = callback;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    jniBuyProduct(kFeatureAId);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOSPayment::shared()->buyFeature(kFeatureAId);
#endif
}

bool PaymentManager::canConsumeProduct(const char *productName, int quantity)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return IOSPayment::shared()->canConsumeProduct(productName, quantity);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return false;
#endif
}

bool PaymentManager::consumeProduct(const char *productName, int quantity)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return IOSPayment::shared()->consumeProduct(productName, quantity);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return false;
#endif
}

bool PaymentManager::removeAllKeychainData()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return IOSPayment::shared()->removeAllKeychainData();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return false;
#endif
}

void PaymentManager::onComplete(const char *productID)
{
    CCLOG("hackyhack: onComplete prodID=%s", productID);
    
    unsigned int coins = 0;
    
    if (strcmp(productID, IAP1_CONST_CHAR_ID) == 0) {
        coins = IAP1_VALUE;
    } else if (strcmp(productID, IAP2_CONST_CHAR_ID) == 0) {
        coins = IAP2_VALUE;
    } else if (strcmp(productID, IAP3_CONST_CHAR_ID) == 0) {
        coins = IAP3_VALUE;
    } else if (strcmp(productID, IAP4_CONST_CHAR_ID) == 0) {
        coins = IAP4_VALUE;
    } else if (strcmp(productID, IAP5_CONST_CHAR_ID) == 0) {
        coins = IAP5_VALUE;
    } else if (strcmp(productID, IAP6_CONST_CHAR_ID) == 0) {
        coins = IAP6_VALUE;
    } else if (strcmp(productID, IAP_PIGGY_BANK_ID.c_str()) == 0) {
        coins = PlayerProfile::getInstance().breakPiggyBank();
    } else if (IAP_VIP1_ID == productID) {
        PlayerProfile::getInstance().activateVipSubscription(3, false);
    } else if (IAP_VIP2_ID == productID) {
        PlayerProfile::getInstance().activateVipSubscription(7, false);
    } else if (IAP_VIP3_ID == productID) {
        PlayerProfile::getInstance().activateVipSubscription(14, false);
    } else if (IAP_VVIP1_ID == productID) {
        PlayerProfile::getInstance().activateVipSubscription(3, true);
    } else if (IAP_VVIP2_ID == productID) {
        PlayerProfile::getInstance().activateVipSubscription(7, true);
    } else if (IAP_VVIP3_ID == productID) {
        PlayerProfile::getInstance().activateVipSubscription(14, true);
    }
    
    if (coins > 0)
    {
        auto scene = BaseScene::findBaseScene();
        
        if (scene)
        {
            scene->increasePlayerValues(coins, 0, false);
        }
    }
    
    if (__BuyCoinsCallback)
        __BuyCoinsCallback(0, productID);
}

void PaymentManager::onCancel()
{
    CCLOG("Payment Failed");
    if (_delegate) {
        _delegate->paymentOnCancel();
    }
}
