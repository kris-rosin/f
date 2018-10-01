//
//  CapsaRoom.hpp
//  DovemobiCasino
//
//  Created by Admin on 10/7/17.
//
//

#ifndef CapsaRoom_hpp
#define CapsaRoom_hpp

#include <stdio.h>
#include <vector>

#include "CapsaDefine.h"
#include "CapsaPlayer.hpp"

class CapsaRoom
{
public:
    static
    std::string         generateRandomRoomID();
    
public:
    CapsaRoom();
    void                init();
    
    void                toMap(std::map<std::string, std::string> & properties);
    void                fromMap(const std::map<std::string, std::string> & properties);
    
    bool                isFull();
    
    void                removeUser(const std::string & user);
    
    void                stripNonUsers(const std::vector<std::string> & users);
    
    bool                isSelectedNewMaster();
    
    int                 getPlayerCount();
    int                 getPlayerIndex(const std::string & user);
    
    std::string         master;
    std::string         players[MAX_PLAYERS];
    std::vector<int>    cards[MAX_PLAYERS];
    std::vector<int>    thrownCards[MAX_PLAYERS];
    
    CapsaPlayer         gamePlayers[MAX_PLAYERS];
    
    unsigned int        deposite;
    unsigned int        maxPlayers;
    float               maxTime;
    
    bool                isPrivate;
    
    std::string         roomID;
    
};

#endif /* CapsaRoom_hpp */
