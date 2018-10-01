//
//  FreeSpinView.h
//
//  Created by Stanislav Zheronkin on 12.12.16.
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include <functional>

class FreeSpinView : public cocos2d::Node
{
private:
    std::function< void() > callback;
    
    cocos2d::Sprite * animation;
    
    int currentFrame;
    float totalTime;
    bool preloaded;
private:
    FreeSpinView();
    bool init(int amount, std::function< void() > callback);
    void hide();
    
    void update(float deltaTime);
public:
    static FreeSpinView * create(int amount, std::function< void() > callback = nullptr);
    
    void show(cocos2d::Node * parent);
};
