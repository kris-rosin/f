//
//  CapsaInviteUserManager.cpp
//  DovemobiCasino
//
//  Created by Frank on 11/9/17.
//
//

#include "CapsaInviteUserManager.hpp"
#include "CapsaInviteUser.hpp"

static CapsaInviteUserManager* _sharedInviteUserManager = NULL;

CapsaInviteUserManager* CapsaInviteUserManager::shard()
{
    if (_sharedInviteUserManager == NULL)
        _sharedInviteUserManager = new CapsaInviteUserManager();
    
    return _sharedInviteUserManager;
}


CapsaInviteUserManager::CapsaInviteUserManager()
{
    m_pVecUsers = new Vector<CapsaInviteUser*>;
}

CapsaInviteUserManager::~CapsaInviteUserManager()
{
    if (m_pVecUsers != NULL)
    {
        m_pVecUsers->clear();
        
        delete m_pVecUsers;
        m_pVecUsers = NULL;
    }
}

void CapsaInviteUserManager::generateInviteUsers()
{
    for (int i = 0; i < 50; i++)
    {
        CapsaInviteUser* pUser = CapsaInviteUser::create(NULL, i, ((i%2) == 0));
        m_pVecUsers->pushBack(pUser);
    }
}

void CapsaInviteUserManager::resetInviteUsers()
{
    for (int i = (int)(m_pVecUsers->size())-1; i >= 0; i--)
    {
        CapsaInviteUser* pUser = (CapsaInviteUser*)m_pVecUsers->at(i);
        if (pUser != NULL)
            pUser->removeFromParent();
    }
    m_pVecUsers->clear();
}

int CapsaInviteUserManager::getLobbyUsers()
{
    int nUsers = 0;
    for (int i = 0; i < m_pVecUsers->size(); i++)
    {
        CapsaInviteUser* pUser = (CapsaInviteUser*)m_pVecUsers->at(i);
        if (pUser != NULL && !pUser->m_fIsLobby)
        {
            nUsers += 1;
        }
    }
    return nUsers;
}

int CapsaInviteUserManager::getFriendUsers()
{
    int nUsers = 0;
    for (int i = 0; i < m_pVecUsers->size(); i++)
    {
        CapsaInviteUser* pUser = (CapsaInviteUser*)m_pVecUsers->at(i);
        if (pUser != NULL && pUser->m_fIsLobby)
        {
            nUsers += 1;
        }
    }
    return nUsers;
}

void CapsaInviteUserManager::addToParent(Node* pParent, bool fFlag, float rY)
{
    int nUser = 0;
    
    for (int i = 0; i < m_pVecUsers->size(); i++)
    {
        CapsaInviteUser* pUser = (CapsaInviteUser*)m_pVecUsers->at(i);
        if (pUser != NULL && pUser->m_fIsLobby == fFlag)
        {
            Vec2 pos(170 * ((nUser % 4) + 1) - 65, rY - 130 * (nUser / 4 + 1) + 70);
            
            pUser->addToParent(pParent, pos);
            
            nUser += 1;
        }
    }
}
