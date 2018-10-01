//
//  PayoutsView.h
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 22.11.16.
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include <string>
#include <functional>

class PayoutsView : public cocos2d::Node
{
private:
    cocos2d::ui::Button * block;
    cocos2d::ui::PageView * list;
    
    std::string currentTheme;
    int         currentLines;
private:
    PayoutsView();
    bool init(std::string theme, int lines);
    
    void BuildPages();
    
    void onButtonPressed(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType event);
    void ProcessButtonAction(cocos2d::ui::Button * button);
public:
    static PayoutsView * create(std::string theme, int lines);
    
    void show(cocos2d::Node * parent);
    void hide();
};
