//
//  PopupMenu.h
//
//  Created by Stanislav Zheronkin on 20.04.17.
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class PopupMenu : public cocos2d::Node
{
private:
    PopupMenu();
    bool init();
    void hide();
    
    void onButtonPressed(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType event);
    void ProcessButtonAction(cocos2d::ui::Widget * button);
public:
    static PopupMenu * create();
    
    void show(cocos2d::Node * parent);
};
