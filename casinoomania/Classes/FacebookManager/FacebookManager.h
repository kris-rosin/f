//
//  FacebookManager.h
//
//  Created by Stanislav Zheronkin on 06.09.16.
//  Copyright (c) 2016 CodeExotics. All rights reserved.
//
//  Version history:
//
//  1.1 - Code refactored, new methods are added
//  1.0 - Initial version

#pragma once

#include <functional>
#include <vector>
#include <string>
#include <map>
#include "cocos2d.h"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#include "network/HttpResponse.h"

class FacebookManager
{
public:
    struct GameRequestRecord
    {
        std::string request_id;
        std::string sender;
        std::string name;
        std::string object;
    };
    
    struct ProfilePictureHelper
    {
        std::string user_id;
        
        std::function<void(std::string, std::string)> callback;
        
        void onResponce(std::string path);
        
        ProfilePictureHelper(std::string user_id, std::function<void(std::string, std::string)> callback);
    };
    
    struct FriendDetails
    {
        std::string name;
        std::string user_id;
    };
    
    enum FriendsFilter
    {
        ALL_FRIENDS,
        APP_USERS,
        APP_NON_USERS
    };
private:
    std::vector<FacebookManager::GameRequestRecord> requests;
    std::map<std::string, std::string> userpics;
    std::vector<FacebookManager::FriendDetails> friends;
    std::string currentUserName;
private:
    FacebookManager() {};
    
    void onUserProfileResponce(std::string userID, std::string path);
    void onFinishedDownloadPicture(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response);
public:
    void onGotRequests(std::vector<FacebookManager::GameRequestRecord> result);

    static FacebookManager & getInstance();
    
    void Login(std::function<void(bool)> completionHandler = nullptr);
    void Logout(std::function<void(bool)> completionHandler = nullptr);
    bool isLoggedIn();
    
    bool hasWritePermission();
    void RequestWritePermission(std::function<void(bool)> completionHandler = nullptr);
    
    void AskForFriendsRequest(std::string requestID, std::string requestTitle, std::string requestMessage, FriendsFilter filter);
    void GetRequests();
    void AcceptRequest(GameRequestRecord request, std::string responceID, std::string requestTitle, std::string requestMessage);
    void DeleteRequest(GameRequestRecord request);
    
    std::vector<FacebookManager::GameRequestRecord> * getGameRequests();
    
    std::string GetUserProfilePicture(std::string userID, bool download = false);
    void GetUserProfilePictureAsync(std::string userID, std::function<void(std::string, std::string)> callback);
    void GetProfilePictureURL(std::string user_id, int size, std::function<void(std::string)> callback);
    
    std::vector<std::string> getFriendsIDs();
    
    std::string GetUserID();
    void GetCurrentUserName(std::function<void(std::string, std::string)> completionHandler);
    
    void GetFriends(std::function<void(bool)> completionHandler = nullptr);
    void GetFriends(std::function<void(bool, const std::vector<std::string> &)> completionHandler);
    void SetFriends(std::vector<FacebookManager::FriendDetails> new_friends);
    
    void ActivateApp();
    
    void LoginAndGetName(std::function<void(bool, const std::string &, const std::string &)> callback);
    void LoginAndGetNameAndPictureURL(std::function<void(bool, const std::string &, const std::string &, const std::string &)> callback);
    void LoginAndGetNameEmailAndPictureURL(std::function<void(bool, const std::string &, const std::string &, const std::string &, const std::string &)> callback);
    
    void InviteFriends(std::string title, std::string message, std::function<void(bool, std::vector<std::string>)> callback);
    void GetUsersInfo(std::vector<std::string> users, std::function<void(bool, std::vector<FriendDetails>)> callback);
};
