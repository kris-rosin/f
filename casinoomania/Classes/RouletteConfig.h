//
//  RouletteConfig.h
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 20.02.17.
//
//

#pragma once

#include <string>
#include <vector>
#include "math/CCGeometry.h"

#define FORCE_WIN_NUMBER false
#define WIN_NUMBER_VALUE 34

struct RouletteConfig
{
    enum NumberColor
    {
        ZERO = 0,
        RED,
        BLACK
    };
    
    enum RateTypes
    {
        RATE_TYPE_A,        //1  number,  35:1
        RATE_TYPE_B,        //2  number,  17:1
        RATE_TYPE_C,        //3  number,  11:1
        RATE_TYPE_D,        //4  number,   8:1
        RATE_TYPE_F,        //6  number,   5:1
        RATE_TYPE_G,        //12 number,   2:1
        RATE_TYPE_H,        //12 number,   2:1
        RATE_TYPE_EVENODD,  //1:1
        RATE_TYPE_REDBLACK, //1:1
        RATE_TYPE_LOWHIGH,  //1:1
        
        RATE_TYPE_COUNT
    };
    
    struct BetTableInfo
    {
        RateTypes type;
        
        cocos2d::Rect area;
        
        std::vector<int> values;
        
        bool red;
        
        BetTableInfo(RateTypes type, cocos2d::Rect area, std::vector<int> values, bool black = false) : type(type), area(area), values(values), red(!black)
        {
            this->area.origin.x -= this->area.size.width  / 2;
            this->area.origin.y += this->area.size.height / 2;
        }
    };
    
    static const std::vector< std::string > Themes;
    static const std::vector< int > ThemeRequirements;
    static const std::vector< BetTableInfo > BettablePoints;
    static const std::vector< int > BetValues;
    static const std::vector< int > SectorNumbers;
    static const std::map< RouletteConfig::RateTypes, int > BetRates;
    
    static int themeNameToChapter(std::string theme);
    
    static NumberColor getNumberColor(int number);
    static BetTableInfo getNumberInfo(int number);
};
