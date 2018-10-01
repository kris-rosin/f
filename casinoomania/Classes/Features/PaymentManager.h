//
//  PaymentManager.h
//  ZombieSmasher
//
//  Created by papo on 10/31/13.
//
//

#ifndef __ZombieSmasher__PaymentManager__
#define __ZombieSmasher__PaymentManager__

#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOSPayment.h"
//#include "MoreBrainScene.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//#include "Scenes/MoreBrainScene.h"
#endif

extern bool isJustBuyCoin;
extern bool hasUpdatedCoinMainMenu;
extern bool hasUpdatedCoinShop;

class PaymentDelegate{
public:
    virtual ~PaymentDelegate(){}
    
    virtual void paymentOnComplete(const char* featureAId) = 0;
    virtual void paymentOnCancel() = 0;
};

class PaymentManager {
    PaymentDelegate*    _delegate;
//    std::function<void()>   _completionBlock;
//    std::function<void(const char*)> _onError;
    
public:
    
    PaymentManager();
    ~PaymentManager();
    
    static PaymentManager* shared();
    
    bool isFeaturePurchased(const char* kFeatureID);
    bool isSubscriptionActive(const char* kFeatureID);
    void buyFeature(const char* kFeatureAId);
    void buyFeatureCallback(const char* kFeatureAId, std::function<void(int, std::string)> callback);
    
//    void restorePreviousTransactionsOnComplete(const std::function<void()>&completionBlock, const std::function<void(const char*)>&
//                                               onError);
    
    bool canConsumeProduct(const char* productName, int quantity);
    bool consumeProduct(const char* productName,int quantity);
    
    bool removeAllKeychainData();
    
    
    void setDelegate(PaymentDelegate* d){ _delegate = d;}
    
    //private:
    void onComplete(const char* productID);
    void onCancel();
    
private:

};

#endif /* defined(__ZombieSmasher__PaymentManager__) */
