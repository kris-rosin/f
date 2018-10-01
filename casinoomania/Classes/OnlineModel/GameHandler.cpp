//
//  GameHandler.cpp
//  DovemobiCasino
//
//  Created by liu on 9/7/17.
//
//

#include "GameHandler.hpp"

#include "App42Define.h"
#include "App42API.h"
#include "appwarp.h"

#include "CardSprite.hpp"
#include "CapsaDefine.h"
#include "Label64.hpp"
#include "ProcessLayer.hpp"
#include "PlayerProfile.h"

USING_NS_CC;

GameHandler * GameHandler::m_sharedInstance = NULL;
GameHandler * GameHandler::getInstance()
{
    if (!m_sharedInstance)
    {
        m_sharedInstance = new GameHandler();
        m_sharedInstance->init();
    }
    return m_sharedInstance;
}

bool GameHandler::init()
{
    if (!Node::init())
        return false;
    
    loadSettings();
    loadCards();
    
    return true;
}

void GameHandler::initAppwarp()
{
    if (m_initedAppwarp)
        return;
    
    App42API::Initialize(APP42_API_KEY, APP42_SEC_KEY);
    AppWarp::Client::initialize(APP42_API_KEY, APP42_SEC_KEY);
    
    m_initedAppwarp = true;
}

void GameHandler::loadSettings()
{
    UserDefault *settings = UserDefault::getInstance();
    m_player.username = settings->getStringForKey("playername", "");
//    m_playerEmail = settings->getStringForKey("playeremail", "");
//    m_coin = settings->getIntegerForKey("coin", DEFAULT_COIN_COUNT);
}

std::string GameHandler::getPlayerName()
{
    return m_player.username;
}

void GameHandler::setPlayerName(const std::string &playerName)
{
    m_player.username = playerName;
}

void GameHandler::savePlayerName()
{
    UserDefault *settings = UserDefault::getInstance();
    settings->setStringForKey("playername", m_player.username);
    settings->flush();
    
//    saveRequestChip();
}

void GameHandler::showProgress(bool show)
{
    
}

void GameHandler::showProcess(Layer * parent, const std::string &title)
{
    if (!m_processLayer)
    {
        m_processLayer = ProcessLayer::create();
        m_processLayer->retain();
    }
    
    hideProcess();
    parent->addChild(m_processLayer, 100);
    m_processLayer->setTitle(title);
}

void GameHandler::hideProcess()
{
    if (m_processLayer)
    {
        if (m_processLayer->getParent())
            m_processLayer->removeFromParentAndCleanup(true);
    }
}

void GameHandler::loadCards()
{
    m_cards = __Array::create();
    m_cards->retain();
    for (int i = 0; i < MAX_CARDS; i++)
    {
        m_cards->addObject(CardSprite::create(i));
    }
}

CardSprite * GameHandler::getCard(int cardNumber)
{
    return (CardSprite *)m_cards->getObjectAtIndex(cardNumber);
}

void GameHandler::resetCards()
{
    m_cards->removeAllObjects();
    for (int i = 0; i < MAX_CARDS; i++)
    {
        m_cards->addObject(CardSprite::create(i));
    }
}

void GameHandler::makeRoom()
{
    m_capsaRoom.init();
    std::string roomID = CapsaRoom::generateRandomRoomID();
    m_capsaRoom.roomID = roomID;
    m_capsaRoom.players[0] = m_player.username;
    m_capsaRoom.master = m_player.username;
}

CapsaRoom * GameHandler::currentRoom()
{
    return &m_capsaRoom;
}

bool GameHandler::isMaster()
{
    return !m_capsaRoom.master.compare(m_player.username);
}

void GameHandler::setConnected(bool connected)
{
    m_connected = connected;
}

bool GameHandler::isConnected()
{
    return m_connected;
}

void GameHandler::sendPlayerProfile()
{
    if (!m_player.username.length())
        return;
    
    m_player.init(PlayerProfile::getInstance());
    
    AppWarp::Client::getInstance()->setCustomUserData(m_player.username, m_player.toJSONString());
}

int GameHandler::getSelectedCityIndex()
{
    return m_selectedIndex;
}

void GameHandler::setSelectedCityIndex(int index)
{
    
}
