//
//  BaccaratConfig.h
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 27.03.17.
//
//

#pragma once

#include <string>
#include <vector>

struct BaccaratConfig
{
    static const std::vector< std::string > Themes;
    static const std::vector< int > ThemeRequirements;
    
    static int themeNameToChapter(std::string theme);
    
    static const std::vector< int > BetValues;
};
