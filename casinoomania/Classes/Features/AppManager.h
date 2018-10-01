//
//  AppManager.h
//  ZombieSmasher
//
//  Created by papo on 10/31/13.
//
//

#ifndef __Casinomania__AppManager__
#define __Casinomania__AppManager__

#include "cocos2d.h"

class AppManagerDelegate
{
    
};


class AppManager
{
public:
    static AppManager* shared();
    
    void init();
    
    void vibration();
    
    void loginGameCenter();
    void showLeaderBoard();
    
    void submitAchievement(const char* identifier);
    void sendScore(int score, const char* cagatory);
    
    void openUrl(const std::string& url);
    void rateMe();
    void moreGames();
    
    void likeFacebookPage();
    void likeCommunityPage();
    void followTwitter();
    
    void shareTheGame(int shareType);
    
    void handleQuitFromMenu();
    
    void giveVideoReward(int value);
    
    //hackyhack_setting_push_notification
    void setIOSDeviceToken(const char* deviceToken);
    std::string getIOSDeviceToken();
    
private:
};

#endif
