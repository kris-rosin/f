//
//  RatemeAlert.cpp
//
//  Created by Stanislav Zheronkin on 14.05.17.
//
//

#include "RatemeAlert.h"
#include "AudioManager.h"
#include "Features/AppManager.h"
#include "PlayerProfile.h"
#include "AndroidCompatibility.h"
#include "BaseScene.h"

USING_NS_CC;

RatemeAlert::RatemeAlert()
{
    
}

RatemeAlert * RatemeAlert::create(std::string  message)
{
    RatemeAlert * pRet = new RatemeAlert();
    if (pRet && pRet->init(message))
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

bool RatemeAlert::init(std::string message)
{
    if (!Node::init())
        return false;
    
    setCascadeOpacityEnabled(true);
    
    auto background = Sprite::create("ui/MainMenu/popup_back.png");

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    if (PlayerProfile::getInstance().isUserGivenRating() == false) {
        background = Sprite::create("ui/MainMenu/popup_back_5000.png");
    }
#endif
    
    background->setAnchorPoint(Vec2::ZERO);
    background->setCascadeOpacityEnabled(true);
    addChild(background);
    setContentSize(background->getContentSize());
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);

//    auto background = Sprite::create("ui/MainMenu/alert_box.png");
//    background->setAnchorPoint(Vec2::ZERO);
//    background->setCascadeOpacityEnabled(true);
//    addChild(background);
//    setContentSize(background->getContentSize());
//    setAnchorPoint(Vec2::ANCHOR_MIDDLE);

    //close button
    auto button = ui::Button::create("ui/MainMenu/close_button.png", "ui/MainMenu/close_button.png");
    button->setName("close");
    button->setAnchorPoint(Vec2(1.0f, 1.0f));
    button->setPosition(Vec2(getContentSize().width*34/50, getContentSize().height*42/50));
    button->addTouchEventListener(std::bind(&RatemeAlert::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    background->addChild(button);

    //rateme button
    auto buttonRateme = ui::Button::create("ui/MainMenu/btnRateme2.png", "ui/MainMenu/btnRateme2.png");
    buttonRateme->setName("rateme");
    buttonRateme->setAnchorPoint(Vec2(0.5f, 0.5f));
    buttonRateme->setPosition(Vec2(getContentSize().width / 2, getContentSize().height * 0.37f));
    buttonRateme->addTouchEventListener(std::bind(&RatemeAlert::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    
    auto buttonRateme1 = ui::Button::create("ui/MainMenu/btnRateme2.png", "ui/MainMenu/btnRateme2.png");
    buttonRateme1->setName("rateme");
    buttonRateme1->setAnchorPoint(Vec2(0.5f, 0.5f));
    buttonRateme1->setPosition(Vec2(getContentSize().width / 2, getContentSize().height * 0.37f));
    buttonRateme1->addTouchEventListener(std::bind(&RatemeAlert::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));

    auto buttonRateme2 = ui::Button::create("ui/MainMenu/btnRateme2.png", "ui/MainMenu/btnRateme2.png");
    buttonRateme2->setName("rateme");
    buttonRateme2->setAnchorPoint(Vec2(0.5f, 0.5f));
    buttonRateme2->setPosition(Vec2(getContentSize().width / 2, getContentSize().height * 0.37f));
    buttonRateme2->addTouchEventListener(std::bind(&RatemeAlert::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));

    //hackyhack_publish - arc4random_uniform
//    int random = arc4random_uniform(100);
    int random = rand() % 100;
    
    if (random < 30) {
        background->addChild(buttonRateme);
    } else if (random < 70) {
        background->addChild(buttonRateme1);
    } else if (random < 100) {
        background->addChild(buttonRateme2);
    }
    
    //label
    auto label = Label::createWithTTF(message, "fonts/Berlin.ttf", 48);
    label->setColor(Color3B::WHITE);
    label->setPosition(getContentSize() / 2);
    label->setDimensions(getContentSize().width * 0.8f, getContentSize().height * 0.8f);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    background->addChild(label);
    
    // enable modal behaviour
    auto eventListener = EventListenerTouchOneByOne::create();
    eventListener->setSwallowTouches(true);
    
    eventListener->onTouchBegan = [](Touch * touch, Event * event)
    {
        return true;
    };
    
    eventListener->onTouchEnded = [this, eventListener](Touch * touch, Event * event)
    {
        if (this->getParent())
        {
            auto point = this->getParent()->convertTouchToNodeSpace(touch);
            if (this->getBoundingBox().containsPoint(point) == false)
            {
                Director::getInstance()->getEventDispatcher()->removeEventListener(eventListener);
                this->hide();
            }
        }
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);
    
    return true;
}

void RatemeAlert::show(cocos2d::Node * parent)
{
    parent->addChild(this, 100);
    
    setPosition(parent->getContentSize() / 2);
}

void RatemeAlert::hide()
{
    this->runAction(Sequence::create(FadeOut::create(0.2f), RemoveSelf::create(), nullptr));
}

void RatemeAlert::onButtonPressed(Ref * sender, ui::Widget::TouchEventType event)
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

void RatemeAlert::ProcessButtonAction(cocos2d::ui::Widget * button)
{
    if (button->getName() == "close")
    {
        hide();
    }
    else if (button->getName() == "rateme")
    {
        hide();
        AppManager::shared()->rateMe();
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        if (PlayerProfile::getInstance().isUserGivenRating() == false) {
            PlayerProfile::getInstance().setUserGivenRating();
            
            auto scene = BaseScene::findBaseScene();
            if (scene)
            {
                scene->increasePlayerValues(5000, 0, false);
            }
        }
#endif
    }

}
