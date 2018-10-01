#import "GameCenterIos.h"

#import <GameKit/GameKit.h>
#import "AppController.h"
#import "RootViewController.h"

@interface GameCenterIos(intern)

@end

@implementation GameCenterIos

static GameCenterIos* instance = nil;

+ (GameCenterIos*)shared
{
    @synchronized(self) {
        if (instance == nil) {
            instance = [[self alloc] init];
        }
    }
    return instance;
}

- (void)login
{
    GKLocalPlayer* localPlayer = [GKLocalPlayer localPlayer];
    if (localPlayer.isAuthenticated) {
        return;
    }
    
    localPlayer.authenticateHandler = ^(UIViewController *viewController, NSError *error){
        if (viewController != nil) {
            AppController* appController = (AppController*) [UIApplication sharedApplication].delegate;
            
            [appController.viewController presentViewController:viewController animated:YES completion:nil];
        } else {
            if ([GKLocalPlayer localPlayer].authenticated) {
                return;
            } else {
                if (error.code != GKErrorAuthenticationInProgress) {
                    NSLog(@"[GameCenter] login failed: %@", error.localizedDescription);
                }
            }
        }
    };
}

- (void)logout
{
    GKLocalPlayer * localPlayer = [GKLocalPlayer localPlayer];
    if (localPlayer.isAuthenticated) {
        [localPlayer unregisterAllListeners];
    }
}

- (BOOL)isLoggedIn
{
    return ([GKLocalPlayer localPlayer].isAuthenticated);
}

- (NSString*)getUserName
{
    if ([GKLocalPlayer localPlayer].isAuthenticated)
    {
        if ([[GKLocalPlayer localPlayer] respondsToSelector:@selector(displayName)])
        {
            return [GKLocalPlayer localPlayer].displayName;
        }
        else
        {
            return [GKLocalPlayer localPlayer].alias;
        }
    }
    
    return nil;
}

#pragma mark -
#pragma mark Achievements

- (BOOL)showAchievements
{
    if (![GKLocalPlayer localPlayer].isAuthenticated) {
        return NO;
    }

    AppController * appController = (AppController*) [UIApplication sharedApplication].delegate;
    
    GKGameCenterViewController * gameCenterController = [[[GKGameCenterViewController alloc] init] autorelease];
    
    if (gameCenterController != nil)
    {
        gameCenterController.gameCenterDelegate = appController.viewController;
        gameCenterController.viewState = GKGameCenterViewControllerStateAchievements;
        [appController.viewController presentViewController: gameCenterController animated: YES completion:nil];
        
        return YES;
    }

    return NO;
}

- (void)postAchievement:(const char*)idName percent:(NSNumber*)percentComplete
{
    if (![GKLocalPlayer localPlayer].isAuthenticated) {
        return;
    }
    
    GKAchievement* achievement = [[[GKAchievement alloc] init] autorelease];
    achievement.identifier = [NSString stringWithUTF8String:idName];
    achievement.percentComplete = [percentComplete doubleValue];
    achievement.showsCompletionBanner = YES;
    
    [GKAchievement reportAchievements:[NSArray arrayWithObject:achievement] withCompletionHandler:^(NSError* error) {
        if (error) {
            NSLog(@"[GameCenter] postAchievement for %s failed: %@", idName, error.localizedDescription);
        }
    }];
}

- (void)clearAllAchievements
{
    // clear remote achievements
    [GKAchievement resetAchievementsWithCompletionHandler:^(NSError* error) {
        if (error) {
            NSLog(@"[GameCenter] clearAllAchivements failed: %@", error.localizedDescription);
        }
    }];
}

#pragma mark -
#pragma mark Leaderboard

- (BOOL)showScores
{
    if (![GKLocalPlayer localPlayer].isAuthenticated) {
        return NO;
    }

    AppController * appController = (AppController*) [UIApplication sharedApplication].delegate;
    
    GKGameCenterViewController * gameCenterController = [[GKGameCenterViewController alloc] init];
    if (gameCenterController != nil)
    {
        gameCenterController.gameCenterDelegate = appController.viewController;
        gameCenterController.viewState = GKGameCenterViewControllerStateLeaderboards;
        gameCenterController.leaderboardTimeScope = GKLeaderboardTimeScopeAllTime;
        [appController.viewController presentViewController: gameCenterController animated: YES completion:nil];
        
        return YES;
    }
    
    return NO;
}

- (void)postScore:(const char*)idName score:(NSNumber*)score;
{
    if (![GKLocalPlayer localPlayer].isAuthenticated) {
        return;
    }
    
    GKScore* gkScore = [[[GKScore alloc] init] autorelease];
    gkScore.leaderboardIdentifier = [NSString stringWithUTF8String:idName];
    gkScore.value = [score longLongValue];
    gkScore.shouldSetDefaultLeaderboard = YES;
    
    [GKScore reportScores:[NSArray arrayWithObject:gkScore] withCompletionHandler:^(NSError* error) {
        if (error) {
            NSLog(@"[GameCenter] postScore for %s failed: %@", idName, error.localizedDescription);
        }
    }];
}

- (void)clearAllScores
{
    NSLog(@"[GameCenter] WARNING! clearAllScores is not supported on this platform");
}

@end
