//
//  DataBuffer.h
//  RestaurantDreams
//
//  Created by Dmitry Fadeev on 10.04.14.
//  Copyright (c) 2014 Tachyon Labs. All rights reserved.
//

#pragma once

#include <string>

class DataBuffer
{
private:
    unsigned char * data;
    unsigned int    size;
    unsigned int    bufferSize;
    unsigned int    offset;
    
    static   int    _instancesCounter;
private:
    void extendIfNeeded(unsigned int size);
public:
    DataBuffer();
    DataBuffer(unsigned char * buffer, unsigned int size);
    virtual ~DataBuffer();
    unsigned char * getData() const;
    unsigned int getSize() const;
    bool writeByte(unsigned char value);
    bool writeWord(unsigned short value);
    bool writeWord(short value);
    bool writeInt(unsigned int value);
    bool writeInt(int value);
    bool writeLong(unsigned long long value);
    bool writeLong(long long value);
    bool writeFloat(float value);
    bool writeDouble(double value);
    bool writeString(const std::string & value);
    bool writeBytes(void * buffer, unsigned int length);
    unsigned char readByte();
    unsigned short readUnsignedWord();
    short readWord();
    unsigned int readUnsignedInt();
    int readInt();
    unsigned long long readUnsignedLong();
    long long readLong();
    float readFloat();
    double readDouble();
    std::string readString();
    bool readBytes(void * buffer, unsigned int length);
    void reset();
    void seekBy(int offset);
    void seekTo(int offset);
    unsigned int getPosition();
};
