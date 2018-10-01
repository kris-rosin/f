/****************************************************************************
 Copyright (c) 2010 cocos2d-x.org

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

#import "AppController.h"
#import "cocos2d.h"
#import "AppDelegate.h"
#import "RootViewController.h"
#import <Chartboost/Chartboost.h>
#import <GoogleMobileAds/GoogleMobileAds.h>
#import "AdmobChartboost.h"
#import "LocalNotificationsWrapper.h"
#import "mConstants.h"
#import "IOSPayment.h"
#import "AppManager.h"
#import "mConstants.h"
#import "FacebookManager-iOS.h"
#import <FBSDKCoreKit/FBSDKCoreKit.h>
#import <Fabric/Fabric.h>
#import <Crashlytics/Crashlytics.h>


@interface AppController () <ChartboostDelegate, GADInterstitialDelegate, GADRewardBasedVideoAdDelegate>

@property(nonatomic, strong) GADInterstitial *admobInterstitial;

@end

@implementation AppController

@synthesize window;

#pragma mark -
#pragma mark Application lifecycle

// cocos2d application instance
static AppDelegate s_sharedApplication;

// Additional Variable from Hammy
static bool _isAdmobRewardedVideoDelegateRegistered = false;
AppController* instance;

+ (AppController*) getInstance
{
    return instance;
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    [Fabric with:@[[Crashlytics class]]];

    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];

    // Use RootViewController to manage CCEAGLView
    _viewController = [[RootViewController alloc]init];
    _viewController.wantsFullScreenLayout = YES;
    

    // Set RootViewController to window
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        // warning: addSubView doesn't work on iOS6
        [window addSubview: _viewController.view];
    }
    else
    {
        // use this method on ios6
        [window setRootViewController:_viewController];
    }

    [window makeKeyAndVisible];

    [[UIApplication sharedApplication] setStatusBarHidden:true];
    

    //MARK: Chartboost
    // initialize the Chartboost library
    NSString *mChartboostAppID = [NSString stringWithCString:chartboostAppID.c_str() encoding:[NSString defaultCStringEncoding]];
    NSString *mChartboostAppSignature = [NSString stringWithCString:chartboostAppSignature.c_str() encoding:[NSString defaultCStringEncoding]];
    [Chartboost startWithAppId:mChartboostAppID appSignature:mChartboostAppSignature delegate:self];
    [Chartboost cacheInterstitial:CBLocationHomeScreen];
    [Chartboost cacheMoreApps:CBLocationHomeScreen];
    [Chartboost cacheRewardedVideo:CBLocationItemStore];
    
    
    //MARK: Admob
    self.admobInterstitial = [self createAndLoadInterstitial];
    [self createAndLoadRewardedVideo];
    
    //MARK: Instance
    instance = self;
    
    //MARK: Notification Periodic!
    [LocalNotificationsWrapper registerLocalNotifications];
    
    int time = 60 * 60 * 8; //150 minutes, from 3 minutes * 50 fuels;
    [LocalNotificationsWrapper scheduleNotificationRepeated:[NSDate dateWithTimeIntervalSinceNow:time] repeatInterval:NSCalendarUnitDay];

    //hackyhack_facebook
    FacebookManager_iOS::Initialize(application, launchOptions);
    
    //Payment initialisation
    IOSPayment::shared();
    
    return YES;
}
    
//hackyhack_facebook
- (BOOL)application:(UIApplication *)application
            openURL:(NSURL *)url
            options:(NSDictionary<UIApplicationOpenURLOptionsKey,id> *)options {
    
    return FacebookManager_iOS::handleOpenURL(application, url, options);
}


- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
     //We don't need to call this method any more. It will interupt user defined game pause&resume logic
    /* cocos2d::Director::getInstance()->pause(); */
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
     //We don't need to call this method any more. It will interupt user defined game pause&resume logic
    /* cocos2d::Director::getInstance()->resume(); */
    application.applicationIconBadgeNumber = 0;
    [FBSDKAppEvents activateApp];
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    cocos2d::Application::getInstance()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    application.applicationIconBadgeNumber = 0;
    cocos2d::Application::getInstance()->applicationWillEnterForeground();
}

- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
}

#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
    cocos2d::log("Attempt to free memory");
    cocos2d::Director::getInstance()->getTextureCache()->removeUnusedTextures();
}


#if __has_feature(objc_arc)
#else
- (void)dealloc {
    [window release];
    [_viewController release];
    [super dealloc];
}
#endif

//******************************************************************************
//   ADS JNI START_HERE
//******************************************************************************
//MARK: Chartboost

- (void)didCacheInterstitial:(CBLocation)location
{
    cocos2d::log("Chartboost didCacheInterstitial");
}

- (void)didFailToLoadInterstitial:(CBLocation)location
                        withError:(CBLoadError)error
{
    cocos2d::log("Chartboost didFailToLoadInterstitial");
}

+(void)chartboostShowInterstitial
{
    [Chartboost showInterstitial:CBLocationHomeScreen];
}

+ (bool)chartboostInterstitialIsAvailabled
{
    return true;
    return [Chartboost hasInterstitial:CBLocationHomeScreen];
}

+(void)chartboostShowMoreApps
{
    if( [Chartboost hasMoreApps:CBLocationHomeScreen ] )
    {
        [Chartboost showMoreApps:CBLocationHomeScreen];
    }
}

+(void)chartboostShowRewardedVideo
{
    [Chartboost showRewardedVideo:CBLocationHomeScreen];
}

+(bool)chartboostRewardedVideoIsAvailabled
{
    return true;
    return [Chartboost hasRewardedVideo:CBLocationHomeScreen];
}

//-- BEGIN delegate video --

+(void)playRewardedVideo
{
    if ([GADRewardBasedVideoAd sharedInstance].isReady) {
        NSLog(@"hackyhack: ADMOB VIDEO ad ready");
        [[AppController getInstance] showAdmobRewardedVideo];
    } else {
        NSLog(@"hackyhack: ADMOB VIDEO not ready");
        if (_isAdmobRewardedVideoDelegateRegistered == false) {
            _isAdmobRewardedVideoDelegateRegistered = true;
            //hackyhack_weird: somehow, this is not working anymore!
            //            [GADRewardBasedVideoAd sharedInstance].delegate = self;
        }
        NSString *mAdmobRewardedID = [NSString stringWithCString:admobRewardedVideoID.c_str() encoding:[NSString defaultCStringEncoding]];
        [[GADRewardBasedVideoAd sharedInstance] loadRequest:[GADRequest request]
                                               withAdUnitID:mAdmobRewardedID];
        [GADRewardBasedVideoAd sharedInstance].delegate = self;
        
        [self chartboostRewardedVideo];
    }
}

- (void)showAdmobRewardedVideo {
    if ([GADRewardBasedVideoAd sharedInstance].isReady) {
        [[GADRewardBasedVideoAd sharedInstance] presentFromRootViewController:window.rootViewController];
    }
}

- (void)showAdmobInterstitial {
    //hackyhack_admob
    if (self.admobInterstitial.isReady) {
        NSLog(@"hackyhack: ready");
        [self.admobInterstitial presentFromRootViewController:window.rootViewController];
    } else {
        NSLog(@"hackyhack: not ready");
        self.admobInterstitial = [self createAndLoadInterstitial];
        [AppController chartboostShowInterstitial];
    }
}

+(void)chartboostRewardedVideo
{
    NSLog(@"hackyhack - show Reward Video");
    if ([Chartboost hasRewardedVideo:CBLocationItemStore]) {
        [Chartboost showRewardedVideo:CBLocationItemStore];
    } else {
        cocos2d::MessageBox( "Ads is currently unavailable, please wait a minute.", "No ads available" );
    }
}

- (void)didFailToLoadRewardedVideo:(CBLocation)location
                         withError:(CBLoadError)error
{
    NSLog(@"hackyhack - Reward Video Fail");
    //    cocos2d::MessageBox( "No ads available3. Please try again later.", "" );
}

- (void)didDismissRewardedVideo:(CBLocation)location
{
}

- (void)didCloseRewardedVideo:(CBLocation)location
{
}

- (void)didClickRewardedVideo:(CBLocation)location{
}

- (void)didCompleteRewardedVideo:(CBLocation)location
                      withReward:(int)reward
{
    NSLog(@"hackyhack: CHARTBOOST reward %d", reward);
    AppManager::shared()->giveVideoReward(reward);
}
//-- END delegate video --


//MARK: Charboost Delegate
- (void)didDisplayMoreApps:(CBLocation)location;
{
}

- (void)didCacheMoreApps:(CBLocation)location;
{
}

- (void)didDismissMoreApps:(CBLocation)location;
{
}

- (void)didFailToLoadMoreApps:(CBLocation)location
                    withError:(CBLoadError)error
{
}

+(void)admobShowInterstitial
{
    CCLOG("hackyhack: Show Interstitial");
    [[AppController getInstance] showAdmobInterstitial];
}


//hackyhack_admob_position
+(void)admobShowBanner
{
    UIWindow* window = [[UIApplication sharedApplication] keyWindow];
    if (!window) {
        window = (UIWindow*)[[[UIApplication sharedApplication] windows] objectAtIndex:0];
    }
    UIViewController* rootViewController = window.rootViewController;
    
    NSString *mAdmobBannerID = [NSString stringWithCString:admobBannerID.c_str() encoding:[NSString defaultCStringEncoding]];
    GADBannerView* admobBannerView = [[GADBannerView alloc] initWithAdSize:kGADAdSizeSmartBannerLandscape];
    admobBannerView.adUnitID = mAdmobBannerID;
    admobBannerView.rootViewController = rootViewController;
    
    [rootViewController.view addSubview:admobBannerView];
    
    GADRequest* request = [GADRequest request];
    [admobBannerView loadRequest:request];
}

- (void)interstitialDidDismissScreen:(GADInterstitial *)ad
{
    NSLog(@"hackyhack: interstitialDidDismissScreen:");
}

- (GADInterstitial *)createAndLoadInterstitial {
    
    NSString *mAdmobInterstitialID = [NSString stringWithCString:admobInterstitialID.c_str() encoding:[NSString defaultCStringEncoding]];
    GADInterstitial *interstitial = [[GADInterstitial alloc] initWithAdUnitID:mAdmobInterstitialID];
    //hackyhack_weird: somehow, this is not working anymore!
    interstitial.delegate = self;
    GADRequest *request = [GADRequest request];
    [interstitial loadRequest:request];
    
    return interstitial;
}

- (void)createAndLoadRewardedVideo {
    if (_isAdmobRewardedVideoDelegateRegistered == false) {
        _isAdmobRewardedVideoDelegateRegistered = true;
        //hackyhack_weird: somehow, this is not working anymore!
        [GADRewardBasedVideoAd sharedInstance].delegate = self;
    }
    GADRequest *request = [GADRequest request];
    NSString *mAdmobRewardedID = [NSString stringWithCString:admobRewardedVideoID.c_str() encoding:[NSString defaultCStringEncoding]];
    [[GADRewardBasedVideoAd sharedInstance] loadRequest:request
                                           withAdUnitID:mAdmobRewardedID];
}


#pragma mark - GADInterstitialDelegate

- (void)interstitialDidReceiveAd:(GADInterstitial *)ad
{
    NSLog(@"hackyhack ADMob: interstitialDidReceiveAd");
}

- (void)interstitial:(GADInterstitial *)ad didFailToReceiveAdWithError:(GADRequestError *)error
{
    NSLog(@"hackyhack ADMob: interstitialDidFailToReceiveAdWithError: %@", [error localizedDescription]);
}

- (void)interstitialWillPresentScreen:(GADInterstitial *)ad
{
    NSLog(@"hackyhack ADMob: interstitialWillPresentScreen:");
}
- (void)interstitialWillDismissScreen:(GADInterstitial *)ad
{
    self.admobInterstitial = [self createAndLoadInterstitial];
    NSLog(@"hackyhack ADMob: interstitialWillDismissScreen:");
}
- (void)interstitialWillLeaveApplication:(GADInterstitial *)ad
{
    self.admobInterstitial = [self createAndLoadInterstitial];
    NSLog(@"hackyhack ADMob: interstitialWillLeaveApplication:");
}


#pragma mark - GADRewardedVideoDelegate

- (void)rewardBasedVideoAd:(GADRewardBasedVideoAd *)rewardBasedVideoAd didRewardUserWithReward:(GADAdReward *)reward {
    
    NSLog(@"hackyhack: ADMOB reward %d", [reward.amount intValue]);
    AppManager::shared()->giveVideoReward([reward.amount intValue]);
}

- (void)rewardBasedVideoAdDidReceiveAd:(GADRewardBasedVideoAd *)rewardBasedVideoAd {
    NSLog(@"hackyhack: Reward based video ad is received.");
}

- (void)rewardBasedVideoAdDidOpen:(GADRewardBasedVideoAd *)rewardBasedVideoAd {
    NSLog(@"hackyhack: Opened reward based video ad.");
}

- (void)rewardBasedVideoAdDidStartPlaying:(GADRewardBasedVideoAd *)rewardBasedVideoAd {
    NSLog(@"hackyhack: Reward based video ad started playing.");
}

- (void)rewardBasedVideoAdDidClose:(GADRewardBasedVideoAd *)rewardBasedVideoAd {
    NSLog(@"hackyhack: Reward based video ad is closed.");
    [[AppController getInstance] createAndLoadRewardedVideo];
}

- (void)rewardBasedVideoAdWillLeaveApplication:(GADRewardBasedVideoAd *)rewardBasedVideoAd {
    NSLog(@"hackyhack: Reward based video ad will leave application.");
    [[AppController getInstance] createAndLoadRewardedVideo];
}

- (void)rewardBasedVideoAd:(GADRewardBasedVideoAd *)rewardBasedVideoAd
    didFailToLoadWithError:(NSError *)error {
    NSLog(@"hackyhack: Reward based video ad failed to load.");
}

//******************************************************************************
//   ADS JNI END_HERE
//******************************************************************************


//******************************************************************************
//   SHARING START_HERE
//******************************************************************************

+ (void)shareTheGame:(int)shareType {
    //Building Text, taking 3 hours!
    NSString *rawPrologue = [NSString stringWithUTF8String:MCONSTANTS_TEXT_SHARE_GAME];
    NSString *textPrologue = [rawPrologue stringByAddingPercentEncodingWithAllowedCharacters:[NSCharacterSet URLHostAllowedCharacterSet]];
    
//    NSString *rawGP = [NSString stringWithFormat:@"\n\nGoogle Play:\n%s", ID_ANDROID];
//    NSString *textGP = [rawGP stringByAddingPercentEncodingWithAllowedCharacters:[NSCharacterSet URLHostAllowedCharacterSet]];
    NSString *rawGP = [NSString stringWithFormat:@""];
    NSString *textGP = [rawGP stringByAddingPercentEncodingWithAllowedCharacters:[NSCharacterSet URLHostAllowedCharacterSet]];
    
    NSString *rawIOS = [NSString stringWithFormat:@"\n\niTunesStore:\n%s", ID_IOS];
    NSString *textIOS = [rawIOS stringByAddingPercentEncodingWithAllowedCharacters:[NSCharacterSet URLHostAllowedCharacterSet]];

    //Whatsapp
    if (shareType == 0) {
        NSString *whatsappString = [NSString stringWithFormat:@"whatsapp://send?text=%@%@%@",textPrologue, textIOS, textGP];
        NSURL *whatsappURL = [NSURL URLWithString:whatsappString];
        if ([[UIApplication sharedApplication] canOpenURL: whatsappURL]) {
            [[UIApplication sharedApplication] openURL: whatsappURL];
        }
        
    //Email
    } else if (shareType == 1) {
        NSString *emailString = [NSString stringWithFormat:@"%@%@%@",rawPrologue, rawIOS, rawGP];
        
        NSArray *activityItems = @[emailString];
        UIActivityViewController *activityVC = [[UIActivityViewController alloc] initWithActivityItems:activityItems applicationActivities:nil];
        activityVC.excludedActivityTypes = @[UIActivityTypeAssignToContact, UIActivityTypePrint];
        UIViewController *vc_self = (UIViewController*)[[(AppController*)
                                                    [[UIApplication sharedApplication]delegate] window] rootViewController];
//            [vc_self presentViewController:activityVC animated:TRUE completion:nil];
            
        //if iPhone
        if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
            [vc_self presentViewController:activityVC animated:YES completion:nil];
        }
        //if iPad
        else {
            // Change Rect to position Popover
            UIPopoverController *popup = [[UIPopoverController alloc] initWithContentViewController:activityVC];
            [popup presentPopoverFromRect:CGRectMake(vc_self.view.frame.size.width/2, vc_self.view.frame.size.height/4, 0, 0)inView:vc_self.view permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
        }
        
    //SMS
    } else if (shareType == 2) {
//        NSString *rawGP = [NSString stringWithFormat:@"\n\nGoogle Play:\n%s", ID_SHORT_ANDROID];
//        NSString *textGP = [rawGP stringByAddingPercentEncodingWithAllowedCharacters:[NSCharacterSet URLHostAllowedCharacterSet]];
        NSString *rawGP = [NSString stringWithFormat:@""];
        NSString *textGP = [rawGP stringByAddingPercentEncodingWithAllowedCharacters:[NSCharacterSet URLHostAllowedCharacterSet]];

        NSString *rawIOS = [NSString stringWithFormat:@"\n\niTunesStore:\n%s", ID_SHORT_IOS];
        NSString *textIOS = [rawIOS stringByAddingPercentEncodingWithAllowedCharacters:[NSCharacterSet URLHostAllowedCharacterSet]];

        NSString *whatsappString = [NSString stringWithFormat:@"sms:&body=%@%@%@",textPrologue, textIOS, textGP];
        NSURL *whatsappURL = [NSURL URLWithString:whatsappString];
        if ([[UIApplication sharedApplication] canOpenURL: whatsappURL]) {
            [[UIApplication sharedApplication] openURL: whatsappURL];
        }
    
    }
}

//******************************************************************************
//   SHARING END_HERE
//******************************************************************************

@end
