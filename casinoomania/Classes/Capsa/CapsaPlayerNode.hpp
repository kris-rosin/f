//
//  CapsaPlayerNode.hpp
//  DovemobiCasino
//
//  Created by liu on 9/21/17.
//
//

#ifndef CapsaPlayerNode_hpp
#define CapsaPlayerNode_hpp

#include "cocos2d.h"

#include "network/HttpClient.h"
using namespace cocos2d::network;

USING_NS_CC;

class CapsaPlayer;
class CapsaPlayerNode : public Sprite
{
public:
    virtual
    bool            init() override;

    void            setPlayerName(const std::string & playerName);
    std::string     getPlayerName();
    
    CREATE_FUNC(CapsaPlayerNode);
    
    void            setActiveTurn(bool);
    
    void            setMaxTime(float);
    
    void            showPlayerInfo(bool visible);

    void            updatePlayer(const CapsaPlayer & player, bool onlyPicture = false);
    
    void            loadFromURL(const char *url);
    void            onLoadedFromURL(HttpClient *sender, HttpResponse *response);
    
private:
    void            updateTurnTimer(bool);
    void            onTimer(float dt);
    
    void            adjustChildSprite(Sprite *);
    
    std::string     m_playerName;
    
    Label *         m_lblPlayerName;
    Sprite *        m_spTurn;
    
    float           m_maxTime;
    float           m_time;
    ProgressTimer * m_progressTimer;
    
    Label *         m_lblLevel;
    Label *         m_lblGold;
    Node *          m_goldLevelNode;
    
    bool            m_loadedPicture;
    Node *          m_child;
};

#endif /* CapsaPlayerNode_hpp */
