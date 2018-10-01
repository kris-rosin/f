//
//  BJConfig.h
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 03.03.17.
//
//

#pragma once

#include <string>
#include <vector>

#define MAX_PLAYERS_HANDS 2

// cheat control, all MUST BE "false" for release
#define FORCE_INSURANCE_CASE false
#define FORCE_DEALER_BJ      false
#define FORCE_PLAYER_SPLIT   false
#define FORCE_ACE_SPLIT      false
#define FORCE_PLAYER_BJ      false

struct BJConfig
{
    static const std::vector< std::string > Themes;
    static const std::vector< int > ThemeRequirements;
    
    static int themeNameToChapter(std::string theme);
    
    static const std::vector< int > BetValues;
};

enum BlackJackActions
{
    STAND,
    HIT,
    SPLIT,
    SURRENDER,
    DOUBLE,
    INSURANCE,
    INSURANCE_DENY,
    DEAL,
    EQUAL_MONEY,
    REBET
};
