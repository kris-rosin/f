//
//  WheelView.cpp
//
//  Created by Stanislav Zheronkin on 08.12.16.
//
//

#include "WheelView.h"
#include "AudioManager.h"
#include "PlayerProfile.h"
#include "AndroidCompatibility.h"

USING_NS_CC;

std::vector< int > WheelView::rewards = {
    20, 0, 100, 0, 20, 50, 0, 20, 0, 50
};

WheelView::WheelView(): result(0), lastSubSector(0)
{
    
}

WheelView * WheelView::create(int spins, int winCoins, std::function< void(int) > callback)
{
    WheelView * pRet = new WheelView();
    if (pRet && pRet->init(spins, winCoins, callback))
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

bool WheelView::init(int spins, int winCoins, std::function< void(int) > callback)
{
    if (!Node::init())
        return false;
    
    this->spinsRemains = spins;
    this->winAmount = winCoins;
    this->callback  = callback;
    
    setCascadeOpacityEnabled(true);
    
    background = Sprite::create("ui/WheelRotator/back.png");
    background->setAnchorPoint(Vec2::ZERO);
    addChild(background);
    setContentSize(background->getContentSize());
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    //blocker
    auto block = ui::Button::create();
    block->setScale9Enabled(true);
    block->setContentSize(Director::getInstance()->getWinSize());
    block->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    block->setPosition(getContentSize() / 2);
    addChild(block, -1);
    
    auto label = Label::createWithTTF("Press the spin button to start your bonus spin.", "fonts/Berlin.ttf", 30);
    label->setTextColor(Color4B::WHITE);
    label->setPosition(Vec2(background->getContentSize().width / 2, background->getContentSize().height * 0.8f));
    background->addChild(label);
    
    wheel = Sprite::create("ui/WheelRotator/wheel.png");
    wheel->setPosition(Vec2(background->getContentSize().width / 2, background->getContentSize().height * 0.12f));
    background->addChild(wheel);
    
    auto line = Sprite::create("ui/WheelRotator/bottomline.png");
    line->setAnchorPoint(Vec2::ZERO);
    background->addChild(line, 4);
    
    arrow = Sprite::create("ui/WheelRotator/arrow.png");
    arrow->setAnchorPoint(Vec2(0.5f, (arrow->getContentSize().height - arrow->getContentSize().width / 2) / arrow->getContentSize().height));
    arrow->setPosition(wheel->getPosition() + Vec2(0, wheel->getContentSize().height / 2 + 60));
    background->addChild(arrow, 1);
    
    spinButton = ui::Button::create("ui/WheelRotator/spin.png", "ui/WheelRotator/spin_disabled.png", "ui/WheelRotator/spin_disabled.png");
    spinButton->setPosition(wheel->getPosition());
    spinButton->addClickEventListener(std::bind(&WheelView::onSpinPressed, this, std::placeholders::_1));
    background->addChild(spinButton, 1);
    
    spinEffect = Sprite::create("ui/WheelRotator/spin_effect.png");
    spinEffect->setPosition(spinButton->getContentSize() / 2);
    spinEffect->runAction(RepeatForever::create(RotateBy::create(0.6f, 360)));
    spinButton->addChild(spinEffect);
    
    return true;
}

void WheelView::update(float deltaTime)
{
    if (spinSpeed > 0)
    {
        float speedAcc = spinSpeed;
        spinSpeed -= 0.10f * deltaTime;
        speedAcc = (speedAcc + spinSpeed) / 2; //average spin speed
        
        wheel->setRotation(wheel->getRotation() + 360.0f * speedAcc * deltaTime);
        
        int subSector = 0.5f + wheel->getRotation() / (360.0f / 10 / 5); // sectors and subsectors per sector
        if (subSector != lastSubSector)
        {
            lastSubSector = subSector;
            arrow->stopAllActions();
            arrow->runAction(Sequence::createWithTwoActions(RotateTo::create(0.05f, -5), RotateTo::create(0.05f, 0)));
        }
    }
    else
    {
        unscheduleUpdate();
        
        if (spinsRemains > 0)
        {
            spinButton->setEnabled(true);
            spinEffect->setVisible(true);
        }
        
        int coins = getCurrentReward();
        
        if (coins == 0)
        {
            result += winAmount;
        }
        else
        {
            result += coins;
        }
        
        showResult(coins);
        
        log("Current sector is: %d, reward is: %d", getCurrentSector(), getCurrentReward());
    }
}

void WheelView::onSpinPressed(cocos2d::Ref * sender)
{
    spinsRemains--;
    AudioManager::getInstance().PlaySoundEffect("music/SlotsSound/RotatorSpin");
    spinButton->setEnabled(false);
    spinEffect->setVisible(false);
    
    //hackyhack_publish - arc4random_uniform
//    spinSpeed = 1.0f + 0.01f * arc4random_uniform(100); //rotations per second (1 ... 2)
    spinSpeed = 1.0f + 0.01f * (rand() % (100)); //rotations per second (1 ... 2)
    
    log("Speed: %f", spinSpeed);
    
    scheduleUpdate();
}

int WheelView::getCurrentSector()
{
    return (((int)wheel->getRotation() + 18) % 360) / 36;
}

int WheelView::getCurrentReward()
{
    return rewards[getCurrentSector()];
}

void WheelView::showResult(int coins)
{
    if (coins > 0)
    {
        AudioManager::getInstance().PlaySoundEffect("music/SlotsSound/CoinDrop");
    }
    
    Sprite * panel = Sprite::create(coins == 0 ? "ui/Chests/getcoinsx2.png" : StringUtils::format("ui/Chests/getcoins%d.png", coins));
    if (panel)
    {
        background->addChild(panel, 6);
        panel->setPosition(Vec2(background->getContentSize().width / 2, background->getContentSize().height * 0.7f));
        panel->setScale(0.1f);
        panel->runAction(Sequence::create(ScaleTo::create(0.5f, 1.0f), DelayTime::create(2.0f), ScaleTo::create(0.1f, 0.1f), RemoveSelf::create(), spinsRemains <= 0 ? CallFunc::create(std::bind(&WheelView::hide, this)) : NULL, NULL));
        
    //it means maybe the file name is using getcoinsX2.png
    } else {
        panel = Sprite::create(coins == 0 ? "ui/Chests/getcoinsX2.png" : StringUtils::format("ui/Chests/getcoins%d.png", coins));
        if (panel)
        {
            background->addChild(panel, 6);
            panel->setPosition(Vec2(background->getContentSize().width / 2, background->getContentSize().height * 0.7f));
            panel->setScale(0.1f);
            panel->runAction(Sequence::create(ScaleTo::create(0.5f, 1.0f), DelayTime::create(2.0f), ScaleTo::create(0.1f, 0.1f), RemoveSelf::create(), spinsRemains <= 0 ? CallFunc::create(std::bind(&WheelView::hide, this)) : NULL, NULL));
        }
    }
}

void WheelView::show(cocos2d::Node * parent)
{
    parent->addChild(this, 100);
    
    setPosition(parent->getContentSize() / 2);
}

void WheelView::hide()
{
    this->runAction(Sequence::create(CallFunc::create([this](){ if (callback) callback(result); }), RemoveSelf::create(), NULL));
}
