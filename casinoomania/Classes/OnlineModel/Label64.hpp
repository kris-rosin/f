//
//  Label64.hpp
//  2pb2
//
//  Created by liu on 6/30/17.
//
//

#ifndef Label64_hpp
#define Label64_hpp

#include "cocos2d.h"
#include "app42base64.h"

USING_NS_CC;

bool        isAscii(const char * szString);
void        getLocalString(const char *key, char *buf);

Label *     createLabelWith64String(const std::string & string, float fontSize);
Label *     createLabelWithMultiString(const std::string & string, float fontSize);

std::string &ltrim(std::string &s);

// trim from end
std::string &rtrim(std::string &s);

// trim from both ends
std::string &trim(std::string &s) ;

bool replace(std::string& str, const std::string& from, const std::string& to);

#endif /* Label64_hpp */
