#pragma once

#include "cocos2d.h"

#include "BaseScene.h"

class ServerMessage;

class LoadingScene : public BaseScene
{
private:
    float delaySeconds;
    
    bool timeSynced;
    bool profileCreated;
    bool migrateFromOldVersion;
    
    int attemps;
private:
    void SyncTime();
    bool onTimeSynced(ServerMessage * message);
    
    void InitProfile();
    void LoadProfile();
    void CreateProfile();
    
    bool onProfileLoaded(ServerMessage * message);
    bool onSessionCreated(ServerMessage * message);
    bool onDataSaved(ServerMessage * message);
public:
    static cocos2d::Scene * createScene();

    virtual bool init();
    
    virtual void update(float deltaTime);
    
    CREATE_FUNC(LoadingScene);
};
