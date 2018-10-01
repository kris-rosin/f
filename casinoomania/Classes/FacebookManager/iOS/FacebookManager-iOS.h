//
//  FacebookManager-iOS.h
//
//  Created by Stanislav Zheronkin on 06.09.16.
//  Copyright (c) 2016 CodeExotics. All rights reserved.
//

#pragma once

#include <functional>
#include <vector>
#include <string>

#include "FacebookManager.h"

class FacebookManager_iOS
{
public:
    static void Login(std::function<void(bool)> completionHandler = nullptr);
    static void Logout(std::function<void(bool)> completionHandler = nullptr);
    static bool isLoggedIn();
    
    static void LoginAndGetName(std::function<void(bool, const std::string &, const std::string &)> callback);
    
    static void AskFriendsForLife(std::string life_id, std::string title, std::string message);
    
    static bool hasWritePermission();
    static void RequestWritePermission(std::function<void(bool)> completionHandler = nullptr);
    
    static void AskForFriendsRequest(std::string object_id, std::string title, std::string message, FacebookManager::FriendsFilter filter);
    static void MakeFriendsRequest(std::string title, std::string message, FacebookManager::FriendsFilter filter, std::function<void(bool, std::vector<std::string>)> callback);
    static void GetRequests(std::function<void(std::vector<FacebookManager::GameRequestRecord>)> completionHandler = nullptr);
    static void AcceptAskForRequest(FacebookManager::GameRequestRecord request, std::string response_object_id, std::string title, std::string message);
    static void DeleteRequest(FacebookManager::GameRequestRecord request);
    static void GetUserProfilePictureAsync(std::string userID, int size, std::function<void(std::string)> callback);
    
    static std::string GetUserID();
    static void GetCurrentUserName(std::function<void(std::string, std::string)> completionHandler);
    
    static void GetFriends(std::vector<FacebookManager::FriendDetails> & friendslist, std::function<void(bool)> completionHandler = nullptr, std::string page = "");
    
    static void ActivateApp();
    static void Initialize(void * application, void * launchOptions);
    static bool handleOpenURL(void * application, void * url, void * options);
    
    static void InviteFriends(std::string AppLink);
    
    static void GetUsersInfo(std::vector<std::string> users, std::function<void(bool, std::vector<FacebookManager::FriendDetails>)> callback);
    
    static void LogPurchase(double price, void * currency, void * product, void * productID);
};
