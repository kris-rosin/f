//
//  RoomSprite.cpp
//  DovemobiCasino
//
//  Created by Admin on 10/7/17.
//
//

#include "RoomSprite.hpp"

#include "CapsaPlayerNode.hpp"
#include "Label64.hpp"

RoomSprite* RoomSprite::create()
{
    RoomSprite *roomSprite = new (std::nothrow) RoomSprite();
    if (roomSprite && roomSprite->initWithFile("ui/Capsa/capsalobby_circle.png"))
    {
        roomSprite->setup();
        return roomSprite;
    }
    CC_SAFE_DELETE(roomSprite);
    return nullptr;
}

CapsaRoom * RoomSprite::room()
{
    return &m_capsaRoom;
}

void RoomSprite::setup()
{
    m_btnPlay = ui::Button::create("ui/Capsa/capsalobby_play.png");
    addChild(m_btnPlay);
    m_btnPlay->setPosition(POS_SIZE(getContentSize(), Vec2(0.5f, 0.6f)));
    
    float angle[] = { M_PI * 0.9f, M_PI * 0.5f, M_PI * 0.1f, - M_PI * 0.5f };
    float r = getContentSize().width * 0.5f;
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        m_playerNode[i] = CapsaPlayerNode::create();
        addChild(m_playerNode[i]);
        
        Vec2 pos;
        pos.x = r * cosf(angle[i]);
        pos.y = r * sinf(angle[i]);
        
        Vec2 nodePos = Vec2(getContentSize() * 0.5f) + pos;
        if (i == MAX_PLAYERS - 1)
            nodePos.y += getContentSize().height * 0.4f;
        
        m_playerNode[i]->setPosition(nodePos);
    }
}

void RoomSprite::updateRoom()
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!m_capsaRoom.players[i].length())
        {
            m_playerNode[i]->setVisible(false);
            continue;
        }
        
        m_playerNode[i]->setPlayerName(m_capsaRoom.players[i]);
    }
    
    if (m_spMaxPlayers)
        m_spMaxPlayers->removeFromParentAndCleanup(true);
    
    char szFileName[64];
    sprintf(szFileName, "ui/Capsa/%d.png", m_capsaRoom.maxPlayers);
    m_spMaxPlayers = Sprite::create(szFileName);
    m_btnPlay->addChild(m_spMaxPlayers);
    m_spMaxPlayers->setPosition(POS_SIZE(m_btnPlay->getContentSize(), Vec2(0.5f, 0.75f)));
    
    m_btnPlay->setEnabled(!m_capsaRoom.isFull());
}

ui::Button * RoomSprite::getPlayButton()
{
    return m_btnPlay;
}

CapsaPlayerNode * RoomSprite::getPlayerNode(int playerIndex)
{
    return m_playerNode[playerIndex];
}
