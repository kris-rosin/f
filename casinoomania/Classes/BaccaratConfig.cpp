//
//  BaccaratConfig.cpp
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 27.03.17.
//
//

#include "BaccaratConfig.h"

const std::vector< std::string > BaccaratConfig::Themes = {
    "city1",
    "city2",
    "city3",
    "city4",
    "city5",
    "city6",
    "city7",
    "city8"
};

const std::vector< int > BaccaratConfig::ThemeRequirements = {
    1, 4, 7, 10, 13, 16, 19, 22
};

int BaccaratConfig::themeNameToChapter(std::string theme)
{
    for (int i = 0; i < Themes.size(); i++)
    {
        if (Themes[i] == theme)
            return i + 1;
    }
    
    return 1;
}

const std::vector< int > BaccaratConfig::BetValues = {
    2, 10, 50, 100, 500, 1000, 2000, 5000, 10000, 20000, 50000
};
