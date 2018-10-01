//
//  BottomBar.cpp
//
//  Created by Stanislav Zheronkin on 24.04.17.
//
//

#include "BottomBar.h"
#include "AudioManager.h"
#include "ChestsView.h"
#include "PlayerProfile.h"
#include "Platform.h"
#include "MessageAlert.h"
#include "FriendsView.h"
#include "Features/AppManager.h"
#include "Features/AdmobChartboost.h"
#include "Features/mConstants.h"

USING_NS_CC;

BottomBar::BottomBar()
{
    
}

BottomBar * BottomBar::create()
{
    BottomBar * pRet = new BottomBar();
    
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

bool BottomBar::init()
{
    if (!Node::init())
        return false;
    
    setCascadeOpacityEnabled(true);
    
    auto background = Sprite::create("ui/MainMenu/bottombar.png");
    background->setAnchorPoint(Vec2::ZERO);
    addChild(background);
    
    //stat button
    auto statButton = ui::Button::create("ui/MainMenu/stats_button.png", "ui/MainMenu/stats_button.png");
    background->addChild(statButton);
    statButton->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    statButton->setPosition(Vec2(background->getContentSize().width / 2 - 255, 0));
    statButton->setName("stats");
    statButton->addTouchEventListener(std::bind(&BottomBar::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    
    //gifts button
    auto giftsButton = ui::Button::create("ui/MainMenu/gifts_button.png", "ui/MainMenu/gifts_button.png");
    background->addChild(giftsButton);
    giftsButton->setPosition(Vec2(background->getContentSize().width / 2 + 255, 0));
    giftsButton->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    giftsButton->setName("rateus");
    giftsButton->addTouchEventListener(std::bind(&BottomBar::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    
    //leaderboard button
    auto leaderButton = ui::Button::create("ui/MainMenu/leaderboard_button.png", "ui/MainMenu/leaderboard_button.png");
    background->addChild(leaderButton);
    leaderButton->setPosition(Vec2(background->getContentSize().width / 2 - 601, 0));
    leaderButton->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    leaderButton->setName("leaderboard");
    leaderButton->addTouchEventListener(std::bind(&BottomBar::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    
    //profile button
    auto profileButton = ui::Button::create("ui/MainMenu/profile_button.png", "ui/MainMenu/profile_button.png");
    background->addChild(profileButton);
    profileButton->setPosition(Vec2(background->getContentSize().width / 2 + 601, 0));
    profileButton->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    profileButton->setName("profile");
    profileButton->addTouchEventListener(std::bind(&BottomBar::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    
    //shadow
    shadow = Sprite::create("ui/MainMenu/coin_slot.png");
    shadow->setPosition(Vec2(background->getContentSize().width / 2, 63));
    background->addChild(shadow);
    
    //clipped coin
    coinNode = ui::Layout::create();
    coinNode->setContentSize(Size(510, 255));
    coinNode->setClippingEnabled(true);
    coinNode->setClippingType(cocos2d::ui::Layout::ClippingType::STENCIL);
    coinNode->setAnchorPoint(Vec2::ZERO);
    coinNode->setPosition(Vec2(background->getContentSize().width / 2 - 255, 63));
    
    background->addChild(coinNode);
    
    auto coin = ui::Button::create("ui/MainMenu/big_coin.png", "ui/MainMenu/big_coin.png");
    coin->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    coin->setPosition(Vec2(coinNode->getContentSize().width / 2, 0));
    coin->setName("coin");
    coin->addTouchEventListener(std::bind(&BottomBar::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    coinNode->addChild(coin, 1);
    
    collect_label = Sprite::create("ui/MainMenu/collect_label.png");
    collect_label->setPosition(Vec2(background->getContentSize().width / 2, 27));
    background->addChild(collect_label);
    
    auto shine1 = Sprite::create("ui/MainMenu/shine.png");
    shine1->setPosition(Vec2(coinNode->getContentSize().width / 2, 0));
    shine1->runAction( RepeatForever::create(RotateBy::create(10, 360)));
    shine1->setBlendFunc(BlendFunc::ADDITIVE);
    shine1->setColor(Color3B(255, 227, 72));
    //shine1->setOpacity(192);
    coinNode->addChild(shine1);
    
    auto shine2 = Sprite::create("ui/MainMenu/shine.png");
    shine2->setPosition(Vec2(coinNode->getContentSize().width / 2, 0));
    shine2->runAction(RepeatForever::create(RotateBy::create(8, -360)));
    shine2->setBlendFunc(BlendFunc::ADDITIVE);
    shine2->setFlippedX(true);
    shine2->setColor(Color3B(255, 227, 72));
    //shine2->setOpacity(192);
    coinNode->addChild(shine2);
    
    auto shine3 = Sprite::create("ui/MainMenu/shine.png");
    shine3->setPosition(Vec2(coinNode->getContentSize().width / 2, 0));
    shine3->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.1f, 1.2f), ScaleTo::create(0.3f, 0.9f), DelayTime::create(0.25f), NULL)));
    shine3->setBlendFunc(BlendFunc::ADDITIVE);
    shine3->setColor(Color3B(255, 227, 72));
    //shine2->setOpacity(192);
    coinNode->addChild(shine3);
    
    remainTimer = Label::createWithTTF("00:00:00", "fonts/Berlin.ttf", 64);
    remainTimer->setPosition(Vec2(background->getContentSize().width / 2, 63));
    background->addChild(remainTimer);
    
    setContentSize(background->getContentSize());
    setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    
    update(0);
    schedule(std::bind(&BottomBar::update, this, std::placeholders::_1), 0.5f, "daily_bonus_timer");
    
    return true;
}

void BottomBar::onButtonPressed(Ref * sender, ui::Widget::TouchEventType event)
{
    auto button = dynamic_cast<ui::Button*>(sender);
    
    if (event == ui::Widget::TouchEventType::BEGAN)
    {
        AudioManager::getInstance().PlaySoundEffect("music/SlotsSound/TapBt");
        button->setColor(Color3B(178, 178, 178));
    }
    else if (event == ui::Widget::TouchEventType::ENDED)
    {
        button->setColor(Color3B::WHITE);
        
        ProcessButtonAction(button);
    }
    else if (event == ui::Widget::TouchEventType::CANCELED)
    {
        button->setColor(Color3B::WHITE);
    }
}

void BottomBar::ProcessButtonAction(cocos2d::ui::Button * button)
{
    //hackyhack_publish_buttons --> Buttons Bottom Bar
    
    if (button->getName() == "stats")
    {
        AdmobChartboost::shared()->playRewardedVideo(0);
    }
    else if (button->getName() == "rateus")
    {
        AppManager::shared()->rateMe();
    }
    else if (button->getName() == "leaderboard")
    {
        AppManager::shared()->showLeaderBoard();
    }
    else if (button->getName() == "profile")
    {
        FriendsView::create()->show(this);
    }
    else if (button->getName() == "coin")
    {
        ChestsView::create(ChestsView::BonusType::DAILY)->show(this);
    }
}

void BottomBar::update(float deltaTime)
{
    auto lastTime = PlayerProfile::getInstance().getUIntValue("DAILY_BONUS_TIME", 0);
    
    unsigned int elapsedTime = Platform::unixTime() - lastTime;
    
    if (elapsedTime > DAILY_BONUS_COOLDOWN)
    {
        coinNode->setVisible(true);
        collect_label->setVisible(true);
        shadow->setVisible(true);
        remainTimer->setVisible(false);
    }
    else
    {
        coinNode->setVisible(false);
        collect_label->setVisible(false);
        shadow->setVisible(false);
        remainTimer->setVisible(true);
        
        remainTimer->setString(getTimeString(DAILY_BONUS_COOLDOWN - elapsedTime));
    }
}

std::string BottomBar::getTimeString(unsigned int value)
{
    int hours = value / 3600;
    value -= hours * 3600;
    
    int minutes = value / 60;
    value -= minutes * 60;
    
    int seconds = value;
    
    return StringUtils::format("%02d:%02d:%02d", hours, minutes, seconds);
}

