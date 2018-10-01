//
//  FriendsItem.h
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

#include "ServerMessage.h"

class FriendsItem : public cocos2d::ui::Button
{
private:
    long reward;
    
    FBFriendInfo userInfo;
private:
    bool init(FBFriendInfo user);
    
    void onButtonPressed(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType event);
    void ProcessButtonAction(cocos2d::ui::Button * button);

    bool onRewardReceived(ServerMessage * message);
    void onUserpicReceived(std::string userID, std::string userPic);
public:
    static FriendsItem * create(FBFriendInfo user);
};
