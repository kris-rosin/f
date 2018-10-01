//
//  SwitchControl.h
//
//  Created by Stanislav Zheronkin on 20.04.17.
//
//

#pragma once

#include "cocos2d.h"

class SwitchControl : public cocos2d::Node
{
private:
    cocos2d::Sprite * switcher;
    bool currentState;
    
    std::string name;
private:
    SwitchControl();
    bool init(std::string background, std::string thumb, std::function<void(bool, std::string)> callback, std::string name);
    
    std::function<void(bool, std::string)> callback;
public:
    static SwitchControl * create(std::string background, std::string thumb, std::function<void(bool, std::string)> callback, std::string name);
    
    void setState(bool switchedOn, bool animated = true);
    void switchState();
    bool getState();
};
