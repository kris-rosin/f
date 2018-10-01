#pragma once

#include "cocos2d.h"

class LoadingSplash : public cocos2d::Layer
{
public:
    enum TargetSceneType
    {
        MAIN_MENU,
        SLOTS_THEME,
        SLOTS_GAME,
        ROULETTE_THEME,
        ROULETTE_GAME,
        BACCARAT_THEME,
        BACCARAT_GAME,
        BLACKJACK_THEME,
        BLACKJACK_GAME,
        CASINOWAR_THEME,
        CASINOWAR_GAME,
        CAPSA_THEME,
        CAPSA_GAME
    };
private:
    float delaySeconds;
    
    TargetSceneType target;
    std::string     theme;
    
public:
    static cocos2d::Scene * createScene(TargetSceneType target, std::string theme = "");

    virtual bool init();
    
    virtual void update(float deltaTime);
    
    CREATE_FUNC(LoadingSplash);
};
