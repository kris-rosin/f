//
//  WheelView.h
//
//  Created by Stanislav Zheronkin on 08.12.16.
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include <functional>

class WheelView : public cocos2d::Node
{
private:
    std::function< void(int) > callback;
    
    static std::vector< int > rewards;
    
    cocos2d::ui::Button * spinButton;
    cocos2d::Sprite     * wheel;
    cocos2d::Sprite     * spinEffect;
    cocos2d::Sprite     * background;
    cocos2d::Sprite     * arrow;
    
    int winAmount;
    int result;
    int spinsRemains;
    int lastSubSector;
    
    float spinSpeed;
private:
    WheelView();
    bool init(int spins, int winCoins, std::function< void(int) > callback);
    void hide();
    
    void onSpinPressed(cocos2d::Ref * sender);
    void update(float deltaTime);
    int  getCurrentSector();
    int  getCurrentReward();
    void showResult(int coins);
public:
    static WheelView * create(int spins, int winCoins, std::function< void(int) > callback = nullptr);
    
    void show(cocos2d::Node * parent);
};
