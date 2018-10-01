//
//  RouletteConfig.cpp
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 20.02.17.
//
//

#include "RouletteConfig.h"

#include <stdio.h>
#include <sstream>

const std::vector< std::string > RouletteConfig::Themes = {
    "city1",
    "city2",
    "city3",
    "city4",
    "city5",
    "city6",
    "city7",
    "city8"
};

const std::vector< int > RouletteConfig::ThemeRequirements = {
    1, 4, 7, 10, 13, 16, 19, 22
};

const std::vector< int > RouletteConfig::BetValues = {
    2, 10, 50, 100, 500, 1000, 2000, 5000, 10000, 20000, 50000
};

const std::vector< int > RouletteConfig::SectorNumbers = {
    0, 32, 15, 19, 4, 21, 2, 25, 17, 34, 6, 27, 13, 36, 11, 30, 8, 23, 10, 5, 24, 16, 33, 1, 20, 14, 31, 9, 22, 18, 29, 7, 28, 12, 35, 3, 26
};

const std::map< RouletteConfig::RateTypes, int > RouletteConfig::BetRates = {
    { RATE_TYPE_A, 35 },
    { RATE_TYPE_B, 17 },
    { RATE_TYPE_C, 11 },
    { RATE_TYPE_D,  8 },
    { RATE_TYPE_F,  5 },
    { RATE_TYPE_G,  2 },
    { RATE_TYPE_H,  2 },
    
    { RATE_TYPE_EVENODD , 1 },
    { RATE_TYPE_LOWHIGH , 1 },
    { RATE_TYPE_REDBLACK, 1 },
};

const std::vector< RouletteConfig::BetTableInfo > RouletteConfig::BettablePoints = {
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_F, cocos2d::Rect(134, 325, 32, 40), std::vector<int>{ 1,  2,  3,  4,  5,  6}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_F, cocos2d::Rect(199, 325, 32, 40), std::vector<int>{ 4,  5,  6,  7,  8,  9}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_F, cocos2d::Rect(264, 325, 32, 40), std::vector<int>{ 7,  8,  9, 10, 11, 12}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_F, cocos2d::Rect(330, 325, 32, 40), std::vector<int>{10, 11, 12, 13, 14, 15}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_F, cocos2d::Rect(395, 325, 32, 40), std::vector<int>{13, 14, 15, 16, 17, 18}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_F, cocos2d::Rect(460, 325, 32, 40), std::vector<int>{16, 17, 18, 19, 20, 21}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_F, cocos2d::Rect(527, 325, 32, 40), std::vector<int>{19, 20, 21, 22, 23, 24}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_F, cocos2d::Rect(592, 325, 32, 40), std::vector<int>{22, 23, 24, 25, 26, 27}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_F, cocos2d::Rect(657, 325, 32, 40), std::vector<int>{25, 26, 27, 28, 29, 30}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_F, cocos2d::Rect(723, 325, 32, 40), std::vector<int>{28, 29, 30, 31, 32, 33}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_F, cocos2d::Rect(789, 325, 32, 40), std::vector<int>{31, 32, 33, 34, 35, 36}),
    //11
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_D, cocos2d::Rect(134, 163, 32, 40), std::vector<int>{ 2,  3,  6,  5}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_D, cocos2d::Rect(199, 163, 32, 40), std::vector<int>{ 5,  6,  9,  8}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_D, cocos2d::Rect(264, 163, 32, 40), std::vector<int>{ 8,  9, 12, 11}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_D, cocos2d::Rect(330, 163, 32, 40), std::vector<int>{11, 12, 15, 14}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_D, cocos2d::Rect(395, 163, 32, 40), std::vector<int>{14, 15, 18, 17}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_D, cocos2d::Rect(460, 163, 32, 40), std::vector<int>{17, 18, 21, 20}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_D, cocos2d::Rect(527, 163, 32, 40), std::vector<int>{20, 21, 24, 23}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_D, cocos2d::Rect(592, 163, 32, 40), std::vector<int>{23, 24, 27, 26}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_D, cocos2d::Rect(657, 163, 32, 40), std::vector<int>{26, 27, 30, 29}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_D, cocos2d::Rect(723, 163, 32, 40), std::vector<int>{29, 30, 33, 32}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_D, cocos2d::Rect(789, 163, 32, 40), std::vector<int>{32, 33, 36, 35}),
    //22
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_D, cocos2d::Rect(134, 244, 32, 40), std::vector<int>{ 1,  2,  5,  4}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_D, cocos2d::Rect(199, 244, 32, 40), std::vector<int>{ 4,  5,  8,  7}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_D, cocos2d::Rect(264, 244, 32, 40), std::vector<int>{ 7,  8, 11, 10}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_D, cocos2d::Rect(330, 244, 32, 40), std::vector<int>{10, 11, 14, 13}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_D, cocos2d::Rect(395, 244, 32, 40), std::vector<int>{13, 14, 17, 16}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_D, cocos2d::Rect(460, 244, 32, 40), std::vector<int>{16, 17, 20, 19}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_D, cocos2d::Rect(527, 244, 32, 40), std::vector<int>{19, 20, 23, 22}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_D, cocos2d::Rect(592, 244, 32, 40), std::vector<int>{22, 23, 26, 25}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_D, cocos2d::Rect(657, 244, 32, 40), std::vector<int>{25, 26, 29, 28}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_D, cocos2d::Rect(723, 244, 32, 40), std::vector<int>{28, 29, 32, 31}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_D, cocos2d::Rect(789, 244, 32, 40), std::vector<int>{31, 32, 35, 34}),
    //33
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_C, cocos2d::Rect( 66, 163, 32, 40), std::vector<int>{ 0,  3,  2}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_C, cocos2d::Rect( 66, 244, 32, 40), std::vector<int>{ 0,  2,  1}),
    //35
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_C, cocos2d::Rect(100, 325, 32, 40), std::vector<int>{ 1,  2,  3}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_C, cocos2d::Rect(166, 325, 32, 40), std::vector<int>{ 4,  5,  6}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_C, cocos2d::Rect(233, 325, 32, 40), std::vector<int>{ 7,  8,  9}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_C, cocos2d::Rect(297, 325, 32, 40), std::vector<int>{10, 11, 12}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_C, cocos2d::Rect(362, 325, 32, 40), std::vector<int>{13, 14, 15}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_C, cocos2d::Rect(429, 325, 32, 40), std::vector<int>{16, 17, 18}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_C, cocos2d::Rect(494, 325, 32, 40), std::vector<int>{19, 20, 21}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_C, cocos2d::Rect(560, 325, 32, 40), std::vector<int>{22, 23, 24}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_C, cocos2d::Rect(625, 325, 32, 40), std::vector<int>{25, 26, 27}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_C, cocos2d::Rect(690, 325, 32, 40), std::vector<int>{28, 29, 30}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_C, cocos2d::Rect(756, 325, 32, 40), std::vector<int>{31, 32, 33}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_C, cocos2d::Rect(823, 325, 32, 40), std::vector<int>{34, 35, 36}),
    //47
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(100, 163, 32, 40), std::vector<int>{ 2,  3}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(166, 163, 32, 40), std::vector<int>{ 5,  6}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(233, 163, 32, 40), std::vector<int>{ 8,  9}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(297, 163, 32, 40), std::vector<int>{11, 13}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(362, 163, 32, 40), std::vector<int>{14, 15}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(429, 163, 32, 40), std::vector<int>{17, 18}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(494, 163, 32, 40), std::vector<int>{20, 21}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(560, 163, 32, 40), std::vector<int>{23, 24}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(625, 163, 32, 40), std::vector<int>{26, 27}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(690, 163, 32, 40), std::vector<int>{29, 30}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(756, 163, 32, 40), std::vector<int>{32, 33}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(823, 163, 32, 40), std::vector<int>{35, 36}),
    //59
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(100, 244, 32, 40), std::vector<int>{ 1,  2}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(166, 244, 32, 40), std::vector<int>{ 4,  5}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(233, 244, 32, 40), std::vector<int>{ 7,  8}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(297, 244, 32, 40), std::vector<int>{10, 11}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(362, 244, 32, 40), std::vector<int>{13, 14}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(429, 244, 32, 40), std::vector<int>{16, 17}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(494, 244, 32, 40), std::vector<int>{19, 20}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(560, 244, 32, 40), std::vector<int>{22, 23}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(625, 244, 32, 40), std::vector<int>{25, 26}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(690, 244, 32, 40), std::vector<int>{28, 29}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(756, 244, 32, 40), std::vector<int>{31, 32}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(823, 244, 32, 40), std::vector<int>{34, 35}),
    //71
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect( 66, 124, 32, 40), std::vector<int>{ 0,  3}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect( 66, 204, 32, 40), std::vector<int>{ 0,  2}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect( 66, 285, 32, 40), std::vector<int>{ 0,  1}),
    //74
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(134, 124, 32, 40), std::vector<int>{ 3,  6}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(134, 204, 32, 40), std::vector<int>{ 2,  5}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(134, 285, 32, 40), std::vector<int>{ 1,  4}),
    //77
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(199, 124, 32, 40), std::vector<int>{ 6,  9}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(199, 204, 32, 40), std::vector<int>{ 5,  8}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(199, 285, 32, 40), std::vector<int>{ 4,  7}),
    //80
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(264, 124, 32, 40), std::vector<int>{ 9, 12}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(264, 204, 32, 40), std::vector<int>{ 8, 11}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(264, 285, 32, 40), std::vector<int>{ 7, 10}),
    //83
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(330, 124, 32, 40), std::vector<int>{12, 15}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(330, 204, 32, 40), std::vector<int>{11, 14}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(330, 285, 32, 40), std::vector<int>{10, 13}),
    //86
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(395, 124, 32, 40), std::vector<int>{15, 18}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(395, 204, 32, 40), std::vector<int>{14, 17}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(395, 285, 32, 40), std::vector<int>{13, 16}),
    //89
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(460, 124, 32, 40), std::vector<int>{18, 21}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(460, 204, 32, 40), std::vector<int>{17, 20}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(460, 285, 32, 40), std::vector<int>{16, 19}),
    //92
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(527, 124, 32, 40), std::vector<int>{21, 24}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(527, 204, 32, 40), std::vector<int>{20, 23}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(527, 285, 32, 40), std::vector<int>{19, 22}),
    //95
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(592, 124, 32, 40), std::vector<int>{24, 27}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(592, 204, 32, 40), std::vector<int>{23, 26}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(592, 285, 32, 40), std::vector<int>{22, 25}),
    //98
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(657, 124, 32, 40), std::vector<int>{27, 30}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(657, 204, 32, 40), std::vector<int>{26, 29}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(657, 285, 32, 40), std::vector<int>{25, 28}),
    //101
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(723, 124, 32, 40), std::vector<int>{30, 33}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(723, 204, 32, 40), std::vector<int>{29, 32}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(723, 285, 32, 40), std::vector<int>{28, 31}),
    //104
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(789, 124, 32, 40), std::vector<int>{33, 36}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(789, 204, 32, 40), std::vector<int>{32, 35}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_B, cocos2d::Rect(789, 285, 32, 40), std::vector<int>{31, 34}),
    //107
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(100, 124, 32, 40), std::vector<int>{ 3}, 0),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(166, 124, 32, 40), std::vector<int>{ 6}, 1),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(233, 124, 32, 40), std::vector<int>{ 9}, 0),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(297, 124, 32, 40), std::vector<int>{12}, 0),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(362, 124, 32, 40), std::vector<int>{15}, 1),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(429, 124, 32, 40), std::vector<int>{18}, 0),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(494, 124, 32, 40), std::vector<int>{21}, 0),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(560, 124, 32, 40), std::vector<int>{24}, 1),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(625, 124, 32, 40), std::vector<int>{27}, 0),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(690, 124, 32, 40), std::vector<int>{30}, 0),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(756, 124, 32, 40), std::vector<int>{33}, 1),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(823, 124, 32, 40), std::vector<int>{36}, 0),
    //119
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(100, 204, 32, 40), std::vector<int>{ 2}, 1),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(166, 204, 32, 40), std::vector<int>{ 5}, 0),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(233, 204, 32, 40), std::vector<int>{ 8}, 1),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(297, 204, 32, 40), std::vector<int>{11}, 1),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(362, 204, 32, 40), std::vector<int>{14}, 0),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(429, 204, 32, 40), std::vector<int>{17}, 1),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(494, 204, 32, 40), std::vector<int>{20}, 1),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(560, 204, 32, 40), std::vector<int>{23}, 0),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(625, 204, 32, 40), std::vector<int>{26}, 1),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(690, 204, 32, 40), std::vector<int>{29}, 1),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(756, 204, 32, 40), std::vector<int>{32}, 0),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(823, 204, 32, 40), std::vector<int>{35}, 1),
    //131
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(100, 285, 32, 40), std::vector<int>{ 1}, 0),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(166, 285, 32, 40), std::vector<int>{ 4}, 1),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(233, 285, 32, 40), std::vector<int>{ 7}, 0),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(297, 285, 32, 40), std::vector<int>{10}, 1),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(362, 285, 32, 40), std::vector<int>{13}, 1),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(429, 285, 32, 40), std::vector<int>{16}, 0),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(494, 285, 32, 40), std::vector<int>{19}, 0),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(560, 285, 32, 40), std::vector<int>{22}, 1),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(625, 285, 32, 40), std::vector<int>{25}, 0),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(690, 285, 32, 40), std::vector<int>{28}, 1),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(756, 285, 32, 40), std::vector<int>{31}, 1),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect(823, 285, 32, 40), std::vector<int>{34}, 0),
    //143
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_A, cocos2d::Rect( 32, 204, 32, 40), std::vector<int>{ 0}),
    //144
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_G, cocos2d::Rect(199, 369, 196, 40), std::vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_G, cocos2d::Rect(460, 369, 196, 40), std::vector<int>{13,14,15,16,17,18,19,20,21,22,23,24}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_G, cocos2d::Rect(723, 369, 196, 40), std::vector<int>{25,26,27,28,29,30,31,32,33,34,35,36}),
    //147
    // 1nd, 2nd, 3rd
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_H, cocos2d::Rect(889, 124,  32, 40), std::vector<int>{ 3, 6, 9,12,15,18,21,24,27,30,33,36}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_H, cocos2d::Rect(889, 204,  32, 40), std::vector<int>{ 2, 5, 8,11,14,17,20,23,26,29,32,35}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_H, cocos2d::Rect(889, 285,  32, 40), std::vector<int>{ 1, 4, 7,10,13,16,19,22,25,28,31,34}),
    //150
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_EVENODD,  cocos2d::Rect(756, 447, 132, 40), std::vector<int>{ 2, 4, 6, 8,10,12,14,16,18,20,22,24,26,28,30,32,34,36}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_EVENODD,  cocos2d::Rect(166, 447, 132, 40), std::vector<int>{ 1, 3, 5, 7, 9,11,13,15,17,19,21,23,25,27,29,31,33,35}),
    //152
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_REDBLACK, cocos2d::Rect(362, 447, 132, 40), std::vector<int>{ 1, 3, 5, 7, 9,12,14,16,18,19,21,23,25,27,30,32,34,36}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_REDBLACK, cocos2d::Rect(560, 447, 132, 40), std::vector<int>{ 2, 4, 6, 8,10,11,13,15,17,20,22,24,26,28,29,31,33,35}),
    //154
    // 1 ~ 18, 19 ~ 36
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_LOWHIGH,  cocos2d::Rect(264,  42, 330, 40), std::vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18}),
    RouletteConfig::BetTableInfo(RouletteConfig::RateTypes::RATE_TYPE_LOWHIGH,  cocos2d::Rect(657,  42, 330, 40), std::vector<int>{19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36})
    // 156
};

RouletteConfig::BetTableInfo RouletteConfig::getNumberInfo(int number)
{
    for (auto &item: BettablePoints)
    {
        if (item.type == RATE_TYPE_A && item.values[0] == number)
        {
            return item;
        }
    }
    
    return BetTableInfo(RateTypes::RATE_TYPE_A, cocos2d::Rect::ZERO, std::vector<int>());
}

RouletteConfig::NumberColor RouletteConfig::getNumberColor(int number)
{
    if (number == 0)
        return NumberColor::ZERO;
    
    auto item = getNumberInfo(number);
    
    if (item.area.equals(cocos2d::Rect::ZERO) == false)
    {
        return item.red ? NumberColor::RED : NumberColor::BLACK;
    }

    return NumberColor::BLACK;
}

int RouletteConfig::themeNameToChapter(std::string theme)
{
    for (int i = 0; i < Themes.size(); i++)
    {
        if (Themes[i] == theme)
            return i + 1;
    }
    
    return 1;
}
