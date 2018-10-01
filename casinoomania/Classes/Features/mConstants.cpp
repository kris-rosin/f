//
//  mConstants.cpp
//  DovemobiCasino
//
//  Created by Hammy Rahardja on 4/4/17.
//
//

#include "mConstants.h"

//hackyhack_publish

//************************************************************
//* GAME TITLE
//************************************************************
const char* GAME_TITLE              = "Casinoomania";
int LIKE_FACEBOOK_REWARD_COIN       = 10000;
int SHARE_FACEBOOK_REWARD_COIN      = 10000;
int BONUS_LEVEL_UP                  = 250;

//************************************************************
//* IOS
//************************************************************

std::string admobBannerID           = "ca-app-pub-2247343494301229/8498020793";
std::string admobInterstitialID     = "ca-app-pub-2247343494301229/9974753996";
std::string admobRewardedVideoID    = "ca-app-pub-2247343494301229/2451487195";
std::string chartboostAppID         = "59391dfc04b0160763c98fce";
std::string chartboostAppSignature  = "909c18ed89c6535c77e8587729966cf47c304992";

int spinningCount                   = 0;

std::string NOTIFICATION_ALERT1     = "Collect Your Bonus Now!";
std::string NOTIFICATION_ALERT2     = "Casinooomania is waiting for you!";
std::string NOTIFICATION_ALERT3     = "You just get 1000 FREE COINS!";
std::string NOTIFICATION_ALERT4     = "Time to open your CHEST!";
std::string NOTIFICATION_ALERT5     = "WOW! Somebody just win a jackpot!";

//************************************************************
//* MISC
//************************************************************

std::string comingSoonMessage = "Not Available for Now, it would be for upcoming update!";

std::string IAP1_STRING_ID  = "com.dovemobi.games.casinoomania.house.of.casino.iap1";
std::string IAP2_STRING_ID  = "com.dovemobi.games.casinoomania.house.of.casino.iap2";
std::string IAP3_STRING_ID  = "com.dovemobi.games.casinoomania.house.of.casino.iap3";
std::string IAP4_STRING_ID  = "com.dovemobi.games.casinoomania.house.of.casino.iap4";
std::string IAP5_STRING_ID  = "com.dovemobi.games.casinoomania.house.of.casino.iap5";
std::string IAP6_STRING_ID  = "com.dovemobi.games.casinoomania.house.of.casino.iap6";
std::string IAP7_STRING_ID  = "free";

const char* IAP1_CONST_CHAR_ID = "com.dovemobi.games.casinoomania.house.of.casino.iap1";
const char* IAP2_CONST_CHAR_ID = "com.dovemobi.games.casinoomania.house.of.casino.iap2";
const char* IAP3_CONST_CHAR_ID = "com.dovemobi.games.casinoomania.house.of.casino.iap3";
const char* IAP4_CONST_CHAR_ID = "com.dovemobi.games.casinoomania.house.of.casino.iap4";
const char* IAP5_CONST_CHAR_ID = "com.dovemobi.games.casinoomania.house.of.casino.iap5";
const char* IAP6_CONST_CHAR_ID = "com.dovemobi.games.casinoomania.house.of.casino.iap6";
const char* IAP7_CONST_CHAR_ID = "free";

//************************************************************
//* TEXT SHARING UPDATE
//************************************************************

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

const char* MCONSTANTS_TEXT_SHARE_GAME = "Hello! Come and play Casino Game 'Casinoomania' : Slots, BlackJack, Roulette, Baccarat, and Casino War:";
const char* MCONSTANTS_TEXT_SUBJECT = "You love playing Casino Game? Come and play with us!";
const char* MCONSTANTS_TEXT_RATEME = "";

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

const char* MCONSTANTS_TEXT_SHARE_GAME = "Hello! Come and play Casino Game 'Casinoomania' : Slots, BlackJack, Roulette, Baccarat, and Casino War:";
const char* MCONSTANTS_TEXT_SUBJECT = "You love playing Casino Game? Come and play with us!";
const char* MCONSTANTS_TEXT_RATEME = "";

#endif

//************************************************************
//* NOT CHANGED! ONLY IF NECESSARY
//************************************************************
bool TESTING_MODE = true;

std::string IAP1_PRICE  = "$19.99";
std::string IAP2_PRICE  = "$9.99";
std::string IAP3_PRICE  = "$4.99";
std::string IAP4_PRICE  = "$2.99";
std::string IAP5_PRICE  = "$1.99";
std::string IAP6_PRICE  = "$0.99";
std::string IAP7_PRICE  = "FREE";

// Piggy Bank
std::string IAP_PIGGY_BANK_PRICE = "$2.99";
std::string IAP_PIGGY_BANK_ID    = "com.dovemobi.games.casinoomania.house.of.casino.piggybank";
float       PIGGY_BANK_PROCENT   = 0.40f;

// VIP Subscription
std::vector<std::tuple<int, std::string> > vipPrices =
{
    { 3, "$0.99"}, // days, price
    { 7, "$1.99"},
    {14, "$2.99"}
};

std::vector<std::tuple<int, std::string> > vvipPrices =
{
    { 3, "$1.99"},
    { 7, "$2.99"},
    {14, "$3.99"}
};

std::tuple<int, int, int> vipBonuses =
{
    2, 1000, 2 // experience multiplier, level bonus, daily bonus multiplier
};

std::tuple<int, int, int> vvipBonuses =
{
    5, 2500, 5 // experience multiplier, level bonus, daily bonus multiplier
};

std::string IAP_VIP1_ID  = "com.dovemobi.games.casinoomania.house.of.casino.mvip1";
std::string IAP_VIP2_ID  = "com.dovemobi.games.casinoomania.house.of.casino.mvip2";
std::string IAP_VIP3_ID  = "com.dovemobi.games.casinoomania.house.of.casino.mvip3";

std::string IAP_VVIP1_ID = "com.dovemobi.games.casinoomania.house.of.casino.mvvip1";
std::string IAP_VVIP2_ID = "com.dovemobi.games.casinoomania.house.of.casino.mvvip2";
std::string IAP_VVIP3_ID = "com.dovemobi.games.casinoomania.house.of.casino.mvvip3";

// Game Stats
std::string GAME_SLOTS       = "SLOTS";
std::string GAME_ROULETTE    = "ROULETTE";
std::string GAME_BACCARAT    = "BACCARAT";
std::string GAME_BLACK_JACK  = "BLACK_JACK";
std::string GAME_CASINO_WAR  = "CASINO_WAR";
