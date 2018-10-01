//
//  CoinsPanel.h
//
//  Created by Stanislav Zheronkin on 18.11.16.
//
//

#pragma once

#include "cocos2d.h"

class CoinsPanel : public cocos2d::Node
{
private:
    cocos2d::Label      * coinsLabel;
private:
    CoinsPanel();
    bool init();
public:
    static CoinsPanel * create();
    
    void update(float deltaTime);
};
