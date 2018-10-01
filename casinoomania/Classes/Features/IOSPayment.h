//
//  IOSPayment.h
//  ZombieSmasher
//
//  Created by papo on 10/31/13.
//
//

#ifndef __IOSPayment_h__
#define __IOSPayment_h__

#include "cocos2d.h"

class IOSPayment {
private:
    static bool initialized;
public:
    static IOSPayment* shared();
    bool isFeaturePurchased(const char* kFeatureID);
    bool isSubscriptionActive(const char* kFeatureID);
    void buyFeature(const char* kFeatureAId);
    
    bool canConsumeProduct(const char* productName, int quantity);
    bool consumeProduct(const char* productName,int quantity);
    
    bool removeAllKeychainData();
};


#endif /* define __IOSPayment_h__ */
