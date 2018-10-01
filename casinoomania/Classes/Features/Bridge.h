//
//  Bridge.h
//  Steampunk
//
//  Created by Hammy Rahardja on 11/29/16.
//
//

#ifndef Bridge_h
#define Bridge_h

#include "cocos2d.h"

#endif /* Bridge_h */

class BridgeDelegate
{
    
};

class Bridge{
    public:
    static Bridge* shared();
    
    void init();

    void showBannerAdmob();

    void showInterstitialChartboost();
    void showInterstitialAdmob();
    
    void playRewardedVideo();
    void giveVideoReward(int value);
    
    void shareTheGame(int shareType);
};
