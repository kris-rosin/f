//
//  CapsaRoom.cpp
//  DovemobiCasino
//
//  Created by Admin on 10/7/17.
//
//

#include "CapsaRoom.hpp"

#include "App42API.h"

std::string CapsaRoom::generateRandomRoomID()
{
    std::string charStr;
    srand ((unsigned int)time(NULL));
    
    for (int i = 0; i < 10; ++i) {
        charStr += (char)(65+(rand() % (26)));
    }
    
    return charStr;
}

CapsaRoom::CapsaRoom()
{
    roomID = "";
    init();
}

void CapsaRoom::init()
{
    master = "";
    maxTime = 0.0f;
    
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        players[i] = "";
        cards[i].clear();
        thrownCards[i].clear();
    }
    
    isPrivate = false;
}

void CapsaRoom::toMap(std::map<std::string, std::string> & properties)
{
    properties[MASTER_KEY] = master;
    
    std::stringstream ssMaxPlayer;
    ssMaxPlayer << maxPlayers;
    properties[MAX_PLAYERS_KEY] = ssMaxPlayer.str();
    
    std::stringstream ssDeposite;
    ssDeposite << deposite;
    properties[DEPOSITE_KEY] = ssDeposite.str();
    
    properties[PRIVATE_KEY] = isPrivate ? "1" : "0";
    
    std::stringstream ssMaxTime;
    ssMaxTime << maxTime;
    properties[MAX_TIME_KEY] = ssMaxTime.str();
    
    cJSON *jsonPlayers = cJSON_CreateObject();
    cJSON *jsonCards = cJSON_CreateObject();
    cJSON *jsonThrownCards = cJSON_CreateObject();
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        std::stringstream ss;
        ss << i;
        
        const char *key = ss.str().c_str();
        cJSON_AddStringToObject(jsonPlayers, key, players[i].c_str());
        
        int pCards[MAX_PLAYER_CARDS];
        for (int j = 0; j < cards[i].size(); j++)
            pCards[j] = cards[i].at(j);
        cJSON *pJsonCard = cJSON_CreateIntArray(pCards, (int)cards[i].size());
        cJSON_AddItemToObject(jsonCards, key, pJsonCard);
        
        int pThrownCards[MAX_DRAW_CARDS];
        for (int j = 0; j < thrownCards[i].size(); j++)
            pThrownCards[j] = thrownCards[i].at(j);
        cJSON *pJsonThrownCard = cJSON_CreateIntArray(pThrownCards, (int)thrownCards[i].size());
        cJSON_AddItemToObject(jsonThrownCards, key, pJsonThrownCard);
    }
    properties[PLAYERS_KEY] = cJSON_Print(jsonPlayers);
    properties[CARDS_KEY] = cJSON_Print(jsonCards);
    properties[THROWN_CARDS_KEY] = cJSON_Print(jsonThrownCards);
    cJSON_Delete(jsonPlayers);
    cJSON_Delete(jsonCards);
    cJSON_Delete(jsonThrownCards);
}

void CapsaRoom::fromMap(const std::map<std::string, std::string> &properties)
{
    init();
    
    if (!properties.size())
        return;
    
    master = properties.at(MASTER_KEY);
    std::string szMaxPlayers = properties.at(MAX_PLAYERS_KEY);
    std::stringstream ssMaxPlayers;
    ssMaxPlayers << szMaxPlayers;
    ssMaxPlayers >> maxPlayers;
    
    std::string szDeposite = properties.at(DEPOSITE_KEY);
    std::stringstream ssDeposite;
    ssDeposite << szDeposite;
    ssDeposite >> deposite;
    
    std::string szPrivate = properties.at(PRIVATE_KEY);
    isPrivate =  !szPrivate.compare("1");
    
    std::string szMaxTime = properties.at(MAX_TIME_KEY);
    std::stringstream ssMaxTime;
    ssMaxTime << szMaxTime;
    ssMaxTime >> maxTime;
    
    cJSON *jsonPlayers = cJSON_Parse(properties.at(PLAYERS_KEY).c_str());
    cJSON *jsonCards = cJSON_Parse(properties.at(CARDS_KEY).c_str());
    cJSON *jsonThrownCards = cJSON_Parse(properties.at(THROWN_CARDS_KEY).c_str());
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        std::stringstream ss;
        ss << i;
        
        const char *key = ss.str().c_str();
        cJSON *jsonPlayer = cJSON_GetObjectItem(jsonPlayers, key);
        players[i] = jsonPlayer->valuestring;
        
        cJSON *jsonCard = cJSON_GetObjectItem(jsonCards, key);
        int nCards = cJSON_GetArraySize(jsonCard);
        for (int j = 0; j < nCards; j++)
        {
            cJSON *jsonOneCard = cJSON_GetArrayItem(jsonCard, j);
            cards[i].push_back(jsonOneCard->valueint);
        }
        
        cJSON *jsonThrownCard = cJSON_GetObjectItem(jsonThrownCards, key);
        int nThrownCards = cJSON_GetArraySize(jsonThrownCard);
        for (int j = 0; j < nThrownCards; j++)
        {
            cJSON *jsonOneCard = cJSON_GetArrayItem(jsonThrownCard, j);
            thrownCards[i].push_back(jsonOneCard->valueint);
        }
    }
    cJSON_Delete(jsonPlayers);
    cJSON_Delete(jsonCards);
    cJSON_Delete(jsonThrownCards);
}

bool CapsaRoom::isFull()
{
    int playerCount = 0;
    for (int i = 0; i < this->maxPlayers; i++)
    {
        if (players[i].length())
            playerCount++;
    }
    
    return this->maxPlayers == playerCount;
}

void CapsaRoom::removeUser(const std::string &user)
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!players[i].compare(user))
            players[i] = "";
    }
}

void CapsaRoom::stripNonUsers(const std::vector<std::string> &users)
{
    return;
    /*
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!players[i].length())
            continue;
        
        bool isExistUser = false;
        for (int j = 0; j < users.size(); j++)
        {
            if (!players[i].compare(users.at(j)))
            {
                isExistUser = true;
                break;
            }
        }
        
        if (!isExistUser)
            players[i] = "";
    }
    */
}

bool CapsaRoom::isSelectedNewMaster()
{
    bool hasLeftMaster = true;
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!players[i].length())
            continue;
        
        if (!players[i].compare(master))
        {
            hasLeftMaster = false;
            break;
        }
    }
    
    // if mater is left, compare playername by a-z and select one by min value
    if (hasLeftMaster)
    {
        std::string newMaster = "";
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if (!players[i].length())
                continue;
            
            if (!newMaster.length())
            {
                newMaster = players[i];
                continue;
            }
            
            if (players[i].compare(newMaster) < 0)
            {
                newMaster = players[i];
            }
        }
    }
    
    return hasLeftMaster;
}

int CapsaRoom::getPlayerCount()
{
    int nPlayers = 0;
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!players[i].length())
            continue;
        
        nPlayers++;
    }
    
    return nPlayers;
}

int CapsaRoom::getPlayerIndex(const std::string &user)
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!user.compare(players[i]))
        {
            return i;
            break;
        }
    }
    return -1;
}
