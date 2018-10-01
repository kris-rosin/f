//
//  CapsaLobbyScene.cpp
//  DovemobiCasino
//
//  Created by liu on 9/6/17.
//
//

#include "CapsaLobbyScene.hpp"

#include "CapsaDefine.h"
#include "GameHandler.hpp"
#include "CapsaGameScene.hpp"
#include "CapsaThemeScene.hpp"
#include "PlayerProfile.h"
#include "CapsaTutorialScene.hpp"
#include "CapsaRuleLayer.hpp"

#include "App42API.h"
#include "App42Define.h"

#include "Label64.hpp"
#include "RoomSprite.hpp"
#include "CapsaPlayer.hpp"
#include "CapsaCreateRoomLayer.hpp"

#include "TopBar.h"
#include "BottomBar.h"


USING_NS_CC;

std::string CapsaLobbyScene::m_theme = "";

Scene * CapsaLobbyScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = CapsaLobbyScene::create();
    scene->addChild(layer);
    
    return scene;
}

void CapsaLobbyScene::setTheme(const std::string &theme)
{
    m_theme = theme;
}

bool CapsaLobbyScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    setupUI();
    setupAppwarp();
    
    return true;
}

void CapsaLobbyScene::setupUI()
{
    Sprite *bgSprite = Sprite::create("ui/Capsa/capsalobby_bg.png");
    addChild(bgSprite);
    bgSprite->setPosition(WIN_SIZE * 0.5f);
    
    m_roomView = Layer::create();
    addChild(m_roomView);
    
//    ui::Button *btnBack = ui::Button::create("ui/Capsa/capsalobby_back.png");
//    btnBack->addClickEventListener(CC_CALLBACK_0(CapsaLobbyScene::onBack, this));
//    addChild(btnBack);
//    btnBack->setPosition(Vec2(WIN_SIZE.width - VISIBLE_ORIGIN.x - btnBack->getContentSize().width * 0.8f, WIN_SIZE.height - btnBack->getContentSize().height * 0.8f));
    
    //topbar
    auto topbar = TopBar::create(std::bind(&CapsaLobbyScene::onBack, this));
    topbar->setPosition(Size(WIN_SIZE.width / 2, WIN_SIZE.height));
    addChild(topbar, 90);
    
    //bottombar
    auto bottombar = BottomBar::create();
    bottombar->setPosition(Size(WIN_SIZE.width / 2, 0));
    bottombar->setScale(Director::getInstance()->getVisibleSize().width / bottombar->getContentSize().width);
    addChild(bottombar, 90);
    
    m_btnCreate = ui::Button::create("ui/Capsa/capsalobby_roomcreate.png");
    addChild(m_btnCreate);
    m_btnCreate->setPosition(Vec2(WIN_SIZE.width * 0.5f, WIN_SIZE.height * 0.25f));
    m_btnCreate->addClickEventListener(CC_CALLBACK_0(CapsaLobbyScene::onWantToCreateRoom, this));
    m_btnCreate->setVisible(false);
}

void CapsaLobbyScene::onBack()
{
    uninstallAppwarp();
    AppWarp::Client::getInstance()->disconnect();
    GameHandler::getInstance()->setConnected(false);
    Director::getInstance()->replaceScene(CapsaThemeScene::createScene());
}

void CapsaLobbyScene::onWantToCreateRoom()
{
    GameHandler *handler = GameHandler::getInstance();
    if (!handler->getPlayerName().length())
        return;
    
    m_createRoomLayer = CapsaCreateRoomLayer::create();
    addChild(m_createRoomLayer, 500);
    m_createRoomLayer->setCallback(
                                   CC_CALLBACK_0(CapsaLobbyScene::onCreateRoom, this),
                                   CC_CALLBACK_0(CapsaLobbyScene::onRoomRule, this),
                                   CC_CALLBACK_0(CapsaLobbyScene::onRoomHelp, this));
}

void CapsaLobbyScene::onWantToPlay(Ref * buttonObj)
{
    ui::Button *btnPlay = (ui::Button *)buttonObj;
    RoomSprite *roomSprite = (RoomSprite *)btnPlay->getParent();
    GameHandler *handler = GameHandler::getInstance();
    std::map<std::string, std::string> properties;
    roomSprite->room()->toMap(properties);
    handler->currentRoom()->fromMap(properties);
    handler->currentRoom()->roomID = roomSprite->room()->roomID;
    AppWarp::Client::getInstance()->joinRoom(roomSprite->room()->roomID);
}

void CapsaLobbyScene::onCreateRoom()
{
    GameHandler *handler = GameHandler::getInstance();
    AppWarp::Client *warpClientRef = AppWarp::Client::getInstance();
    handler->makeRoom();
    std::map<std::string, std::string> roomProperties;
    CapsaRoom *pRoom = handler->currentRoom();
    pRoom->maxPlayers = m_createRoomLayer->getMaxPlayers();
    pRoom->maxTime = m_createRoomLayer->isFastSpeed() ? CAPSA_FAST_TURN_TIME : CAPSA_NORMAL_TURN_TIME;
    pRoom->isPrivate = m_createRoomLayer->isPrivateRoom();
    pRoom->deposite = m_createRoomLayer->getDeposite();
    pRoom->toMap(roomProperties);
    warpClientRef->createTurnRoom(pRoom->roomID, handler->getPlayerName(), pRoom->maxPlayers, roomProperties, pRoom->maxTime);
    m_createRoomLayer->removeFromParentAndCleanup(true);
}

void CapsaLobbyScene::onRoomRule()
{
    CapsaRuleLayer *m_createRuleLayer = CapsaRuleLayer::create();
    addChild(m_createRuleLayer, 500);
}

void CapsaLobbyScene::onRoomHelp()
{
    CapsaTutorialScene *m_createTutorialLayer = CapsaTutorialScene::create();
    addChild(m_createTutorialLayer, 500);
}

void CapsaLobbyScene::onEnter()
{
    Layer::onEnter();
    
    setupTouch(true);
}

void CapsaLobbyScene::onExit()
{
    setupTouch(false);
    
    uninstallAppwarp();
    
    Layer::onExit();
}

void CapsaLobbyScene::setupAppwarp()
{
    GameHandler *handler = GameHandler::getInstance();
    handler->initAppwarp();
    
    AppWarp::Client *warpClientRef = AppWarp::Client::getInstance();
    warpClientRef->setRecoveryAllowance(60);
    warpClientRef->setConnectionRequestListener(this);
    warpClientRef->setNotificationListener(this);
    warpClientRef->setRoomRequestListener(this);
    warpClientRef->setZoneRequestListener(this);
    warpClientRef->setLobbyRequestListener(this);
    
    std::string playerName = GameHandler::getInstance()->getPlayerName();
    if (!playerName.length())
        registerPlayer();
    else
    {
        bool needRegisterNewly = false;
        if (PlayerProfile::getInstance().getFBUserID().length())
        {
            std::string decodePlayerName = base64_decode(playerName);
            if (decodePlayerName.find(FBUSER_PREFIX) == std::string::npos)
                needRegisterNewly = true;
        }
        if (needRegisterNewly)
            registerPlayer();
        else
        {
            handler->showProcess(this, "Connecting...");
            if (handler->isConnected())
                connectedToAppwarp();
            else
                warpClientRef->connect(playerName);
        }
    }
}

void CapsaLobbyScene::uninstallAppwarp()
{
    AppWarp::Client *warpClientRef = AppWarp::Client::getInstance();
    if (m_enteredRoom)
        return;
    
    warpClientRef->setConnectionRequestListener(NULL);
    warpClientRef->setNotificationListener(NULL);
    warpClientRef->setRoomRequestListener(NULL);
    warpClientRef->setZoneRequestListener(NULL);
    warpClientRef->setLobbyRequestListener(NULL);
}

void CapsaLobbyScene::registerPlayer()
{
    PlayerProfile profile = PlayerProfile::getInstance();
    if (profile.getFBUserID().length())
    {
        std::stringstream ssPlayerName;
        ssPlayerName << profile.getFBUserName() << FBUSER_PREFIX << profile.getPlayerID();
        std::string playerName = ssPlayerName.str();
        
        unsigned char *szUserName = (unsigned char *)playerName.c_str();
        size_t len = strlen((const char *)szUserName);
        std::string encodedPlayerName = base64_encode(szUserName, (unsigned int)len);
        m_userName = encodedPlayerName;
        m_userEmail = encodedPlayerName;
        
        GameHandler *handler = GameHandler::getInstance();
        handler->showProcess(this, "Registering User...");
        
        retain();
        
        UserService *userService = App42API::BuildUserService();
        userService->CreateUser(m_userName.c_str(), "123456", m_userName.c_str(), app42callback(CapsaLobbyScene::onRegisteredPlayer, this));
    }
    else
    {
        registerByCount();
    }
}

void CapsaLobbyScene::registerByCount()
{
    UserService *userService = App42API::BuildUserService();
    userService->GetAllUsersCount(app42callback(CapsaLobbyScene::onGetUsers, this));
}

void CapsaLobbyScene::onGetUsers(void *response)
{
    App42UserResponse *userResponse = (App42UserResponse*)response;
    if (userResponse->isSuccess)
    {
        int nTotalUsers = userResponse->getTotalRecords() + 100000;
        std::stringstream ssPlayer;
        ssPlayer << "Guest" << nTotalUsers;
        
        std::string playerName = ssPlayer.str();
        
        unsigned char *szUserName = (unsigned char *)playerName.c_str();
        size_t len = strlen((const char *)szUserName);
        std::string encodedPlayerName = base64_encode(szUserName, (unsigned int)len);
        m_userName = encodedPlayerName;
        m_userEmail = encodedPlayerName;
        
        GameHandler *handler = GameHandler::getInstance();
        handler->showProcess(this, "Registering User...");
        
        retain();
        
        UserService *userService = App42API::BuildUserService();
        userService->CreateUser(m_userName.c_str(), "123456", m_userName.c_str(), app42callback(CapsaLobbyScene::onRegisteredPlayer, this));
    }
    else
    {
    }
}

void CapsaLobbyScene::onRegisteredPlayer(void *response)
{
    GameHandler *handler = GameHandler::getInstance();
    handler->hideProcess();
    
    App42UserResponse *userResponse = (App42UserResponse *)response;
    if (userResponse->isSuccess)
    {
        App42User user = userResponse->users.at(0);
        
        handler->setPlayerName(user.userName);
        handler->savePlayerName();
        
        handler->showProcess(this, "Connecting...");
        AppWarp::Client::getInstance()->connect(user.userName);
    }
    else
    {
        log("error details: %s", userResponse->errorDetails.c_str());
        log("error message: %s", userResponse->errorMessage.c_str());
        
        std::string details = userResponse->errorDetails;
        if (details.find(m_userName) != string::npos)
        {
            replace(details, m_userName, base64_decode(m_userName));
        }
        
        MessageBox(details.c_str(), userResponse->errorMessage.c_str());
    }
    
    release();
}

void CapsaLobbyScene::onConnectDone(int res, int reasonCode)
{
    GameHandler *handler = GameHandler::getInstance();
    handler->hideProcess();
    if (res==AppWarp::ResultCode::success)
    {
        unscheduleRecover();
        handler->setConnected(true);
        connectedToAppwarp();
    }
    else if (res==AppWarp::ResultCode::auth_error)
    {
        unscheduleRecover();
        // this->connectToAppWarp(this);
    }
    else if (res==AppWarp::ResultCode::success_recovered)
    {
        unscheduleRecover();
        handler->setConnected(true);
        connectedToAppwarp();
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

void CapsaLobbyScene::scheduleRecover()
{
    if (!this->isScheduled(schedule_selector(CapsaLobbyScene::recover)))
    {
        this->schedule(schedule_selector(CapsaLobbyScene::recover), CONNECTION_RECOVER_TIME);
        
        GameHandler *handler = GameHandler::getInstance();
        handler->showProcess(this, "Reconnecting ...");
    }
}

void CapsaLobbyScene::unscheduleRecover()
{
    unschedule(schedule_selector(CapsaLobbyScene::recover));
}

void CapsaLobbyScene::recover(float dt)
{
    AppWarp::Client::getInstance()->recoverConnection();
}

void CapsaLobbyScene::connectedToAppwarp()
{
    GameHandler::getInstance()->showProcess(this, "Loading Rooms...");
    GameHandler::getInstance()->sendPlayerProfile();
    AppWarp::Client::getInstance()->getAllRooms();
    AppWarp::Client::getInstance()->joinLobby();
}

void CapsaLobbyScene::onGetAllRoomsDone(AppWarp::liveresult res)
{
    GameHandler::getInstance()->hideProcess();
    if (res.result)
        return;
    
    m_btnCreate->setVisible(true);
    
    if (!res.list.size())
        return;
    
    m_roomView->removeAllChildrenWithCleanup(true);
    m_roomSprites.clear();
    
    for (int i = 0; i < res.list.size(); i++)
    {
        AppWarp::Client::getInstance()->getLiveRoomInfo(res.list.at(i));
    }
}

void CapsaLobbyScene::onGetLiveRoomInfoDone(AppWarp::liveroom revent)
{
    if (revent.result)
        return;
    
    if (!revent.users.size())
        return;
    
    if (m_roomSprites.size() < ROOM_VISIBLE_COUNT)
        addRoomSprite(revent);
}

void CapsaLobbyScene::addRoomSprite(const AppWarp::liveroom &revent)
{
    CapsaRoom room;
    room.fromMap(revent.properties);
    if (room.isFull())
        return;
    
    if (room.isPrivate)
        return;
    
    RoomSprite *roomSprite = RoomSprite::create();
    roomSprite->room()->fromMap(revent.properties);
    roomSprite->room()->roomID = revent.rm.roomId;
    roomSprite->room()->stripNonUsers(revent.users);
    roomSprite->updateRoom();
    for (int i = 0; i < revent.users.size(); i++)
        requestUserProfile(revent.users.at(i));
    roomSprite->getPlayButton()->addClickEventListener(CC_CALLBACK_1(CapsaLobbyScene::onWantToPlay, this));
    m_roomView->addChild(roomSprite);
    m_roomSprites.push_back(roomSprite);
    
    //AppWarp::Client::getInstance()->subscribeRoom(revent.rm.roomId);
    
    
    adjustRoomSpritePosition();
    adjustRoomSpriteScale();
}

void CapsaLobbyScene::adjustRoomSpritePosition()
{
    if (!m_roomSprites.size())
        return;
    
    float distanceBetweenRoom = m_roomSprites.at(0)->getContentSize().width * ROOM_DISTANCE_RATIO;
    
    Vec2 pos;
    pos.y = WIN_SIZE.height * 0.5f;
    for (int i = 0; i < m_roomSprites.size(); i++)
    {
        pos.x = WIN_SIZE.width * 0.5f + distanceBetweenRoom * i;
        m_roomSprites.at(i)->setPosition(pos);
    }
    m_roomViewerWidth = WIN_SIZE.width * 0.5f + pos.x;
}

void CapsaLobbyScene::adjustRoomSpriteScale()
{
    if (!m_roomSprites.size())
        return;
    
    float distanceBetweenRoom = m_roomSprites[0]->getContentSize().width * ROOM_DISTANCE_RATIO;
    Vec2 innerPos = m_roomView->getPosition();
    for (int i = 0; i < m_roomSprites.size(); i ++)
    {
        Sprite *sprite = (Sprite *)m_roomSprites.at(i);
        float posX = sprite->getPositionX() + innerPos.x;
        float diffFromCenter = fabs(WIN_SIZE.width / 2.0f - posX);
        
        float maxDecScale = 0.5f;
        float decreaseScale = MIN(maxDecScale, diffFromCenter / distanceBetweenRoom * maxDecScale);
        sprite->setScale(1.0f - decreaseScale);
    }
}

void CapsaLobbyScene::adjustRoomViewPosition()
{
    if (!m_roomSprites.size())
    {
        m_roomView->setPosition(Vec2::ZERO);
        return;
    }
    
    float distanceBetweenRoom = m_roomSprites[0]->getContentSize().width * ROOM_DISTANCE_RATIO;
    Vec2 innerPos = m_roomView->getPosition();
    int step = fabs(innerPos.x) / distanceBetweenRoom + 0.5f;
    innerPos.x = step * distanceBetweenRoom * (innerPos.x < 0 ? -1 : 1);
    
    MoveTo *moveTo = MoveTo::create(0.3f, innerPos);
    CallFunc *callFunc = CallFunc::create(CC_CALLBACK_0(CapsaLobbyScene::adjustRoomSpriteScale, this));
    Sequence *action = Sequence::create(moveTo, callFunc, NULL);
    m_roomView->runAction(action);
}

void CapsaLobbyScene::setupTouch(bool setup)
{
    if (setup)
    {
        auto pListener = EventListenerTouchOneByOne::create();
        pListener->setSwallowTouches(true);
        pListener->onTouchBegan = CC_CALLBACK_2(CapsaLobbyScene::onTouchBegan, this);
        pListener->onTouchMoved = CC_CALLBACK_2(CapsaLobbyScene::onTouchMoved, this);
        pListener->onTouchEnded = CC_CALLBACK_2(CapsaLobbyScene::onTouchEnded, this);
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(pListener, this);
    }
    else
    {
        getEventDispatcher()->removeEventListenersForTarget(this);
    }
}

bool CapsaLobbyScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    if (!m_roomSprites.size())
        return false;
    
    return true;
}

void CapsaLobbyScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    if (!m_roomSprites.size())
        return;
    
    Vec2 prevPos = touch->getPreviousLocation();
    Vec2 pos = touch->getLocation();
    Vec2 diff = pos - prevPos;
    diff.y = 0;
    Vec2 newPos = m_roomView->getPosition() + diff;
    newPos.x = MIN(0, newPos.x);
    newPos.x = MAX(-m_roomViewerWidth + WIN_SIZE.width, newPos.x);
    m_roomView->setPosition(newPos);
    
    adjustRoomSpriteScale();
}

void CapsaLobbyScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    adjustRoomViewPosition();
}

void CapsaLobbyScene::onCreateRoomDone (AppWarp::room revent)
{
    if (revent.result==0)
    {
        GameHandler *handler = GameHandler::getInstance();
        handler->showProcess(this, "Joining Room...");
        AppWarp::Client::getInstance()->joinRoom(revent.roomId);
    }
    else
        printf("\nonCreateRoomDone .. FAILED\n");
}

void CapsaLobbyScene::onJoinRoomDone(AppWarp::room revent)
{
    GameHandler *handler = GameHandler::getInstance();
    if (revent.result)
    {
        handler->hideProcess();
        return;
    }
    
    uninstallAppwarp();
    
    handler->currentRoom()->roomID = revent.roomId;
    handler->hideProcess();
    AppWarp::Client::getInstance()->subscribeRoom(revent.roomId);
    
    m_enteredRoom = true;
    Director::getInstance()->replaceScene(CapsaGameScene::createScene());
}

RoomSprite * CapsaLobbyScene::getRoomSpriteByRoomID(const std::string &roomID, int *index)
{
    for (int i = 0; i < m_roomSprites.size(); i++)
    {
        RoomSprite *roomSprite = m_roomSprites.at(i);
        if (!roomSprite->room()->roomID.compare(roomID))
        {
            if (index)
                *index = i;
            return roomSprite;
        }
    }
    if (index)
        *index = -1;
    return NULL;
}

void CapsaLobbyScene::onJoinLobbyDone(AppWarp::lobby levent)
{
    if (levent.result)
        return;
    
    AppWarp::Client *warpClientRef = AppWarp::Client::getInstance();
    warpClientRef->subscribeLobby();
}

void CapsaLobbyScene::onLeaveLobbyDone(AppWarp::lobby levent)
{
    if (levent.result)
        return;
    
    AppWarp::Client *warpClientRef = AppWarp::Client::getInstance();
    warpClientRef->unsubscribeLobby();
}

void CapsaLobbyScene::onRoomCreated(AppWarp::room rData)
{
    if (!rData.owner.compare(GameHandler::getInstance()->getPlayerName()))
        return;
    
    AppWarp::Client *warpClientRef = AppWarp::Client::getInstance();
    warpClientRef->getLiveRoomInfo(rData.roomId);
}

void CapsaLobbyScene::onRoomDestroyed(AppWarp::room rData)
{
    int index = -1;
    RoomSprite *roomSprite = getRoomSpriteByRoomID(rData.roomId, &index);
    if (!roomSprite)
        return;
    
    m_roomSprites.erase(m_roomSprites.begin() + index);
    roomSprite->removeFromParentAndCleanup(true);
    adjustRoomSpritePosition();
    adjustRoomSpriteScale();
}

void CapsaLobbyScene::onUserJoinedRoom(AppWarp::room rData, std::string user)
{
    
}

void CapsaLobbyScene::onUserLeftRoom(AppWarp::room rData, std::string user)
{
    int index = -1;
    RoomSprite *roomSprite = getRoomSpriteByRoomID(rData.roomId, &index);
    if (!roomSprite)
    {
        if (m_roomSprites.size() < ROOM_VISIBLE_COUNT)
            AppWarp::Client::getInstance()->getLiveRoomInfo(rData.roomId);
        return;
    }
    roomSprite->room()->removeUser(user);
    roomSprite->updateRoom();
    
    if (!roomSprite->room()->getPlayerCount())
    {
        m_roomSprites.erase(m_roomSprites.begin() + index);
        roomSprite->removeFromParentAndCleanup(true);
        
        adjustRoomSpritePosition();
        adjustRoomSpriteScale();
    }
}

void CapsaLobbyScene::onUserChangeRoomProperty(AppWarp::room rData, std::string user, std::map<std::string, std::string> properties, std::map<std::string, std::string> lockTable)
{
    int index = -1;
    RoomSprite *roomSprite = getRoomSpriteByRoomID(rData.roomId, &index);
    if (!roomSprite)
        return;
    
    roomSprite->room()->fromMap(properties);
    if (roomSprite->room()->isFull())
    {
        m_roomSprites.erase(m_roomSprites.begin() + index);
        roomSprite->removeFromParentAndCleanup(true);
        adjustRoomSpritePosition();
        adjustRoomSpriteScale();
    }
    else
    {
        requestUserProfile(user);
        roomSprite->updateRoom();
    }
}

void CapsaLobbyScene::requestUserProfile(const std::string & user)
{
    AppWarp::Client::getInstance()->getLiveUserInfo(user);
}

void CapsaLobbyScene::onGetLiveUserInfoDone(AppWarp::liveuser uevent)
{
    if (uevent.result)
        return;
    CapsaPlayer player;
    player.fromJSONString(uevent.customData);
    
    for (int i = 0; i < m_roomSprites.size(); i++)
    {
        RoomSprite *roomSprite = m_roomSprites.at(i);
        if (!roomSprite->getPlayerNode(i)->getPlayerName().compare(uevent.name))
        {
            roomSprite->getPlayerNode(i)->updatePlayer(player, true);
        }
    }
}
