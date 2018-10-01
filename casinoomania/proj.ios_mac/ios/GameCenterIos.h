@interface GameCenterIos : NSObject
{
}

+ (GameCenterIos*)shared;

- (void)login;
- (void)logout;
- (BOOL)isLoggedIn;

- (BOOL)showAchievements;
- (void)postAchievement:(const char*)idName percent:(NSNumber*)percentComplete;
- (void)clearAllAchievements;

- (BOOL)showScores;
- (void)postScore:(const char*)idName score:(NSNumber*)score;
- (void)clearAllScores;

- (NSString*)getUserName;

@end
