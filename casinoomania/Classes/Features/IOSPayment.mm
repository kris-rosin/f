//
//  IOSPayment.m
//  ZombieSmasher
//
//  Created by papo on 10/31/13.
//
//

#include "IOSPayment.h"
#import "MKStoreKit.h"
#import "PaymentManager.h"

static IOSPayment   _sharedIOSPayment;

bool IOSPayment::initialized = false;

IOSPayment* IOSPayment::shared()
{
    if (!initialized)
    {
        initialized = true;
        [[MKStoreKit sharedKit] startProductRequest];
        
        [[NSNotificationCenter defaultCenter] addObserverForName:kMKStoreKitProductPurchasedNotification
                                                          object:nil
                                                           queue:[[NSOperationQueue alloc] init]
                                                      usingBlock:^(NSNotification *note) {
                                                          
                                                          NSLog(@"Purchased/Subscribed to product with id: %@", [note object]);
                                                          
                                                          NSString * purchasedFeature = [note object];
                                                          
                                                          NSLog(@"%@", purchasedFeature);
                                                          
                                                          PaymentManager::shared()->onComplete([purchasedFeature UTF8String]);
                                                      }];
        
        [[NSNotificationCenter defaultCenter] addObserverForName:kMKStoreKitProductPurchaseFailedNotification
                                                          object:nil
                                                           queue:[[NSOperationQueue alloc] init]
                                                      usingBlock:^(NSNotification *note) {
                                                          PaymentManager::shared()->onCancel();
                                                      }];
    }
    
    return &_sharedIOSPayment;
}

bool IOSPayment::isFeaturePurchased(const char* kFeatureID)
{
    NSString * key = [NSString stringWithUTF8String:kFeatureID];
    
    return [[MKStoreKit sharedKit] isProductPurchased:key];
}

bool IOSPayment::isSubscriptionActive(const char* kFeatureID)
{
    NSString* key = [NSString stringWithUTF8String:kFeatureID];
    
    return [[[MKStoreKit sharedKit] expiryDateForProduct:key] compare:[NSDate date]] == NSComparisonResult::NSOrderedDescending;
}

void IOSPayment::buyFeature(const char *kFeatureAId) {
    NSString* key = [NSString stringWithUTF8String:kFeatureAId];
    NSLog(@"%@", [NSString stringWithUTF8String:kFeatureAId]);
    
    [[MKStoreKit sharedKit] initiatePaymentRequestForProductWithIdentifier:key];
}

bool IOSPayment::canConsumeProduct(const char *productName, int quantity)
{
    return true;
}

bool IOSPayment::consumeProduct(const char *productName, int quantity)
{
    return true;
}

bool IOSPayment::removeAllKeychainData()
{
    return true;
}
