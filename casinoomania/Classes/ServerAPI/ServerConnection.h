//
//  ServerConnection.h
//
//  Created by Stanislav Zheronkin on 06.09.17.
//  Copyright (c) 2017 Codeexotics Studio. All rights reserved.
//

#pragma once

#include "cocos2d.h"
#include "network/HttpClient.h"

class ServerMessage;

class ServerConnection
{
private:
    unsigned int sessionID;
    
    std::map<cocos2d::network::HttpRequest*, std::pair<cocos2d::Ref*, std::function< bool(ServerMessage*)> > > requestsMap;
private:
    ServerConnection();
    ServerConnection(const ServerConnection & other);
    ~ServerConnection();
    
    ServerConnection & operator =(const ServerConnection & other);
    
    void handleResponse(cocos2d::network::HttpClient * client, cocos2d::network::HttpResponse * response);
public:
    static ServerConnection & getInstance();

    void sendMessage(ServerMessage * message, cocos2d::Ref * sender, std::function<bool(ServerMessage*)> callback);
    
    void ResetSessionID();
    bool isSessionCreated() const;
    void setSessionID(unsigned int value);
};
