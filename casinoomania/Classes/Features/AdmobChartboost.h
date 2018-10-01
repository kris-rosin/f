//
//  AdmobChartboost.h
//  IslandDefense
//
//  Created by Hammy Rahardja on 7/11/15.
//
//

#ifndef __IslandDefense__AdmobChartboost__
#define __IslandDefense__AdmobChartboost__

#include "cocos2d.h"

extern int CLICKED_VIDEO_FROM;

class AdmobChartboostDelegate
{
    
};

class AdmobChartboost{
public:
    
    static AdmobChartboost* shared();
    
    void init();

    void showBannerAdmob();
    
    void showInterstitialChartboost();
    void showInterstitialAdmob();
    
    void playRewardedVideo(int clickedFrom);
    void giveVideoReward(int value);    
};

#endif /* defined(__IslandDefense__AdmobChartboost__) */
