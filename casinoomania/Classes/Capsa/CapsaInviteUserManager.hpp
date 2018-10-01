//
//  CapsaInviteUserManager.hpp
//  DovemobiCasino
//
//  Created by Frank on 11/9/17.
//
//

#ifndef CapsaInviteUserManager_hpp
#define CapsaInviteUserManager_hpp

#include "cocos2d.h"

USING_NS_CC;

class CapsaInviteUser;

class CapsaInviteUserManager
{
public:
    static CapsaInviteUserManager* shard();
    
    CapsaInviteUserManager();
    ~CapsaInviteUserManager();
    
    void    generateInviteUsers();
    void    resetInviteUsers();
    
    int     getLobbyUsers();
    int     getFriendUsers();
    
    void    addToParent(Node* pParent, bool fFlag, float rY);
    
private:
    Vector<CapsaInviteUser*>* m_pVecUsers;
    
};

#endif /* CapsaInviteUserManager_hpp */
