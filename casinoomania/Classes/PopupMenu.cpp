//
//  PopupMenu.cpp
//
//  Created by Stanislav Zheronkin on 20.04.17.
//
//

#include "PopupMenu.h"
#include "AudioManager.h"
#include "PlayerProfile.h"
#include "SettingsView.h"
#include "Features/AppManager.h"
#include "MessageAlert.h"

USING_NS_CC;

PopupMenu::PopupMenu()
{
    
}

PopupMenu * PopupMenu::create()
{
    PopupMenu * pRet = new PopupMenu();
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

bool PopupMenu::init()
{
    if (!Node::init())
        return false;
    
    setCascadeOpacityEnabled(true);
    
    auto background = Sprite::create("ui/MainMenu/popup_menu/popup_menu.png");
    background->setAnchorPoint(Vec2::ZERO);
    background->setCascadeOpacityEnabled(true);
    addChild(background);
    setContentSize(background->getContentSize());
    setAnchorPoint(Vec2( 0.8f, 1.0f));
    
    //buttons
    std::vector< std::pair<std::string, std::string> > buttonDefinitions =
    {
        { "rate_us"   , "menu_rateus"   },
        { "more_games", "menu_moregames"},
        { "settings"  , "menu_settings" },
        { "about"     , "menu_about"    }
    };
    
    int counter = 0;
    for (auto &item: buttonDefinitions)
    {
        std::string imagePath = StringUtils::format("ui/MainMenu/popup_menu/%s.png", item.second.c_str());
        auto button = ui::Button::create(imagePath);
        button->setZoomScale(0.05f);
        button->setName(item.first);
        button->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        button->setPosition(Vec2(getContentSize().width / 2, 305 - counter * button->getContentSize().height));
        button->addTouchEventListener(std::bind(&PopupMenu::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
        background->addChild(button);
        ++counter;
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

void PopupMenu::show(cocos2d::Node * parent)
{
    parent->addChild(this, 100);
    
    setPosition(parent->getContentSize() / 2);
}

void PopupMenu::hide()
{
    this->runAction(Sequence::create(FadeOut::create(0.2f), RemoveSelf::create(), nullptr));
}

void PopupMenu::onButtonPressed(Ref * sender, ui::Widget::TouchEventType event)
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

void PopupMenu::ProcessButtonAction(cocos2d::ui::Widget * button)
{
    if (button->getName() == "settings")
    {
        hide();
        SettingsView::create()->show(this);
    }
    else if (button->getName() == "about")
    {
        hide();
        MessageAlert::create("Sorry, This feature will be available for the next update!")->show(this);
        //TODO: implement
    }
    else if (button->getName() == "rate_us")
    {
        hide();
        AppManager::shared()->rateMe();
        //TODO: implement
    }
    else if (button->getName() == "more_games")
    {
        hide();
        AppManager::shared()->moreGames();
        //TODO: implement
    }
}
