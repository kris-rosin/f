//
//  FriendsItem.cpp
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 04.09.17.
//
//

#include "FriendsItem.h"
#include "AudioManager.h"
#include "Features/mConstants.h"
#include "Utilities.h"
#include "ServerConnection.h"
#include "PlayerProfile.h"
#include "FacebookManager.h"
#include "BaseScene.h"

USING_NS_CC;


FriendsItem * FriendsItem::create(FBFriendInfo user)
{
    FriendsItem * pRet = new FriendsItem();
    if (pRet && pRet->init(user))
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

bool FriendsItem::init(FBFriendInfo user)
{
    if (!ui::Button::init())
        return false;
    
    reward = 0;
    
    userInfo = user;
    
    setZoomScale(0);
    setScale9Enabled(true);
    setContentSize(Size(830, 110));
    
    setCascadeOpacityEnabled(true);
    
    //avatar
    std::string avatarPath = FacebookManager::getInstance().GetUserProfilePicture(user.facebookID, true);
    
    if (avatarPath.empty())
    {
        avatarPath = "ui/Friends/avatar_empty.png";
    }
    
    auto avatar = Sprite::create(avatarPath);
    avatar->setPosition(Vec2(45, 55));
    addChild(avatar);
    
    auto userName = Label::createWithTTF(user.name, "fonts/arial.ttf", 31);
    userName->setTextColor(Color4B::WHITE);
    userName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    userName->setPosition(Vec2(117, 68));
    addChild(userName);
    
    auto subLabel = Label::createWithTTF("", "fonts/arial.ttf", 24);
    subLabel->setTextColor(Color4B::WHITE);
    subLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    subLabel->setPosition(Vec2(117, 33));
    addChild(subLabel);
    
    switch (user.status)
    {
        case FBFriendStatus::Invited:
        {
            auto fakeButton = Sprite::create("ui/Friends/btn_invite_sent.png");
            fakeButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            fakeButton->setPosition(Vec2(819, 55));
            addChild(fakeButton);
            
            subLabel->setString("To Get More Coins Gifts!");
        }
            break;
            
        case FBFriendStatus::Accepted:
        {
            auto collectButton = ui::Button::create("ui/Friends/btn_collect.png");
            collectButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            collectButton->setPosition(Vec2(819, 55));
            collectButton->setName("collect");
            collectButton->setZoomScale(0);
            collectButton->addTouchEventListener(std::bind(&FriendsItem::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
            addChild(collectButton);
            
            reward = FB_INVITE_REWARD;
            
            subLabel->setString(StringUtils::format("Collect %s Coins. Thanks for the invitation!", FormatWithCommas(FB_INVITE_REWARD).c_str()));
        }
            break;
        
        case FBFriendStatus::Playing:
        {
            
        }
            break;
            
        default:
            break;
    }
    
    return true;
}

void FriendsItem::onButtonPressed(Ref * sender, ui::Widget::TouchEventType event)
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

void FriendsItem::ProcessButtonAction(cocos2d::ui::Button * button)
{
    if (button->getName() == "collect")
    {
        auto message = new CollectInviteReward(PlayerProfile::getInstance().getFBUserID(), userInfo.facebookID);
        ServerConnection::getInstance().sendMessage(message, this, std::bind(&FriendsItem::onRewardReceived, this, std::placeholders::_1));
        delete message;
        
        button->setEnabled(false);
    }
}

bool FriendsItem::onRewardReceived(ServerMessage * message)
{
    auto result = dynamic_cast<GenericRequestResult*>(message);
    
    if (result)
    {
        if (result->getResult())
        {
            auto scene = BaseScene::findBaseScene();
            if (scene)
            {
                scene->increasePlayerValues(reward, 0, false);
            }
            AudioManager::getInstance().PlaySoundEffect("music/SlotsSound/CoinDrop.mp3");
        }
    }
    
    return true;
}

void FriendsItem::onUserpicReceived(std::string userID, std::string userPic)
{
    
}
