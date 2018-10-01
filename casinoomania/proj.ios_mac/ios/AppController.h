#import <UIKit/UIKit.h>

@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate> {

}

+ (AppController*)getInstance;

+ (void)admobShowBanner;

+ (void)chartboostShowInterstitial;
+ (bool)chartboostInterstitialIsAvailabled;
+ (void)chartboostShowMoreApps;
+ (void)chartboostRewardedVideo;

+ (void)shareTheGame:(int)shareType;

+ (void)playRewardedVideo;
+ (void)admobShowInterstitial;


@property(nonatomic, readonly) RootViewController* viewController;

@end

