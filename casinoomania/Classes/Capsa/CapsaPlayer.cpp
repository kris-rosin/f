//
//  CapsaPlayer.cpp
//  DovemobiCasino
//
//  Created by Admin on 10/14/17.
//
//

#include "CapsaPlayer.hpp"

#include "cJSON.h"

CapsaPlayer::CapsaPlayer()
{
    level = 0;
    gold = 0;
    pictureURL = "";
    username = "";
}

void CapsaPlayer::init(const PlayerProfile & profile)
{
    level = profile.getLevel();
    gold = (int)profile.getCoins();
    pictureURL = profile.getFBUserPictureURL();
}

std::string CapsaPlayer::toJSONString()
{
    cJSON *jsonProfile = cJSON_CreateObject();
    cJSON_AddNumberToObject(jsonProfile, "level", level);
    cJSON_AddNumberToObject(jsonProfile, "gold", gold);
    cJSON_AddStringToObject(jsonProfile, "url", pictureURL.c_str());
    const char *jsonString = cJSON_Print(jsonProfile);
    cJSON_Delete(jsonProfile);
    return jsonString;
}

void CapsaPlayer::fromJSONString(const std::string & jsonString)
{
    cJSON * jsonProfile = cJSON_Parse(jsonString.c_str());
    cJSON *jsonLevel = cJSON_GetObjectItem(jsonProfile, "level");
    cJSON *jsonGold = cJSON_GetObjectItem(jsonProfile, "gold");
    cJSON *jsonPictureURL = cJSON_GetObjectItem(jsonProfile, "url");
    level = jsonLevel->valueint;
    gold = jsonGold->valueint;
    pictureURL = jsonPictureURL->valuestring;
    cJSON_Delete(jsonProfile);
}
