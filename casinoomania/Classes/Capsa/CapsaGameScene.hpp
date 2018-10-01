//
//  CapsaGameScene.hpp
//  DovemobiCasino
//
//  Created by liu on 9/7/17.
//
//

#ifndef CapsaGameScene_hpp
#define CapsaGameScene_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "appwarp.h"
#include "CapsaDefine.h"
#include "CapsaPlayerNode.hpp"
#include "Big2PokerModel.hpp"

class TopBar;
class ProcessLayer;
class CapsaGameScene : public cocos2d::Layer, public AppWarp::ConnectionRequestListener,public AppWarp::RoomRequestListener,public AppWarp::NotificationListener,public AppWarp::ZoneRequestListener, public AppWarp::TurnBasedRoomRequestListener, public AppWarp::LobbyRequestListener
{
public:
    static
    cocos2d::Scene *    createScene();
    
    virtual
    bool                init() override;
    
    CREATE_FUNC(CapsaGameScene);
    
    ~CapsaGameScene();
    
protected:
    virtual
    void                onEnter() override;
    virtual
    void                onExit() override;
    
    bool                onTouchBegan(Touch *touch, Event *unused_event) override;
    void                onTouchMoved(Touch *touch, Event *unused_event) override;
    void                onTouchEnded(Touch *touch, Event *unused_event) override;
    
#pragma mark Delegate Reference
    virtual void onLeaveRoomDone (AppWarp::room revent) override;
    virtual void onConnectDone(int res, int reasonCode) override;
    virtual void onDisconnectDone(int) override;
    virtual void onChatReceived(AppWarp::chat chatevent) override;
    virtual void onUserPaused(std::string user,std::string locId,bool isLobby) override;
    virtual void onUserResumed(std::string user,std::string locId,bool isLobby) override;
    virtual void onGetLiveRoomInfoDone(AppWarp::liveroom revent) override;
    virtual void onUserChangeRoomProperty(AppWarp::room rData, std::string user,std::map<std::string, std::string> properties, std::map<std::string, std::string>lockTable) override;
    
    virtual void onUserJoinedRoom(AppWarp::room rData, std::string user) override;
    virtual void onUserLeftRoom(AppWarp::room rdata, std::string user) override;
    
    virtual void onGameStarted(std::string sender, std::string id, std::string nextTurn) override;
    virtual void onGameStopped(std::string sender, std::string id) override;
    virtual void onStartGameDone(int res) override;
    virtual void onMoveCompleted(AppWarp::move event) override;
    virtual void onStopGameDone(int res) override;
    virtual void onSendMoveDone(int res) override{}
    virtual void onGetMoveHistoryDone(int res, std::vector<AppWarp::move> history) override;
    virtual void onSetNextTurnDone(int res) override{}
    virtual void onGetLiveUserInfoDone (AppWarp::liveuser uevent) override;
    
    virtual void onGetLiveLobbyInfoDone(AppWarp::livelobby levent) override;
    void scheduleRecover();
    void unscheduleRecover();
    void recover(float dt);
    
    void                restartSchedule(float dt);

private:
    void                setupUI();
    void                setupAppwarp();
    void                uninstallAppwarp();
    
    void                requestRoomInfo();
    void                updateGameRoom();
    
    void                initPlayerIndex();
    void                updatePlayerIndex();
    
    void                dealCards();
    
    void                sendChat(const std::string & message);
    
    void                deliverSpotForPlayer(const std::string & player);
    void                removeSpotForPlayer(const std::string & player);
    
    void                updatePlayerSpots();
    void                updatePlayers();
    void                updatePlayerInfo(AppWarp::liveuser uevent);
    void                updateCards();
    void                updateThrownCard(int playerIndex);
    
    void                startGame();
    void                resetGame();
    
    void                getFirstPlayer();
    
    void                sortCardArray(cocos2d::__Array *arrCard);
    
    void                showCards();
    void                removePlayerCards(int);
    void                removePlayerThrownCards(const std::string & player, int index = -1);
    
    void                setPlayerTurn(int);
    void                showButtonPanel(bool);
    void                updateButtonPanel();
    
    void                setupTouch(bool);
    
    void                selectCardByTouchPos(cocos2d::Vec2 pos);
    
    void                setPlayerTurnByNextTurn(const std::string nextTurn);
    
    void                updatePlayerCard(const std::string &, __Array *, __Array *);
    void                updateCard(int playerIndex);
    
    void                clearThrownCards(bool fromScreen = false);
    
    void                sendUpdatedRoom();
    
    void                onInvitePlayer(Ref *);
    
    void                goBackLobby();
    
    std::string         getNextTurn();
    
    void                showResultDialog();
    void                hideResultDialog();
    
    void                leaveRoom();
    
    void                updateAvailableCardPairs();
    
    TopBar *            m_topBar;
    ProcessLayer *      m_processLayer;
    cocos2d::Sprite *   m_background;
    
    Layer *             m_cardLayer;
    cocos2d::__Array *  m_cards[MAX_PLAYERS];
    cocos2d::__Array *  m_thrownCards[MAX_PLAYERS];
    Layer *             m_buttonLayer;
    ui::Button *        m_btnPass;
    ui::Button *        m_btnGo;
    ui::Button *        m_btnReselect;
    ui::Button *        m_btnHint;
    
    bool                m_gameStarted;
    bool                m_gameEnded;
    
    bool                m_isMyTurn;
    
    int                 m_playerIndex;
    CapsaPlayerNode *   m_playerNode[MAX_PLAYERS];
    ui::Button *        m_inviteButton[MAX_PLAYERS];
    
    int                 m_countDownRestart;
    
    bool                m_isCheckedLeaveRoom;
    
    Layer *             m_resultLayer;
    
    bool                m_needToUpdateUserProfile;
    
    std::string         m_winner;
    
    __Array *           m_arrCardPairs;
    Layer *             m_cardPairLayer;
    int                 m_cardPairCounter[Big2PokerModel::PairTypeCount];
    
    bool                m_hasCardToWin;
    
    bool                m_dealedCards[MAX_PLAYERS];
    int                 m_currentTurnIndex;
    
    bool                m_iPad;
    
private:
#pragma event Handler
    void                onBackButtonPressed();
    
    void                onGo();
    void                onPass();
    void                onHint();
    void                onReselect();
    
    void                onSelectCardPair(Ref *);
};

#endif /* CapsaGameScene_hpp */
