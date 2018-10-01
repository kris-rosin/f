//
//  ExperiencePanel.h
//
//  Created by Stanislav Zheronkin on 18.11.16.
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class ExperiencePanel : public cocos2d::Node
{
private:
    cocos2d::Label * levelLabel;
    
    cocos2d::ProgressTimer * level_bar_fill;
private:
    ExperiencePanel();
    bool init();
public:
    static ExperiencePanel * create();
    
    void update(float deltaTime);
};
