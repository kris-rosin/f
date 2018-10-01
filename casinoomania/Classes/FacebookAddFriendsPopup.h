//
//  FacebookAddFriendsPopup.h
//
//  Created by Stanislav Zheronkin on 14.05.17.
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include <functional>

class FacebookAddFriendsPopup : public cocos2d::Node
{
private:
    FacebookAddFriendsPopup();
    
    bool init(std::string message);
    void hide();
    
    void onButtonPressed(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType event);
    void ProcessButtonAction(cocos2d::ui::Widget * button);
public:
    static FacebookAddFriendsPopup * create(std::string message);
    
    void show(cocos2d::Node * parent);
    void onSharingCompleted(bool success);
};
