//
//  MessageAlert.cpp
//
//  Created by Stanislav Zheronkin on 14.05.17.
//
//

#include "MessageAlert.h"
#include "AudioManager.h"

USING_NS_CC;

MessageAlert::MessageAlert()
{
    
}

MessageAlert * MessageAlert::create(std::string  message, bool withCloseButton /*= true*/, std::string actionButtonText /*= ""*/, std::function<void()> actionCallback /*= nullptr*/)
{
    MessageAlert * pRet = new MessageAlert();
    if (pRet && pRet->init(message, withCloseButton, actionButtonText, actionCallback))
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

bool MessageAlert::init(std::string message, bool withCloseButton, std::string actionButtonText, std::function<void()> actionCallback)
{
    if (!Node::init())
        return false;
    
    callback = actionCallback;
    
    setCascadeOpacityEnabled(true);
    
    auto background = Sprite::create("ui/MainMenu/alert_box.png");
    background->setAnchorPoint(Vec2::ZERO);
    background->setCascadeOpacityEnabled(true);
    addChild(background);
    setContentSize(background->getContentSize());
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);

    //close button
    if (withCloseButton)
    {
        auto button = ui::Button::create("ui/MainMenu/close_button.png", "ui/MainMenu/close_button.png");
        button->setName("close");
        button->setAnchorPoint(Vec2(0.75f, 0.75f));
        button->setPosition(Vec2(getContentSize().width, getContentSize().height));
        button->addTouchEventListener(std::bind(&MessageAlert::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
        background->addChild(button);
    }
    
    //label
    auto label = Label::createWithTTF(message, "fonts/Berlin.ttf", 48);
    label->setColor(Color3B::WHITE);
    label->setPosition(getContentSize() / 2);
    label->setDimensions(getContentSize().width * 0.8f, getContentSize().height * 0.8f);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    background->addChild(label);
    
    if (actionButtonText.empty() == false)
    {
        auto button = ui::Button::create("ui/MainMenu/alert_button_normal.png", "ui/MainMenu/alert_button_selected.png");
        auto btnSize = button->getContentSize();
        button->setScale9Enabled(true);
        button->setName("action");
        button->setPosition(Vec2(getContentSize().width / 2, button->getContentSize().height * 0.7f));
        button->addTouchEventListener(std::bind(&MessageAlert::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
        background->addChild(button);
        
        auto buttonlabel = Label::createWithTTF(actionButtonText, "fonts/Berlin.ttf", 24);
        auto labelSize = buttonlabel->getContentSize();
        
        buttonlabel->setColor(Color3B::WHITE);
        buttonlabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        buttonlabel->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        button->addChild(buttonlabel);
        
        button->setContentSize(Size(labelSize.width + btnSize.width, btnSize.height));
        buttonlabel->setPosition(button->getContentSize() / 2);
        
        //adjust label
        label->setPositionY(label->getPositionY() + btnSize.height / 2);
    }
    
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

void MessageAlert::show(cocos2d::Node * parent)
{
    auto scene = findScene(parent);
    
    if (scene)
    {
        scene->addChild(this, 100);
    
        setPosition(scene->getContentSize() / 2);
    }
}

void MessageAlert::hide()
{
    this->runAction(Sequence::create(FadeOut::create(0.2f), RemoveSelf::create(), nullptr));
}

void MessageAlert::onButtonPressed(Ref * sender, ui::Widget::TouchEventType event)
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

void MessageAlert::ProcessButtonAction(cocos2d::ui::Widget * button)
{
    if (button->getName() == "close")
    {
        hide();
    }
    else if (button->getName() == "action")
    {
        if (callback) callback();
        
        hide();
    }
}

cocos2d::Scene * MessageAlert::findScene(cocos2d::Node * node)
{
    if (node == nullptr)
        return nullptr;
    
    auto scene = dynamic_cast<Scene*>(node);
    if (scene)
    {
        return scene;
    }
    
    return findScene(node->getParent());
}
