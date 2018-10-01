//
//  CapsaInviteUser.hpp
//  DovemobiCasino
//
//  Created by Frank on 11/9/17.
//
//

#ifndef CapsaInviteUser_hpp
#define CapsaInviteUser_hpp

#include "cocos2d.h"
USING_NS_CC;

class CapsaInviteUser: public Ref
{
public:
    CapsaInviteUser();
    ~CapsaInviteUser();
    
    static CapsaInviteUser* create(const char* szPath, int nLevel, bool fLobby);
    
    bool init(const char* szPath, int nLevel, bool fLobby);
    void addToParent(Node* pParent, Vec2 pos);
    void removeFromParent();

public:
    //char    m_szFacePath[0x100];
    int     m_nLevel;
    bool    m_fIsLobby;

private:
    Sprite* m_pSpImage;
    
    Sprite* m_pSpLevel;
    Sprite* m_pSpName;
    
    Label*  m_pLblLevel;
    Label*  m_pLblName;
};

#endif /* CapsaInviteUser_hpp */
