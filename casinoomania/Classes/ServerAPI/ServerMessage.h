//
//  ServerMessage.h
//
//  Created by Stanislav Zheronkin on 06.09.17.
//  Copyright (c) 2017 Codeexotics Studio. All rights reserved.
//

#pragma once

#include "DataBuffer.h"

#include <memory>
#include <vector>
#include <string>

class ServerMessage
{
private:
    unsigned char messageID;
protected:
    ServerMessage(unsigned char messageID);
public:
    virtual ~ServerMessage();
    static ServerMessage * parse(DataBuffer * buffer, unsigned char versionAPI);
    virtual DataBuffer * pack() const;
    unsigned char getMessageID() const;
    
    ServerMessage * copy() const;
};

typedef enum
{
    ErrorMessageTypeUnknown          = 0,
    ErrorMessageTypeInvalidChecksum  = 1,
    ErrorMessageTypeNotImplemented   = 2,
    ErrorMessageTypeUnknownMessage   = 3,
    ErrorMessageTypeInvalidSession   = 4,
    ErrorMessageTypeDBError          = 5,
    ErrorMessageTypeInvalidPlayer    = 6,
    ErrorMessageTypeMultipleSessions = 7,
    ErrorMessageTypeNoConnection     = 8
}
ErrorMessageType;

enum class FBInviteResultType
{
    Unknown = 0,
    Invited,
    AlreadyInvited,
    AlreadyPlaying
};

enum class SigninResultType
{
    UnexpectedError = 0,
    SuccessfullyLoggedin = 1,
    WrongLoginOrPassword = 2,
    LoginAlreadyInUse = 3
};

enum class FBFriendStatus
{
    Invited = 0,
    Accepted,
    Playing
};

struct FBFriendInfo
{
    std::string name;
    std::string facebookID;
    FBFriendStatus status;
};

class ErrorMessage : public ServerMessage
{
private:
    ErrorMessageType type;
    std::string      message;
public:
    ErrorMessage(ErrorMessageType type, const std::string & message);
    ErrorMessage(DataBuffer * buffer, unsigned char versionAPI);
    virtual ~ErrorMessage();
    virtual DataBuffer * pack() const;
    void setType(ErrorMessageType value);
    ErrorMessageType getType() const;
    void setMessage(const std::string & value);
    std::string getMessage() const;
};

class FBConnect : public ServerMessage
{
private:
    std::string facebookID;
    std::string name;
    std::string email;
public:
    FBConnect(const std::string & facebookID, const std::string & name, const std::string & email);
    FBConnect(DataBuffer * buffer, unsigned char versionAPI);
    
    virtual ~FBConnect();
    virtual DataBuffer * pack() const;
};

class FBConnectResult : public ServerMessage
{
private:
    unsigned char result;
public:
    FBConnectResult(DataBuffer * buffer, unsigned char versionAPI);
    
    virtual ~FBConnectResult();
    virtual DataBuffer * pack() const;
    
    unsigned char getResult() const;
};

class RegisterFBInvite : public ServerMessage
{
private:
    std::string facebookID;
    std::vector<std::string> friends;
public:
    RegisterFBInvite(const std::string & facebookID, const std::vector<std::string> & friendsList);
    RegisterFBInvite(DataBuffer * buffer, unsigned char versionAPI);
    
    virtual ~RegisterFBInvite();
    virtual DataBuffer * pack() const;
};

class RegisterFBInviteResult : public ServerMessage
{
private:
    std::vector<std::pair<std::string, unsigned char> > results;
public:
    RegisterFBInviteResult(DataBuffer * buffer, unsigned char versionAPI);
    
    virtual ~RegisterFBInviteResult();
    virtual DataBuffer * pack() const;
    
    std::vector<std::pair<std::string, unsigned char> > getResult() const;
};

class RequestFBInvitedFriendsList : public ServerMessage
{
private:
    std::string facebookID;
public:
    RequestFBInvitedFriendsList(const std::string & facebookID);
    RequestFBInvitedFriendsList(DataBuffer * buffer, unsigned char versionAPI);
    
    virtual ~RequestFBInvitedFriendsList();
    virtual DataBuffer * pack() const;
};

class FBInvitedFriendsList : public ServerMessage
{
private:
    std::vector<FBFriendInfo> results;
public:
    FBInvitedFriendsList(DataBuffer * buffer, unsigned char versionAPI);
    
    virtual ~FBInvitedFriendsList();
    virtual DataBuffer * pack() const;
    
    std::vector<FBFriendInfo> getResult() const;
};

class CollectInviteReward : public ServerMessage
{
private:
    std::string facebookID;
    std::string friendID;
public:
    CollectInviteReward(const std::string & facebookID, const std::string & friendID);
    CollectInviteReward(DataBuffer * buffer, unsigned char versionAPI);
    
    virtual ~CollectInviteReward();
    virtual DataBuffer * pack() const;
};

class GenericRequestResult : public ServerMessage
{
private:
    bool result;
public:
    GenericRequestResult(DataBuffer * buffer, unsigned char versionAPI);
    
    virtual ~GenericRequestResult();
    virtual DataBuffer * pack() const;
    
    bool getResult() const;
};

class SyncTimeMessage : public ServerMessage
{
private:
    unsigned int timestamp;
public:
    SyncTimeMessage(unsigned int timestamp);
    SyncTimeMessage(DataBuffer * buffer, unsigned char versionAPI);
    
    virtual ~SyncTimeMessage();
    virtual DataBuffer * pack() const;
};

class SyncTimeResultMessage : public ServerMessage
{
private:
    int delta;
public:
    SyncTimeResultMessage(DataBuffer * buffer, unsigned char versionAPI);
    
    virtual ~SyncTimeResultMessage();
    virtual DataBuffer * pack() const;
    
    int getDelta() const;
};

class CreateSessionMessage : public ServerMessage
{
public:
    CreateSessionMessage();
    CreateSessionMessage(DataBuffer * buffer, unsigned char versionAPI);
    
    virtual ~CreateSessionMessage();
    virtual DataBuffer * pack() const;
};

class NewSessionMessage : public ServerMessage
{
private:
    unsigned int sessionID;
public:
    NewSessionMessage(DataBuffer * buffer, unsigned char versionAPI);
    
    virtual ~NewSessionMessage();
    virtual DataBuffer * pack() const;
    
    unsigned int getSession() const;
};

class GetUserDataMessage : public ServerMessage
{
public:
    GetUserDataMessage();
    GetUserDataMessage(DataBuffer * buffer, unsigned char versionAPI);
    
    virtual ~GetUserDataMessage();
    virtual DataBuffer * pack() const;
};

class UserDataMessage : public ServerMessage
{
private:
    unsigned int userID;
    std::string username;
    unsigned int level;
    unsigned int experience;
    unsigned long long coins;
    unsigned int timestamp;
    std::string facebook;
    std::string email;
    
    DataBuffer * savefile;
public:
    UserDataMessage(unsigned int userID, std::string username, unsigned int level, unsigned int experience, unsigned long long coins, unsigned int timestamp, std::string facebook,  DataBuffer * savefile);
    UserDataMessage(DataBuffer * buffer, unsigned char versionAPI);
    
    virtual ~UserDataMessage();
    virtual DataBuffer * pack() const;
    
    unsigned int getUserID() const;
    unsigned int getUserLevel() const;
    unsigned int getUserExperience() const;
    unsigned long long getCoins() const;
    unsigned int getTimestamp() const;
    std::string getUserName() const;
    std::string getFacebookID() const;
    std::string getEmail() const;
    
    DataBuffer * getSaveFileData() const;
};

class SyncMoneyChange : public ServerMessage
{
private:
    unsigned int level;
    unsigned int experience;
    int coinsChange;
public:
    SyncMoneyChange(int coinsChange, unsigned int expChange, unsigned int levelChange);
    SyncMoneyChange(DataBuffer * buffer, unsigned char versionAPI);
    
    virtual ~SyncMoneyChange();
    virtual DataBuffer * pack() const;
};

class SyncMoneyResult : public ServerMessage
{
private:
    unsigned int level;
    unsigned int experience;
    unsigned long long coins;
public:
    SyncMoneyResult(DataBuffer * buffer, unsigned char versionAPI);
    
    virtual ~SyncMoneyResult();
    virtual DataBuffer * pack() const;
    
    unsigned int getLevel() const;
    unsigned int getExperience() const;
    unsigned long long getCoins() const;
};

class PasswordRecoveryMessage : public ServerMessage
{
private:
    std::string email;
public:
    PasswordRecoveryMessage(std::string email);
    PasswordRecoveryMessage(DataBuffer * buffer, unsigned char versionAPI);
    
    virtual ~PasswordRecoveryMessage();
    virtual DataBuffer * pack() const;
};

class SignupMessage : public ServerMessage
{
private:
    std::string email;
    std::string pass;
    std::string username;
public:
    SignupMessage(std::string email, std::string pass, std::string username);
    SignupMessage(DataBuffer * buffer, unsigned char versionAPI);
    
    virtual ~SignupMessage();
    virtual DataBuffer * pack() const;
};

class SigninMessage : public ServerMessage
{
private:
    std::string email;
    std::string pass;
public:
    SigninMessage(std::string email, std::string pass);
    SigninMessage(DataBuffer * buffer, unsigned char versionAPI);
    
    virtual ~SigninMessage();
    virtual DataBuffer * pack() const;
};

class SigninResult : public ServerMessage
{
private:
    unsigned char result;
public:
    SigninResult(DataBuffer * buffer, unsigned char versionAPI);
    
    virtual ~SigninResult();
    virtual DataBuffer * pack() const;
    
    SigninResultType getResult() const;
};
