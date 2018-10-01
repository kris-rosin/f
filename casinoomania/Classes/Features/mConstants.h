//
//  mConstants.hpp
//  DovemobiCasino
//
//  Created by Hammy Rahardja on 4/4/17.
//
//

#ifndef mConstants_h
#define mConstants_h

#include <stdio.h>
#include <string>
#include "ui/CocosGUI.h"

using namespace std;

extern const char* GAME_TITLE;

extern bool TESTING_MODE;

extern std::string comingSoonMessage;

extern std::string admobBannerID;
extern std::string admobInterstitialID;
extern std::string admobRewardedVideoID;
extern std::string chartboostAppID;
extern std::string chartboostAppSignature;

extern int spinningCount;

extern std::string IAP1_STRING_ID;
extern std::string IAP2_STRING_ID;
extern std::string IAP3_STRING_ID;
extern std::string IAP4_STRING_ID;
extern std::string IAP5_STRING_ID;
extern std::string IAP6_STRING_ID;
extern std::string IAP7_STRING_ID;

extern std::string IAP1_PRICE;
extern std::string IAP2_PRICE;
extern std::string IAP3_PRICE;
extern std::string IAP4_PRICE;
extern std::string IAP5_PRICE;
extern std::string IAP6_PRICE;
extern std::string IAP7_PRICE;

extern const char* IAP1_CONST_CHAR_ID;
extern const char* IAP2_CONST_CHAR_ID;
extern const char* IAP3_CONST_CHAR_ID;
extern const char* IAP4_CONST_CHAR_ID;
extern const char* IAP5_CONST_CHAR_ID;
extern const char* IAP6_CONST_CHAR_ID;
extern const char* IAP7_CONST_CHAR_ID;

extern std::string NOTIFICATION_ALERT1;
extern std::string NOTIFICATION_ALERT2;
extern std::string NOTIFICATION_ALERT3;
extern std::string NOTIFICATION_ALERT4;
extern std::string NOTIFICATION_ALERT5;

// Piggy Bank
extern std::string IAP_PIGGY_BANK_PRICE;
extern std::string IAP_PIGGY_BANK_ID;
extern float       PIGGY_BANK_PROCENT;

// VIP Subscription
extern std::vector<std::tuple<int, std::string> > vipPrices;
extern std::vector<std::tuple<int, std::string> > vvipPrices;
extern std::tuple<int, int, int> vipBonuses;
extern std::tuple<int, int, int> vvipBonuses;

extern std::string IAP_VIP1_ID;
extern std::string IAP_VIP2_ID;
extern std::string IAP_VIP3_ID;

extern std::string IAP_VVIP1_ID;
extern std::string IAP_VVIP2_ID;
extern std::string IAP_VVIP3_ID;

// Game Stats
extern std::string GAME_SLOTS;
extern std::string GAME_ROULETTE;
extern std::string GAME_BACCARAT;
extern std::string GAME_BLACK_JACK;
extern std::string GAME_CASINO_WAR;



extern const char* MCONSTANTS_TEXT_SHARE_GAME;
extern const char* MCONSTANTS_TEXT_SUBJECT;
extern const char* MCONSTANTS_TEXT_RATEME;

extern int LIKE_FACEBOOK_REWARD_COIN;
extern int SHARE_FACEBOOK_REWARD_COIN;
extern int BONUS_LEVEL_UP;

//hackyhack_publish

#define GAME_TITLE_CONST                "Casinoomania"

//************************************************************
//* IOS
//************************************************************

#define RATE_ME_IOS                     "itms-apps://itunes.apple.com/app/id1246175547"
#define MORE_GAMES_IOS                  "https://itunes.apple.com/us/developer/leoni-podoan/id1225486608"
#define DOWNLOAD_ANOTHER_APP_IOS        ""
#define ID_IOS                          "https://itunes.apple.com/us/app/casinoomania-house-of-casino/id1246175547"
//https://goo.gl/
#define ID_SHORT_IOS                    "https://goo.gl/h68WP4"

//************************************************************
//* ANDROID
//************************************************************

#define RATE_ME_ANDROID                 "market://details?id=com.dovemobi.games.casinoomania.house.of.casino"
#define MORE_GAMES_ANDROID              "market://search?q=pub:Dovemobi+Games"
#define DOWNLOAD_ANOTHER_APP_ANDROID    ""
#define ID_ANDROID                      "https://play.google.com/store/apps/details?id=com.dovemobi.games.casinoomania.house.of.casino"
//https://goo.gl/
#define ID_SHORT_ANDROID                "https://goo.gl/zh8zDo"

//************************************************************
//* LEADERBOARD IOS & ANDROID
//************************************************************

#define ACHIEVEMENT_TROPHY_100_VALUE        100
#define ACHIEVEMENT_TROPHY_500_VALUE        500
#define ACHIEVEMENT_TROPHY_1000_VALUE       1000
#define ACHIEVEMENT_TROPHY_5000_VALUE       5000
#define ACHIEVEMENT_TROPHY_10000_VALUE      10000
#define ACHIEVEMENT_TROPHY_50000_VALUE      50000
#define ACHIEVEMENT_TROPHY_100000_VALUE     100000

//************************************************************
//* LEADERBOARD IOS
//************************************************************

#define IOS_LEADERBOARD_ID                      "com.dovemobi.games.casinoomania.house.of.casino.leaderboard"

#define IOS_ACHIEVEMENT_TROPHY_100              "com.dovemobi.games.casinoomania.house.of.casino.trophy100"
#define IOS_ACHIEVEMENT_TROPHY_500              "com.dovemobi.games.casinoomania.house.of.casino.trophy500"
#define IOS_ACHIEVEMENT_TROPHY_1000             "com.dovemobi.games.casinoomania.house.of.casino.trophy1000"
#define IOS_ACHIEVEMENT_TROPHY_5000             "com.dovemobi.games.casinoomania.house.of.casino.trophy5000"
#define IOS_ACHIEVEMENT_TROPHY_10000            "com.dovemobi.games.casinoomania.house.of.casino.trophy10000"
#define IOS_ACHIEVEMENT_TROPHY_50000            "com.dovemobi.games.casinoomania.house.of.casino.trophy50000"
#define IOS_ACHIEVEMENT_TROPHY_100000           "com.dovemobi.games.casinoomania.house.of.casino.trophy100000"

//************************************************************
//* LEADERBOARD ANDROID
//************************************************************

#define AND_LEADERBOARD_ID                      "CgkI_dat_9cWEAIQBw"

#define AND_ACHIEVEMENT_TROPHY_100              "CgkI_dat_9cWEAIQAA"
#define AND_ACHIEVEMENT_TROPHY_500              "CgkI_dat_9cWEAIQAQ"
#define AND_ACHIEVEMENT_TROPHY_1000             "CgkI_dat_9cWEAIQAg"
#define AND_ACHIEVEMENT_TROPHY_5000             "CgkI_dat_9cWEAIQAw"
#define AND_ACHIEVEMENT_TROPHY_10000            "CgkI_dat_9cWEAIQBA"
#define AND_ACHIEVEMENT_TROPHY_50000            "CgkI_dat_9cWEAIQBQ"
#define AND_ACHIEVEMENT_TROPHY_100000           "CgkI_dat_9cWEAIQBg"

//************************************************************
//* MISC
//************************************************************

#define ENCODE_KEY "n^0!biM25Mhn8D1%"

#define INITIAL_COINS           25000
#define INITIAL_LEVEL           1
#define DAILY_BONUS_COOLDOWN    3600

#define LIKE_FACEBOOK_PAGE      "https://www.facebook.com/Casinoomania-273305929808539/"
#define LIKE_COMMUNITY_PAGE     ""

#define IAP1_ID             "com.dovemobi.games.casinoomania.house.of.casino.iap1"
#define IAP2_ID             "com.dovemobi.games.casinoomania.house.of.casino.iap2"
#define IAP3_ID             "com.dovemobi.games.casinoomania.house.of.casino.iap3"
#define IAP4_ID             "com.dovemobi.games.casinoomania.house.of.casino.iap4"
#define IAP5_ID             "com.dovemobi.games.casinoomania.house.of.casino.iap5"
#define IAP6_ID             "com.dovemobi.games.casinoomania.house.of.casino.iap6"

#define CYCLE_INTERSTITIAL_SLOTS            8
#define CYCLE_INTERSTITIAL_ROULETTE         8
#define CYCLE_INTERSTITIAL_BLACKJACK        8
#define CYCLE_INTERSTITIAL_BACCARAT         8
#define CYCLE_INTERSTITIAL_CASINOWAR        8
#define CYCLE_RATEME_POPUP                  3

#define CYCLE_SPIN_RATEME                   5

//DONT CHANGE!
#define SPIN_SLOTS                          0
#define SPIN_ROULETTE                       1
#define SPIN_BLACKJACK                      2
#define SPIN_BACCARAT                       3
#define SPIN_CASINO_WAR                     4

//************************************************************
//* NOT CHANGED! ONLY IF NECESSARY
//************************************************************

#define IAP1_VALUE              2500000
#define IAP2_VALUE              1000000
#define IAP3_VALUE              400000
#define IAP4_VALUE              150000
#define IAP5_VALUE          	60000
#define IAP6_VALUE              25000
#define IAP7_VALUE          	2500

#define IAP1_ORI_VALUE          2500000
#define IAP2_ORI_VALUE          1000000
#define IAP3_ORI_VALUE          400000
#define IAP4_ORI_VALUE          150000
#define IAP5_ORI_VALUE          60000
#define IAP6_ORI_VALUE          25000
#define IAP7_ORI_VALUE          2500

#define IAP1_ORI_PERCENTAGE     0
#define IAP2_ORI_PERCENTAGE     0
#define IAP3_ORI_PERCENTAGE     0
#define IAP4_ORI_PERCENTAGE     0
#define IAP5_ORI_PERCENTAGE     0
#define IAP6_ORI_PERCENTAGE     0
#define IAP7_ORI_PERCENTAGE     0

//************************************************************
//* NOT CHANGED! SHARING CODE
//************************************************************

#define SHARE_WHATSAPP          0
#define SHARE_EMAIL             1
#define SHARE_SMS               2

//************************************************************
//* FACEBOOK FRIENDS INVITE REWARDS
//************************************************************

#define SERVER_API_VERSION      5
#define SERVER_API_URL          "http://casinoomania.com/api.php"
#define FB_CONNECT_REWARD       100000
#define FB_INVITE_REWARD        10000

#define CAPSA_IN_DEVELOPING

#endif /* mConstants_hpp */
