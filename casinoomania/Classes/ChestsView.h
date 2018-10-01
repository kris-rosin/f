//
//  ChestsView.h
//
//  Created by Stanislav Zheronkin on 04.12.16.
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include <functional>

class ChestsView : public cocos2d::Node
{
public:
    enum BonusType
    {
        DAILY = 0,
        BONUS,
    };
private:
    BonusType type;
    std::function< void() > callback;
    
    std::vector<cocos2d::Sprite*> chests;
    std::vector<cocos2d::ui::Button*> buttons;
private:
    ChestsView();
    bool init(BonusType type, std::function< void() > callback);
    void hide();
    
    void onChestOpen(cocos2d::Ref* sender);
    void showResult(int coins, int chestID);
    
    cocos2d::Scene * findScene(Node * node);
public:
    static ChestsView * create(BonusType type, std::function< void() > callback = nullptr);
    
    void show(cocos2d::Node * parent);
};
