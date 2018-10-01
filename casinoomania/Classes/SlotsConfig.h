//
//  SlotsConfig.h
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 18.11.16.
//
//

#pragma once

#include <string>
#include <vector>

#define FORCE_SLOTS_BONUS_FREE_SPINS    false
#define FORCE_SLOTS_BONUS_WHEEL         false
#define FORCE_SLOTS_BONUS_CHEST         false
#define FREE_SPINS                      5

struct SlotsConfig
{
    static const std::vector< std::string > themes;
    static const std::vector< std::vector< int > > conditions;
    static const std::vector< std::vector< unsigned char> > lines_colors;
    static const std::vector< std::tuple< int, bool, bool > > linebuttons; // 1st - order number, 2nd - shifted, 3rd - right
    static const std::vector< std::vector<std::string> > wheel_items; //1st - wheel number (0 - 4), 2nd - item number (0 - 23)
    static const std::vector< std::string> payouts_items;
    static const std::vector< std::vector< int > > payouts;
    
    static const int lines;
    static const int wheel_items_amount;
    static const float wheel_items_height;
    static const float wheel_items_width;
    
    static int themeNameToChapter(std::string theme);
    static std::vector<int> getBetsForLevel(int level, int chapter);
    
    static int itemNameToID(std::string item);
    static int getRandomPositionOfSymbolOnWheel(std::string symbol, int wheelID);
    
    static int getUnlockLevelForTheme(std::string theme);
    static std::string getThemeUnlockedOnLevel(int level);
};
