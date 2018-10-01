//
//  CapsaDefine.h
//  DovemobiCasino
//
//  Created by liu on 8/28/17.
//
//

#ifndef CapsaDefine_h
#define CapsaDefine_h

#define MAIN_FONT_FILE              "fonts/Berlin.ttf"
#define THIN_FONT_FILE              "fonts/gadugi.ttf"
#define ARIAL_FONT                  "arial"

// window
#define WIN_SIZE                    (Director::getInstance()->getWinSize())
#define VISIBLE_ORIGIN              (Director::getInstance()->getVisibleOrigin())

#define POS_SIZE(size, pos)         (Vec2(size.width * pos.x, size.height * pos.y))

#define MONITOR_USER    "monitor!#!"

#define MAX_PLAYERS         4
#define MAX_CARDS           52
#define MAX_PLAYER_CARDS    (MAX_CARDS / MAX_PLAYERS)
#define MAX_DRAW_CARDS      5

#define MESSAGE_TYPE                    "TYPE"
#define MESSAGE_TYPE_USERS              "USERS"
#define MESSAGE_TYPE_CARDS              "CARDS"
#define MESSAGE_TYPE_CARDS_UPDATE       "CARDSUPDATE"
#define MESSAGE_TYPE_THROWN_CARDS       "THROWNCARDS"
#define MESSAGE_TYPE_CLEAR_THROWN       "CLEARTHROWN"

//card
#define OTHER_CARD_SCALE            0.8f
#define THROWN_CARD_SCALE           0.8f

#define CARD_MOVE_TIME              0.3f
#define CARD_SHOW_TIME              0.1f

#define THROWN_CARD_ARC_ANGLE       10.0f

//key
#define     MASTER_KEY              "master"
#define     PLAYERS_KEY             "players"
#define     CARDS_KEY               "cards"
#define     THROWN_CARDS_KEY        "throwncards"
#define     DEPOSITE_KEY            "deposite"
#define     MAX_PLAYERS_KEY         "maxplayers"
#define     PRIVATE_KEY             "private"
#define     MAX_TIME_KEY            "maxtime"

// ui
#define     ROOM_DISTANCE_RATIO     1.1f

//room
#define     ROOM_VISIBLE_COUNT      10

#define     CAPSA_FAST_TURN_TIME    15
#define     CAPSA_NORMAL_TURN_TIME  30

//coin
#define     CITY_DEPOSITE           {100, 500, 1000, 2500, 5000, 10000, 25000, 50000}

#endif /* CapsaDefine_h */
