//
//  Label64.cpp
//  2pb2
//
//  Created by liu on 6/30/17.
//
//

#include "Label64.hpp"

#include "App42Define.h"
#include "CapsaDefine.h"

bool isAscii(const char * szString)
{
    size_t nLen = strlen(szString);
    for (int i = 0; i < nLen; i++)
    {
        if (szString[i] < 0)
        {
            return false;
        }
    }
    return true;
}

void getLocalString(const char *key, char *buf)
{
    /*
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    iOSHelper::localizationString(key, buf);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    GameDocument::getInstance()->getAndroidLocalString(key, buf);
#endif
     */
    strcpy(buf, key);
}

Label *     createLabelWith64String(const std::string & string, float fontSize)
{
    std::string decodeString = base64_decode(string);
    if (!decodeString.length())
        decodeString = " ";
    if (IS_CONTAIN_FBUSER_PRE(decodeString))
        decodeString = decodeString.substr(0, FBUSER_PRE_POS(decodeString));
    return createLabelWithMultiString(decodeString, fontSize);
}

Label *     createLabelWithMultiString(const std::string & string, float fontSize)
{
    const char *szString = string.c_str();
    
    bool ascii = isAscii(szString);
    
    Label *label = NULL;
    if (ascii)
        label = Label::createWithTTF(string, MAIN_FONT_FILE, fontSize);
    else
        label = Label::createWithSystemFont(string, ARIAL_FONT, fontSize);
        
    return label;
}

std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                    std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}


