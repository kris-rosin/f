//
//  MessageAlert.h
//
//  Created by Stanislav Zheronkin on 14.05.17.
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class MessageAlert : public cocos2d::Node
{
private:
    std::function<void()> callback;
private:
    MessageAlert();
    
    bool init(std::string message, bool withCloseButton, std::string actionButtonText, std::function<void()> actionCallback);
    void hide();
    
    void onButtonPressed(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType event);
    void ProcessButtonAction(cocos2d::ui::Widget * button);
    
    cocos2d::Scene * findScene(cocos2d::Node * node);
public:
    static MessageAlert * create(std::string message, bool withCloseButton = true, std::string actionButtonText = "", std::function<void()> actionCallback = nullptr);
    
    void show(cocos2d::Node * parent);
};
