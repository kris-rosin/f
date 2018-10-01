//
//  CapsaGameScene.cpp
//  DovemobiCasino
//
//  Created by liu on 9/7/17.
//
//

#include "CapsaGameScene.hpp"

#include "App42Define.h"
#include "GameHandler.hpp"
#include "ProcessLayer.hpp"
#include "TopBar.h"
#include "CapsaPlayerNode.hpp"
#include "CardSprite.hpp"
#include "LoadingSplash.h"

USING_NS_CC;

Vec2 gCardPosRatio[MAX_PLAYERS] =
{
    Vec2(0.5f, 0.2f),
    Vec2(0.06f, 0.6f),
    Vec2(0.5f, 0.8f),
    Vec2(0.94f, 0.6f)
};

Vec2 gCardPosRatio_3[MAX_PLAYERS] =
{
    Vec2(0.5f, 0.2f),
    Vec2(0.06f, 0.6f),
    Vec2(0.94f, 0.6f),
    Vec2(0.5f, 0.8f),
};

Vec2 gCardPosRatio_2[MAX_PLAYERS] =
{
    Vec2(0.5f, 0.2f),
    Vec2(0.5f, 0.8f),
    Vec2(0.06f, 0.6f),
    Vec2(0.94f, 0.6f)
};

Vec2 gCardPosRatioForThrown[MAX_PLAYERS] =
{
    Vec2(0.5f, 0.45f),
    Vec2(0.25f, 0.6f),
    Vec2(0.5f, 0.6f),
    Vec2(0.75f, 0.6f)
};

Vec2 gCardPosRatioForThrown_3[MAX_PLAYERS] =
{
    Vec2(0.5f, 0.45f),
    Vec2(0.25f, 0.6f),
    Vec2(0.75f, 0.6f),
    Vec2(0.5f, 0.6f),
};

Vec2 gCardPosRatioForThrown_2[MAX_PLAYERS] =
{
    Vec2(0.5f, 0.45f),
    Vec2(0.5f, 0.6f),
    Vec2(0.25f, 0.6f),
    Vec2(0.75f, 0.6f)
};

Vec2 gCardPosRatioForIPad[MAX_PLAYERS] =
{
    Vec2(0.5f, 0.2f),
    Vec2(0.2f, 0.6f),
    Vec2(0.5f, 0.8f),
    Vec2(0.8f, 0.6f)
};

Vec2 gCardPosRatioForIPad_3[MAX_PLAYERS] =
{
    Vec2(0.5f, 0.2f),
    Vec2(0.2f, 0.6f),
    Vec2(0.8f, 0.6f),
    Vec2(0.5f, 0.8f),
};

Vec2 gCardPosRatioForIPad_2[MAX_PLAYERS] =
{
    Vec2(0.5f, 0.2f),
    Vec2(0.5f, 0.8f),
    Vec2(0.2f, 0.6f),
    Vec2(0.8f, 0.6f)
};

Vec2 gCardPosRatioForThrownForIPad[MAX_PLAYERS] =
{
    Vec2(0.5f, 0.45f),
    Vec2(0.33f, 0.6f),
    Vec2(0.5f, 0.6f),
    Vec2(0.66f, 0.6f)
};

Vec2 gCardPosRatioForThrownForIPad_3[MAX_PLAYERS] =
{
    Vec2(0.5f, 0.45f),
    Vec2(0.33f, 0.6f),
    Vec2(0.66f, 0.6f),
    Vec2(0.5f, 0.6f),
};

Vec2 gCardPosRatioForThrownForIPad_2[MAX_PLAYERS] =
{
    Vec2(0.5f, 0.45f),
    Vec2(0.5f, 0.6f),
    Vec2(0.33f, 0.6f),
    Vec2(0.66f, 0.6f)
};

Vec2 gPlayerAnchorPoint[MAX_PLAYERS] =
{
    Vec2(0.5f, 0.1f),
    Vec2(0.125f, 0.6f),
    Vec2(0.5f, 0.75f),
    Vec2(0.875f, 0.6f)
};

Vec2 gPlayerAnchorPoint_3[MAX_PLAYERS] =
{
    Vec2(0.5f, 0.1f),
    Vec2(0.125f, 0.6f),
    Vec2(0.875f, 0.6f),
    Vec2(0.5f, 0.75f),
};

Vec2 gPlayerAnchorPoint_2[MAX_PLAYERS] =
{
    Vec2(0.5f, 0.1f),
    Vec2(0.5f, 0.75f),
    Vec2(0.125f, 0.6f),
    Vec2(0.875f, 0.6f)
};

Vec2 gPlayerAnchorPointForIPad[MAX_PLAYERS] =
{
    Vec2(0.5f, 0.1f),
    Vec2(0.2f, 0.6f),
    Vec2(0.5f, 0.75f),
    Vec2(0.8f, 0.6f)
};

Vec2 gPlayerAnchorPointForIPad_3[MAX_PLAYERS] =
{
    Vec2(0.5f, 0.1f),
    Vec2(0.5f, 0.75f),
    Vec2(0.8f, 0.6f),
    Vec2(0.2f, 0.6f),
};

Vec2 gPlayerAnchorPointForIPad_2[MAX_PLAYERS] =
{
    Vec2(0.5f, 0.1f),
    Vec2(0.5f, 0.75f),
    Vec2(0.8f, 0.6f),
    Vec2(0.2f, 0.6f),
};

Scene * CapsaGameScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = CapsaGameScene::create();
    scene->addChild(layer);
    
    return scene;
}

bool CapsaGameScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    setupUI();
    setupAppwarp();
    
    initPlayerIndex();
    
    if (GameHandler::getInstance()->isMaster())
        updateGameRoom();
    
    return true;
}

void CapsaGameScene::initPlayerIndex()
{
    m_playerIndex = -1;
    
    updatePlayerIndex();
}

void CapsaGameScene::updatePlayerIndex()
{
    if (m_playerIndex >= 0)
        return;
    
    GameHandler *handler = GameHandler::getInstance();
    m_playerIndex = handler->currentRoom()->getPlayerIndex(handler->getPlayerName());
    if (m_playerIndex >= 0)
        m_needToUpdateUserProfile = true;
}

void CapsaGameScene::setupUI()
{
    Size viewSize = Director::getInstance()->getOpenGLView()->getFrameSize();
    float weightHeightRatio = viewSize.width / viewSize.height;
    m_iPad = fabs(weightHeightRatio - 4 / 3.0f) < fabs(weightHeightRatio - 16 / 9.0f);
    m_background = Sprite::create("ui/Capsa/capsalobby_bg.png");
    addChild(m_background);
    m_background->setPosition(WIN_SIZE * 0.5f);
    
    m_cardLayer = Layer::create();
    addChild(m_cardLayer);
    
    m_cardPairLayer = Layer::create();
    addChild(m_cardPairLayer);
    
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        m_cards[i] = __Array::create();
        m_cards[i]->retain();
        m_thrownCards[i] = __Array::create();
        m_thrownCards[i]->retain();
    }
    m_arrCardPairs = __Array::create();
    m_arrCardPairs->retain();
    
    //topbar
    m_topBar = TopBar::create(std::bind(&CapsaGameScene::onBackButtonPressed, this));
    m_topBar->setPosition(WIN_SIZE.width * 0.5f, WIN_SIZE.height);
    addChild(m_topBar, INT_MAX);
    
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        m_playerNode[i] = CapsaPlayerNode::create();
        addChild(m_playerNode[i]);
        m_playerNode[i]->setVisible(false);
        
        m_inviteButton[i] = ui::Button::create("ui/Capsa/invite.png");
        m_inviteButton[i]->setTag(i);
        addChild(m_inviteButton[i]);
        m_inviteButton[i]->setVisible(false);
        m_inviteButton[i]->addClickEventListener(CC_CALLBACK_1(CapsaGameScene::onInvitePlayer, this));
    }
    
    updatePlayerSpots();
    
    // buttons
    m_btnReselect = ui::Button::create("ui/Capsa/btnClear.png");
    addChild(m_btnReselect);
    m_btnReselect->setPosition(POS_SIZE(WIN_SIZE, Vec2(m_iPad ? 0.2f : 0.1f, 0.1f)));
    m_btnReselect->addClickEventListener(CC_CALLBACK_0(CapsaGameScene::onReselect, this));
    m_btnReselect->setEnabled(false);
    
    m_buttonLayer = Layer::create();
    addChild(m_buttonLayer);
    
    m_btnGo = ui::Button::create("ui/Capsa/btnPlay.png");
    m_buttonLayer->addChild(m_btnGo);
    m_btnGo->setPosition(POS_SIZE(WIN_SIZE, Vec2(m_iPad ? 0.8f : 0.9f, 0.25f)));
    m_btnGo->addClickEventListener(CC_CALLBACK_0(CapsaGameScene::onGo, this));
    m_btnGo->setEnabled(false);
    
    m_btnPass = ui::Button::create("ui/Capsa/btnPass.png");
    m_buttonLayer->addChild(m_btnPass);
    m_btnPass->addClickEventListener(CC_CALLBACK_0(CapsaGameScene::onPass, this));
    m_btnPass->setEnabled(false);
    m_btnPass->setAnchorPoint(Vec2::ZERO);
    m_btnPass->setPosition(Vec2(m_btnGo->getPositionX(), m_btnGo->getBoundingBox().getMaxY()));
    
    m_btnHint = ui::Button::create("ui/Capsa/btnHint.png");
    m_buttonLayer->addChild(m_btnHint);
    m_btnHint->setAnchorPoint(Vec2(0.5f, 1.0f));
    m_btnHint->setPosition(Vec2(m_btnGo->getBoundingBox().getMidX(), m_btnGo->getBoundingBox().getMinY()));
    m_btnHint->addClickEventListener(CC_CALLBACK_0(CapsaGameScene::onHint, this));
    m_btnHint->setEnabled(false);
    
    m_buttonLayer->setVisible(false);
    
    GameHandler::getInstance()->resetCards();
}

void CapsaGameScene::updatePlayerSpots()
{
    GameHandler *handler = GameHandler::getInstance();
    CapsaRoom *room = handler->currentRoom();
    unsigned int allPlayers = room->maxPlayers;
    for (int i = 0; i < allPlayers; i++)
    {
        int playerIndex = (i + allPlayers - m_playerIndex) % allPlayers;
        Vec2 pos = POS_SIZE(WIN_SIZE, (m_iPad ? gPlayerAnchorPointForIPad[playerIndex] : gPlayerAnchorPoint[playerIndex]));

        switch (allPlayers) {
            case 2:
                pos = POS_SIZE(WIN_SIZE, (m_iPad ? gPlayerAnchorPointForIPad_2[playerIndex] : gPlayerAnchorPoint_2[playerIndex]));
                break;
            case 3:
                pos = POS_SIZE(WIN_SIZE, (m_iPad ? gPlayerAnchorPointForIPad_3[playerIndex] : gPlayerAnchorPoint_3[playerIndex]));
                break;
            case 4:
                pos = POS_SIZE(WIN_SIZE, (m_iPad ? gPlayerAnchorPointForIPad[playerIndex] : gPlayerAnchorPoint[playerIndex]));
                break;
            default:
                break;
        }

        m_playerNode[i]->setPosition(pos);
        m_inviteButton[i]->setPosition(pos);
    }

//    for (int i = 0; i < MAX_PLAYERS; i++)
//    {
//        int playerIndex = (i + MAX_PLAYERS - m_playerIndex) % MAX_PLAYERS;
//        Vec2 pos = POS_SIZE(WIN_SIZE, (m_iPad ? gPlayerAnchorPointForIPad[playerIndex] : gPlayerAnchorPoint[playerIndex]));
//        m_playerNode[i]->setPosition(pos);
//        m_inviteButton[i]->setPosition(pos);
//    }
}

void CapsaGameScene::updatePlayers()
{
    if (!m_needToUpdateUserProfile)
        return;
    
    GameHandler *handler = GameHandler::getInstance();
    CapsaRoom *room = handler->currentRoom();
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!room->players[i].length())
        {
            m_playerNode[i]->setVisible(false);
            removePlayerCards(i);
            
            if (i < room->maxPlayers) {
                if (!room->isFull())
                    m_inviteButton[i]->setVisible(true);
            }
            
            continue;
        }
        
        m_playerNode[i]->setPlayerName(room->players[i]);
        AppWarp::Client::getInstance()->getLiveUserInfo(room->players[i]);
        m_playerNode[i]->setVisible(true);
        m_inviteButton[i]->setVisible(false);
        m_playerNode[i]->setMaxTime(room->maxTime);
        
        m_needToUpdateUserProfile = false;
    }
    updatePlayerSpots();
}

void CapsaGameScene::updatePlayerInfo(AppWarp::liveuser uevent)
{
    GameHandler *handler = GameHandler::getInstance();
    CapsaRoom *room = handler->currentRoom();
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!room->players[i].length())
            continue;
        
        if (room->players[i].compare(uevent.name))
            continue;
        
        room->gamePlayers[i].fromJSONString(uevent.customData);
        m_playerNode[i]->updatePlayer(room->gamePlayers[i]);
        
        return;
    }
}

void CapsaGameScene::setupAppwarp()
{
    AppWarp::Client *warpClientRef = AppWarp::Client::getInstance();
    warpClientRef->setConnectionRequestListener(this);
    warpClientRef->setNotificationListener(this);
    warpClientRef->setRoomRequestListener(this);
    warpClientRef->setZoneRequestListener(this);
    warpClientRef->setTurnBasedRoomRequestListener(this);
    warpClientRef->setLobbyRequestListener(this);
}

void CapsaGameScene::onConnectDone(int res, int reasonCode)
{
    GameHandler::getInstance()->hideProcess();
    
    if (res==AppWarp::ResultCode::success)
    {
        unscheduleRecover();
    }
    else if (res==AppWarp::ResultCode::auth_error)
    {
        unscheduleRecover();
    }
    else if (res==AppWarp::ResultCode::success_recovered)
    {
        unscheduleRecover();
        requestRoomInfo();
    }
    else if (res==AppWarp::ResultCode::connection_error_recoverable)
    {
        scheduleRecover();
    }
    else if (res==AppWarp::ResultCode::bad_request)
    {
        unscheduleRecover();
    }
    else
    {
        unscheduleRecover();
    }
}

void CapsaGameScene::onDisconnectDone(int)
{
    GameHandler::getInstance()->setConnected(false);
}

void CapsaGameScene::scheduleRecover()
{
    if (!this->isScheduled(schedule_selector(CapsaGameScene::recover)))
    {
        this->schedule(schedule_selector(CapsaGameScene::recover), 1.0f);
        GameHandler::getInstance()->showProcess(this, "Reconnecting ...");
    }
}

void CapsaGameScene::unscheduleRecover()
{
    unschedule(schedule_selector(CapsaGameScene::recover));
}

void CapsaGameScene::recover(float dt)
{
    AppWarp::Client::getInstance()->recoverConnection();
}

void CapsaGameScene::requestRoomInfo()
{
    AppWarp::Client::getInstance()->getLiveRoomInfo(GameHandler::getInstance()->currentRoom()->roomID);
}

void CapsaGameScene::onUserChangeRoomProperty(AppWarp::room rData, std::string user, std::map<std::string, std::string> properties, std::map<std::string, std::string> lockTable)
{
    GameHandler::getInstance()->currentRoom()->fromMap(properties);
    updateGameRoom();
}

void CapsaGameScene::onChatReceived(AppWarp::chat chatevent)
{
}

void CapsaGameScene::onGetLiveRoomInfoDone(AppWarp::liveroom revent)
{
    if (revent.result)
        return;
    
    GameHandler *handler = GameHandler::getInstance();
    handler->currentRoom()->fromMap(revent.properties);
    handler->currentRoom()->stripNonUsers(revent.users);
    if (handler->currentRoom()->isSelectedNewMaster())
    {
        if (handler->isMaster())
        {
            sendUpdatedRoom();
        }
    }
    updateGameRoom();
}

void CapsaGameScene::onUserPaused(std::string user,std::string locId,bool isLobby)
{
//    std::string message = "Waiting for "+ base64_decode(user) + " response...";
//    showProcess(message);
}

void CapsaGameScene::onUserResumed(std::string user,std::string locId,bool isLobby)
{
//    hideProcess();
}

void CapsaGameScene::onUserJoinedRoom(AppWarp::room rData, std::string user)
{
    GameHandler *handler = GameHandler::getInstance();
    if (!user.compare(handler->getPlayerName()))
        return;
    
    std::string roomID = rData.roomId;
    std::string currentRoomID = handler->currentRoom()->roomID;
    if (roomID.compare(currentRoomID))
        return;
    
    if (handler->isMaster())
    {
        deliverSpotForPlayer(user);
        if (!m_gameStarted && handler->currentRoom()->getPlayerCount() > 1)
        {
            startGame();
        }
    }
    m_needToUpdateUserProfile = true;
}

void CapsaGameScene::onLeaveRoomDone(AppWarp::room revent)
{
    AppWarp::Client::getInstance()->unsubscribeRoom(revent.roomId);
    if (!m_resultLayer)
        goBackLobby();
}

void CapsaGameScene::onUserLeftRoom(AppWarp::room rData, std::string user)
{
    GameHandler *handler = GameHandler::getInstance();
    handler->hideProcess();
    if (!user.compare(handler->getPlayerName()))
        return;
    
    if (rData.roomId.compare(handler->currentRoom()->roomID))
        return;
    
    if (handler->isMaster())
    {
        removeSpotForPlayer(user);
    }
    else
    {
        AppWarp::Client::getInstance()->getLiveRoomInfo(rData.roomId);
    }
}

void CapsaGameScene::dealCards()
{
    GameHandler *handler = GameHandler::getInstance();
    CapsaRoom *room = handler->currentRoom();
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        room->cards[i].clear();
        room->thrownCards[i].clear();
    }
        
    std::vector<int> cardStacks;
    for (int i = 0; i < MAX_CARDS; i++)
        cardStacks.push_back(i);
    
    srand((unsigned int)time(NULL));
    for (int i = 0; i < MAX_CARDS; i++)
    {
        int index = rand() % cardStacks.size();
        int playerIndex = i % MAX_PLAYERS;
        if (!room->players[playerIndex].length())
            continue;
        room->cards[playerIndex].push_back(cardStacks.at(index));
        cardStacks.erase(cardStacks.begin() + index);
    }
    
    sendUpdatedRoom();
}

void CapsaGameScene::sendChat(const std::string & message)
{
    AppWarp::Client::getInstance()->sendChat(message);
}

void CapsaGameScene::deliverSpotForPlayer(const std::string & player)
{
    GameHandler *handler = GameHandler::getInstance();
    CapsaRoom *room = handler->currentRoom();
    
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (room->players[i].length())
        {
            if (!room->players[i].compare(player))
                return;
        }
    }
    
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!room->players[i].length())
        {
            room->players[i] = player;
            break;
        }
    }
    sendUpdatedRoom();
}

void CapsaGameScene::removeSpotForPlayer(const std::string & player)
{
    GameHandler *handler = GameHandler::getInstance();
    CapsaRoom *room = handler->currentRoom();
    
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!room->players[i].compare(player))
        {
            room->players[i] = "";
            break;
        }
    }
    
    sendUpdatedRoom();
}

void CapsaGameScene::onBackButtonPressed()
{
    if (m_playerIndex < 0)
    {
        leaveRoom();
        return;
    }
    
    if (!m_cards[m_playerIndex]->count())
    {
        leaveRoom();
        return;
    }
    
    m_isCheckedLeaveRoom = true;
}

void CapsaGameScene::resetGame()
{
    m_cardLayer->removeAllChildrenWithCleanup(true);
    m_cardPairLayer->removeAllChildrenWithCleanup(true);
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        m_cards[i]->removeAllObjects();
        m_thrownCards[i]->removeAllObjects();
        m_dealedCards[i] = false;
    }
    showButtonPanel(false);
    m_gameEnded = false;
    m_isMyTurn = false;
    m_needToUpdateUserProfile = true;
    GameHandler::getInstance()->resetCards();
}

void CapsaGameScene::getFirstPlayer()
{
    GameHandler *handler = GameHandler::getInstance();
    CapsaRoom *room = handler->currentRoom();
    
    int minCardNum = MAX_CARDS;
    int firstPlayerIndex = 0;
    
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!room->players[i].length())
            continue;
        
        for (int cardIndex = 0; cardIndex < room->cards[i].size(); cardIndex++)
        {
            int cardNum = room->cards[i].at(cardIndex);
            if (cardNum < minCardNum) {
                minCardNum = cardNum;
                firstPlayerIndex = i;
            }
        }
    }
    
    if (room->players[firstPlayerIndex].length()) {
        m_winner = room->players[firstPlayerIndex];
    }

}

void CapsaGameScene::startGame()
{
    dealCards();
    
    if (!m_winner.length()) { // First Starting, find first player
        getFirstPlayer();
    }

    AppWarp::Client * warpClientRef = AppWarp::Client::getInstance();
    warpClientRef->startGame(true, m_winner);
    
    m_gameStarted = true;
}

void CapsaGameScene::onGameStarted(std::string sender, std::string id, std::string nextTurn)
{
//    setPlayerTurnByNextTurn(nextTurn);
    if (!sender.compare(m_winner)) {
        setPlayerTurnByNextTurn(m_winner);
    } else {
        AppWarp::Client::getInstance()->sendMove("", m_winner);
    }
}

void CapsaGameScene::onGameStopped(std::string sender, std::string id)
{
    GameHandler *handler = GameHandler::getInstance();
    if (handler->currentRoom()->getPlayerIndex(sender) < 0)
        return;
    
    m_winner = sender;
    
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        __Array *cards = m_cards[i];
        for (int j = 0; j < cards->count(); j++)
        {
            CardSprite *cardSprite = (CardSprite *)cards->getObjectAtIndex(j);
            cardSprite->show(true);
        }
        
        m_playerNode[i]->setActiveTurn(false);
    }
    
    m_isMyTurn = false;
    showButtonPanel(false);
    
    m_countDownRestart = 10;
    
    showResultDialog();
    if (m_isCheckedLeaveRoom)
        leaveRoom();
    else
        schedule(schedule_selector(CapsaGameScene::restartSchedule), 1.0f);
}

void CapsaGameScene::onStartGameDone(int res)
{
    
}

void CapsaGameScene::onStopGameDone(int res)
{
    
}

void CapsaGameScene::restartSchedule(float dt)
{
    m_countDownRestart--;
    if (m_countDownRestart)
    {
        //showAnimation
        return;
    }
    else
        unschedule(schedule_selector(CapsaGameScene::restartSchedule));
    
    hideResultDialog();
    resetGame();
    
    GameHandler *handler = GameHandler::getInstance();
    if (handler->isMaster() && handler->currentRoom()->getPlayerCount() > 1)
    {
        startGame();
    }
    else
        m_gameStarted = false;
}

void CapsaGameScene::onMoveCompleted(AppWarp::move event)
{
    // move turn when i dont have cards. it means i am obsever
    GameHandler *handler = GameHandler::getInstance();
    bool myTurn = !handler->getPlayerName().compare(event.nextTurn);
    if (myTurn && m_playerIndex >= 0)
    {
        if (!m_dealedCards[m_playerIndex])
        {
            AppWarp::Client::getInstance()->sendMove("");
            return;
        }
    }
    
    removePlayerThrownCards(event.nextTurn);
    setPlayerTurnByNextTurn(event.nextTurn);
}

void CapsaGameScene::sortCardArray(cocos2d::__Array *arrCard)
{
    __Array *arrBuf = __Array::create();
    for (int i = 0; i < arrCard->count(); i++)
    {
        CardSprite *card = (CardSprite *)arrCard->getObjectAtIndex(i);
        int j = 0;
        for (j = 0; j < arrBuf->count(); j++)
        {
            CardSprite *bufCard = (CardSprite *)arrBuf->getObjectAtIndex(j);
            if (bufCard->getCardNumber() > card->getCardNumber())
                break;
        }
        arrBuf->insertObject(card, j);
    }
    
    arrCard->removeAllObjects();
    for (int i = 0; i < arrBuf->count(); i++)
    {
        arrCard->addObject(arrBuf->getObjectAtIndex(i));
    }
}

void CapsaGameScene::showCards()
{
    if (m_playerIndex < 0)
        return;
    
    GameHandler *handler = GameHandler::getInstance();
    CapsaRoom *room = handler->currentRoom();
    unsigned int allPlayers = room->maxPlayers;

    for (int playerIndex = 0; playerIndex < allPlayers; playerIndex++)
    {
        bool mine = playerIndex == m_playerIndex;
        __Array *cards = m_cards[playerIndex];
        m_dealedCards[playerIndex] = cards->count();
        for (int i = 0; i < cards->count(); i++)
        {
            CardSprite *card = (CardSprite *)cards->getObjectAtIndex(i);
            m_cardLayer->addChild(card);
            int viewIndex = (playerIndex + allPlayers - m_playerIndex) % allPlayers;
            Vec2 pos = POS_SIZE(WIN_SIZE, (m_iPad ? gCardPosRatioForIPad[viewIndex] : gCardPosRatio[viewIndex]));
            switch (allPlayers) {
                case 2:
                    pos = POS_SIZE(WIN_SIZE, (m_iPad ? gCardPosRatioForIPad_2[viewIndex] : gCardPosRatio_2[viewIndex]));
                    break;
                case 3:
                    pos = POS_SIZE(WIN_SIZE, (m_iPad ? gCardPosRatioForIPad_3[viewIndex] : gCardPosRatio_3[viewIndex]));
                    break;
                case 4:
                    pos = POS_SIZE(WIN_SIZE, (m_iPad ? gCardPosRatioForIPad[viewIndex] : gCardPosRatio[viewIndex]));
                    break;
                default:
                    break;
            }

            float angle = 90 * viewIndex;
            switch (allPlayers) {
                case 2:
                    angle = 0;
                    if (viewIndex == 1) {
                        angle = 180;
                    }
                    break;
                case 3:
                    angle = 0;
                    if (viewIndex == 1) {
                        angle = 90;
                    } else if (viewIndex == 2) {
                        angle = 270;
                    }
                    break;
                case 4:
                    angle = 90 * viewIndex;
                    break;
                    
                default:
                    break;
            }

            card->setPosition(pos);
            card->setRotation(angle);
            if (!mine)
                card->setScale(m_iPad ? OTHER_CARD_SCALE * 0.8f : OTHER_CARD_SCALE);
            else
                card->setScale(m_iPad ? 0.8f : 1.0f);
            card->show(mine);
            
            Vec2 destPos = pos;
            float distanceIndex = i - (cards->count() - 1) / 2.0f;
            float distanceBetweenCards = card->getContentSize().width * 0.5f * card->getScale();
            switch (allPlayers) {
                case 2:
                    destPos.x = pos.x + distanceIndex * distanceBetweenCards;
                    break;
                case 3:
                    if (viewIndex == 1 || viewIndex == 2) {
                        destPos.y = pos.y + distanceIndex * distanceBetweenCards * 0.5f;
                    } else if (viewIndex == 2) {
                        destPos.x = pos.x + distanceIndex * distanceBetweenCards;
                    }
                    break;
                case 4:
                    if ( viewIndex % 2)
                        destPos.y = pos.y + distanceIndex * distanceBetweenCards * 0.5f;
                    else
                        destPos.x = pos.x + distanceIndex * distanceBetweenCards;
                    break;
                default:
                    break;
            }

            MoveTo *moveTo = MoveTo::create(CARD_SHOW_TIME * fabs(distanceIndex), destPos);
            card->runAction(moveTo);
        }
        
        updateThrownCard(playerIndex);
    }
    
    updateAvailableCardPairs();
}

void CapsaGameScene::removePlayerCards(int playerIndex)
{
    __Array *cards = m_cards[playerIndex];
    for (int i = 0; i < cards->count(); i++)
    {
        Node *card = (Node *)cards->getObjectAtIndex(i);
        card->removeFromParentAndCleanup(true);
    }
    cards->removeAllObjects();
    
    /*
    __Array *thrownCards = m_cards[playerIndex];
    for (int i = 0; i < thrownCards->count(); i++)
    {
        Node *card = (Node *)cards->getObjectAtIndex(i);
        card->removeFromParentAndCleanup(true);
    }
    thrownCards->removeAllObjects();
    */
}

void CapsaGameScene::setPlayerTurn(int playerIndex)
{
    CapsaRoom *room = GameHandler::getInstance()->currentRoom();
    
    m_currentTurnIndex = playerIndex;
    
    bool isMyTurn = (m_playerIndex == playerIndex);
    if (isMyTurn)
    {
        removePlayerThrownCards(room->players[m_playerIndex]);
    }
    else if (m_isMyTurn)
    {
        onReselect();
    }
    
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        m_playerNode[i]->setActiveTurn(playerIndex == i);
    }
    m_isMyTurn = isMyTurn;
    showButtonPanel(m_isMyTurn);
    
    if (m_isMyTurn)
    {
        room->thrownCards[m_playerIndex].clear();
        sendUpdatedRoom();
        AppWarp::Client::getInstance()->setNextTurn(getNextTurn());
    }
}

void CapsaGameScene::onGetLiveUserInfoDone(AppWarp::liveuser uevent)
{
    if (uevent.result)
        return;
    
    updatePlayerInfo(uevent);
}

void CapsaGameScene::onGetMoveHistoryDone(int res, std::vector<AppWarp::move> history)
{
    /*
    if (!res)
    {
        bool someonePlayedCard = false;
        for (int i = history.size() - 1; i >= 0; i--)
        {
            AppWarp::move move = history.at(i);
            if (!move.sender.compare(m_players[m_playerIndex]))
            {
                break;
            }
            else if (move.moveData.length())
            {
                someonePlayedCard = true;
                break;
            }
        }
        
        if (!someonePlayedCard)
        {
            clearThrownCards();
        }
    }
    */
}

void CapsaGameScene::showButtonPanel(bool visible)
{
    if (visible)
    {
        updateButtonPanel();
    }
    m_buttonLayer->setVisible(visible);
}

void CapsaGameScene::updateButtonPanel()
{
    m_btnPass->setEnabled(m_isMyTurn);
    
    __Array *arrCard = m_cards[m_playerIndex];
    __Array *selectedCards = __Array::create();
    for (int i = 0; i < arrCard->count(); i++)
    {
        CardSprite *card = (CardSprite *)arrCard->getObjectAtIndex(i);
        if (card->isSelected())
            selectedCards->addObject(card);
    }
    
    m_btnReselect->setEnabled(selectedCards->count());
    if (!selectedCards->count())
    {
        m_btnGo->setEnabled(false);
        return;
    }
    
    Big2PokerModel model(selectedCards);
    __Array *arrOtherThrownCard = NULL;
    for (int i = 1; i < MAX_PLAYERS; i++)
    {
        int previousPlayerIndex = (m_playerIndex - i + MAX_PLAYERS) % MAX_PLAYERS;
        arrOtherThrownCard = m_thrownCards[previousPlayerIndex];
        if (arrOtherThrownCard->count())
            break;
    }
    if (arrOtherThrownCard->count())
    {
        Big2PokerModel otherModel(arrOtherThrownCard);
        m_btnGo->setEnabled(model.canGo(&otherModel));
    }
    else
        m_btnGo->setEnabled(model.canGo());
}

void CapsaGameScene::onGo()
{
    m_thrownCards[m_playerIndex]->removeAllObjects();
    for (int i = 0; i < m_cards[m_playerIndex]->count(); i++)
    {
        CardSprite *card = (CardSprite *)m_cards[m_playerIndex]->getObjectAtIndex(i);
        if (card->isSelected())
        {
            m_thrownCards[m_playerIndex]->addObject(card);
            m_cards[m_playerIndex]->removeObject(card);
            i--;
        }
    }
    
    CapsaRoom *room = GameHandler::getInstance()->currentRoom();
    room->cards[m_playerIndex].clear();
    room->thrownCards[m_playerIndex].clear();
    
    
    __Array *arrCard = m_cards[m_playerIndex];
    size_t cntCard = arrCard->count();
    for (int i = 0; i < cntCard; i++)
    {
        CardSprite *cardSprite = (CardSprite *)arrCard->getObjectAtIndex(i);
        int cardNum = cardSprite->getCardNumber();
        room->cards[m_playerIndex].push_back(cardNum);
    }
    
    __Array *arrThrownCard = m_thrownCards[m_playerIndex];
    size_t cntThrownCard = arrThrownCard->count();
    for (int i = 0; i < cntThrownCard; i++)
    {
        CardSprite *cardSprite = (CardSprite *)arrThrownCard->getObjectAtIndex(i);
        int cardNum = cardSprite->getCardNumber();
        room->thrownCards[m_playerIndex].push_back(cardNum);
    }
    sendUpdatedRoom();
    
    if (arrCard->count())
    {
        AppWarp::Client::getInstance()->sendMove("thrown");
        updateAvailableCardPairs();
    }
    else
        AppWarp::Client::getInstance()->stopGame();
}

void CapsaGameScene::onReselect()
{
    __Array *arrCard = m_cards[m_playerIndex];
    for (int i = 0; i < arrCard->count(); i++)
    {
        CardSprite *card = (CardSprite *)arrCard->getObjectAtIndex(i);
        if (card->isSelected())
        {
            card->setSelected(false);
            float cardOriginY = POS_SIZE(WIN_SIZE, (m_iPad ? gCardPosRatioForIPad[0] : gCardPosRatio[0]) ).y;
            Vec2 pos = card->getPosition();
            pos.y = cardOriginY;
            MoveTo *moveTo = MoveTo::create(CARD_SHOW_TIME, pos);
            card->runAction(moveTo);
        }
    }
    m_btnReselect->setEnabled(false);
    m_btnGo->setEnabled(false);
}

void CapsaGameScene::onPass()
{
    AppWarp::Client::getInstance()->sendMove("");
}

void CapsaGameScene::onHint()
{
    
}

void CapsaGameScene::onEnter()
{
    Layer::onEnter();
    
    setupTouch(true);
}

void CapsaGameScene::onExit()
{
    setupTouch(false);
    
    Layer::onExit();
}

void CapsaGameScene::setupTouch(bool setup)
{
    if (setup)
    {
        auto pListener = EventListenerTouchOneByOne::create();
        pListener->setSwallowTouches(true);
        pListener->onTouchBegan = CC_CALLBACK_2(CapsaGameScene::onTouchBegan, this);
        pListener->onTouchMoved = CC_CALLBACK_2(CapsaGameScene::onTouchMoved, this);
        pListener->onTouchEnded = CC_CALLBACK_2(CapsaGameScene::onTouchEnded, this);
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(pListener, this);
    }
    else
    {
        getEventDispatcher()->removeEventListenersForTarget(this);
    }
}

bool CapsaGameScene::onTouchBegan(Touch *touch, Event *unused_event)
{
    Vec2 pos = touch->getLocation();
    
    if (m_gameEnded)
        return false;
    
    if (!m_isMyTurn)
        return false;
    
    selectCardByTouchPos(pos);
    
    return true;
}

void CapsaGameScene::onTouchMoved(Touch *touch, Event *unused_event)
{
}

void CapsaGameScene::onTouchEnded(Touch *touch, Event *unused_event)
{
    
}

void CapsaGameScene::selectCardByTouchPos(cocos2d::Vec2 pos)
{
    __Array *arrCard = m_cards[m_playerIndex];
    CardSprite *selectedCard = NULL;
    for (int i = 0; i < arrCard->count(); i++)
    {
        CardSprite *card = (CardSprite *)arrCard->getObjectAtIndex(i);
        if (card->getBoundingBox().containsPoint(pos))
            selectedCard = card;
    }
    if (selectedCard)
    {
        if (selectedCard->getNumberOfRunningActions() > 0)
            return;
        
        bool selected = selectedCard->isSelected();
        selected = !selected;
        selectedCard->setSelected(selected);
        float cardRiseY = selectedCard->getContentSize().height * 0.25f;
        float deltaY = selected ? cardRiseY : 0;
        float cardOriginY = POS_SIZE(WIN_SIZE, (m_iPad ? gCardPosRatioForIPad[0] : gCardPosRatio[0])).y;
        Vec2 pos = selectedCard->getPosition();
        pos.y = cardOriginY + deltaY;
        MoveTo *moveTo = MoveTo::create(CARD_SHOW_TIME, pos);
        selectedCard->runAction(moveTo);
        
        updateButtonPanel();
    }
}

void CapsaGameScene::setPlayerTurnByNextTurn(const std::string nextTurn)
{
    CapsaRoom *room = GameHandler::getInstance()->currentRoom();
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!nextTurn.compare(room->players[i]) && m_cards[i]->count())
        {
            setPlayerTurn(i);
            break;
        }
    }
}

void CapsaGameScene::removePlayerThrownCards(const std::string &player, int playerIndex)
{
    CapsaRoom *room = GameHandler::getInstance()->currentRoom();
    if (playerIndex < 0)
        playerIndex = room->getPlayerIndex(player);
    if (playerIndex < 0)
        return;
    
    __Array *arrCard = m_thrownCards[playerIndex];
    for (int i = 0; i < arrCard->count(); i++)
    {
        Node *node = (Node *)arrCard->getObjectAtIndex(i);
        node->removeFromParentAndCleanup(true);
    }
    arrCard->removeAllObjects();
}

void CapsaGameScene::updatePlayerCard(const std::string & player, cocos2d::__Array * arrCard, cocos2d::__Array * arrThrownCard)
{
    CapsaRoom *room = GameHandler::getInstance()->currentRoom();
    int playerIndex = room->getPlayerIndex(player);
    if (playerIndex < 0)
        return;
    
    __Array *prevArrCard = m_cards[playerIndex];
    for (int i = 0; i < prevArrCard->count(); i++)
    {
        CardSprite *cardSprite = (CardSprite *)prevArrCard->getObjectAtIndex(i);
        if (!arrCard->containsObject(cardSprite))
        {
            if (!arrThrownCard->containsObject(cardSprite))
                cardSprite->removeFromParentAndCleanup(true);
            prevArrCard->removeObject(cardSprite);
            i--;
        }
    }
    
    m_thrownCards[playerIndex]->removeAllObjects();
    m_thrownCards[playerIndex]->addObjectsFromArray(arrThrownCard);
    
    updateCard(playerIndex);
}

void CapsaGameScene::updateCard(int playerIndex)
{
    GameHandler *handler = GameHandler::getInstance();
    CapsaRoom *room = handler->currentRoom();
    unsigned int allPlayers = room->maxPlayers;

    __Array *cards = m_cards[playerIndex];
    for (int i = 0; i < cards->count(); i++)
    {
        CardSprite *card = (CardSprite *)cards->getObjectAtIndex(i);
        int viewIndex = (playerIndex + allPlayers - m_playerIndex) % allPlayers;
        Vec2 pos = POS_SIZE(WIN_SIZE, (m_iPad ? gCardPosRatioForIPad[viewIndex] : gCardPosRatio[viewIndex]));
        
        switch (allPlayers) {
            case 2:
                pos = POS_SIZE(WIN_SIZE, (m_iPad ? gCardPosRatioForIPad_2[viewIndex] : gCardPosRatio_2[viewIndex]));
                break;
            case 3:
                pos = POS_SIZE(WIN_SIZE, (m_iPad ? gCardPosRatioForIPad_3[viewIndex] : gCardPosRatio_3[viewIndex]));
                break;
            case 4:
                pos = POS_SIZE(WIN_SIZE, (m_iPad ? gCardPosRatioForIPad[viewIndex] : gCardPosRatio[viewIndex]));
                break;
            default:
                break;
        }

        Vec2 destPos = pos;
        float distanceIndex = i - (cards->count() - 1) / 2.0f;
        float distanceBetweenCards = card->getContentSize().width * 0.5f * card->getScale();
        
        switch (allPlayers) {
            case 2:
                destPos.x = pos.x + distanceIndex * distanceBetweenCards;
                break;
            case 3:
                if ( viewIndex == 1 || viewIndex == 2)
                    destPos.y = pos.y + distanceIndex * distanceBetweenCards * 0.5f;
                else
                    destPos.x = pos.x + distanceIndex * distanceBetweenCards;
                break;
            case 4:
                if ( viewIndex % 2)
                    destPos.y = pos.y + distanceIndex * distanceBetweenCards * 0.5f;
                else
                    destPos.x = pos.x + distanceIndex * distanceBetweenCards;
                break;
            default:
                break;
        }
        
        card->stopAllActions();
        MoveTo *moveTo = MoveTo::create(CARD_SHOW_TIME * fabs(distanceIndex), destPos);
        card->runAction(moveTo);
    }
    
    updateThrownCard(playerIndex);
}

void CapsaGameScene::updateThrownCard(int playerIndex)
{
    __Array *thrownCards = m_thrownCards[playerIndex];
    if (!thrownCards->count())
        return;
    
    GameHandler *handler = GameHandler::getInstance();
    CapsaRoom *room = handler->currentRoom();
    unsigned int allPlayers = room->maxPlayers;

    int diffTurn = m_currentTurnIndex - playerIndex;
    if (diffTurn < 0)
        diffTurn += allPlayers;
    
    int viewIndex = (playerIndex + allPlayers - m_playerIndex) % allPlayers;
    float cardAngle = 90 * viewIndex;
    switch (allPlayers) {
        case 2:
            cardAngle = 0;
            if (viewIndex == 1) {
                cardAngle = 180;
            }
            break;
        case 3:
            cardAngle = 0;
            if (viewIndex == 1) {
                cardAngle = 90;
            } else if (viewIndex == 2) {
                cardAngle = 270;
            }
            break;
        case 4:
            cardAngle = 90 * viewIndex;
            break;
            
        default:
            break;
    }
    
    CardSprite *prevCard = NULL;
    for (int i = (int)thrownCards->count() - 1; i >=0; i--)
    {
        CardSprite *card = (CardSprite *)thrownCards->getObjectAtIndex(i);
        if (!prevCard)
        {
            prevCard = card;
            if (thrownCards->count() > 1)
            {
                float diffAngle = THROWN_CARD_ARC_ANGLE * (thrownCards->count() - 1) * 0.5f;
                card->setRotation(cardAngle + diffAngle);
                card->setLocalZOrder(allPlayers - diffTurn);
            }
            continue;
        }
        else
        {
            card->removeFromParentAndCleanup(true);
            prevCard->addChild(card, -1);
            if (i < thrownCards->count() >> 1)
            {
                card->setAnchorPoint(Vec2(1.0f, 1.0f));
                card->setPosition(card->getContentSize().width * 0.5f, card->getContentSize().height);
            }
            else
            {
                card->setAnchorPoint(Vec2(0.5f, 1.0f));
                card->setPosition(0, card->getContentSize().height);
            }
            card->setRotation(-THROWN_CARD_ARC_ANGLE);
            prevCard = card;
        }
    }
    
    for (int i = 0; i < thrownCards->count(); i++)
    {
        CardSprite *card = (CardSprite *)thrownCards->getObjectAtIndex(i);
        card->show(true);
        card->setScale(1.0f);
        if (i == thrownCards->count() - 1)
        {
            card->setScale(THROWN_CARD_SCALE);
            int viewIndex = (playerIndex + allPlayers - m_playerIndex) % allPlayers;
            Vec2 pos = POS_SIZE(WIN_SIZE, (m_iPad ? gCardPosRatioForThrownForIPad[viewIndex] :gCardPosRatioForThrown[viewIndex]));
            
            switch (allPlayers) {
                case 2:
                    pos = POS_SIZE(WIN_SIZE, (m_iPad ? gCardPosRatioForThrownForIPad_2[viewIndex] :gCardPosRatioForThrown_2[viewIndex]));
                    break;
                case 3:
                    pos = POS_SIZE(WIN_SIZE, (m_iPad ? gCardPosRatioForThrownForIPad_3[viewIndex] :gCardPosRatioForThrown_3[viewIndex]));
                    break;
                case 4:
                    pos = POS_SIZE(WIN_SIZE, (m_iPad ? gCardPosRatioForThrownForIPad[viewIndex] :gCardPosRatioForThrown[viewIndex]));
                    break;
                default:
                    break;
            }

            Vec2 destPos = pos;
            float distanceIndex = i - (thrownCards->count() - 1) / 2.0f;
            float distanceBetweenCards = card->getContentSize().width * 0.5f;
            if ( viewIndex % 2)
                destPos.y = pos.y + distanceIndex * distanceBetweenCards * (viewIndex == 1 ? - 1 : 1);
            else
                destPos.x = pos.x + distanceIndex * distanceBetweenCards;
            
            MoveTo *moveTo = MoveTo::create(CARD_SHOW_TIME * fabs(distanceIndex), destPos);
            card->runAction(moveTo);
        }
    }
}

void CapsaGameScene::clearThrownCards(bool fromScreen)
{
    if (fromScreen)
    {
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            __Array * cards = m_thrownCards[i];
            for (int j = 0; j < cards->count(); j++)
            {
                Node *card = (Node *)cards->getObjectAtIndex(j);
                card->removeFromParentAndCleanup(true);
            }
            cards->removeAllObjects();
        }
    }
    else
    {
        cJSON *jsonRoot = cJSON_CreateObject();
        cJSON_AddStringToObject(jsonRoot, MESSAGE_TYPE, MESSAGE_TYPE_CLEAR_THROWN);
    
        std::string message = cJSON_Print(jsonRoot);
        cJSON_Delete(jsonRoot);
    
        sendChat(message);
    }
}

void CapsaGameScene::updateGameRoom()
{
    updatePlayerIndex();
    updatePlayers();
    updateCards();
}

void CapsaGameScene::updateCards()
{
    GameHandler *handler = GameHandler::getInstance();
    CapsaRoom *room = handler->currentRoom();
    
    int allCardCount = 0;
    size_t prevCardCount[MAX_PLAYERS];
    size_t prevThrownCount[MAX_PLAYERS];
    
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        allCardCount += m_cards[i]->count() + m_thrownCards[i]->count();
        
        prevCardCount[i] = m_cards[i]->count();
        prevThrownCount[i] = m_thrownCards[i]->count();
        
        m_cards[i]->removeAllObjects();
        m_thrownCards[i]->removeAllObjects();
    }
    
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!room->players[i].length())
            continue;
        
        for (int j = 0; j < room->cards[i].size(); j++)
        {
            int cardNum = room->cards[i].at(j);
            CardSprite * card = handler->getCard(cardNum);
            if (i == m_playerIndex)
                card->setSelected(false);
            
            m_cards[i]->addObject(card);
        }
        
        for (int j = 0; j < room->thrownCards[i].size(); j++)
        {
            int thrownCardNum = room->thrownCards[i].at(j);
            CardSprite * thrownCard = handler->getCard(thrownCardNum);
            m_thrownCards[i]->addObject(thrownCard);
        }
        
        sortCardArray(m_cards[i]);
        sortCardArray(m_thrownCards[i]);
    }
    
    bool isFirstDeal = !m_cardLayer->getChildren().size();
    if (isFirstDeal)
        showCards();
    else
    {
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if (!room->players[i].length())
                continue;
            
            updateCard(i);
        }
    }
    
    if (m_playerIndex < 0)
        m_btnReselect->setVisible(false);
    else
        m_btnReselect->setVisible(m_cards[m_playerIndex]->count());
}

void CapsaGameScene::sendUpdatedRoom()
{
    std::map<std::string, std::string> properties;
    std::vector<std::string> removeProperties;
    CapsaRoom *room = GameHandler::getInstance()->currentRoom();
    room->toMap(properties);
    std::string roomID = room->roomID;
    AppWarp::Client::getInstance()->updateRoomProperties(roomID, properties, removeProperties);
}

void CapsaGameScene::onInvitePlayer(cocos2d::Ref * sender)
{
    AppWarp::Client::getInstance()->getLiveLobbyInfo();
}

void CapsaGameScene::onGetLiveLobbyInfoDone(AppWarp::livelobby levent)
{
    
}

void CapsaGameScene::goBackLobby()
{
    AppWarp::Client * warpClientRef = AppWarp::Client::getInstance();
    warpClientRef->setConnectionRequestListener(NULL);
    warpClientRef->setNotificationListener(NULL);
    warpClientRef->setRoomRequestListener(NULL);
    warpClientRef->setZoneRequestListener(NULL);
    warpClientRef->setTurnBasedRoomRequestListener(NULL);
    
    auto nextScene = LoadingSplash::createScene(LoadingSplash::CAPSA_GAME);
    Director::getInstance()->replaceScene(nextScene);
}

std::string CapsaGameScene::getNextTurn()
{
    CapsaRoom *room = GameHandler::getInstance()->currentRoom();
    for (int i = 1; i <= MAX_PLAYERS; i++)
    {
        int nextIndex = (i + m_playerIndex) % MAX_PLAYERS;
        if (room->players[nextIndex].length() && m_cards[nextIndex]->count())
        {
            std::string realUserName = base64_decode(room->players[nextIndex]);
            log("NEXT TURN IS %s", realUserName.c_str());
            return room->players[nextIndex];
        }
    }

    return "";
}

void CapsaGameScene::showResultDialog()
{
    GameHandler *handler = GameHandler::getInstance();
    CapsaRoom *room = handler->currentRoom();
    int nRemainedCards = 0;
    int nPlayers = 0;
    int nPlayerIndex[MAX_PLAYERS];
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!m_dealedCards[i])
            continue;
        
        nRemainedCards += room->cards[i].size();
        nPlayerIndex[nPlayers] = i;
        
        nPlayers++;
    }
    
    //sort by remained cards
    for (int i = 0; i < nPlayers - 1; i++)
    {
        for (int j = i + 1; j < nPlayers; j++)
        {
            int ii = nPlayerIndex[i];
            int jj = nPlayerIndex[j];
            if (room->cards[jj].size() < room->cards[ii].size())
            {
                int buf = nPlayerIndex[i];
                nPlayerIndex[i] = nPlayerIndex[j];
                nPlayerIndex[j] = buf;
            }
        }
    }
    
    float averageRemainCards = nRemainedCards / (float)nPlayers;
    
    m_resultLayer = Layer::create();
    addChild(m_resultLayer, 10);
    
    Sprite *bg = Sprite::create("ui/Capsa/capsalobby_bg.png");
    m_resultLayer->addChild(bg);
    bg->setPosition(WIN_SIZE * 0.5f);
    
    Sprite *goldLine = Sprite::create("ui/Capsa/gold_line.png");
    m_resultLayer->addChild(goldLine);
    goldLine->setPosition(WIN_SIZE.width * 0.5f, WIN_SIZE.height * 0.8f);
    
    Sprite *resultMark = Sprite::create("ui/Capsa/result_title.png");
    m_resultLayer->addChild(resultMark);
    resultMark->setAnchorPoint(Vec2(0.5f, 0.0f));
    resultMark->setPosition(WIN_SIZE.width * 0.5f, WIN_SIZE.height * 0.82f);
    
    float yPos = goldLine->getPositionY();
    for (int i = 0; i < nPlayers; i++)
    {
        int index = nPlayerIndex[i];
        CapsaPlayerNode *playerNode = CapsaPlayerNode::create();
        playerNode->setPlayerName(room->players[index]);
        playerNode->updatePlayer(room->gamePlayers[index], true);
        float cellH = playerNode->getContentSize().height * 1.2f;
        yPos -= cellH * 0.5f;
        m_resultLayer->addChild(playerNode);
        playerNode->setPosition(WIN_SIZE.width * 0.2f, yPos);
        
        int gold = (averageRemainCards - room->cards[index].size()) * room->deposite;
        std::stringstream ssGold;
        if (gold > 0)
            ssGold << "+";
        ssGold << gold;
        Label *lblScore = Label::createWithTTF(ssGold.str(), MAIN_FONT_FILE, cellH * 0.5f);
        m_resultLayer->addChild(lblScore);
        lblScore->setPosition(WIN_SIZE.width * 0.5f, yPos);
        
        Sprite *spSeparator = Sprite::create("ui/Capsa/gold_separator.png");
        m_resultLayer->addChild(spSeparator);
        yPos -= cellH * 0.5f;
        spSeparator->setPosition(WIN_SIZE.width * 0.5f, yPos);
        
        if (!room->players[index].compare(handler->getPlayerName()))
        {
            PlayerProfile profile = PlayerProfile::getInstance();
            profile.setCoins(profile.getCoins() + gold);
            handler->sendPlayerProfile();
            m_needToUpdateUserProfile = true;
        }
    }
    
    ui::Button *btnOk = ui::Button::create("ui/Capsa/btnOk.png");
    m_resultLayer->addChild(btnOk);
    btnOk->setPosition(Vec2(WIN_SIZE.width * 0.5f, WIN_SIZE.height * 0.1f));
    btnOk->addClickEventListener(CC_CALLBACK_0(CapsaGameScene::hideResultDialog, this));
}

void CapsaGameScene::hideResultDialog()
{
    if (m_resultLayer)
    {
        m_resultLayer->removeFromParentAndCleanup(true);
        m_resultLayer = NULL;
        if (m_isCheckedLeaveRoom)
            goBackLobby();
    }
}

void CapsaGameScene::leaveRoom()
{
    GameHandler::getInstance()->showProcess(this, "Leave Room...");
    AppWarp::Client::getInstance()->leaveRoom(GameHandler::getInstance()->currentRoom()->roomID);
}

void CapsaGameScene::updateAvailableCardPairs()
{
    m_arrCardPairs->removeAllObjects();
    m_cardPairLayer->removeAllChildrenWithCleanup(true);
    
    for (int i = Big2PokerModel::None; i < Big2PokerModel::PairTypeCount; i++)
    {
        m_cardPairCounter[i] = 0;
        __Array *arrPair = __Array::create();
        m_arrCardPairs->addObject(arrPair);
    }
    
    __Array * arrAllCards = m_cards[m_playerIndex];
    for (int i = 2; i < 5; i++)
    {
        if (arrAllCards->count() < i)
            break;
        
        for (int j = 0; j < arrAllCards->count() - i + 1; j++)
        {
            __Array *arrCards = __Array::create();
            for (int k = 0; k < i; k++)
            {
                arrCards->addObject(arrAllCards->getObjectAtIndex(j + k));
            }
            
            Big2PokerModel::Big2PokerCardPairInfo cardPairInfo = Big2PokerModel::getHigherCardPairFromArray(arrCards);
            if (cardPairInfo < Big2PokerModel::Pair)
                continue;
            
            __Array * arrPair = (__Array *)m_arrCardPairs->getObjectAtIndex(cardPairInfo);
            arrPair->addObject(arrCards);
        }
    }
    
    /*
    //check if has triple
    __Array * arrTriplePair = (__Array *)m_arrCardPairs->getObjectAtIndex(Big2PokerModel::Triple);
    __Array * arrTwoPair = (__Array *)m_arrCardPairs->getObjectAtIndex(Big2PokerModel::Pair);
    __Array * arrFullHousePair = (__Array *)m_arrCardPairs->getObjectAtIndex(Big2PokerModel::FullHouse);
    if (arrTriplePair->count() && arrTwoPair->count())
    {
        //full house
        for (int j = 0; j < arrTriplePair->count(); j++)
        {
            __Array *arrTriple = (__Array *)arrTriplePair->getObjectAtIndex(j);
            for (int i = 0; i < arrTwoPair->count(); i++)
            {
                __Array *arrTwo = (__Array *)arrTwoPair->getObjectAtIndex(i);
                Big2PokerModel tripleModel(arrTriple);
                Big2PokerModel pairModel(arrTwo);
                if (tripleModel.getHighestCardWeight() == pairModel.getHighestCardWeight())
                    continue;
                
                __Array *arrFullHouse = __Array::create();
                arrFullHouse->addObjectsFromArray(arrTriple);
                arrFullHouse->addObjectsFromArray(arrTwo);
                arrFullHousePair->addObject(arrFullHouse);
            }
        }
    }
    */
    
    // check for 5 cards
    for (int i = 0; i < arrAllCards->count() - 4; i++)
    {
        for (int j = i + 1; j < arrAllCards->count() - 3; j++)
        {
            for (int k = j + 1; k < arrAllCards->count() - 2; k++)
            {
                for (int l = k + 1; l < arrAllCards->count() - 1; l++)
                {
                    for (int m = l + 1; m < arrAllCards->count(); m++)
                    {
                        __Array *arrCards = __Array::create();
                        arrCards->addObject(arrAllCards->getObjectAtIndex(i));
                        arrCards->addObject(arrAllCards->getObjectAtIndex(j));
                        arrCards->addObject(arrAllCards->getObjectAtIndex(k));
                        arrCards->addObject(arrAllCards->getObjectAtIndex(l));
                        arrCards->addObject(arrAllCards->getObjectAtIndex(m));
                        
                        Big2PokerModel::Big2PokerCardPairInfo cardPairInfo = Big2PokerModel::getHigherCardPairFromArray(arrCards);
                        if (cardPairInfo < Big2PokerModel::Straight || cardPairInfo == Big2PokerModel::Bomb)
                            continue;
                        
                        __Array * arrPair = (__Array *)m_arrCardPairs->getObjectAtIndex(cardPairInfo);
                        arrPair->addObject(arrCards);
                    }
                }
            }
        }
    }
    
    Vec2 buttonPos = m_btnReselect->getPosition();
    for (int i = Big2PokerModel::Triple; i < Big2PokerModel::PairTypeCount; i++)
    {
        __Array * arrPair = (__Array *)m_arrCardPairs->getObjectAtIndex(i);
        if (arrPair->count())
        {
            ui::Button *btnPair = ui::Button::create("ui/Capsa/cardpair_placeholder.png");
            btnPair->setTag(i);
            btnPair->setTitleText(Big2PokerModel::getPairTitle(i));
            btnPair->setTitleFontSize(btnPair->getContentSize().height * 0.5f);
            btnPair->addClickEventListener(CC_CALLBACK_1(CapsaGameScene::onSelectCardPair, this));
            buttonPos.y += btnPair->getContentSize().height * 1.1f;
            btnPair->setPosition(buttonPos);
            m_cardPairLayer->addChild(btnPair);
        }
    }
    
    //m_hasCardToWin = //
}

CapsaGameScene::~CapsaGameScene()
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        m_cards[i]->removeAllObjects();
        m_cards[i]->release();
        m_thrownCards[i]->removeAllObjects();
        m_thrownCards[i]->release();
    }
    m_arrCardPairs->removeAllObjects();
    m_arrCardPairs->release();
}

void CapsaGameScene::onSelectCardPair(Ref * button)
{
    float cardOriginY = POS_SIZE(WIN_SIZE, (m_iPad ? gCardPosRatioForIPad[0] : gCardPosRatio[0])).y;
    __Array *arrAllCard = m_cards[m_playerIndex];
    for (int i = 0; i < arrAllCard->count(); i++)
    {
        CardSprite *card = (CardSprite *)arrAllCard->getObjectAtIndex(i);
        if (card->isSelected())
        {
            card->setSelected(false);
            card->setPositionY(cardOriginY);
        }
    }
    
    ui::Button *btnCardPair = (ui::Button *)button;
    int pairType = btnCardPair->getTag();
    __Array *arrPair = (__Array *)m_arrCardPairs->getObjectAtIndex(pairType);
    
    __Array *arrCard = (__Array *)arrPair->getObjectAtIndex(m_cardPairCounter[pairType]);
    for (int i = 0; i < arrCard->count(); i++)
    {
        CardSprite *cardSprite = (CardSprite *)arrCard->getObjectAtIndex(i);
        cardSprite->setSelected(true);
        float cardRiseY = cardSprite->getContentSize().height * 0.25f;
        cardSprite->setPositionY(cardOriginY + cardRiseY);
    }
    updateButtonPanel();
    
    m_cardPairCounter[pairType]++;
    m_cardPairCounter[pairType] %= arrPair->count();
}
