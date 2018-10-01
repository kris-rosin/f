//
//  ShopView.h
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 03.04.17.
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include <string>
#include <functional>

class ShopView : public cocos2d::Node
{
private:
    std::string selectedProductID;
    cocos2d::Sprite * selector;
    
    cocos2d::Vector<cocos2d::Node*> Tabs;
    
    cocos2d::Node  * activeTab;
    cocos2d::Label * coinsLabel;
private:
    ShopView();
    bool init();
    
    void onButtonPressed(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType event);
    void ProcessButtonAction(cocos2d::ui::Button * button);
    cocos2d::Scene * findScene(cocos2d::Node * node);
    
    void setSparkBlink(cocos2d::Sprite * spark);
    
    void onTabClicked(cocos2d::Ref * sender);
    void showVipInfo(cocos2d::Node * sender, bool supervip);
public:
    static ShopView * create();
    
    void show(cocos2d::Node * parent);
    void hide();
    void onSharingCompleted(bool success);

};
