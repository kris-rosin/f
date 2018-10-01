//
//  SlotsConfig.cpp
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 18.11.16.
//
//

#include "SlotsConfig.h"

#include "AndroidCompatibility.h"

#include <stdio.h>
#include <sstream>

const int   SlotsConfig::lines = 30;
const int   SlotsConfig::wheel_items_amount = 24;
const float SlotsConfig::wheel_items_height = 159.0f;
const float SlotsConfig::wheel_items_width  = 148.0f;

const std::vector< std::string > SlotsConfig::themes = {
    "theme1",
    "theme2",
    "theme3",
    "theme4",
    "theme5",
    "theme6",
    "theme7",
    "theme8"
};



const std::vector< std::vector< int > > SlotsConfig::conditions = {
    { 0, 0, 0, 0, 0}, //1
    {-1,-1,-1,-1,-1}, //2
    { 1, 1, 1, 1, 1}, //3
    {-1, 0, 1, 0,-1}, //4
    { 1, 0,-1, 0, 1}, //5
    { 0,-1,-1,-1, 0}, //6
    { 0, 1, 1, 1, 0}, //7
    {-1,-1, 0, 1, 1}, //8
    { 1, 1, 0,-1,-1}, //9
    { 0,-1, 0, 1, 0}, //10
    { 0, 1, 0,-1, 0}, //11
    {-1, 0, 0, 0,-1}, //12
    { 1, 0, 0, 0, 1}, //13
    {-1, 0,-1, 0,-1}, //14
    { 1, 0, 1, 0, 1}, //15
    { 0, 0,-1, 0, 0}, //16
    { 0, 0, 1, 0, 0}, //17
    {-1,-1, 0,-1,-1}, //18
    { 1, 1, 0, 1, 1}, //19
    { 0, 1,-1, 1, 0}, //20
    { 0,-1, 1,-1, 0}, //21
    {-1, 1,-1, 1,-1}, //22
    { 1,-1, 1,-1, 1}, //23
    {-1, 1, 1, 1,-1}, //24
    { 1,-1,-1,-1, 1}, //25
    { 0,-1, 0,-1, 0}, //26
    { 0, 1, 0, 1, 0}, //27
    { 1, 1, 1, 0,-1}, //28
    {-1,-1,-1, 0, 1}, //29
    { 1,-1,-1,-1,-1}  //30
};

const std::vector< std::vector< unsigned char> > SlotsConfig::lines_colors = {
    {246, 244,  26}, //1
    {  3, 133, 250}, //2
    {128, 200,  53}, //3
    {  3, 222, 250}, //4
    {232,   3, 250}, //5
    { 21, 250,   3}, //6
    {250, 153,   3}, //7
    {250,  71,   3}, //8
    {203,  51,  80}, //9
    {128,   3, 250}, //10
    {  3, 250, 242}, //11
    {196,   3, 250}, //12
    {192, 250,  23}, //13
    {158,   2,  50}, //14
    { 69, 208, 255}, //15
    {252, 121, 156}, //16
    {253, 175, 121}, //17
    {198, 252, 121}, //18
    {252, 127, 121}, //19
    {203,  52,  51}, //20
    { 37, 146,  52}, //21
    {121, 198, 252}, //52
    {184, 252, 121}, //23
    {246,   9,   7}, //24
    { 93,  93,  93}, //25
    {  7,  76, 246}, //26
    {176,   7, 246}, //27
    {  6,  15, 205}, //28
    {155, 117,  56}, //29
    {246,   7, 156}  //30
};

const std::vector< std::tuple< int, bool, bool > > SlotsConfig::linebuttons = {
    { 8, true , false}, // 1
    {12, true , false}, // 2
    { 4, true , false}, // 3
    {15, false, false}, // 4
    { 1, false, false}, // 5
    {10, true , false}, // 6
    { 6, true , false}, // 7
    {13, false, false}, // 8
    { 3, false, false}, // 9
    { 9, false, false}, // 10
    { 7, false, false}, // 11
    {14, true , false}, // 12
    { 2, true , false}, // 13
    {11, false, false}, // 14
    { 5, false, false}, // 15
    { 9, false, true }, // 16
    { 7, false, true }, // 17
    {14, true , true }, // 18
    { 2, true , true }, // 19
    { 8, true , true }, // 20
    { 5, false, true }, // 21
    {13, false, true }, // 22
    { 3, false, true }, // 23
    {12, true , true }, // 24
    { 4, true , true }, // 25
    {10, true , true }, // 26
    { 6, true , true }, // 27
    {11, false, true }, // 28
    { 1, false, true }, // 29
    {15, false , true}  // 30
};

const std::vector< std::vector<std::string> > SlotsConfig::wheel_items = {
    {"J", "7", "10", "K", "7", "J", "10", "A", "Apple", "Wild", "Q", "K", "K", "Bonus", "Apple", "Q", "Diamond", "Apple", "Lemon", "A", "Star", "Q", "J", "10"},           // 1
    {"K", "Star", "Q", "Lemon", "Apple", "J", "Diamond", "A", "Bonus", "Apple", "Q", "A", "K", "7", "10", "7", "10", "K", "Q", "7", "10", "Lemon", "J", "A"},             // 2 no wild
//    {"K", "Star", "Q", "Lemon", "Apple", "J", "Diamond", "A", "Bonus", "Apple", "Q", "A", "K", "Wild", "10", "7", "10", "K", "Q", "7", "10", "Lemon", "J", "A"},           // 2 with wild
    {"K", "10", "J", "Lemon", "Q", "Diamond", "Bonus", "A", "Apple", "Star", "Q", "J", "Lemon", "7", "K", "A", "10", "Q", "K", "10", "A", "J", "Apple", "Wild"},             // 3
    {"Apple", "Lemon", "7", "K", "A", "Q", "10", "K", "Diamond", "Bonus", "J", "Star", "J", "7", "Apple", "10", "Q", "A", "J", "7", "10", "Star", "Q", "K"},                 // 4 no wild
//    {"Apple", "Lemon", "7", "K", "A", "Q", "10", "K", "Diamond", "Bonus", "J", "Star", "J", "7", "Apple", "10", "Q", "A", "J", "Wild", "10", "Star", "Q", "K"},              // 4 with wild
    {"Bonus", "A", "7", "Diamond", "10", "K", "Q", "J", "Apple", "Lemon", "Wild", "10", "Star", "K", "Star", "Q", "Lemon", "J", "Diamond", "Apple", "A", "10", "7", "Star"} // 5
};

const std::vector<std::string> SlotsConfig::payouts_items = {
    "Diamond", "7", "Star", "Lemon", "Apple", "A", "K", "Q", "J", "10", "Bonus", "Wild"
};

const std::vector< std::vector< int > > SlotsConfig::payouts = {
    {   5,   4,   3,   2,   1,   0,  0,  0,  0,  0, 0,   10 }, // 2
    {  50,  30,  20,  15,  12,  10,  8,  7,  6,  5, 0,  100 }, // 3
    { 250, 150, 125, 100,  75,  50, 40, 30, 25, 20, 0, 1000 }, // 4
    { 800, 400, 300, 250, 200, 100, 80, 70, 60, 50, 0, 5000 }  // 5
};

int SlotsConfig::getRandomPositionOfSymbolOnWheel(std::string symbol, int wheelID)
{
    if (wheelID >= wheel_items.size())
    {
        printf("Error: Wrong wheel ID passed: %d", wheelID);
        return -1;
    }
    
    std::vector< int > tmpIds;
    for (int i = 0; i < wheel_items[wheelID].size(); i++)
    {
        if (wheel_items[wheelID][i] == symbol)
        {
            tmpIds.push_back(i);
        }
    }
    
    //hackyhack_publish - arc4random_uniform
//    return tmpIds[arc4random_uniform(tmpIds.size())];
    return tmpIds[(rand() % tmpIds.size())];
}

int SlotsConfig::themeNameToChapter(std::string theme)
{
    for (int i = 0; i < themes.size(); i++)
    {
        if (themes[i] == theme)
            return i + 1;
    }
    
    return 1;
}

std::vector<int> SlotsConfig::getBetsForLevel(int level, int chapter)
{
    std::vector<int> result;
    result.reserve(10);
    
    result.push_back(1);
    result.push_back(2);
    result.push_back(5);
    result.push_back(10);
    result.push_back(20);
    
    if(level >= 3)
        result.push_back(40);
    if(level >= 5)
        result.push_back(80);
    if(level >= 7)
        result.push_back(100);
    if(level >= 10)
        result.push_back(200);
    if(level >= 15)
        result.push_back(500);
    if(level >= 20)
        result.push_back(1000);
    if(level >= 25)
        result.push_back(2000);
    if(level >= 30)
        result.push_back(5000);
    
    if(chapter > 1)
        result.erase(result.begin());
    if(chapter > 3)
        result.erase(result.begin());
    if(chapter > 5)
        result.erase(result.begin());
    if(chapter > 7)
        result.erase(result.begin());
    if(chapter > 9)
        result.erase(result.begin());
    if(chapter > 11)
        result.erase(result.begin());
    if(chapter > 13)
        result.erase(result.begin());

    return result;
}

int SlotsConfig::itemNameToID(std::string item)
{
    for (int i = 0; i < payouts_items.size(); i++)
    {
        if (payouts_items[i] == item)
            return i;
    }
    
    return -1;
}

int SlotsConfig::getUnlockLevelForTheme(std::string theme)
{
    int chapter = themeNameToChapter(theme);
    
    return 1 + (chapter - 1) * 3;
}

std::string SlotsConfig::getThemeUnlockedOnLevel(int level)
{
    for (int i = 0; i < themes.size(); i++)
    {
        int unlock = getUnlockLevelForTheme(themes[i]);
        
        if (level == unlock)
        {
            std::stringstream result;
            result << themes[i] << "(" << lines << " lines)";
            return result.str();
        }
        else if (unlock > level)
        {
            return "";
        }
    }
    
    return "";
}
