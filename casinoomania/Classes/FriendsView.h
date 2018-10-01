//
//  FriendsView.h
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 04.09.17.
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include <string>
#include <functional>

#include "FacebookManager.h"
#include "ServerMessage.h"

class FriendsView : public cocos2d::Node
{
public:
    enum class ViewMode
    {
        NORMAL = 0,
        FRIENDS_TAB = 1
    };
    enum class ViewAction
    {
        NONE = 0,
        FB_CONNECT = 1
    };
private:
    cocos2d::Vector<cocos2d::Node*> Tabs;
    
    cocos2d::Node  * activeTab;
    cocos2d::Node  * activitySpinner;
    
    cocos2d::Node  * inactiveNode;
    cocos2d::Node  * activeNode;
    
    cocos2d::ui::Button * inviteFriends;
    cocos2d::ui::Button * fbConnect;
    
    cocos2d::ui::ScrollView * tableFriends;
    
    cocos2d::Label  * userNameLabel;
    cocos2d::Sprite * avatar;
    
    std::vector<FBFriendInfo> friendsList;
    
    cocos2d::EventListenerCustom * imageDownloaderListener;
private:
    FriendsView();
    virtual ~FriendsView();
    
    bool init(ViewMode mode);
    
    void onButtonPressed(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType event);
    void ProcessButtonAction(cocos2d::ui::Button * button);
    cocos2d::Scene * findScene(cocos2d::Node * node);
    
    void showActivityIndicatior();
    void hideActivityIndicatior();
    void refreshFriendsList();
    void reloadFriendsList();
    void buildStatisticsTable(cocos2d::ui::ScrollView * table);
    
    void onTabClicked(cocos2d::Ref * sender);
    bool onFriendsInvited(ServerMessage * message);
    bool onFriendsListReceived(ServerMessage * message);
    void onFriendsNamesReceived(bool success, std::vector<FacebookManager::FriendDetails> details);
    bool onRegistrationCallback(ServerMessage * message);
    
    void onUserPicDownloaded(cocos2d::EventCustom * event);
    
    void updateUserAvatar(std::string userAvatar);
public:
    static FriendsView * create(ViewMode mode = ViewMode::NORMAL);
    
    void show(cocos2d::Node * parent, ViewAction action = ViewAction::NONE);
    void hide();
};
