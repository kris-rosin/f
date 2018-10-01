//
//  ScrollBar.h
//
//  Created by Stanislav Zheronkin on 23.10.15.
//  Copyright (c) 2015 CodeExotics Studio. All rights reserved.
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class ScrollBar : public cocos2d::Node
{
private:
    cocos2d::ui::ScrollView   * _target;
    cocos2d::Sprite           * scrollThumb;
    
    int _lastContentSize;
private:
    ScrollBar();
public:
    virtual ~ScrollBar();
    
    bool initWithScrollView(cocos2d::ui::ScrollView * targetControl, std::string backgroundSprite, std::string thumbSprite);
    
    static ScrollBar * create(cocos2d::ui::ScrollView * targetControl, std::string backgroundSprite, std::string thumbSprite);
    
    void update(float delta);
};
