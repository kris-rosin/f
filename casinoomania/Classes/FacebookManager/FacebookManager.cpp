//
//  FacebookManager.mm
//
//  Created by Stanislav Zheronkin on 06.09.16.
//  Copyright (c) 2016 CodeExotics. All rights reserved.
//

#include "FacebookManager.h"

#include "InternetStatus.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #include "FacebookManager-iOS.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include "platform/android/jni/JniHelper.h"
    #define LOG_TAG "FacebookManager"
    #define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

static std::map<unsigned int, std::function<void(bool)>> boolCallbacks;
static std::map<unsigned int, std::function<void(std::string)>> stringCallbacks;
static std::map<unsigned int, std::function<void(std::string, std::string)>> twoStringsCallbacks;
static std::map<unsigned int, std::function<void(bool, std::vector<std::string>)>> friendsCallbacks;
static std::map<unsigned int, std::function<void(bool, std::vector<FacebookManager::FriendDetails>)>> friendsDetailsCallbacks;

static unsigned int callbackCounter = 0;

#endif

USING_NS_CC;
using namespace cocos2d::network;

FacebookManager & FacebookManager::getInstance()
{
    static FacebookManager instance;
    
    return instance;
}

void FacebookManager::Login(std::function<void(bool)> completionHandler)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    FacebookManager_iOS::Login(completionHandler);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    boolCallbacks[callbackCounter] = completionHandler;
    
    cocos2d::JniMethodInfo method;
    
    if(!cocos2d::JniHelper::getStaticMethodInfo(method, "com/codeexotics/tools/FacebookManager", "Login", "(I)V"))
    {
        LOGD("Failed to find static method id of com.codeexotics.tools.FacebookManager.Login()");
        return;
    }
    
    method.env->CallStaticVoidMethod(method.classID, method.methodID, callbackCounter);
    
    method.env->DeleteLocalRef(method.classID);
    
    ++callbackCounter;
    return;
#endif
}

void FacebookManager::Logout(std::function<void(bool)> completionHandler)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    FacebookManager_iOS::Logout(completionHandler);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo method;
    
    if(!cocos2d::JniHelper::getStaticMethodInfo(method, "com/codeexotics/tools/FacebookManager", "Logout", "()V"))
    {
        LOGD("Failed to find static method id of com.codeexotics.tools.FacebookManager.Logout()");
        
        if (completionHandler)
            completionHandler(false);
        
        return;
    }
    
    method.env->CallStaticVoidMethod(method.classID, method.methodID);
    
    method.env->DeleteLocalRef(method.classID);
    
    if (completionHandler)
        completionHandler(true);
    
    return;
#endif
}

bool FacebookManager::isLoggedIn()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return FacebookManager_iOS::isLoggedIn();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo method;
    
    if(!cocos2d::JniHelper::getStaticMethodInfo(method, "com/codeexotics/tools/FacebookManager", "isLoggedIn", "()Z"))
    {
        LOGD("Failed to find static method id of com.codeexotics.tools.FacebookManager.isLoggedIn()");
        
        return false;
    }
    
    bool result = method.env->CallStaticBooleanMethod(method.classID, method.methodID);
    
    method.env->DeleteLocalRef(method.classID);
    
    return result;
#endif
}

std::string FacebookManager::GetUserID()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return FacebookManager_iOS::GetUserID();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo method;
    
    if(!cocos2d::JniHelper::getStaticMethodInfo(method, "com/codeexotics/tools/FacebookManager", "GetUserID", "()Ljava/lang/String;"))
    {
        LOGD("Failed to find static method id of com.codeexotics.tools.FacebookManager.GetUserID()");
        
        return "";
    }
    
    jstring returnString = (jstring) method.env->CallStaticObjectMethod(method.classID, method.methodID);
    
    std::string result = cocos2d::JniHelper::jstring2string(returnString);
    
    method.env->DeleteLocalRef(method.classID);
    
    return result;
#endif
}

void FacebookManager::GetCurrentUserName(std::function<void(std::string, std::string)> completionHandler)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    FacebookManager_iOS::GetCurrentUserName(completionHandler);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    twoStringsCallbacks[callbackCounter] = completionHandler;
    
    cocos2d::JniMethodInfo method;
    
    if(!cocos2d::JniHelper::getStaticMethodInfo(method, "com/codeexotics/tools/FacebookManager", "GetCurrentUserName", "(I)V"))
    {
        LOGD("Failed to find static method id of com.codeexotics.tools.FacebookManager.GetCurrentUserName()");
    }
    
    method.env->CallStaticVoidMethod(method.classID, method.methodID, callbackCounter);
    
    method.env->DeleteLocalRef(method.classID);
    
    ++callbackCounter;
#endif
}

void FacebookManager::LoginAndGetName(std::function<void(bool, const std::string &, const std::string &)> callback)
{
    if (isLoggedIn())
    {
        GetCurrentUserName([this, callback](std::string name, std::string email)
        {
            if (callback)
                callback(!name.empty(), GetUserID(), name);
        });
    }
    else
    {
        Login([this, callback](bool result)
        {
            if (result)
            {
                GetCurrentUserName([this, callback](std::string name, std::string email)
                {
                    if (callback)
                        callback(!name.empty(), GetUserID(), name);
                });
            }
            else
            {
                if (callback)
                    callback(false, "", "");
            }
        });
    }
}

void FacebookManager::LoginAndGetNameAndPictureURL(std::function<void(bool, const std::string &, const std::string &, const std::string &)> callback)
{
    if (isLoggedIn())
    {
        GetCurrentUserName([this, callback](std::string name, std::string email)
                           {
                               this->GetProfilePictureURL(GetUserID(), 64, [this, callback, name](std::string path)
                               {
                                   if (callback)
                                       callback(!name.empty(), GetUserID(), name, path);
                               });
                               
                           });
    }
    else
    {
        Login([this, callback](bool result)
              {
                  if (result)
                  {
                      GetCurrentUserName([this, callback](std::string name, std::string email)
                                         {
                                             this->GetProfilePictureURL(GetUserID(), 64, [this, callback, name](std::string path)
                                             {
                                                 if (callback)
                                                     callback(!name.empty(), GetUserID(), name, path);
                                             });
                                             
                                         });
                  }
                  else
                  {
                      if (callback)
                          callback(false, "", "", "");
                  }
              });
    }
}

void FacebookManager::LoginAndGetNameEmailAndPictureURL(std::function<void(bool, const std::string &, const std::string &, const std::string &, const std::string &)> callback)
{
    if (isLoggedIn())
    {
        GetCurrentUserName([this, callback](std::string name, std::string email)
                           {
                               this->GetProfilePictureURL(GetUserID(), 64, [this, callback, name, email](std::string path)
                               {
                                   if (callback)
                                       callback(!name.empty(), GetUserID(), name, email, path);
                               });
                           });
    }
    else
    {
        Login([this, callback](bool result)
              {
                  if (result)
                  {
                      GetCurrentUserName([this, callback](std::string name, std::string email)
                                         {
                                             this->GetProfilePictureURL(GetUserID(), 64, [this, callback, name, email](std::string path)
                                             {
                                                 if (callback)
                                                     callback(!name.empty(), GetUserID(), name, email, path);
                                             });
                                         });
                  }
                  else
                  {
                      if (callback)
                          callback(false, "", "", "", "");
                  }
              });
    }
}

void FacebookManager::AskForFriendsRequest(std::string requestID, std::string requestTitle, std::string requestMessage, FriendsFilter filter)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    FacebookManager_iOS::AskForFriendsRequest(requestID, requestTitle, requestMessage, filter);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo method;
    
    if(!cocos2d::JniHelper::getStaticMethodInfo(method, "com/codeexotics/tools/FacebookManager", "AskFriendsForLife", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"))
    {
        LOGD("Failed to find static method id of com.codeexotics.tools.FacebookManager.AskFriendsForLife()");
        
        return;
    }
    
    jstring object_id = method.env->NewStringUTF(requestID.c_str());
    jstring title     = method.env->NewStringUTF(requestTitle.c_str());
    jstring message   = method.env->NewStringUTF(requestMessage.c_str());
    
    method.env->CallStaticVoidMethod(method.classID, method.methodID, object_id, title, message);
    
    method.env->DeleteLocalRef(object_id);
    method.env->DeleteLocalRef(title);
    method.env->DeleteLocalRef(message);
    
    method.env->DeleteLocalRef(method.classID);
#endif
}

void FacebookManager::GetRequests()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    FacebookManager_iOS::GetRequests(std::bind(&FacebookManager::onGotRequests, this, std::placeholders::_1));
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo method;
    
    if(!cocos2d::JniHelper::getStaticMethodInfo(method, "com/codeexotics/tools/FacebookManager", "GetRequests", "()V"))
    {
        LOGD("Failed to find static method id of com.codeexotics.tools.FacebookManager.GetRequests()");
        return;
    }
    
    method.env->CallStaticVoidMethod(method.classID, method.methodID);
    
    method.env->DeleteLocalRef(method.classID);
    return;
#endif
}

void FacebookManager::onGotRequests(std::vector<FacebookManager::GameRequestRecord> result)
{
    requests = result;
    
    for(auto &item: requests)
    {
        //cache userpics
        if (GetUserProfilePicture(item.sender).empty())
        {
            GetUserProfilePictureAsync(item.sender, std::bind(&FacebookManager::onUserProfileResponce, this, std::placeholders::_1, std::placeholders::_2));
        }

    }
}

bool FacebookManager::hasWritePermission()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return FacebookManager_iOS::hasWritePermission();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo method;
    
    if(!cocos2d::JniHelper::getStaticMethodInfo(method, "com/codeexotics/tools/FacebookManager", "hasWritePermission", "()Z"))
    {
        LOGD("Failed to find static method id of com.codeexotics.tools.FacebookManager.hasWritePermission()");
        
        return false;
    }
    
    bool result = method.env->CallStaticBooleanMethod(method.classID, method.methodID);
    
    method.env->DeleteLocalRef(method.classID);
    
    return result;
#endif
}

void FacebookManager::RequestWritePermission(std::function<void(bool)> completionHandler)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    FacebookManager_iOS::RequestWritePermission(completionHandler);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //android code not implemented
#endif
}

void FacebookManager::AcceptRequest(GameRequestRecord request, std::string responceID, std::string requestTitle, std::string requestMessage)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    FacebookManager_iOS::AcceptAskForRequest(request, responceID, requestTitle, requestMessage);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo method;
    
    if(!cocos2d::JniHelper::getStaticMethodInfo(method, "com/codeexotics/tools/FacebookManager", "AcceptRequest", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"))
    {
        LOGD("Failed to find static method id of com.codeexotics.tools.FacebookManager.AcceptRequest()");
        
        return;
    }
    
    jstring sender    = method.env->NewStringUTF(request.sender.c_str());
    jstring request_id= method.env->NewStringUTF(request.request_id.c_str());
    jstring object_id = method.env->NewStringUTF(responceID.c_str());
    jstring title     = method.env->NewStringUTF(requestTitle.c_str());
    jstring message   = method.env->NewStringUTF(requestMessage.c_str());
    
    method.env->CallStaticVoidMethod(method.classID, method.methodID, sender, request_id, object_id, title, message);
    
    method.env->DeleteLocalRef(sender);
    method.env->DeleteLocalRef(request_id);
    method.env->DeleteLocalRef(object_id);
    method.env->DeleteLocalRef(title);
    method.env->DeleteLocalRef(message);
    
    method.env->DeleteLocalRef(method.classID);
#endif
    
    auto itr = requests.begin();
    while (itr != requests.end())
    {
        if (itr->object == request.object)
        {
            requests.erase(itr);
            break;
        }
        
        ++itr;
    }
}

void FacebookManager::DeleteRequest(GameRequestRecord request)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    FacebookManager_iOS::DeleteRequest(request);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo method;
    
    if(!cocos2d::JniHelper::getStaticMethodInfo(method, "com/codeexotics/tools/FacebookManager", "DeleteRequest", "(Ljava/lang/String;)V"))
    {
        LOGD("Failed to find static method id of com.codeexotics.tools.FacebookManager.DeleteRequest()");
        
        return;
    }
    
    jstring request_id= method.env->NewStringUTF(request.request_id.c_str());
    
    method.env->CallStaticVoidMethod(method.classID, method.methodID, request_id);
    
    method.env->DeleteLocalRef(request_id);
    method.env->DeleteLocalRef(method.classID);
#endif
    
    auto itr = requests.begin();
    while (itr != requests.end())
    {
        if (itr->object == request.object)
        {
            requests.erase(itr);
            break;
        }
        
        ++itr;
    }
}

std::vector<FacebookManager::GameRequestRecord> * FacebookManager::getGameRequests()
{
    return &requests;
}

std::string FacebookManager::GetUserProfilePicture(std::string userID, bool download /* = false*/)
{
    if (userID.empty())
    {
        return "";
    }
    
    std::string path = FileUtils::getInstance()->getWritablePath();
    path.append(userID).append(".jpg");
    
    if (FileUtils::getInstance()->isFileExist(path))
    {
        return path;
    }
    
    if (download)
    {
        GetUserProfilePictureAsync(userID, std::bind(&FacebookManager::onUserProfileResponce, this, std::placeholders::_1, std::placeholders::_2));
    }
    
    return "";
}

void FacebookManager::GetUserProfilePictureAsync(std::string userID, std::function<void(std::string, std::string)> callback)
{
    if (userpics.count(userID) == 0)
    {
        new ProfilePictureHelper(userID, callback);
        userpics[userID] = "";
    }
}

void FacebookManager::onUserProfileResponce(std::string userID, std::string path)
{
    if (path.empty() == false)
    {
        
        
        auto itr = userpics.find(userID);
        
        if (itr != userpics.end())
        {
            itr->second = path;
        }
        
        if (InternetStatus::hasInternetConnection())
        {
            HttpRequest * m_httpRequest = new HttpRequest();
            m_httpRequest->setUrl(path.c_str());
            m_httpRequest->setRequestType(HttpRequest::Type::GET);
            m_httpRequest->setResponseCallback(CC_CALLBACK_2(FacebookManager::onFinishedDownloadPicture, this));
            
            char buff[256];
            snprintf(buff, 256, "%s", userID.c_str());
            m_httpRequest->setTag(buff);
            HttpClient::getInstance()->setTimeoutForConnect(30);
            HttpClient::getInstance()->send(m_httpRequest);
            m_httpRequest->release();
        }
        else
        {
            auto itr = userpics.find(userID);
            
            if (itr != userpics.end())
            {
                userpics.erase(itr);
            }
        }
    }
}

void FacebookManager::onFinishedDownloadPicture(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response)
{
    if (response && response->getResponseCode() == 200)
    {
        std::string path = FileUtils::getInstance()->getWritablePath();
        path.append(response->getHttpRequest()->getTag()).append(".jpg");
        
        FILE * fil = fopen(path.c_str(), "wb");
        if (fil)
        {
            fwrite(response->getResponseData()->data(), response->getResponseData()->size() * sizeof(char), 1, fil);
            
            fclose(fil);
        }
        
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("FB_USER_PICTURE_DOWNLOADED", (void *)response->getHttpRequest()->getTag() );
    }
}

void FacebookManager::GetFriends(std::function<void(bool)> completionHandler/* = nullptr*/)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    FacebookManager_iOS::GetFriends(friends, completionHandler);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    boolCallbacks[callbackCounter] = completionHandler;
    
    cocos2d::JniMethodInfo method;
    
    if(!cocos2d::JniHelper::getStaticMethodInfo(method, "com/codeexotics/tools/FacebookManager", "GetFriends", "(I)V"))
    {
        LOGD("Failed to find static method id of com.codeexotics.tools.FacebookManager.GetFriends()");
        return;
    }
    
    method.env->CallStaticVoidMethod(method.classID, method.methodID, callbackCounter);
    method.env->DeleteLocalRef(method.classID);
    
    ++callbackCounter;
#endif
}

void FacebookManager::GetFriends(std::function<void(bool, const std::vector<std::string> &)> completionHandler)
{
    GetFriends([this, completionHandler](bool result)
    {
        if (result)
        {
            if (completionHandler)
                completionHandler(true, getFriendsIDs());
        }
        else
        {
            std::vector<std::string> empty;
            if (completionHandler)
                completionHandler(false, empty);
        }
    });
}

std::vector<std::string> FacebookManager::getFriendsIDs()
{
    std::vector<std::string> result;
    result.resize(friends.size());
    
    for (int i = 0; i < friends.size(); i++)
    {
        result[i] = friends[i].user_id;
    }
    
    return result;
}

void FacebookManager::GetUsersInfo(std::vector<std::string> users, std::function<void(bool, std::vector<FriendDetails>)> callback)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    FacebookManager_iOS::GetUsersInfo(users, callback);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    friendsDetailsCallbacks[callbackCounter] = callback;
    
    cocos2d::JniMethodInfo method;
    
    if(!cocos2d::JniHelper::getStaticMethodInfo(method, "com/codeexotics/tools/FacebookManager", "GetUsersInfo", "([Ljava/lang/String;I)V"))
    {
        LOGD("Failed to find static method id of com.codeexotics.tools.FacebookManager.GetUsersInfo()");
        
        return;
    }
    
    jobjectArray usersList = (jobjectArray)method.env->NewObjectArray(users.size(), method.env->FindClass("java/lang/String"), method.env->NewStringUTF(""));
    
    for(int i = 0; i < users.size(); i++)
        method.env->SetObjectArrayElement(usersList, i, method.env->NewStringUTF(users[i].c_str()));;
    
    method.env->CallStaticVoidMethod(method.classID, method.methodID, usersList, callbackCounter);
    
    method.env->DeleteLocalRef(usersList);
    method.env->DeleteLocalRef(method.classID);
    
    ++callbackCounter;
#endif
}

void FacebookManager::InviteFriends(std::string requestTitle, std::string requestMessage, std::function<void(bool, std::vector<std::string>)> callback)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    FacebookManager_iOS::MakeFriendsRequest(requestTitle, requestMessage, FacebookManager::FriendsFilter::APP_NON_USERS, callback);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    friendsCallbacks[callbackCounter] = callback;
    
    cocos2d::JniMethodInfo method;
    
    if(!cocos2d::JniHelper::getStaticMethodInfo(method, "com/codeexotics/tools/FacebookManager", "MakeFriendsRequest", "(Ljava/lang/String;Ljava/lang/String;II)V"))
    {
        LOGD("Failed to find static method id of com.codeexotics.tools.FacebookManager.MakeFriendsRequest()");
        
        return;
    }
    
    jstring title     = method.env->NewStringUTF(requestTitle.c_str());
    jstring message   = method.env->NewStringUTF(requestMessage.c_str());
    
    method.env->CallStaticVoidMethod(method.classID, method.methodID, title, message, (int)FacebookManager::FriendsFilter::APP_NON_USERS, callbackCounter);
    
    method.env->DeleteLocalRef(title);
    method.env->DeleteLocalRef(message);
    method.env->DeleteLocalRef(method.classID);
    
    ++callbackCounter;
#endif
}

void FacebookManager::ActivateApp()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    FacebookManager_iOS::ActivateApp();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo method;
    
    if(!cocos2d::JniHelper::getStaticMethodInfo(method, "com/codeexotics/tools/FacebookManager", "ActivateApp", "()V"))
    {
        LOGD("Failed to find static method id of com.codeexotics.tools.FacebookManager.ActivateApp()");
        
        return;
    }
    
    method.env->CallStaticVoidMethod(method.classID, method.methodID);
    method.env->DeleteLocalRef(method.classID);
    
    return;
#endif
}

void FacebookManager::GetProfilePictureURL(std::string user_id, int size, std::function<void(std::string)> callback)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    FacebookManager_iOS::GetUserProfilePictureAsync(user_id, size, callback);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    stringCallbacks[callbackCounter] = callback;
    
    cocos2d::JniMethodInfo method;
    
    if(!cocos2d::JniHelper::getStaticMethodInfo(method, "com/codeexotics/tools/FacebookManager", "GetUserProfilePictureAsync", "(IILjava/lang/String;)V"))
    {
        LOGD("Failed to find static method id of com.codeexotics.tools.FacebookManager.GetUserProfilePictureAsync()");
        
        return;
    }
    
    jstring object_id = method.env->NewStringUTF(user_id.c_str());
    
    method.env->CallStaticVoidMethod(method.classID, method.methodID, callbackCounter, size, object_id);
    
    method.env->DeleteLocalRef(object_id);
    method.env->DeleteLocalRef(method.classID);
    
    ++callbackCounter;
#endif
}

#pragma mark - ProfilePictureHelper

void FacebookManager::ProfilePictureHelper::onResponce(std::string path)
{
    callback(user_id, path);
    
    delete this;
}
    
FacebookManager::ProfilePictureHelper::ProfilePictureHelper(std::string user_id, std::function<void(std::string, std::string)> callback) : user_id(user_id), callback(callback)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    FacebookManager_iOS::GetUserProfilePictureAsync(user_id, 64, std::bind(&FacebookManager::ProfilePictureHelper::onResponce, this, std::placeholders::_1));
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    stringCallbacks[callbackCounter] = std::bind(&FacebookManager::ProfilePictureHelper::onResponce, this, std::placeholders::_1);
    
    cocos2d::JniMethodInfo method;
    
    if(!cocos2d::JniHelper::getStaticMethodInfo(method, "com/codeexotics/tools/FacebookManager", "GetUserProfilePictureAsync", "(IILjava/lang/String;)V"))
    {
        LOGD("Failed to find static method id of com.codeexotics.tools.FacebookManager.GetUserProfilePictureAsync()");
        
        return;
    }
    
    jstring object_id = method.env->NewStringUTF(user_id.c_str());
    
    method.env->CallStaticVoidMethod(method.classID, method.methodID, callbackCounter, 64, object_id);
    
    method.env->DeleteLocalRef(object_id);
    method.env->DeleteLocalRef(method.classID);
    
    ++callbackCounter;
#endif
    
}

void FacebookManager::SetFriends(std::vector<FacebookManager::FriendDetails> new_friends)
{
    friends = new_friends;
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C"
{
    JNIEXPORT void JNICALL Java_com_codeexotics_tools_FacebookManager_executeBooleanCallback(JNIEnv * environment, jobject thiz, jint handlerID, jboolean result)
    {
        auto callback = boolCallbacks[handlerID];
        
        boolCallbacks.erase(handlerID);
        
        if (callback)
            callback(result);
    }
    
    JNIEXPORT void JNICALL Java_com_codeexotics_tools_FacebookManager_executeStringCallback(JNIEnv * environment, jobject thiz, jint handlerID, jstring result)
    {
        auto callback = stringCallbacks[handlerID];
        
        stringCallbacks.erase(handlerID);
        
        std::string c_str = cocos2d::JniHelper::jstring2string(result);
        
        if (callback)
            callback(c_str);
    }
    
    JNIEXPORT void JNICALL Java_com_codeexotics_tools_FacebookManager_executeTwoStringsCallback(JNIEnv * environment, jobject thiz, jint handlerID, jstring result1, jstring result2)
    {
        auto callback = twoStringsCallbacks[handlerID];
        
        twoStringsCallbacks.erase(handlerID);
        
        std::string c_str1 = cocos2d::JniHelper::jstring2string(result1);
        std::string c_str2 = cocos2d::JniHelper::jstring2string(result2);
        
        if (callback)
            callback(c_str1, c_str2);
    }
    
    JNIEXPORT void JNICALL Java_com_codeexotics_tools_FacebookManager_executeGetRequestsCallback(JNIEnv * environment, jobject thiz, jobjectArray requestIDs, jobjectArray senderIDs, jobjectArray senderNames, jobjectArray objectIDs)
    {
        std::vector<FacebookManager::GameRequestRecord> result;
        
        if(requestIDs != NULL && senderIDs != NULL && senderNames != NULL && objectIDs != NULL)
        {
            int itemsCount = environment->GetArrayLength(requestIDs);
            
            for(int i = 0; i < itemsCount; i++)
            {
                FacebookManager::GameRequestRecord newItem;
                
                newItem.request_id = cocos2d::JniHelper::jstring2string((jstring)environment->GetObjectArrayElement(requestIDs , i));
                newItem.sender     = cocos2d::JniHelper::jstring2string((jstring)environment->GetObjectArrayElement(senderIDs  , i));
                newItem.name       = cocos2d::JniHelper::jstring2string((jstring)environment->GetObjectArrayElement(senderNames, i));
                newItem.object     = cocos2d::JniHelper::jstring2string((jstring)environment->GetObjectArrayElement(objectIDs  , i));
                
                result.push_back(newItem);
            }
        }
        
        FacebookManager::getInstance().onGotRequests(result);
    }
    
    JNIEXPORT void Java_com_codeexotics_tools_FacebookManager_executeGetFriendsCallback(JNIEnv * environment, jobject thiz, jboolean success, jobjectArray usersIDs, jobjectArray usersNames, jint handlerID)
    {
        auto callback = boolCallbacks[handlerID];
        
        boolCallbacks.erase(handlerID);
        
        std::vector<FacebookManager::FriendDetails> friends;
        
        if(usersIDs != NULL && usersNames != NULL)
        {
            int usersCount = environment->GetArrayLength(usersIDs);
            
            for(int i = 0; i < usersCount; i++)
            {
                FacebookManager::FriendDetails newUser;
                
                newUser.user_id = cocos2d::JniHelper::jstring2string((jstring)environment->GetObjectArrayElement(usersIDs, i));
                newUser.name    = cocos2d::JniHelper::jstring2string((jstring)environment->GetObjectArrayElement(usersNames, i));
                
                
                friends.push_back(newUser);
            }
        }
        
        FacebookManager::getInstance().SetFriends(friends);
        
        if (callback)
            callback(success);
    }
    
    JNIEXPORT void Java_com_codeexotics_tools_FacebookManager_executeGetFriendsDetailsCallback(JNIEnv * environment, jobject thiz, jboolean success, jobjectArray usersIDs, jobjectArray usersNames, jint handlerID)
    {
        auto callback = friendsDetailsCallbacks[handlerID];
        
        friendsDetailsCallbacks.erase(handlerID);
        
        std::vector<FacebookManager::FriendDetails> friends;
        
        if(usersIDs != NULL && usersNames != NULL)
        {
            int usersCount = environment->GetArrayLength(usersIDs);
            
            for(int i = 0; i < usersCount; i++)
            {
                FacebookManager::FriendDetails newUser;
                
                newUser.user_id = cocos2d::JniHelper::jstring2string((jstring)environment->GetObjectArrayElement(usersIDs, i));
                newUser.name    = cocos2d::JniHelper::jstring2string((jstring)environment->GetObjectArrayElement(usersNames, i));
                
                friends.push_back(newUser);
            }
        }
        
        if (callback)
            callback(success, friends);
    }
    
    JNIEXPORT void Java_com_codeexotics_tools_FacebookManager_executeRequestRecipients(JNIEnv * environment, jobject thiz, jboolean success, jobjectArray usersIDs, jint handlerID)
    {
        auto callback = friendsCallbacks[handlerID];
        
        friendsCallbacks.erase(handlerID);
        
        std::vector<std::string> friends;
        
        if(usersIDs != NULL)
        {
            int usersCount = environment->GetArrayLength(usersIDs);
            
            for(int i = 0; i < usersCount; i++)
            {
                std::string userID = cocos2d::JniHelper::jstring2string((jstring)environment->GetObjectArrayElement(usersIDs, i));
                
                friends.push_back(userID);
            }
        }
        
        if (callback)
            callback(success, friends);
    }
}
#endif
