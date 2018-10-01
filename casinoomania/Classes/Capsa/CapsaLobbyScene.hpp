//
//  CapsaLobbyScene.hpp
//  DovemobiCasino
//
//  Created by liu on 9/6/17.
//
//

#ifndef CapsaLobbyScene_hpp
#define CapsaLobbyScene_hpp

#include "cocos2d.h"
USING_NS_CC;

#include "ui/CocosGUI.h"

#include "appwarp.h"

class RoomSprite;
class CapsaCreateRoomLayer;

class CapsaLobbyScene : public cocos2d::Layer, public AppWarp::ConnectionRequestListener,public AppWarp::RoomRequestListener,public AppWarp::NotificationListener,public AppWarp::ZoneRequestListener, public AppWarp::LobbyRequestListener
{
public:
    static
    cocos2d::Scene *    createScene();
    
    static
    void                setTheme(const std::string &theme);
    
    virtual
    bool                init() override;
  
    CREATE_FUNC(CapsaLobbyScene);
    
private:
    void                setupUI();

    void                onBack();
    
    void                setupAppwarp();
    void                uninstallAppwarp();
    
    void                registerPlayer();
    void                registerByCount();
    
    void                onRegisteredPlayer(void *response);
    void                onGetUsers(void *response);
    
    void                scheduleRecover();
    void                unscheduleRecover();
    void                recover(float);
    
    void                connectedToAppwarp();
    
    void                onWantToCreateRoom();
    void                onWantToPlay(Ref *);
    
    void                onCreateRoom();
    void                onRoomRule();
    void                onRoomHelp();
    
    void                addRoomSprite(const AppWarp::liveroom & revent);
    void                adjustRoomSpriteScale();
    void                adjustRoomSpritePosition();
    void                adjustRoomViewPosition();
    
    RoomSprite *        getRoomSpriteByRoomID(const std::string & roomID, int *index = NULL);
    
    void                setupTouch(bool);
    
    void                requestUserProfile(const std::string &);
    
    static
    std::string         m_theme;
    
    
    std::string         m_userName;
    std::string         m_userEmail;
    
    Layer *             m_roomView;
    std::vector<RoomSprite *> m_roomSprites;
    
    ui::Button *        m_btnCreate;
    
    float               m_roomViewerWidth;
    
    CapsaCreateRoomLayer *  m_createRoomLayer;
    
    bool                m_enteredRoom;
    
protected:
    virtual
    void                onEnter() override;
    virtual
    void                onExit() override;
    
    virtual
    void                onConnectDone(int, int) override;
    virtual
    void                onGetAllRoomsDone (AppWarp::liveresult res) override;
    virtual
    void                onGetLiveRoomInfoDone(AppWarp::liveroom revent) override;
    virtual
    void                onCreateRoomDone (AppWarp::room revent) override;
    virtual
    void                onJoinRoomDone(AppWarp::room revent) override;
    
    virtual
    void                onJoinLobbyDone(AppWarp::lobby levent) override;
    virtual
    void                onLeaveLobbyDone(AppWarp::lobby levent) override;
    
    virtual
    void                onRoomCreated(AppWarp::room rData) override;
    virtual
    void                onRoomDestroyed(AppWarp::room rData) override;
    virtual
    void                onUserLeftRoom(AppWarp::room rData, std::string user) override;
    virtual
    void                onUserJoinedRoom(AppWarp::room rData, std::string user) override;
    virtual
    void                onUserChangeRoomProperty(AppWarp::room rData, std::string user,std::map<std::string, std::string> properties, std::map<std::string, std::string>lockTable) override;
    
    virtual
    void                onGetLiveUserInfoDone (AppWarp::liveuser uevent) override;
    
    virtual
    bool                onTouchBegan(Touch *touch, Event *unused_event) override;
    virtual
    void                onTouchMoved(Touch *touch, Event *unused_event) override;
    virtual
    void                onTouchEnded(Touch *touch, Event *unused_event) override;

};

#endif /* CapsaLobbyScene_hpp */
