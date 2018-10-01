//
//  CapsaInviteUser.cpp
//  DovemobiCasino
//
//  Created by Frank on 11/9/17.
//
//

#include "CapsaInviteUser.hpp"

CapsaInviteUser::CapsaInviteUser()
{
    m_pSpImage = NULL;
    m_pSpLevel = NULL;
    m_pSpName = NULL;
    
    m_pLblLevel = NULL;
    m_pLblName = NULL;
    
    //strcpy(m_szFacePath, "ui/Capsa/invite/capsa_invite_pop_user_img.png");
    m_nLevel = 1;
    m_fIsLobby = true;
}

CapsaInviteUser::~CapsaInviteUser()
{
    
}

CapsaInviteUser* CapsaInviteUser::create(const char* szPath, int nLevel, bool fLobby)
{
    CapsaInviteUser* pUser = new CapsaInviteUser();
    pUser->init(szPath, nLevel, fLobby);
    pUser->autorelease();
    return pUser;
}

bool CapsaInviteUser::init(const char* szPath, int nLevel, bool fLobby)
{
    m_nLevel = nLevel;
    m_fIsLobby = (nLevel % 2 == 1) ? true : false;
    
//    if (szPath != NULL)
//        strcpy(m_szFacePath, szPath);
    
    return true;
}

void CapsaInviteUser::addToParent(cocos2d::Node *pParent, Vec2 pos)
{
    m_pSpImage = Sprite::create("ui/Capsa/invite/capsa_invite_pop_user_img.png");
    
    m_pSpName = Sprite::create("ui/Capsa/invite/capsa_invite_pop_user_name.png");
    m_pSpLevel = Sprite::create("ui/Capsa/invite/capsa_invite_pop_user_lvl_lab.png");
    
    const char* szName = __String::createWithFormat("name %d", m_nLevel)->getCString();
    
    m_pLblName = Label::createWithTTF(szName, "fonts/arial.ttf", 20);
    m_pLblName->setColor(Color3B::WHITE);
    
    szName = __String::createWithFormat("Lv.%d", m_nLevel)->getCString();
    m_pLblLevel = Label::createWithTTF(szName, "fonts/arial.ttf", 14);
    m_pLblLevel->setColor(Color3B::BLACK);
    
    if (m_pSpImage == NULL)
    {
        m_pSpImage->setPosition(pos);
        pParent->addChild(m_pSpImage);
    }
    
    if (m_pSpName != NULL)
    {
        m_pSpName->setPosition(pos.x, pos.y - 49);
        pParent->addChild(m_pSpName);
    }
    
    if (m_pSpLevel != NULL)
    {
        m_pSpLevel->setPosition(pos.x + 55, pos.y);
        pParent->addChild(m_pSpLevel);
    }
    
    if (m_pLblLevel != NULL)
    {
        m_pLblLevel->setPosition(pos.x + 55, pos.y);
        pParent->addChild(m_pLblLevel);
    }
    
    if (m_pLblName != NULL)
    {
        m_pLblName->setPosition(pos.x, pos.y - 49);
        pParent->addChild(m_pLblName);
    }
}

void CapsaInviteUser::removeFromParent()
{
    if (m_pSpImage != NULL)
    {
        m_pSpImage->removeFromParentAndCleanup(true);
        m_pSpImage = NULL;
    }
        
    if (m_pSpLevel != NULL)
    {
        m_pSpLevel->removeFromParent();
        m_pSpLevel = NULL;
    }
    
    if (m_pSpName != NULL)
    {
        m_pSpName->removeFromParent();
        m_pSpName = NULL;
    }
    
    if (m_pLblLevel != NULL)
    {
        m_pLblLevel->removeFromParent();
        m_pLblLevel = NULL;
    }
    
    if (m_pLblName != NULL)
    {
        m_pLblName->removeFromParent();
        m_pLblName = NULL;
    }
}

