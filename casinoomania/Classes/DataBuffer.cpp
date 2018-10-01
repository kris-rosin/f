//
//  DataBuffer.cpp
//  RestaurantDreams
//
//  Created by Dmitry Fadeev on 10.04.14.
//  Copyright (c) 2014 Tachyon Labs. All rights reserved.
//

#include "DataBuffer.h"
#include <math.h>

int DataBuffer::_instancesCounter = 0;

DataBuffer::DataBuffer() : size(0), bufferSize(1024), offset(0)
{
    data = new unsigned char[bufferSize];
}

DataBuffer::DataBuffer(unsigned char * buffer, unsigned int size) : size(size), bufferSize(size), offset(0)
{
    data = new unsigned char[size];
    memcpy(data, buffer, size);
}

DataBuffer::~DataBuffer()
{
    delete [] data;
}

void DataBuffer::extendIfNeeded(unsigned int addingSize)
{
    if(offset + addingSize <= bufferSize)
        return;
    
    unsigned int newBufferSize = ceil((offset + addingSize) / 1024.0) * 1024;
    
    unsigned char * newData = new unsigned char[newBufferSize];
    
    memcpy(newData, data, size);
    
    delete [] data;
    
    data = newData;
    bufferSize = newBufferSize;
}

unsigned char * DataBuffer::getData() const
{
    return data;
}

unsigned int DataBuffer::getSize() const
{
    return size;
}

bool DataBuffer::writeByte(unsigned char value)
{
    extendIfNeeded(1);
    
    memcpy(&data[offset], &value, 1);
    
    offset += 1;
    size += 1;
    
    return true;
}

bool DataBuffer::writeWord(unsigned short value)
{
    extendIfNeeded(2);
    
    memcpy(&data[offset], &value, 2);
    
    offset += 2;
    size += 2;
    
    return true;
}

bool DataBuffer::writeWord(short value)
{
    extendIfNeeded(2);
    
    memcpy(&data[offset], &value, 2);
    
    offset += 2;
    size += 2;
    
    return true;
}

bool DataBuffer::writeInt(unsigned int value)
{
    extendIfNeeded(4);
    
    memcpy(&data[offset], &value, 4);
    
    offset += 4;
    size += 4;
    
    return true;
}

bool DataBuffer::writeInt(int value)
{
    extendIfNeeded(4);
    
    memcpy(&data[offset], &value, 4);
    
    offset += 4;
    size += 4;
    
    return true;
}

bool DataBuffer::writeLong(unsigned long long value)
{
    extendIfNeeded(8);
    
    memcpy(&data[offset], &value, 8);
    
    offset += 8;
    size += 8;
    
    return true;
}

bool DataBuffer::writeLong(long long value)
{
    extendIfNeeded(8);
    
    memcpy(&data[offset], &value, 8);
    
    offset += 8;
    size += 8;
    
    return true;
}

bool DataBuffer::writeFloat(float value)
{
    extendIfNeeded(4);
    
    memcpy(&data[offset], &value, 4);
    
    offset += 4;
    size += 4;
    
    return true;
}

bool DataBuffer::writeDouble(double value)
{
    extendIfNeeded(8);
    
    memcpy(&data[offset], &value, 8);
    
    offset += 8;
    size += 8;
    
    return true;
}

bool DataBuffer::writeString(const std::string & value)
{
    unsigned short length = value.length();
    
    if(!writeWord(length))
        return false;
    
    if(!writeBytes((void*)&value[0], (unsigned int)value.length()))
        return false;
    
    return true;
}

bool DataBuffer::writeBytes(void * buffer, unsigned int length)
{
    extendIfNeeded(length);
    
    memcpy(&data[offset], buffer, length);
    
    offset += length;
    size += length;
    
    return true;
}

unsigned char DataBuffer::readByte()
{
    if(size - offset < 1)
        return 0;
    
    unsigned char value = data[offset];
    
    offset += 1;
    
    return value;
}

unsigned short DataBuffer::readUnsignedWord()
{
    if(size - offset < 2)
        return 0;
    
    unsigned short value = *((unsigned short*)&data[offset]);
    
    offset += 2;
    
    return value;
}

short DataBuffer::readWord()
{
    if(size - offset < 2)
        return 0;
    
    short value = *((short*)&data[offset]);
    
    offset += 2;
    
    return value;
}

unsigned int DataBuffer::readUnsignedInt()
{
    if(size - offset < 4)
        return 0;
    
    unsigned int value = *((unsigned int*)&data[offset]);
    
    offset += 4;
    
    return value;
}

int DataBuffer::readInt()
{
    if(size - offset < 4)
        return 0;
    
    int value = *((int*)&data[offset]);
    
    offset += 4;
    
    return value;
}

unsigned long long DataBuffer::readUnsignedLong()
{
    if(size - offset < 8)
        return 0;
    
    unsigned long long value = *((unsigned long long*)&data[offset]);
    
    offset += 8;
    
    return value;
}

long long DataBuffer::readLong()
{
    if(size - offset < 8)
        return 0;
    
    long long value = *((long long*)&data[offset]);
    
    offset += 2;
    
    return value;
}

float DataBuffer::readFloat()
{
    if(size - offset < 4)
        return 0;
    
    float value = *((float*)&data[offset]);
    
    offset += 4;
    
    return value;
}

double DataBuffer::readDouble()
{
    if(size - offset < 8)
        return 0;
    
    double value = *((double*)&data[offset]);
    
    offset += 8;
    
    return value;
}

std::string DataBuffer::readString()
{
    unsigned short length = readUnsignedWord();
    
    if(length == 0)
        return "";
    
    char * buffer = new char[length + 1];
    
    if(!readBytes(buffer, length))
    {
        delete [] buffer;
        
        return "";
    }
    
    buffer[length] = 0;
    
    std::string value = buffer;
    
    delete [] buffer;
    
    return value;
}

bool DataBuffer::readBytes(void * buffer, unsigned int length)
{
    if(size - offset < length)
        return false;
    
    memcpy(buffer, &data[offset], length);
    
    offset += length;
    
    return true;
}

void DataBuffer::seekBy(int add_offset)
{
    offset += add_offset;
    if (offset > size) offset = size;
}

void DataBuffer::seekTo(int offset)
{
    if (offset < 0)
        offset = 0;
    
    this->offset = offset;
    this->offset = std::min(this->offset, size);
}

void DataBuffer::reset()
{
    offset = 0;
}

unsigned int DataBuffer::getPosition()
{
    return offset;
}
