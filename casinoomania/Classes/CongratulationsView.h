//
//  CongratulationsView.h
//
//  Created by Stanislav Zheronkin on 03.12.16.
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include <functional>

class CongratulationsView : public cocos2d::Node
{
public:
    enum CongratulationType
    {
        BIGWIN = 0,
        BONUS,
        LEVELUP
    };
private:
    CongratulationType type;
    
    cocos2d::Sprite * light;
    cocos2d::Sprite * title;
    cocos2d::Sprite * arrow;
    
    std::vector<cocos2d::Node *> elements;
    
    std::function< void() > callback;
private:
    CongratulationsView();
    bool init(CongratulationType type, std::function< void() > callback);
    void animate();
    void hide();
    
    std::string tipForLevelUp(int level);
    
    cocos2d::RenderTexture * takeScreenshot();
public:
    static CongratulationsView * create(CongratulationType type, std::function< void() > callback = nullptr);
    
    void show(cocos2d::Node * parent);
};
