//
//  FacebookAddFriendsPopup.cpp
//
//  Created by Stanislav Zheronkin on 14.05.17.
//
//

#include "FacebookAddFriendsPopup.h"
#include "AudioManager.h"
#include "Features/AppManager.h"
#include "PlayerProfile.h"
#include "AndroidCompatibility.h"
#include "FriendsView.h"
#include "ShareManager/ShareManager.h"
#include "BaseScene.h"
#include "mConstants.h"

USING_NS_CC;

FacebookAddFriendsPopup::FacebookAddFriendsPopup()
{
    
}

FacebookAddFriendsPopup * FacebookAddFriendsPopup::create(std::string  message)
{
    FacebookAddFriendsPopup * pRet = new FacebookAddFriendsPopup();
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

bool FacebookAddFriendsPopup::init(std::string message)
{
    if (!Node::init())
        return false;
    
    setCascadeOpacityEnabled(true);
    
    auto background = Sprite::create("ui/Friends/fb_popup_10k.png");
    if (PlayerProfile::getInstance().getFBUserID().empty() ){
        background = Sprite::create("ui/Friends/fb_popup_100k.png");
    }

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
    button->setPosition(Vec2(getContentSize().width, getContentSize().height));
    button->addTouchEventListener(std::bind(&FacebookAddFriendsPopup::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    background->addChild(button);
    
    //invite friends
    auto inviteFriend = ui::Button::create("ui/Friends/fb_invite_friends.png", "ui/Friends/fb_invite_friends.png");
    inviteFriend->setName("invite_friends");
    
    if (PlayerProfile::getInstance().getFBUserID().empty() )
    {
        inviteFriend = ui::Button::create("ui/Friends/fb_connect.png", "ui/Friends/fb_connect.png");
        inviteFriend->setName("fb_connect");
    }
    
    inviteFriend->setAnchorPoint(Vec2(0.5f, 0.5f));
    inviteFriend->setPosition(Vec2(getContentSize().width / 2, getContentSize().height * 0.40f));
    inviteFriend->addTouchEventListener(std::bind(&FacebookAddFriendsPopup::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    background->addChild(inviteFriend);
    
    auto inviteShare = ui::Button::create("ui/Friends/fb_invite_share.png", "ui/Friends/fb_invite_share.png");
    inviteShare->setName("invite_share");
    inviteShare->setAnchorPoint(Vec2(0.5f, 0.5f));
    inviteShare->setPosition(Vec2(getContentSize().width / 2, getContentSize().height * 0.20f));
    inviteShare->addTouchEventListener(std::bind(&FacebookAddFriendsPopup::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    background->addChild(inviteShare);

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

void FacebookAddFriendsPopup::show(cocos2d::Node * parent)
{
    parent->addChild(this, 100);
    
    setPosition(parent->getContentSize() / 2);
}

void FacebookAddFriendsPopup::hide()
{
    this->runAction(Sequence::create(FadeOut::create(0.2f), RemoveSelf::create(), nullptr));
}

void FacebookAddFriendsPopup::onButtonPressed(Ref * sender, ui::Widget::TouchEventType event)
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

void FacebookAddFriendsPopup::ProcessButtonAction(cocos2d::ui::Widget * button)
{
    if (button->getName() == "close")
    {
        hide();
    }
    else if (button->getName() == "invite_friends")
    {
        hide();
        FriendsView::create(FriendsView::ViewMode::FRIENDS_TAB)->show(this);
    }
    else if (button->getName() == "invite_share")
    {
        hide();
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        ShareManager::getInstance().shareURLFacebook("https://www.facebook.com/casinoomania", std::bind(&FacebookAddFriendsPopup::onSharingCompleted, this, std::placeholders::_1));
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        ShareManager::getInstance().shareURLFacebook("https://www.facebook.com/casinoomania", std::bind(&FacebookAddFriendsPopup::onSharingCompleted, this, std::placeholders::_1));
#endif
    }
    else if (button->getName() == "fb_connect")
    {
        hide();
        FriendsView::create(FriendsView::ViewMode::FRIENDS_TAB)->show(this, FriendsView::ViewAction::FB_CONNECT);
    }
}

void FacebookAddFriendsPopup::onSharingCompleted(bool success)
{
    if (success)
    {
        log("Success!");
        //TODO: put here reward
        auto scene = BaseScene::findBaseScene();
        if (scene)
        {
            scene->increasePlayerValues(SHARE_FACEBOOK_REWARD_COIN, 0, false);
        }
        
        return;
    }
    
    log("Fail!");
}
