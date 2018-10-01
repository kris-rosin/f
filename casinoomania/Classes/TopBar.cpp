//
//  TopBar.cpp
//
//  Created by Stanislav Zheronkin on 14.04.17.
//
//

#include "TopBar.h"
#include "AudioManager.h"
#include "ShopView.h"
#include "ExperiencePanel.h"
#include "CoinsPanel.h"
#include "PopupMenu.h"
#include "PlayerProfile.h"
#include "FacebookManager.h"
#include "FriendsView.h"

#include "Features/AppManager.h"
#include "Features/mConstants.h"

USING_NS_CC;

TopBar::TopBar() : imageDownloaderListener(nullptr)
{
    
}

TopBar * TopBar::create(std::function<void()> backCallback)
{
    TopBar * pRet = new TopBar();
    
    if (pRet && pRet->init(backCallback))
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

bool TopBar::init(std::function<void()> backCallback)
{
    if (!Node::init())
        return false;
    
    setCascadeOpacityEnabled(true);
    
    this->backCallback = backCallback;
    
    auto background = ui::Scale9Sprite::create("ui/MainMenu/topbar_back.png");
    background->setPreferredSize(Size(Director::getInstance()->getVisibleSize().width, 90));
    background->setAnchorPoint(Vec2::ZERO);
    addChild(background);
    
    float margin = 15;
    
    //buy button
    buyButton = ui::Button::create("ui/MainMenu/buybutton.png", "ui/MainMenu/buybutton.png");
    background->addChild(buyButton);
    buyButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    buyButton->setPosition(Vec2(background->getContentSize().width / 2, background->getContentSize().height));
    buyButton->setName("buybutton");
    buyButton->addTouchEventListener(std::bind(&TopBar::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    
    //settings
    settings = ui::Button::create("ui/MainMenu/settings.png", "ui/MainMenu/settings.png");
    background->addChild(settings, 2);
    settings->setPosition(Vec2(background->getContentSize()) - Vec2(margin, 0));
    settings->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    settings->setName("settings");
    settings->addTouchEventListener(std::bind(&TopBar::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    
    //back
    if (backCallback)
    {
        auto goBack = ui::Button::create("ui/MainMenu/backbutton.png", "ui/MainMenu/backbutton.png");
        goBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        background->addChild(goBack);
        goBack->setPosition(Vec2(margin, background->getContentSize().height / 2));
        goBack->setName("back");
        goBack->addTouchEventListener(std::bind(&TopBar::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
        refButton = goBack;
    }
    else
    {
        
        std::string userAvatar = PlayerProfile::getInstance().getFBUserID().empty() ? "ui/MainMenu/fb_button.png" : FacebookManager::getInstance().GetUserProfilePicture(PlayerProfile::getInstance().getFBUserID());
        
        auto facebook = ui::Button::create(userAvatar, userAvatar);
        facebook->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        background->addChild(facebook);
        facebook->setPosition(Vec2(margin, background->getContentSize().height / 2));
        facebook->setName("facebook");
        facebook->addTouchEventListener(std::bind(&TopBar::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
        refButton = facebook;
        
        //setup image downloader callback
        imageDownloaderListener = EventListenerCustom::create("FB_USER_PICTURE_DOWNLOADED", std::bind(&TopBar::onUserPicDownloaded, this, std::placeholders::_1));
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(imageDownloaderListener, this);
    }
    
    //level bar
    auto levelBar = ExperiencePanel::create();
    background->addChild(levelBar);
    levelBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    levelBar->setPosition(Vec2(((buyButton->getPositionX() + buyButton->getContentSize().width / 2) + (settings->getPositionX() - settings->getContentSize().width)) / 2,
                               background->getContentSize().height / 2));
    
    //coins panel
    auto coinsPanel = CoinsPanel::create();
    background->addChild(coinsPanel);
    coinsPanel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    coinsPanel->setPosition(Vec2(((buyButton->getPositionX() - buyButton->getContentSize().width / 2) + refButton->getPositionX() + refButton->getContentSize().width) / 2,
                                 background->getContentSize().height / 2));
    
    setContentSize(background->getContentSize());
    setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    
    return true;
}

void TopBar::onButtonPressed(Ref * sender, ui::Widget::TouchEventType event)
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

void TopBar::ProcessButtonAction(cocos2d::ui::Button * button)
{
    //hackyhack_publish_buttons
    if (button->getName() == "settings")
    {
        PopupMenu::create()->show(button);
    }
    else if (button->getName() == "buybutton")
    {
        ShopView::create()->show(this);
    }
    else if (button->getName() == "facebook")
    {
        if (PlayerProfile::getInstance().getFBUserID().empty() )
        {
            AppManager::shared()->openUrl(LIKE_FACEBOOK_PAGE);
        }
        else
        {
            FriendsView::create()->show(this);
        }
    }
    else if (button->getName() == "back")
    {
        if (backCallback)
            backCallback();
    }
}

void TopBar::setEnabled(bool enable)
{
    if (refButton)
        refButton->setEnabled(enable);
    
    if (buyButton)
        buyButton->setEnabled(enable);
    
    if (settings)
        settings->setEnabled(enable);
}

void TopBar::onUserPicDownloaded(EventCustom * event)
{
    if (event)
    {
        std::string userID = static_cast<const char *>(event->getUserData());
        
        if (userID == PlayerProfile::getInstance().getFBUserID())
        {
            //update avatar
            std::string userAvatar = FacebookManager::getInstance().GetUserProfilePicture(userID);
            
            if (userAvatar.empty() == false)
            {
                refButton->loadTextureNormal(userAvatar);
                refButton->loadTexturePressed(userAvatar);
            }
        }
    }
}
