//
//  SignupView.h
//
//  Created by Stanislav Zheronkin on 01.11.17.
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include <functional>

class ServerMessage;

class SignupView : public cocos2d::Node
{
private:
    cocos2d::ui::PageView * pageView;
    
    cocos2d::ui::TextField * emailFieldIn;
    cocos2d::ui::TextField * emailField;
    cocos2d::ui::TextField * passFieldIn;
    cocos2d::ui::TextField * passField;
    cocos2d::ui::TextField * passField2;
    cocos2d::ui::TextField * nameField;
private:
    SignupView();
    bool init();
    void hide();
    
    void onButtonPressed(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType event);
    void ProcessButtonAction(cocos2d::ui::Widget * button);
    
    bool onRegistrationCallback(ServerMessage * message);
    bool onSigninCompleted(ServerMessage * message);
    bool onPasswordRestored(ServerMessage * message);
    
    cocos2d::ui::TextField * createTextField(cocos2d::Size textArea, std::string text = "", std::string placeholder = "", bool password = false);
    
    bool validateEmailAddress(std::string email);
    void restorePassword();
public:
    static SignupView * create();
    
    void show(cocos2d::Node * parent);
};
