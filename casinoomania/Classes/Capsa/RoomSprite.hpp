//
//  RoomSprite.hpp
//  DovemobiCasino
//
//  Created by Admin on 10/7/17.
//
//

#ifndef RoomSprite_hpp
#define RoomSprite_hpp

#include "cocos2d.h"
USING_NS_CC;

#include "CapsaRoom.hpp"
#include "CapsaDefine.h"

#include "ui/CocosGUI.h"

class CapsaPlayerNode;
class RoomSprite : public cocos2d::Sprite
{
public:
    static
    RoomSprite *    create();
    
    CapsaRoom *     room();
    
    void            updateRoom();
    
    ui::Button *    getPlayButton();
    
    CapsaPlayerNode *   getPlayerNode(int);
    
private:
    CapsaRoom           m_capsaRoom;
    
    CapsaPlayerNode *   m_playerNode[MAX_PLAYERS];
    
    ui::Button *        m_btnPlay;
    Label *             m_lblDeposite;
    
    Sprite *            m_spMaxPlayers;
    
    void                setup();
};


#endif /* RoomSprite_hpp */
