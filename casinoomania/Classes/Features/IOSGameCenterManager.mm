//
//  IOSGameCenterManager.m
//  ZombieSmasher
//
//  Created by papo on 10/31/13.
//
//

#include "IOSGameCenterManager.h"
#import <AudioToolbox/AudioToolbox.h>
#import "GameCenterIos.h"

static IOSGameCenterManager   _shareIOSManger;

IOSGameCenterManager* IOSGameCenterManager::shared()
{
    return & _shareIOSManger;
}

void IOSGameCenterManager::vibration()
{
    AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
}

void IOSGameCenterManager::loginGameCenter() {
    [[GameCenterIos shared] login];
}

void IOSGameCenterManager::showLeaderBoard()
{
    [[GameCenterIos shared] showScores];
}

void IOSGameCenterManager::submitAchievement(const char* identifier)
{
    [[GameCenterIos shared] postAchievement:identifier percent:[NSNumber numberWithInt:100]];
}

void IOSGameCenterManager::sendScore(int score, const char *category)
{
    [[GameCenterIos shared] postScore:category score:[NSNumber numberWithInt:score]];
}

