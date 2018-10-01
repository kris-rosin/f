//
//  Bridge.c
//  Steampunk
//
//  Created by Hammy Rahardja on 11/29/16.
//
//

#include "Bridge.h"

#include "AppController.h"

static Bridge _shareAppManager;
static bool _isInitApp = true;

Bridge* Bridge::shared() {
    if (_isInitApp) {
        _isInitApp = false;
        _shareAppManager.init();
    }
    
    return &_shareAppManager;
}

void Bridge::init() {
    
}

void Bridge::showInterstitialChartboost() {
    [AppController chartboostShowInterstitial];
}

void Bridge::showInterstitialAdmob() {
    [AppController admobShowInterstitial];
}

void Bridge::showBannerAdmob() {
    [AppController admobShowBanner];
}

void Bridge::playRewardedVideo() {
    [AppController playRewardedVideo];
}

void Bridge::shareTheGame(int shareType) {
    [AppController shareTheGame:(shareType)];
}
