//
//  GameHandler.hpp
//  DovemobiCasino
//
//  Created by liu on 9/7/17.
//
//

#ifndef GameHandler_hpp
#define GameHandler_hpp

#include "cocos2d.h"
#include "CapsaRoom.hpp"
#include "CapsaPlayer.hpp"

USING_NS_CC;
class CardSprite;
class ProcessLayer;
class GameHandler : public cocos2d::Node
{
public:
    static
    GameHandler *   getInstance();
    
    virtual
    bool            init()  override;
    
    void            initAppwarp();
    
#pragma mark Basic
    std::string     getPlayerName();
    void            setPlayerName(const std::string & playerName);
    void            savePlayerName();
    
#pragma mark Process
    void            showProgress(bool);
    void            showProcess(Layer *parent, const std::string & text);
    void            hideProcess();
    
#pragma mark Card
    CardSprite *    getCard(int cardNumber);
    void            resetCards();
    
#pragma mark Room
    void            makeRoom();
    CapsaRoom *     currentRoom();
    bool            isMaster();
    
#pragma mark Connection
    void            setConnected(bool);
    bool            isConnected();
 
#pragma mark Profile
    void            sendPlayerProfile();
    
//////
    int             getSelectedCityIndex();
    void            setSelectedCityIndex(int index);
    
private:
    void            loadSettings();
    void            loadCards();
    
    static
    GameHandler *   m_sharedInstance;
    
    CapsaPlayer     m_player;
    
    __Array *       m_cards;
    
    ProcessLayer *         m_processLayer;
    
    CapsaRoom       m_capsaRoom;
    
    bool            m_initedAppwarp;
    
    bool            m_connected;
    
    int             m_selectedIndex;
};


#endif /* GameHandler_hpp */
