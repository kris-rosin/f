//
//  JackpotPanel.h
//
//  Created by Stanislav Zheronkin on 18.11.16.
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class JackpotPanel : public cocos2d::Node
{
private:
    cocos2d::Label * label;
private:
    JackpotPanel();
    bool init();
    
    void incrementValue();
public:
    static JackpotPanel * create();
    
    void update();
};
