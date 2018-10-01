//
//  SettingsView.h
//
//  Created by Stanislav Zheronkin on 02.03.17.
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include <functional>

class ServerMessage;

class SettingsView : public cocos2d::Node
{
private:
    std::function< void() > callback;
    
    cocos2d::ui::Button * fb_connect;
private:
    SettingsView();
    bool init(std::function< void() > callback);
    void hide();
    
    void onCheckPressed(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType event);
    void onButtonPressed(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType event);
    void ProcessButtonAction(cocos2d::ui::Widget * button);
    
    void onSwitchStateChanged(bool state, std::string name);
    bool onRegistrationCallback(ServerMessage * message);
    
    cocos2d::Scene * findScene(cocos2d::Node * node);
public:
    static SettingsView * create(std::function< void() > callback = nullptr);
    
    void show(cocos2d::Node * parent);
};
