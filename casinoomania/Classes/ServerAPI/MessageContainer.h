//
//  MessageContainer.h
//
//  Created by Stanislav Zheronkin on 06.09.17.
//  Copyright (c) 2017 Codeexotics Studio. All rights reserved.
//

#pragma once

#include "DataBuffer.h"

#include <string>

class MessageContainer
{
private:
    unsigned int   timestamp;
    unsigned int   checksum;
    unsigned char  version;
    unsigned int   sessionID;
    
    DataBuffer * data;
    
    bool         error;
private:
    DataBuffer * encrypt(unsigned int key, DataBuffer * data) const;
    DataBuffer * decrypt(unsigned int key, DataBuffer * data) const;
    
    unsigned int computeChecksum(DataBuffer * data) const;
public:
    MessageContainer();
    MessageContainer(DataBuffer * buffer);
    ~MessageContainer();
    
    unsigned int getTimestamp() const;
    unsigned int getChecksum() const;
    
    DataBuffer * getData() const;
    
    bool isValid() const;
    bool isError() const;
    
    unsigned char getVersionAPI() const;
    void setVersionAPI(unsigned versionAPI);
    
    void setSessionID(unsigned int session);
    
    DataBuffer * pack(DataBuffer * data);
};
