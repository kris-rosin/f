//
//  MessageContainer.cpp
//
//  Created by Stanislav Zheronkin on 06.09.17.
//  Copyright (c) 2017 Codeexotics Studio. All rights reserved.
//

#include "MessageContainer.h"
#include "Platform.h"
#include "crc32.h"
#include "Base64_RC4.h"
#include "mConstants.h"
#include "PlayerProfile.h"

MessageContainer::MessageContainer() : timestamp(0), checksum(0), data(nullptr), error(false), version(SERVER_API_VERSION), sessionID(0)
{
}

MessageContainer::MessageContainer(DataBuffer * buffer) : timestamp(0), checksum(0), data(nullptr), error(false), version(SERVER_API_VERSION)
{
    if(buffer == nullptr)
        return;
    
    //checking API version
    version   = buffer->readByte();
    timestamp = buffer->readUnsignedInt();
    checksum  = buffer->readUnsignedInt();
    sessionID = buffer->readUnsignedInt();
    
    unsigned int length = buffer->readUnsignedInt();
    unsigned char * contentBuffer = nullptr;
    
    if (length != buffer->getSize() - buffer->getPosition())
    {
        printf("Wrong allocation found!");
        error = true;
        return;
    }
    
    try
    {
        contentBuffer = new unsigned char[length];
    }
    catch (...)
    {
        error = true;
        return;
    }
    
    if(buffer->readBytes(contentBuffer, length))
    {
        DataBuffer content(contentBuffer, length);
        
        data = decrypt(timestamp, &content);
    }
    
    delete [] contentBuffer;
    
    error = ! isValid();
}

MessageContainer::~MessageContainer()
{
    delete data;
}

DataBuffer * MessageContainer::encrypt(unsigned int key, DataBuffer * data) const
{
    if(data == nullptr)
        return nullptr;
    
    int buffSize = 32;
    char buff[buffSize];
    snprintf(buff, buffSize, "%d", key);
    
    RC4 encoder(buff);
    
    encoder.Encode((char*)data->getData(), data->getSize());
    
    return new DataBuffer(data->getData(), data->getSize());
}

DataBuffer * MessageContainer::decrypt(unsigned int key, DataBuffer * data) const
{
    if(data == nullptr)
        return nullptr;
    
    int buffSize = 32;
    char buff[buffSize];
    snprintf(buff, buffSize, "%d", key);
    
    RC4 decoder(buff);
    
    decoder.Decode((char*)data->getData(), data->getSize());
    
    return new DataBuffer(data->getData(), data->getSize());
}

unsigned int MessageContainer::computeChecksum(DataBuffer * data) const
{
    if(data == nullptr)
        return 0;
    
    crc32 checksummer;
    
    checksummer.ProcessCRC(data->getData(), data->getSize());
    
    return checksummer.m_crc32;
}

unsigned int MessageContainer::getTimestamp() const
{
    return timestamp;
}

unsigned int MessageContainer::getChecksum() const
{
    return checksum;
}

unsigned char MessageContainer::getVersionAPI() const
{
    return version;
}

void MessageContainer::setVersionAPI(unsigned versionAPI)
{
    version = versionAPI;
}

void MessageContainer::setSessionID(unsigned int session)
{
    sessionID = session;
}

DataBuffer * MessageContainer::getData() const
{
    return data;
}

bool MessageContainer::isValid() const
{
    return computeChecksum(data) == checksum;
}

bool MessageContainer::isError() const
{
    return error;
}

DataBuffer * MessageContainer::pack(DataBuffer * data)
{
    timestamp = Platform::unixTime();
    checksum = computeChecksum(data);
    this->data = new DataBuffer(data->getData(), data->getSize()); //copy
    
    DataBuffer * buffer = new DataBuffer();
    
    buffer->writeByte(version);
    buffer->writeInt(timestamp);
    buffer->writeInt(checksum);
    buffer->writeInt(sessionID);

    DataBuffer * encryptedData = nullptr;
    
    encryptedData = encrypt(timestamp, data);
    
    buffer->writeInt(encryptedData->getSize());
    buffer->writeBytes(encryptedData->getData(), encryptedData->getSize());
    
    delete encryptedData;
    
    return buffer;
}
