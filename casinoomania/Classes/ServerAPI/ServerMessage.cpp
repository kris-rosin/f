//
//  ServerMessage.cpp
//
//  Created by Stanislav Zheronkin on 06.09.17.
//  Copyright (c) 2017 Codeexotics Studio. All rights reserved.
//

#include "ServerMessage.h"
#include "mConstants.h"

ServerMessage::ServerMessage(unsigned char messageID) : messageID(messageID)
{

}

ServerMessage::~ServerMessage()
{

}

ServerMessage * ServerMessage::parse(DataBuffer * buffer, unsigned char versionAPI)
{
    unsigned char messageID = buffer->readByte();
    
    switch(messageID)
    {
        case 0x01:
            return new FBConnect(buffer, versionAPI);
            
        case 0x02:
            return new FBConnectResult(buffer, versionAPI);
            
        case 0x03:
            return new ErrorMessage(buffer, versionAPI);
            
        case 0x04:
            return new RegisterFBInvite(buffer, versionAPI);
            
        case 0x05:
            return new RegisterFBInviteResult(buffer, versionAPI);
            
        case 0x06:
            return new RequestFBInvitedFriendsList(buffer, versionAPI);
            
        case 0x07:
            return new FBInvitedFriendsList(buffer, versionAPI);
            
        case 0x08:
            return new CollectInviteReward(buffer, versionAPI);
            
        case 0x09:
            return new GenericRequestResult(buffer, versionAPI);
            
        case 0x0A:
            return new SyncTimeMessage(buffer, versionAPI);
            
        case 0x0B:
            return new SyncTimeResultMessage(buffer, versionAPI);
        
        case 0x0C:
            return new CreateSessionMessage(buffer, versionAPI);
            
        case 0x0D:
            return new NewSessionMessage(buffer, versionAPI);
            
        case 0x0E:
            return new UserDataMessage(buffer, versionAPI);
            
        case 0x10:
            return new GetUserDataMessage(buffer, versionAPI);
         
        case 0x11:
            return new SyncMoneyChange(buffer, versionAPI);
            
        case 0x12:
            return new SyncMoneyResult(buffer, versionAPI);
            
        case 0x13:
            return new SignupMessage(buffer, versionAPI);
        
        case 0x14:
            return new SigninMessage(buffer, versionAPI);
        
        case 0x15:
            return new SigninResult(buffer, versionAPI);
            
        case 0x16:
            return new PasswordRecoveryMessage(buffer, versionAPI);
            
        default:
            return nullptr;
    }
}

unsigned char ServerMessage::getMessageID() const
{
    return messageID;
}

DataBuffer * ServerMessage::pack() const
{
    DataBuffer * buffer = new DataBuffer();
    
    buffer->writeByte(messageID);
    
    return buffer;
}

ServerMessage * ServerMessage::copy() const
{
    DataBuffer * buffer = this->pack();
    
    ServerMessage * result = ServerMessage::parse(buffer, SERVER_API_VERSION);
    
    delete buffer;
    
    return result;
}

#pragma mark - FBConnect

FBConnect::FBConnect(const std::string & facebookID, const std::string & name, const std::string & email) : ServerMessage(0x01), facebookID(facebookID), name(name), email(email)
{
    
}

FBConnect::FBConnect(DataBuffer * buffer, unsigned char versionAPI) : ServerMessage(0x01)
{
    facebookID = buffer->readString();
    name       = buffer->readString();
    
    if (versionAPI >= 5)
    {
        email = buffer->readString();
    }
}
    
FBConnect::~FBConnect()
{
    
}

DataBuffer * FBConnect::pack() const
{
    DataBuffer * buffer = ServerMessage::pack();
    
    buffer->writeString(facebookID);
    buffer->writeString(name);
    buffer->writeString(email);
    
    buffer->reset();
    
    return buffer;
}

#pragma mark - FBConnectResult

FBConnectResult::FBConnectResult(DataBuffer * buffer, unsigned char versionAPI) : ServerMessage(0x02)
{
    result = buffer->readByte();
}
    
FBConnectResult::~FBConnectResult()
{
    
}

DataBuffer * FBConnectResult::pack() const
{
    DataBuffer * buffer = ServerMessage::pack();
    
    buffer->writeByte(result);
    
    buffer->reset();
    
    return buffer;
}
    
unsigned char FBConnectResult::getResult() const
{
    return result;
}

#pragma mark - ErrorMessage

ErrorMessage::ErrorMessage(ErrorMessageType type, const std::string & message) : ServerMessage(0x03), type(type), message(message)
{
}

ErrorMessage::ErrorMessage(DataBuffer * buffer, unsigned char versionAPI) : ServerMessage(0x03)
{
    type    = (ErrorMessageType)buffer->readByte();
    message = buffer->readString();
}

ErrorMessage::~ErrorMessage()
{
}

DataBuffer * ErrorMessage::pack() const
{
    DataBuffer * buffer = ServerMessage::pack();
    
    buffer->writeByte((unsigned char)type);
    buffer->writeString(message);
    
    buffer->reset();
    
    return buffer;
}

void ErrorMessage::setType(ErrorMessageType value)
{
    type = value;
}

ErrorMessageType ErrorMessage::getType() const
{
    return type;
}

void ErrorMessage::setMessage(const std::string & value)
{
    message = value;
}

std::string ErrorMessage::getMessage() const
{
    return message;
}

#pragma mark - RegisterFBInvite

RegisterFBInvite::RegisterFBInvite(const std::string & facebookID, const std::vector<std::string> & friendsList) : ServerMessage(0x04), facebookID(facebookID), friends(friendsList)
{
    
}

RegisterFBInvite::RegisterFBInvite(DataBuffer * buffer, unsigned char versionAPI) : ServerMessage(0x04)
{
    facebookID = buffer->readString();
    
    unsigned int amount = buffer->readUnsignedInt();
    
    friends.resize(amount);
    
    for (int i = 0; i < amount; i++)
    {
        friends[i] = buffer->readString();
    }
}
    
RegisterFBInvite::~RegisterFBInvite()
{
    
}

DataBuffer * RegisterFBInvite::pack() const
{
    DataBuffer * buffer = ServerMessage::pack();
    
    buffer->writeString(facebookID);
    
    buffer->writeInt(static_cast<unsigned int>(friends.size()));
    
    for (auto &item: friends)
    {
        buffer->writeString(item);
    }
    
    buffer->reset();
    
    return buffer;
}

#pragma mark - RegisterFBInvite

RegisterFBInviteResult::RegisterFBInviteResult(DataBuffer * buffer, unsigned char versionAPI) : ServerMessage(0x05)
{
    unsigned int amount = buffer->readUnsignedInt();
    
    results.resize(amount);
    
    for (int i = 0; i < amount; i++)
    {
        std::string fbid = buffer->readString();
        unsigned char status = buffer->readByte();
        
        results[i] = { fbid, status};
    }
}
    
RegisterFBInviteResult::~RegisterFBInviteResult()
{
    
}

DataBuffer * RegisterFBInviteResult::pack() const
{
    DataBuffer * buffer = ServerMessage::pack();
    
    buffer->writeInt(static_cast<unsigned int>(results.size()));
    
    for (auto &item: results)
    {
        buffer->writeString(item.first);
        buffer->writeByte(item.second);
    }
    
    buffer->reset();
    
    return buffer;
}
    
std::vector<std::pair<std::string, unsigned char> > RegisterFBInviteResult::getResult() const
{
    return results;
}

#pragma mark - RequestFBInvitedFriendsList

RequestFBInvitedFriendsList::RequestFBInvitedFriendsList(const std::string & facebookID) : ServerMessage(0x06), facebookID(facebookID)
{
    
}

RequestFBInvitedFriendsList::RequestFBInvitedFriendsList(DataBuffer * buffer, unsigned char versionAPI) : ServerMessage(0x06)
{
    facebookID = buffer->readString();
}
    
RequestFBInvitedFriendsList::~RequestFBInvitedFriendsList()
{
    
}

DataBuffer * RequestFBInvitedFriendsList::pack() const
{
    DataBuffer * buffer = ServerMessage::pack();
    
    buffer->writeString(facebookID);
    buffer->reset();
    
    return  buffer;
}

#pragma mark - FBInvitedFriendsList

FBInvitedFriendsList::FBInvitedFriendsList(DataBuffer * buffer, unsigned char versionAPI) : ServerMessage(0x07)
{
    unsigned int amount = buffer->readUnsignedInt();
    
    results.resize(amount);
    
    for (int i = 0; i < amount; i++)
    {
        FBFriendInfo friendItem;
        
        friendItem.facebookID = buffer->readString();
        friendItem.name       = buffer->readString();
        friendItem.status     = static_cast<FBFriendStatus>(buffer->readByte());
        
        results[i] = friendItem;
    }
}
    
FBInvitedFriendsList::~FBInvitedFriendsList()
{
    
}

DataBuffer * FBInvitedFriendsList::pack() const
{
    DataBuffer * buffer = ServerMessage::pack();
    
    buffer->writeInt(static_cast<unsigned int>(results.size()));
    
    for (auto &item: results)
    {
        buffer->writeString(item.facebookID);
        buffer->writeString(item.name);
        buffer->writeByte(static_cast<unsigned char>(item.status));
    }
    
    buffer->reset();
    
    return buffer;
}
    
std::vector<FBFriendInfo> FBInvitedFriendsList::getResult() const
{
    return results;
}

#pragma mark - CollectInviteReward

CollectInviteReward::CollectInviteReward(const std::string & facebookID, const std::string & friendID) : ServerMessage(0x08), facebookID(facebookID), friendID(friendID)
{
    
}

CollectInviteReward::CollectInviteReward(DataBuffer * buffer, unsigned char versionAPI) : ServerMessage(0x08)
{
    facebookID = buffer->readString();
    friendID   = buffer->readString();
}
    
CollectInviteReward::~CollectInviteReward()
{
    
}

DataBuffer * CollectInviteReward::pack() const
{
    DataBuffer * buffer =  ServerMessage::pack();
    
    buffer->writeString(facebookID);
    buffer->writeString(friendID);
    buffer->reset();
    
    return buffer;
}

#pragma mark - GenericRequestResult

GenericRequestResult::GenericRequestResult(DataBuffer * buffer, unsigned char versionAPI) : ServerMessage(0x09)
{
    result = buffer->readByte() == 0x01;
}
    
GenericRequestResult::~GenericRequestResult()
{
    
}

DataBuffer * GenericRequestResult::pack() const
{
    DataBuffer * buffer =  ServerMessage::pack();
    
    buffer->writeByte(result ? 0x01: 0x02);
    buffer->reset();
    
    return buffer;
}
    
bool GenericRequestResult::getResult() const
{
    return result;
}

#pragma mark - SyncTimeMessage

SyncTimeMessage::SyncTimeMessage(unsigned int timestamp) : ServerMessage(0x0A), timestamp(timestamp)
{
    
}

SyncTimeMessage::SyncTimeMessage(DataBuffer * buffer, unsigned char versionAPI) : ServerMessage(0x0A)
{
    timestamp = buffer->readUnsignedInt();
}
    
SyncTimeMessage::~SyncTimeMessage()
{
    
}

DataBuffer * SyncTimeMessage::pack() const
{
    DataBuffer * buffer = ServerMessage::pack();
    
    buffer->writeInt(timestamp);
    
    buffer->reset();
    
    return buffer;
}

#pragma mark - GenericRequestResult

SyncTimeResultMessage::SyncTimeResultMessage(DataBuffer * buffer, unsigned char versionAPI) : ServerMessage(0x0B)
{
    delta = buffer->readInt();
}
    
SyncTimeResultMessage::~SyncTimeResultMessage()
{
    
}

DataBuffer * SyncTimeResultMessage::pack() const
{
    DataBuffer * buffer = ServerMessage::pack();
    
    buffer->writeInt(delta);
    buffer->reset();
    
    return buffer;
}
    
int SyncTimeResultMessage::getDelta() const
{
    return delta;
}

#pragma mark - CreateSessionMessage

CreateSessionMessage::CreateSessionMessage() : ServerMessage(0x0C)
{
}

CreateSessionMessage::CreateSessionMessage(DataBuffer * buffer, unsigned char versionAPI) : ServerMessage(0x0C)
{
}
    
CreateSessionMessage::~CreateSessionMessage()
{
}

DataBuffer * CreateSessionMessage::pack() const
{
    DataBuffer * buffer = ServerMessage::pack();
    buffer->reset();
    
    return buffer;
}

#pragma mark - NewSessionMessage

NewSessionMessage::NewSessionMessage(DataBuffer * buffer, unsigned char versionAPI) : ServerMessage(0x0D)
{
    sessionID = buffer->readUnsignedInt();
}
    
NewSessionMessage::~NewSessionMessage()
{
}

DataBuffer * NewSessionMessage::pack() const
{
    DataBuffer * buffer = ServerMessage::pack();
    
    buffer->writeInt(sessionID);
    buffer->reset();
    
    return buffer;
}
    
unsigned int NewSessionMessage::getSession() const
{
    return sessionID;
}

#pragma mark - GetUserDataMessage

GetUserDataMessage::GetUserDataMessage() : ServerMessage(0x10)
{
}

GetUserDataMessage::GetUserDataMessage(DataBuffer * buffer, unsigned char versionAPI) : ServerMessage(0x10)
{
}

GetUserDataMessage::~GetUserDataMessage()
{
}

DataBuffer * GetUserDataMessage::pack() const
{
    DataBuffer * buffer = ServerMessage::pack();
    buffer->reset();
    
    return buffer;
}

#pragma mark - UserDataMessage

UserDataMessage::UserDataMessage(unsigned int userID, std::string username, unsigned int level, unsigned int experience, unsigned long long coins, unsigned int timestamp, std::string facebook, DataBuffer * savefile) : ServerMessage(0x0E), userID(userID), username(username), level(level), experience(experience), coins(coins), timestamp(timestamp), savefile(savefile), facebook(facebook)
{
}

UserDataMessage::UserDataMessage(DataBuffer * buffer, unsigned char versionAPI) : ServerMessage(0x0E)
{
    userID     = buffer->readUnsignedInt();
    username   = buffer->readString();
    level      = buffer->readUnsignedInt();
    experience = buffer->readUnsignedInt();
    coins      = buffer->readUnsignedLong();
    timestamp  = buffer->readUnsignedInt();
    
    if (versionAPI >= 4)
    {
        facebook   = buffer->readString();
        email      = buffer->readString();
    }
    
    savefile = nullptr;
    
    auto datasize = buffer->readUnsignedInt();
    if (datasize > 0)
    {
        unsigned char * buff = new unsigned char[datasize];
        if (buffer->readBytes(buff, datasize))
        {
            savefile = new DataBuffer(buff, datasize);
        }
        
        delete [] buff;
    }
}

UserDataMessage::~UserDataMessage()
{
    if (savefile)
    {
        delete savefile;
    }
}

DataBuffer * UserDataMessage::pack() const
{
    DataBuffer * buffer = ServerMessage::pack();
    
    buffer->writeInt(userID);
    buffer->writeString(username);
    buffer->writeInt(level);
    buffer->writeInt(experience);
    buffer->writeLong(coins);
    buffer->writeInt(timestamp);
    buffer->writeString(facebook);
    buffer->writeString(email);
    
    if (savefile)
    {
        buffer->writeInt(savefile->getSize());
        buffer->writeBytes(savefile->getData(), savefile->getSize());
    }
    else
    {
        buffer->writeInt((unsigned int)0);
    }
    
    return buffer;
}
    
unsigned int UserDataMessage::getUserID() const
{
    return userID;
}

unsigned int UserDataMessage::getUserLevel() const
{
    return level;
}

unsigned int UserDataMessage::getUserExperience() const
{
    return experience;
}

unsigned int UserDataMessage::getTimestamp() const
{
    return timestamp;
}

unsigned long long UserDataMessage::getCoins() const
{
    return coins;
}

std::string UserDataMessage::getUserName() const
{
    return username;
}

std::string UserDataMessage::getFacebookID() const
{
    return facebook;
}

std::string UserDataMessage::getEmail() const
{
    return email;
}

DataBuffer * UserDataMessage::getSaveFileData() const
{
    return savefile;
}

#pragma mark - SyncMoneyChange

SyncMoneyChange::SyncMoneyChange(int coinsChange, unsigned int experience, unsigned int level) : ServerMessage(0x11), coinsChange(coinsChange), experience(experience), level(level)
{
    
}

SyncMoneyChange::SyncMoneyChange(DataBuffer * buffer, unsigned char versionAPI) : ServerMessage(0x11)
{
    coinsChange = buffer->readInt();
    experience  = buffer->readUnsignedInt();
    level       = buffer->readUnsignedInt();
}
    
SyncMoneyChange::~SyncMoneyChange()
{
    
}

DataBuffer * SyncMoneyChange::pack() const
{
    DataBuffer * buffer = ServerMessage::pack();
    
    buffer->writeInt(coinsChange);
    buffer->writeInt(experience);
    buffer->writeInt(level);
    buffer->reset();
    
    return buffer;
}

#pragma mark - SyncMoneyChange

SyncMoneyResult::SyncMoneyResult(DataBuffer * buffer, unsigned char versionAPI) : ServerMessage(0x12)
{
    coins  = buffer->readUnsignedLong();
    experience = buffer->readUnsignedInt();
    level  = buffer->readUnsignedInt();
}
    
SyncMoneyResult::~SyncMoneyResult()
{
}

DataBuffer * SyncMoneyResult::pack() const
{
    DataBuffer * buffer = ServerMessage::pack();
    
    buffer->writeLong(coins);
    buffer->writeInt(experience);
    buffer->writeInt(level);
    
    buffer->reset();
    
    return buffer;
}
    
unsigned int SyncMoneyResult::getLevel() const
{
    return level;
}

unsigned int SyncMoneyResult::getExperience() const
{
    return experience;
}

unsigned long long SyncMoneyResult::getCoins() const
{
    return coins;
}

#pragma mark - SignupMessage

SignupMessage::SignupMessage(std::string email, std::string pass, std::string username) : ServerMessage(0x13), email(email), pass(pass), username(username)
{
    
}

SignupMessage::SignupMessage(DataBuffer * buffer, unsigned char versionAPI) : ServerMessage(0x13)
{
    email    = buffer->readString();
    pass     = buffer->readString();
    username = buffer->readString();
}
    
SignupMessage::~SignupMessage()
{
}

DataBuffer * SignupMessage::pack() const
{
    DataBuffer * buffer = ServerMessage::pack();
    
    buffer->writeString(email);
    buffer->writeString(pass);
    buffer->writeString(username);
    buffer->reset();
    
    return buffer;
}

#pragma mark - SigninMessage

SigninMessage::SigninMessage(std::string email, std::string pass) : ServerMessage(0x14), email(email), pass(pass)
{
    
}

SigninMessage::SigninMessage(DataBuffer * buffer, unsigned char versionAPI) : ServerMessage(0x14)
{
    email    = buffer->readString();
    pass     = buffer->readString();
}

SigninMessage::~SigninMessage()
{
}

DataBuffer * SigninMessage::pack() const
{
    DataBuffer * buffer = ServerMessage::pack();
    
    buffer->writeString(email);
    buffer->writeString(pass);
    buffer->reset();
    
    return buffer;
}

#pragma mark - SigninResult

SigninResult::SigninResult(DataBuffer * buffer, unsigned char versionAPI) : ServerMessage(0x15)
{
    result = buffer->readByte();
}
    
SigninResult::~SigninResult()
{
}

DataBuffer * SigninResult::pack() const
{
    DataBuffer * buffer = ServerMessage::pack();
    
    buffer->writeByte(result);
    buffer->reset();
    
    return buffer;
}
    
SigninResultType SigninResult::getResult() const
{
    switch (result)
    {
        case 0x01:
            return SigninResultType::SuccessfullyLoggedin;
            
        case 0x02:
            return SigninResultType::WrongLoginOrPassword;
         
        case 0x03:
            return SigninResultType::LoginAlreadyInUse;
            
        default:
            return SigninResultType::UnexpectedError;
            
    }
}

#pragma mark - PasswordRecoveryMessage

PasswordRecoveryMessage::PasswordRecoveryMessage(std::string email) : ServerMessage(0x16), email(email)
{
    
}

PasswordRecoveryMessage::PasswordRecoveryMessage(DataBuffer * buffer, unsigned char versionAPI) : ServerMessage(0x16)
{
    email = buffer->readString();
}
    
PasswordRecoveryMessage::~PasswordRecoveryMessage()
{
}

DataBuffer * PasswordRecoveryMessage::pack() const
{
    DataBuffer * buffer = ServerMessage::pack();
    
    buffer->writeString(email);
    buffer->reset();
    
    return buffer;
}

