//
//  CasinowarConfig.h
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 10.04.17.
//
//

#pragma once

#include <string>
#include <vector>

#define FORCE_TIE_CASE false
#define FORCE_WAR_TIE  false

struct CasinowarConfig
{
    static const std::vector< std::string > Themes;
    static const std::vector< int > ThemeRequirements;
    
    static int themeNameToChapter(std::string theme);
    
    static const std::vector< int > BetValues;
};
