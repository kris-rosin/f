//
//  CoinsPanel.cpp
//
//  Created by Stanislav Zheronkin on 18.11.16.
//
//

#include "CoinsPanel.h"

#include "PlayerProfile.h"
#include "Utilities.h"
#include "ShopView.h"

USING_NS_CC;

CoinsPanel::CoinsPanel() : coinsLabel(nullptr)
{
    
}

CoinsPanel * CoinsPanel::create()
{
    CoinsPanel * pRet = new CoinsPanel();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool CoinsPanel::init()
{
    if (!Node::init())
        return false;
    
    setCascadeOpacityEnabled(true);
    
    //coins panel
    auto coins_panel = Sprite::create("ui/MainMenu/coinbalance.png");
    addChild(coins_panel);
    coins_panel->setAnchorPoint(Vec2::ZERO);
    
    coinsLabel = Label::createWithTTF("0", "fonts/gadugi.ttf", 36.0f);
    coinsLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    coinsLabel->setColor(Color3B::WHITE);
    coinsLabel->setPosition(Vec2(coins_panel->getContentSize().width - 20, coins_panel->getContentSize().height * 0.5f));
    coins_panel->addChild(coinsLabel);
    
    setContentSize(coins_panel->getContentSize());
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    update(0);
    
    schedule(std::bind(&CoinsPanel::update, this, std::placeholders::_1), 0.5f, "COINS_TIMER");
    
    return true;
}

void CoinsPanel::update(float deltaTime)
{
    coinsLabel->setString(FormatWithCommas((int)PlayerProfile::getInstance().getCoins()));
}


