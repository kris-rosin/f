//
//  FriendsView.cpp
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 04.09.17.
//
//

#include "FriendsView.h"
#include "AudioManager.h"
#include "Features/mConstants.h"
#include "Utilities.h"
#include "AndroidCompatibility.h"
#include "PlayerProfile.h"
#include "ServerConnection.h"
#include "FriendsItem.h"
#include "ScrollBar.h"
#include "MessageAlert.h"
#include "LoadingScene.h"
#include "BaseScene.h"

USING_NS_CC;

FriendsView::FriendsView() : activeTab(nullptr), activitySpinner(nullptr)
{

}

FriendsView::~FriendsView()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(imageDownloaderListener);
}

FriendsView * FriendsView::create(ViewMode mode)
{
    FriendsView * pRet = new FriendsView();
    if (pRet && pRet->init(mode))
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

bool FriendsView::init(ViewMode mode)
{
    if (!Node::init())
        return false;
    
    setCascadeOpacityEnabled(true);
    
    //setup modal behaviour
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [](Touch * touch, Event * event) { return true; };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto background = Sprite::create("ui/Friends/background.png");
    background->setCascadeOpacityEnabled(true);
    addChild(background, 1);
    setContentSize(background->getContentSize());
    
    auto close_button = ui::Button::create("ui/MainMenu/close_button.png");
    close_button->setZoomScale(0);
    close_button->setName("close");
    close_button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    close_button->setPosition(background->getContentSize() - Size(45, 45));
    close_button->addTouchEventListener(std::bind(&FriendsView::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    background->addChild(close_button);
    
    // ***********
    // statistic tab
    // ***********
    
    auto statsTab = Node::create();
    
    statsTab->setCascadeOpacityEnabled(true);
    statsTab->setPosition(Vec2(25, 19));
    background->addChild(statsTab);
    
    Tabs.pushBack(statsTab);
    
    auto buttonTab = Sprite::create("ui/Friends/statistic_tab.png");
    buttonTab->setAnchorPoint(Vec2::ZERO);
    buttonTab->setPosition(Vec2(0, 459));
    statsTab->addChild(buttonTab);
    
    auto statsButton = ui::Button::create();
    statsButton->setScale9Enabled(true);
    statsButton->setContentSize(Size(247, 120));
    statsButton->setPosition(Vec2(0, 459));
    statsButton->setAnchorPoint(Vec2::ZERO);
    statsButton->addClickEventListener(std::bind(&FriendsView::onTabClicked, this, std::placeholders::_1));
    statsTab->addChild(statsButton);
    
    auto rootNode1 = Node::create();
    rootNode1->setCascadeOpacityEnabled(true);
    rootNode1->setTag(42);
    statsTab->addChild(rootNode1);
    
    auto stats_back = Sprite::create("ui/Friends/stats_panel.png");
    stats_back->setAnchorPoint(Vec2::ZERO);
    rootNode1->addChild(stats_back);
    
    auto table = ui::ScrollView::create();
    table->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
    table->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    table->setContentSize(Size(832, 266));
    table->setPosition(Vec2(35, 24));
    table->setScrollBarEnabled(false);
    table->setInertiaScrollEnabled(true);
    table->setBounceEnabled(true);
    table->setCascadeOpacityEnabled(true);
    rootNode1->addChild(table);
    
    buildStatisticsTable(table);
    
    auto scrollStats = ScrollBar::create(table, "ui/MainMenu/scrollbar_area.png", "ui/Friends/scroll.png");
    scrollStats->setPosition(table->getPosition() + Vec2(table->getContentSize().width + 5, 0));
    rootNode1->addChild(scrollStats);
    
    // user name
    std::string userName = PlayerProfile::getInstance().getFBUserName().empty() ? "Guest" : PlayerProfile::getInstance().getFBUserName();
    
    userNameLabel = Label::createWithTTF(userName, "fonts/Berlin.ttf", 32.0f);
    userNameLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    userNameLabel->setTextColor(Color4B::WHITE);
    userNameLabel->setPosition(Vec2(246, 421));
    rootNode1->addChild(userNameLabel);
    
    if (PlayerProfile::getInstance().isAnySubscriptionActive())
    {
        auto vipBadge = Sprite::create("ui/Friends/vip_badge.png");
        vipBadge->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        vipBadge->setPosition(Vec2(246, 395));
        rootNode1->addChild(vipBadge);
    }
    
    std::string avatarpath = FacebookManager::getInstance().GetUserProfilePicture(PlayerProfile::getInstance().getFBUserID(), true);
    if (avatarpath.empty())
        avatarpath = "ui/Friends/avatar_big.png";
    
    avatar = Sprite::create(avatarpath);
    avatar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    avatar->setPosition(Vec2(123, 400));
    avatar->setScale(110.0f / avatar->getContentSize().width, 110.0f / avatar->getContentSize().height);
    rootNode1->addChild(avatar);
    
    // ***********
    // friends tab
    // ***********
    
    auto friendsTab = Node::create();
    
    friendsTab->setCascadeOpacityEnabled(true);
    friendsTab->setPosition(Vec2(25, 19));
    background->addChild(friendsTab);
    
    Tabs.pushBack(friendsTab);
    
    auto buttonTab2 = Sprite::create("ui/Friends/friends_tab.png");
    buttonTab2->setAnchorPoint(Vec2::ZERO);
    buttonTab2->setPosition(Vec2(0, 459));
    friendsTab->addChild(buttonTab2);
    
    auto friendsButton = ui::Button::create();
    friendsButton->setScale9Enabled(true);
    friendsButton->setContentSize(Size(242, 120));
    friendsButton->setPosition(Vec2(247, 459));
    friendsButton->setAnchorPoint(Vec2::ZERO);
    friendsButton->addClickEventListener(std::bind(&FriendsView::onTabClicked, this, std::placeholders::_1));
    friendsTab->addChild(friendsButton);
    
    auto rootNode2 = Node::create();
    rootNode2->setCascadeOpacityEnabled(true);
    rootNode2->setTag(42);
    friendsTab->addChild(rootNode2);
    
    // page if user has FB profile connected
    activeNode = Node::create();
    activeNode->setCascadeOpacityEnabled(true);
    rootNode2->addChild(activeNode);
    
    auto collectAll = ui::Button::create("ui/Friends/btn_collect_all.png");
    collectAll->setZoomScale(0);
    collectAll->setName("collect_all");
    collectAll->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    collectAll->setPosition(Vec2(744, 57));
    collectAll->addTouchEventListener(std::bind(&FriendsView::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    activeNode->addChild(collectAll);
    
    inviteFriends = ui::Button::create("ui/Friends/btn_invite_friends.png");
    inviteFriends->setZoomScale(0);
    inviteFriends->setName("invite_friends");
    inviteFriends->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    inviteFriends->setPosition(Vec2(505, 57));
    inviteFriends->addTouchEventListener(std::bind(&FriendsView::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    inviteFriends->setVisible(FacebookManager::getInstance().isLoggedIn());
    activeNode->addChild(inviteFriends);
    
    tableFriends = ui::ScrollView::create();
    tableFriends->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
    tableFriends->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    tableFriends->setContentSize(Size(830, 360));
    tableFriends->setPosition(Vec2(32, 99));
    tableFriends->setScrollBarEnabled(false);
    tableFriends->setInertiaScrollEnabled(true);
    tableFriends->setBounceEnabled(true);
    tableFriends->setCascadeOpacityEnabled(true);
    activeNode->addChild(tableFriends);
    
    auto scroll = ScrollBar::create(tableFriends, "ui/MainMenu/scrollbar_area.png", "ui/MainMenu/scrollbar_knob.png");
    scroll->setPosition(tableFriends->getPosition() + Vec2(tableFriends->getContentSize().width + 5, 0));
    activeNode->addChild(scroll);
    
    // page if user hasn't FB profile
    inactiveNode = Node::create();
    inactiveNode->setCascadeOpacityEnabled(true);
    rootNode2->addChild(inactiveNode);
    
    auto coinsBack = Sprite::create("ui/Friends/want_more_gifts.png");
    coinsBack->setAnchorPoint(Vec2::ZERO);
    inactiveNode->addChild(coinsBack);
    
    fbConnect = ui::Button::create("ui/Friends/fb_connect.png");
    fbConnect->setZoomScale(0);
    fbConnect->setName("fb_connect");
    fbConnect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    fbConnect->setPosition(Vec2(462, 123));
    fbConnect->addTouchEventListener(std::bind(&FriendsView::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    inactiveNode->addChild(fbConnect);
    
    
    activitySpinner = Sprite::create("ui/Friends/loading_indicator.png");
    activitySpinner->setPosition(tableFriends->getPosition() + tableFriends->getContentSize() / 2);
    activitySpinner->setScale(0.5f);
    activitySpinner->setVisible(false);
    activitySpinner->setLocalZOrder(10);
    activitySpinner->runAction(RepeatForever::create(RotateBy::create(3, 360)));
    rootNode2->addChild(activitySpinner);
    
    // ************
    // messages tab
    // ************
    
    auto messagesTab = Node::create();
    
    messagesTab->setCascadeOpacityEnabled(true);
    messagesTab->setPosition(Vec2(25, 19));
    background->addChild(messagesTab);
    
    Tabs.pushBack(messagesTab);
    
    auto buttonTab3 = Sprite::create("ui/Friends/messages_tab.png");
    buttonTab3->setAnchorPoint(Vec2::ZERO);
    buttonTab3->setPosition(Vec2(0, 459));
    messagesTab->addChild(buttonTab3);
    
    auto messagesButton = ui::Button::create();
    messagesButton->setScale9Enabled(true);
    messagesButton->setContentSize(Size(242, 120));
    messagesButton->setPosition(Vec2(489, 459));
    messagesButton->setAnchorPoint(Vec2::ZERO);
    messagesButton->addClickEventListener(std::bind(&FriendsView::onTabClicked, this, std::placeholders::_1));
    messagesTab->addChild(messagesButton);
    
    auto rootNode3 = Node::create();
    rootNode3->setCascadeOpacityEnabled(true);
    rootNode3->setTag(42);
    messagesTab->addChild(rootNode3);
    
    //hide messages tab
    messagesTab->setVisible(false);
    
    // Defaults
    if (mode == ViewMode::FRIENDS_TAB)
    {
        onTabClicked(friendsButton);
    }
    else
    {
        onTabClicked(statsButton);
    }
    
    //setup image downloader callback
    imageDownloaderListener = EventListenerCustom::create("FB_USER_PICTURE_DOWNLOADED", std::bind(&FriendsView::onUserPicDownloaded, this, std::placeholders::_1));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(imageDownloaderListener, this);
    
    if (PlayerProfile::getInstance().getFBUserID().empty())
    {
        inactiveNode->setVisible(true);
        activeNode->setVisible(false);
    }
    else
    {
        inactiveNode->setVisible(false);
        activeNode->setVisible(true);
        
        if (FacebookManager::getInstance().isLoggedIn())
        {
            reloadFriendsList();
        }
        else
        {
            FacebookManager::getInstance().Login([this](bool result)
            {
                if (result)
                {
                    reloadFriendsList();
                }
            });
        }
    }
    
    return true;
}

void FriendsView::reloadFriendsList()
{
    if (FacebookManager::getInstance().isLoggedIn())
    {
        showActivityIndicatior();
        
        auto message = new RequestFBInvitedFriendsList(PlayerProfile::getInstance().getFBUserID());
        ServerConnection::getInstance().sendMessage(message, this, std::bind(&FriendsView::onFriendsListReceived, this, std::placeholders::_1));
        delete message;
    }
}

void FriendsView::show(cocos2d::Node * parent, ViewAction action /*= ViewAction::NONE*/)
{
    auto scene = findScene(parent);
    
    if (scene)
    {
        Vec2 point = scene->convertToNodeSpace(Director::getInstance()->getWinSize() / 2);
        
        this->setPosition(point);
        this->setOpacity(0);
        this->runAction(FadeIn::create(0.3f) );
        
        scene->addChild(this, 10000);
        
        if (action == ViewAction::FB_CONNECT)
        {
            ProcessButtonAction(fbConnect);
        }
    }
}

void FriendsView::hide()
{
    this->runAction(Sequence::create(EaseSineIn::create(FadeOut::create(0.3f)), RemoveSelf::create(), nullptr));
}

Scene * FriendsView::findScene(Node * node)
{
    auto scene = dynamic_cast<Scene*>(node);
    
    if (scene)
    {
        return scene;
    }
    
    return findScene(node->getParent());
}

void FriendsView::onButtonPressed(Ref * sender, ui::Widget::TouchEventType event)
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

void FriendsView::ProcessButtonAction(cocos2d::ui::Button * button)
{
    if (button->getName() == "close")
    {
        button->setEnabled(false);
        hide();
    }
    else if (button->getName() == "invite_friends")
    {
        this->retain(); // lock view
        
        FacebookManager::getInstance().InviteFriends("Invite Friends", "Download Casinomania and play with me!", [this](bool result, std::vector<std::string> friends)
        {
            if (result)
            {
                auto message = new RegisterFBInvite(PlayerProfile::getInstance().getFBUserID(), friends);
                ServerConnection::getInstance().sendMessage(message, this, std::bind(&FriendsView::onFriendsInvited, this, std::placeholders::_1));
                delete message;
            }
            
            this->release(); //unlock view
        });
    }
    else if (button->getName() == "fb_connect")
    {
        FacebookManager::getInstance().LoginAndGetNameEmailAndPictureURL([this](bool success, const std::string & userid, const std::string & username, const std::string & email, const std::string & userpic)
        {
            if (success)
            {
                log("%s, %s, %s", userid.c_str(), username.c_str(), userpic.c_str());
               
                PlayerProfile::getInstance().setFBUserID(userid);
                PlayerProfile::getInstance().setFBUserName(username);
                PlayerProfile::getInstance().setFBUserPictureURL(userpic);
                PlayerProfile::getInstance().setFBUserEmail(email);
                PlayerProfile::getInstance().Save();
                
                log("Email: %s", email.c_str());
                
                //force loading avatar
                std::string userAvatar = FacebookManager::getInstance().GetUserProfilePicture(userid, true);
                
                //update user name and avatar
                if (userNameLabel)
                {
                    userNameLabel->setString(username);
                }
                if (avatar && userAvatar.empty() == false)
                {
                    updateUserAvatar(userAvatar);
                }

                auto message = new FBConnect(userid, username, email);
                ServerConnection::getInstance().sendMessage(message, this, std::bind(&FriendsView::onRegistrationCallback, this, std::placeholders::_1));
                delete message;
            }
        });
    }
}

void FriendsView::onTabClicked(Ref * sender)
{
    auto button = dynamic_cast<ui::Button*>(sender);
    
    if (button && activeTab != button->getParent())
    {
        activeTab = button->getParent();
        
        int i = 0;
        for (auto &tab : Tabs)
        {
            tab->setLocalZOrder(tab == activeTab ? 100 : i);
            
            auto rootNode = tab->getChildByTag(42);
            if (rootNode)
            {
                rootNode->setVisible(tab == activeTab);
            }
            
            i++;
        }
    }
}

bool FriendsView::onFriendsInvited(ServerMessage * message)
{
    auto result = dynamic_cast<RegisterFBInviteResult*>(message);
    
    if (result)
    {
        for (auto &item: result->getResult())
        {
            log("*** FB Friend Invite Status: %s, %d", item.first.c_str(), item.second);
        }
        
        reloadFriendsList();
    }
    
    return true;
}

bool FriendsView::onFriendsListReceived(ServerMessage * message)
{
    auto result = dynamic_cast<FBInvitedFriendsList*>(message);
    
    if (result)
    {
        friendsList = result->getResult();
        
        //getting names for user without names
        std::vector<std::string> usersWithoutNames;
        
        for (auto &item: friendsList)
        {
            if (item.name.empty())
                usersWithoutNames.push_back(item.facebookID);
        }
        
        if (!usersWithoutNames.empty() )
        {
            this->retain(); //lock object
            FacebookManager::getInstance().GetUsersInfo(usersWithoutNames, std::bind(&FriendsView::onFriendsNamesReceived,this, std::placeholders::_1, std::placeholders::_2));
        }
        else
        {
            refreshFriendsList();
            hideActivityIndicatior();
        }
    }
    
    return true;
}

void FriendsView::hideActivityIndicatior()
{
    activitySpinner->runAction(Sequence::create(Spawn::create(FadeOut::create(0.3f), EaseBackIn::create(ScaleTo::create(0.3f, 0.1f)), nullptr), Hide::create(), nullptr));
}

void FriendsView::showActivityIndicatior()
{
    activitySpinner->setVisible(true);
    activitySpinner->setOpacity(255);
    activitySpinner->setScale(0.5f);
}

void FriendsView::onFriendsNamesReceived(bool success, std::vector<FacebookManager::FriendDetails> details)
{
    if (success)
    {
        for (auto &detail: details)
        {
            for (auto &item: friendsList)
            {
                if (item.facebookID == detail.user_id)
                {
                    item.name = detail.name;
                }
            }
        }
    }
    
    refreshFriendsList();
    hideActivityIndicatior();
    
    this->release(); //unlock object
}

bool FriendsView::onRegistrationCallback(ServerMessage * message)
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
            
            MessageAlert::create(StringUtils::format( "You are just received %s coins reward!", FormatWithCommas(FB_CONNECT_REWARD).c_str()))->show(this);
        }
        else if (result->getResult() == 0x02)
        {
            //we should re-download player's data
            auto scene = LoadingScene::createScene();
            Director::getInstance()->replaceScene(scene);
            return true;
        }

        inactiveNode->setVisible(false);
        activeNode->setVisible(true);
        
        inviteFriends->setVisible(FacebookManager::getInstance().isLoggedIn());
        
        reloadFriendsList();
    }
    
    return true;
}

void FriendsView::refreshFriendsList()
{
    float containerHeight = 110 * friendsList.size();
    containerHeight = std::max(containerHeight, tableFriends->getContentSize().height);
    
    tableFriends->getInnerContainer()->removeAllChildren();
    tableFriends->setInnerContainerSize(Size(tableFriends->getContentSize().width, containerHeight));
    
    for (auto &item: friendsList)
    {
        auto friendItem = FriendsItem::create(item);
        friendItem->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        tableFriends->getInnerContainer()->addChild(friendItem);
        friendItem->setPosition(Vec2( tableFriends->getContentSize().width / 2, containerHeight));
        containerHeight -= 110;
    }
}

void FriendsView::buildStatisticsTable(cocos2d::ui::ScrollView * table)
{
    // game id, icon, title
    std::vector< std::tuple<std::string, std::string, std::string> > stats = {
        {GAME_SLOTS     , "icon_slots.png"     , "SLOTS"     },
        {GAME_ROULETTE  , "icon_roulette.png"  , "ROULETTE"  },
        {GAME_BLACK_JACK, "icon_black_jack.png", "BLACK JACK"},
        {GAME_BACCARAT  , "icon_baccarat.png"  , "BACCARAT"  },
        {GAME_CASINO_WAR, "icon_casino_war.png", "CASINO WAR"}
    };
    
    const int itemHeight = 92;
    float posY = itemHeight * stats.size();
    
    table->setInnerContainerSize(Size(table->getContentSize().width, posY));
    
    for (auto &item: stats)
    {
        posY -= itemHeight;
        
        auto itemNode = Node::create();
        itemNode->setCascadeOpacityEnabled(true);
        itemNode->setPosition(Vec2(0, posY));
        itemNode->setAnchorPoint(Vec2::ZERO);
        itemNode->setContentSize(Size(table->getContentSize().width, itemHeight));
        table->getInnerContainer()->addChild(itemNode);
        
        auto icon = Sprite::create(StringUtils::format("ui/Friends/%s", std::get<1>(item).c_str()));
        icon->setPosition(Vec2(72, itemHeight / 2));
        itemNode->addChild(icon);
        
        auto title = Label::createWithTTF(std::get<2>(item), "fonts/Berlin.ttf", 24.0f);
        title->setPosition(Vec2(133, itemHeight / 2));
        title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        title->setTextColor(Color4B::WHITE);
        itemNode->addChild(title);
        
        auto betAmount = PlayerProfile::getInstance().statsGetBetAmount(std::get<0>(item));
        auto winAmount = PlayerProfile::getInstance().statsGetWinAmount(std::get<0>(item));
        
        unsigned long long diffAmount = 0;
        std::string diffString;
        
        if (betAmount > winAmount)
        {
            diffAmount = betAmount - winAmount;
            diffString = "-";
        }
        else
        {
            diffAmount = winAmount - betAmount;
        }
        
        diffString.append(FormatWithCommas(diffAmount));
        
        auto bet = Label::createWithTTF(FormatWithCommas(betAmount), "fonts/Berlin.ttf", 24.0f);
        bet->setPosition(Vec2(473, itemHeight / 2));
        bet->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        bet->setTextColor(Color4B::WHITE);
        itemNode->addChild(bet);
        
        auto win = Label::createWithTTF(FormatWithCommas(winAmount), "fonts/Berlin.ttf", 24.0f);
        win->setPosition(Vec2(646, itemHeight / 2));
        win->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        win->setTextColor(Color4B::WHITE);
        itemNode->addChild(win);
        
        auto diff = Label::createWithTTF(diffString, "fonts/Berlin.ttf", 24.0f);
        diff->setPosition(Vec2(820, itemHeight / 2));
        diff->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        diff->setTextColor(Color4B::WHITE);
        itemNode->addChild(diff);
    }
}

void FriendsView::onUserPicDownloaded(cocos2d::EventCustom * event)
{
    if (event)
    {
        std::string userID = static_cast<const char *>(event->getUserData());
        
        if (userID == PlayerProfile::getInstance().getFBUserID())
        {
            //update avatar
            std::string userAvatar = FacebookManager::getInstance().GetUserProfilePicture(userID);
            
            updateUserAvatar(userAvatar);
        }
    }
}

void FriendsView::updateUserAvatar(std::string userAvatar)
{
    auto new_avatar = Sprite::create(userAvatar);
    if (new_avatar)
    {
        new_avatar->setPosition(avatar->getPosition());
        new_avatar->setAnchorPoint(avatar->getAnchorPoint());
        new_avatar->setScale(110.0f / new_avatar->getContentSize().width, 110.0f / new_avatar->getContentSize().height);
        avatar->getParent()->addChild(new_avatar);
        
        avatar->removeFromParent();
        avatar = new_avatar;
    }
}
