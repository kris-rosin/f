//
//  CapsaPlayer.hpp
//  DovemobiCasino
//
//  Created by Admin on 10/14/17.
//
//

#ifndef CapsaPlayer_hpp
#define CapsaPlayer_hpp

#include <stdio.h>

#include "PlayerProfile.h"
class CapsaPlayer
{
public:
    CapsaPlayer();
    void            init(const PlayerProfile &);
    
    std::string     username;
    
    int             level;
    int             gold;
    
    std::string     pictureURL;
    
    std::string     toJSONString();
    void            fromJSONString(const std::string &);
};

#endif /* CapsaPlayer_hpp */
