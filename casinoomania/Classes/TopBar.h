//
//  TopBar.h
//
//  Created by Stanislav Zheronkin on 14.04.17.
//
//

#pragma once

#include "cocos2d.h"

#include "ui/CocosGUI.h"

class TopBar : public cocos2d::Node
{
private:
    std::function<void()> backCallback;
    
    cocos2d::ui::Button * buyButton;
    cocos2d::ui::Button * settings;
    cocos2d::ui::Button * refButton;
    
    cocos2d::EventListenerCustom * imageDownloaderListener;
private:
    TopBar();
    bool init(std::function<void()> backCallback);
    
    void onButtonPressed(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType event);
    void ProcessButtonAction(cocos2d::ui::Button * button);
    
    void onUserPicDownloaded(cocos2d::EventCustom * event);
public:
    static TopBar * create(std::function<void()> backCallback);
    
    void setEnabled(bool enable);
};
