#include "GameCenter.h"

#include "cocos2d.h"

#include "GameCenterIos.h"

std::vector<GameCenter::GameCenterListener*> GameCenter::listeners;
bool GameCenter::gcEnabled = false;

bool GameCenter::isLoggedIn()
{
    return (gcEnabled || [[GameCenterIos shared] isLoggedIn]);
}

void GameCenter::login()
{
    [[GameCenterIos shared] login];
}

void GameCenter::logout()
{
    [[GameCenterIos shared] logout];
}

std::string GameCenter::getUserName()
{
    NSString * name = [[GameCenterIos shared] getUserName];
    
    if (name != nil)
    {
        return [name UTF8String];
    }
    
    return "Unknown Player";
}

void GameCenter::registerListener(GameCenterListener * listener)
{
    auto i = std::find(listeners.begin(), listeners.end(), listener);
    
    if(i == listeners.end())
        listeners.push_back(listener);
}

void GameCenter::unregisterListener(GameCenterListener * listener)
{
    auto i = std::find(listeners.begin(), listeners.end(), listener);
    
    if(i != listeners.end())
        listeners.erase(i);
}

void GameCenter::onGameCenterLogin()
{
    gcEnabled = true;
    
    for(int i = 0; i < listeners.size(); i++)
        listeners[i]->onGameCenterLoggedIn();
}

void GameCenter::onGameCenterLogout()
{
    gcEnabled = false;
    
    for(int i = 0; i < listeners.size(); i++)
        listeners[i]->onGameCenterLoggedOut();
}

#pragma mark -
#pragma mark Achievements

bool GameCenter::showAchievements()
{
    if (!gcEnabled) return false;
    
    return [[GameCenterIos shared] showAchievements];
}

void GameCenter::postAchievement(const char* idName, int percentComplete)
{
    if (!gcEnabled) return;
    
    [[GameCenterIos shared] postAchievement:idName percent:[NSNumber numberWithInt:percentComplete]];
}

void GameCenter::clearAllAchievements()
{
    if (!gcEnabled) return;
    
    [[GameCenterIos shared] clearAllAchievements];
}

#pragma mark -
#pragma mark Leaderboard

bool GameCenter::showScores()
{
    if (!gcEnabled) return false;
    
    return [[GameCenterIos shared] showScores];
}

void GameCenter::postScore(const char* idName, int score)
{
    if (!gcEnabled) return;
    
    [[GameCenterIos shared] postScore:idName score:[NSNumber numberWithInt:score]];
}

void GameCenter::clearAllScores()
{
    if (!gcEnabled) return;
    
    [[GameCenterIos shared] clearAllScores];
}
