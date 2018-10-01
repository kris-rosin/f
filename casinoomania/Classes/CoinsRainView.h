//
//  CoinsRainView.h
//
//  Created by Stanislav Zheronkin on 14.12.16.
//
//

#pragma once

#include "cocos2d.h"

#include <functional>

class CoinsRainView : public cocos2d::Node
{
private:
    int coinsAmount;
    
    float totalTime;
    
    std::vector<cocos2d::Sprite*> coins;
    std::vector<float> coinSpeed;
private:
    CoinsRainView();
    bool init(float multiple);
    void animate();
    void hide();
    void update(float deltaTime);
public:
    static CoinsRainView * create(float multiple);
    
    void show(cocos2d::Node * parent);
};
