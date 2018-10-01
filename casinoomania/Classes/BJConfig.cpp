//
//  BJConfig.cpp
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 03.03.17.
//
//

#include "BJConfig.h"

#include <stdio.h>
#include <sstream>

const std::vector< std::string > BJConfig::Themes = {
    "city1",
    "city2",
    "city3",
    "city4",
    "city5",
    "city6",
    "city7",
    "city8"
};

const std::vector< int > BJConfig::ThemeRequirements = {
    1, 4, 7, 10, 13, 16, 19, 22
};

int BJConfig::themeNameToChapter(std::string theme)
{
    for (int i = 0; i < Themes.size(); i++)
    {
        if (Themes[i] == theme)
            return i + 1;
    }
    
    return 1;
}

const std::vector< int > BJConfig::BetValues = {
    2, 10, 50, 100, 500, 1000, 2000, 5000, 10000, 20000, 50000
};
