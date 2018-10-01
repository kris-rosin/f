//
//  ServerConnection.cpp
//
//  Created by Stanislav Zheronkin on 06.09.17.
//  Copyright (c) 2017 Codeexotics Studio. All rights reserved.
//

#include "ServerConnection.h"
#include "MessageContainer.h"
#include "ServerMessage.h"
#include "mConstants.h"

USING_NS_CC;

using namespace network;

ServerConnection::ServerConnection() : sessionID(cocos2d::UserDefault::getInstance()->getIntegerForKey("SessionID", 0))
{
}

ServerConnection::ServerConnection(const ServerConnection & other)
{
}

ServerConnection::~ServerConnection()
{
}

ServerConnection & ServerConnection::operator =(const ServerConnection & other)
{
    return *this;
}

ServerConnection & ServerConnection::getInstance()
{
    static ServerConnection instance;
    
    return instance;
}

void ServerConnection::sendMessage(ServerMessage * message, cocos2d::Ref * sender, std::function<bool(ServerMessage*)> callback)
{
    if (sender == nullptr)
        return;
    
    sender->retain(); //lock object
    
    MessageContainer container;
    
    container.setSessionID(sessionID);
    
    DataBuffer * messageBuffer = message->pack();
    DataBuffer * packedMessage = container.pack(messageBuffer);
    
    HttpRequest * request = new HttpRequest();
    
    request->setUrl(SERVER_API_URL);
    request->setRequestData(reinterpret_cast<const char*>(packedMessage->getData()), packedMessage->getSize());
    request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
    request->setResponseCallback(std::bind(&ServerConnection::handleResponse, this, std::placeholders::_1, std::placeholders::_2));
    
    //register request
    requestsMap[request] = { sender, callback };
    
    HttpClient::getInstance()->send(request);
    
    delete packedMessage;
    delete messageBuffer;
}

void ServerConnection::handleResponse(HttpClient * client, HttpResponse * response)
{
    auto request = requestsMap.find( response->getHttpRequest() );
    
    if (request != requestsMap.end())
    {
        if (response->isSucceed())
        {
            auto data = response->getResponseData();
            
            DataBuffer containerBuffer((unsigned char*)data->data(), (unsigned int)data->size());
            
            MessageContainer container(&containerBuffer);
            
            if ( !container.isError() )
            {
                ServerMessage * parsedMessage = ServerMessage::parse(container.getData(), container.getVersionAPI());
                
                if (parsedMessage == NULL)
                {
                    CCLOG("ServerConnection::handleResponse(): Unable to parse message\n");
                    
                    ServerMessage * newMessage = new ErrorMessage(ErrorMessageTypeUnknownMessage, "Invalid buffer received");
                    request->second.second(newMessage);
                    delete newMessage;
                    
                    return;
                }
                
                // error message
                if(parsedMessage->getMessageID() == 0x03)
                    CCLOG("Connection error: %s\n", dynamic_cast<ErrorMessage*>(parsedMessage)->getMessage().c_str());
                
                request->second.second(parsedMessage);
                
                delete parsedMessage;
            }
            else
            {
                CCLOG("ServerConnection::handleResponse(): Wrong message format or checksum\n");
                
                //smf went wrong
                ServerMessage * newMessage = new ErrorMessage(ErrorMessageTypeUnknownMessage, "Wrong message format or checksum");
                request->second.second(newMessage);
                delete newMessage;
            }
        }
        else
        {
            //return error message
            ServerMessage * newMessage = new ErrorMessage(ErrorMessageTypeNoConnection, "Server or internet error.");
            
            request->second.second(newMessage);
            delete newMessage;
        }
        
        //unlock object
        request->second.first->release();
        
        //remove from map
        requestsMap.erase(request);
    }
}

void ServerConnection::ResetSessionID()
{
    setSessionID(0);
}

void ServerConnection::setSessionID(unsigned int value)
{
    sessionID = value;
    
    UserDefault::getInstance()->setIntegerForKey("SessionID", sessionID);
    UserDefault::getInstance()->flush();
}

bool ServerConnection::isSessionCreated() const
{
    return sessionID != 0;
}
