//
//  BottomBar.h
//
//  Created by Stanislav Zheronkin on 24.04.17.
//
//

#pragma once

#include "cocos2d.h"

#include "ui/CocosGUI.h"

class BottomBar : public cocos2d::Node
{
private:
    cocos2d::ui::Layout * coinNode;
    cocos2d::Sprite * collect_label;
    cocos2d::Sprite * shadow;
    cocos2d::Label  * remainTimer;
private:
    BottomBar();
    bool init();
    
    void onButtonPressed(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType event);
    void ProcessButtonAction(cocos2d::ui::Button * button);
    
    std::string getTimeString(unsigned int value);
    
    void update(float deltaTime);
public:
    static BottomBar * create();
};
