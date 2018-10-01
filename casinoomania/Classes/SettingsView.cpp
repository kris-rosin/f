//
//  SettingsView.cpp
//
//  Created by Stanislav Zheronkin on 02.03.17.
//
//

#include "SettingsView.h"
#include "AudioManager.h"
#include "PlayerProfile.h"
#include "LoadingScene.h"

#include "SwitchControl.h"
#include "ScrollBar.h"
#include "MessageAlert.h"
#include "Utilities.h"
#include "FacebookManager.h"

#include "Features/AppManager.h"
#include "Features/mConstants.h"
#include "ServerConnection.h"
#include "ServerMessage.h"
#include "BaseScene.h"

#include "SignupView.h"

USING_NS_CC;

SettingsView::SettingsView() : fb_connect(nullptr)
{
    
}

SettingsView * SettingsView::create(std::function< void() > callback)
{
    SettingsView * pRet = new SettingsView();
    if (pRet && pRet->init(callback))
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

bool SettingsView::init(std::function< void() > callback)
{
    if (!Node::init())
        return false;
    
    this->callback = callback;
    
    setCascadeOpacityEnabled(true);
    
    auto background = Sprite::create("ui/MainMenu/setting_back.png");
    background->setAnchorPoint(Vec2::ZERO);
    background->setCascadeOpacityEnabled(true);
    addChild(background);
    setContentSize(background->getContentSize());
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    //close button
    auto closeButton = ui::Button::create("ui/MainMenu/close_button.png", "ui/MainMenu/close_button.png", "ui/MainMenu/close_button.png");
    closeButton->setName("close");
    closeButton->setPosition(background->getContentSize() - Size(10, 10));
    closeButton->addTouchEventListener(std::bind(&SettingsView::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    background->addChild(closeButton);
    
    //scroll view
    auto scrollView = ui::ScrollView::create();
    scrollView->setContentSize(Size(850, 462));
    scrollView->setAnchorPoint(Vec2::ZERO);
    scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
    scrollView->setPosition(Vec2(49, 40));
    scrollView->setInnerContainerSize(Size(scrollView->getContentSize().width, 538+200));
    scrollView->setScrollBarEnabled(false);
    scrollView->setInertiaScrollEnabled(true);
    scrollView->setBounceEnabled(true);
    background->addChild(scrollView);
    
    //hackyhack_bonus_free_coins
    std::string message("Like our page and get ");
    message.append(FormatWithCommas((int) LIKE_FACEBOOK_REWARD_COIN));
    message.append(" FREE COINS");

    std::vector< std::tuple<std::string, Vec2, float> > labels =
    {
        { "Remove games"                                          , Vec2(5 ,  46), 40 },
        { "Background music"                                      , Vec2(5 , 125), 40 },
        { "Notifications"                                         , Vec2(5 , 210), 40 },
        { "Sounds"                                                , Vec2(5 , 295), 40 },
        { "Invite friend"                                         , Vec2(5 , 380), 40 },
        { "Connect to Facebook & get 100K FREE COINS!"            , Vec2(5 , 462), 22 },
        { "Login"                                                 , Vec2(5 , 505), 40 },
        { "Register and keep playing on every device."            , Vec2(5 , 462+100), 22 },
        { "Sign-in/Sign-up"                                       , Vec2(5 , 505+100), 40 },
        { message                                                 , Vec2(5 , 462+200), 22 },
        { "Like Facebook Page"                                    , Vec2(5 , 505+200), 40 }
    };
    
    for (auto &item: labels)
    {
        auto label = Label::createWithTTF(std::get<0>(item), "fonts/Berlin.ttf", std::get<2>(item));
        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        label->setPosition(std::get<1>(item));
        label->setColor(Color3B::WHITE);
        scrollView->getInnerContainer()->addChild(label);
    }
    
    //                  name, button or switch, pos, selected, button_image
    std::vector<std::tuple<std::string, bool, Vec2, bool, std::string> > buttons =
    {
        { "RemoveInfo"          , true , Vec2(753,  40), true, "info_button" },
        { "SettingsMusic"       , false, Vec2(753, 125), true, "" },
        { "SettingsNotification", false, Vec2(753, 210), true, "" },
        { "SettingsSounds"      , false, Vec2(753, 295), true, "" },
        { "Signup"              , true , Vec2(694, 491+100), true, "register" },
        { "FacebookLikePage"    , true , Vec2(694, 491+200), true, "fb_like"  },
        { "FacebookConnect"     , true , Vec2(694, 491), true, "fb_connect"   },
        { "InviteEMail"         , true , Vec2(753, 380), true, "email_button" },
        { "InviteSMS"           , true , Vec2(541, 380), true, "sms_button"   },
        { "InviteWhatsapp"      , true , Vec2(331, 380), true, "whatsapp_button"}
    };
    
    for (auto &item: buttons)
    {
        if (std::get<1>(item)) // button
        {
            std::string imagePath = StringUtils::format("ui/MainMenu/%s.png", std::get<4>(item).c_str());
            auto button = ui::Button::create(imagePath, imagePath);
            button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            button->setPosition(std::get<2>(item));
            button->setName(std::get<0>(item));
            button->addTouchEventListener(std::bind(&SettingsView::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
            scrollView->getInnerContainer()->addChild(button);
            
            if (button->getName() == "FacebookConnect")
            {
                fb_connect = button;
                
                if (FacebookManager::getInstance().isLoggedIn())
                {
                    fb_connect->setEnabled(false);
                }
            }
        }
        else // switcher
        {
            auto switcher = SwitchControl::create("ui/MainMenu/switcher_back.png", "ui/MainMenu/switcher_knob.png",
                                                  std::bind(&SettingsView::onSwitchStateChanged, this, std::placeholders::_1, std::placeholders::_2), std::get<0>(item));
            switcher->setState(std::get<3>(item), false);
            switcher->setPosition(std::get<2>(item));
            switcher->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            scrollView->getInnerContainer()->addChild(switcher);
        }
    }
    
    auto line = Sprite::create("ui/MainMenu/delimiter_line.png");
    line->setPosition(Vec2(scrollView->getContentSize().width / 2, 424));
    scrollView->getInnerContainer()->addChild(line);
    
    auto scroll = ScrollBar::create(scrollView, "ui/MainMenu/scrollbar_area.png", "ui/MainMenu/scrollbar_knob.png");
    scroll->setPosition(scrollView->getPosition() + Vec2(scrollView->getContentSize().width + 5, 0));
    background->addChild(scroll);
    
    // enable modal behaviour
    auto eventListener = EventListenerTouchOneByOne::create();
    eventListener->setSwallowTouches(true);
    
    eventListener->onTouchBegan = [](Touch * touch, Event * event) {
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);
    
    return true;
}

void SettingsView::show(cocos2d::Node * parent)
{
    auto scene = findScene(parent);
    
    if (scene)
    {
        scene->addChild(this, 100);
        
        setPosition(scene->getContentSize() / 2);
    }
}

void SettingsView::hide()
{
    this->runAction(Sequence::create(FadeOut::create(0.2f), CallFunc::create([this](){ if (callback) callback(); }), RemoveSelf::create(), nullptr));
}

void SettingsView::onCheckPressed(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType event)
{
    auto check = dynamic_cast<ui::CheckBox*>(sender);
    
    if (check)
    {
        if (event == ui::Widget::TouchEventType::BEGAN)
        {
            AudioManager::getInstance().PlaySoundEffect("music/SlotsSound/TapBt");
        }
        else if (event == ui::Widget::TouchEventType::ENDED)
        {
            ProcessButtonAction(check);
        }
        else if (event == ui::Widget::TouchEventType::CANCELED)
        {
            ProcessButtonAction(check);
        }
    }
}

void SettingsView::onButtonPressed(Ref * sender, ui::Widget::TouchEventType event)
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

void SettingsView::ProcessButtonAction(cocos2d::ui::Widget * button)
{
    //hackyhack_settings
    if (button->getName() == "close")
    {
        button->setEnabled(false);
        
        PlayerProfile::getInstance().Save();
        
        hide();
    }
    else if (button->getName() == "RemoveInfo")
    {
        MessageAlert::create("Sorry, This feature will be available for the next update!")->show(this);
    }
    else if (button->getName() == "FacebookLikePage")
    {
        AppManager::shared()->openUrl(LIKE_FACEBOOK_PAGE);
        
        if (!PlayerProfile::getInstance().isFacebookPageLiked())
        {
            auto scene = BaseScene::findBaseScene();
            if (scene)
            {
                scene->increasePlayerValues(LIKE_FACEBOOK_REWARD_COIN, 0, false);
            }
            
            std::string message("Congratulations, you have earned ");
            message.append(FormatWithCommas((int) LIKE_FACEBOOK_REWARD_COIN));
            message.append(" Coins");
            MessageAlert::create(message.c_str())->show(this);
            PlayerProfile::getInstance().setFacebookPageLiked();
        }
    }
    else if (button->getName() == "FacebookConnect")
    {
        FacebookManager::getInstance().LoginAndGetNameEmailAndPictureURL([this](bool success, const std::string & userid, const std::string & username, const std::string & email, const std::string & userpic)
        {
            if (success)
            {
                log("%s, %s", userid.c_str(), username.c_str());
                
                PlayerProfile::getInstance().setFBUserID(userid);
                PlayerProfile::getInstance().setFBUserName(username);
                PlayerProfile::getInstance().setFBUserPictureURL(userpic);
                PlayerProfile::getInstance().setFBUserEmail(email);
                PlayerProfile::getInstance().Save();
                
                log("Email: %s", email.c_str());
                
                //force loading avatar
                FacebookManager::getInstance().GetUserProfilePicture(userid, true);
                
                if (fb_connect)
                    fb_connect->setEnabled(false);
                
                auto message = new FBConnect(userid, username, email);
                ServerConnection::getInstance().sendMessage(message, this, std::bind(&SettingsView::onRegistrationCallback, this, std::placeholders::_1));
                delete message;
            }
        });
    }
    else if (button->getName() == "Signup")
    {
        hide();
        SignupView::create()->show(this);
    }
    else if (button->getName() == "InviteEMail")
    {
        AppManager::shared()->shareTheGame(SHARE_EMAIL);
    }
    else if (button->getName() == "InviteSMS")
    {
        AppManager::shared()->shareTheGame(SHARE_SMS);
    }
    else if (button->getName() == "InviteWhatsapp")
    {
        AppManager::shared()->shareTheGame(SHARE_WHATSAPP);
    }
}

bool SettingsView::onRegistrationCallback(ServerMessage * message)
{
    FBConnectResult * result = dynamic_cast<FBConnectResult*>(message);
    
    if (result)
    {
        if (result->getResult() == 0x00)
        {
            log("*** SERVER API: FB Registration failed!");
        }
        else if (result->getResult() == 0x01)
        {
            log("*** SERVER API: FB Rewarded Registration!");
            
            auto scene = BaseScene::findBaseScene();
            if (scene)
            {
                scene->increasePlayerValues(FB_CONNECT_REWARD, 0, false);
            }
            
            MessageAlert::create(StringUtils::format( "Congratulations, You are just received %s coins reward!", FormatWithCommas(FB_CONNECT_REWARD).c_str()))->show(this);
        }
        else if (result->getResult() == 0x02)
        {
            //we should re-download player's data
            auto scene = LoadingScene::createScene();
            Director::getInstance()->replaceScene(scene);
            return true;
        }
    }
    
    return true;
}

void SettingsView::onSwitchStateChanged(bool state, std::string name)
{
    if (name == "SettingsNotification")
    {
        PlayerProfile::getInstance().setUIntValue(name, state ? 1 : 0);
    }
    else if(name == "SettingsMusic")
    {
        if (state)
        {
            AudioManager::getInstance().setMusicVolume(1);
            AudioManager::getInstance().ResumeBackgroundMusic();
            PlayerProfile::getInstance().setUIntValue(name, 1);
        }
        else
        {
            AudioManager::getInstance().PauseBackgroundMusic();
            AudioManager::getInstance().setMusicVolume(0);
            PlayerProfile::getInstance().setUIntValue(name, 0);
        }
    }
    else if(name == "SettingsSounds")
    {
        if (state)
        {
            AudioManager::getInstance().setSoundsVolume(1.0f);
            PlayerProfile::getInstance().setUIntValue(name, 1);
        }
        else
        {
            AudioManager::getInstance().setSoundsVolume(0);
            PlayerProfile::getInstance().setUIntValue(name, 0);
        }
    }
}

cocos2d::Scene * SettingsView::findScene(cocos2d::Node * node)
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
