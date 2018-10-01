#include "LoadingScene.h"

#include "MainMenuScene.h"
#include "ServerConnection.h"
#include "ServerMessage.h"
#include "Platform.h"
#include "PlayerProfile.h"
#include "MessageAlert.h"

USING_NS_CC;

Scene * LoadingScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = LoadingScene::create();
    scene->addChild(layer);

    return scene;
}

bool LoadingScene::init()
{
    if ( !BaseScene::init() )
    {
        return false;
    }
    
    timeSynced = false;
    profileCreated = false;
    migrateFromOldVersion = false;
    
    auto winSize = Director::getInstance()->getWinSize();

    auto sprite = Sprite::create("ui/dovemobi_logo.png");

    sprite->setPosition(winSize / 2);

    this->addChild(sprite, 0);
    
    delaySeconds = 2.0f;
    
    scheduleUpdate();
    
    attemps = 0;
    
    SyncTime();
    
    return true;
}

void LoadingScene::update(float deltaTime)
{
    delaySeconds -= deltaTime;
    
    if (delaySeconds <  0 && timeSynced && profileCreated)
    {
        unscheduleUpdate();
        
        auto scene = MainMenuScene::createScene();
        
        Director::getInstance()->replaceScene(TransitionCrossFade::create(1.0f, scene));
    }
}

void LoadingScene::SyncTime()
{
    auto message = new SyncTimeMessage(Platform::unixTime());
    ServerConnection::getInstance().sendMessage(message, this, std::bind(&LoadingScene::onTimeSynced, this, std::placeholders::_1));
    delete message;
}

bool LoadingScene::onTimeSynced(ServerMessage * message)
{
    auto result = dynamic_cast<SyncTimeResultMessage*>(message);
    
    if (result)
    {
        Platform::setDeltaTime(result->getDelta());
        timeSynced = true;
        
        InitProfile();
    }
    else
    {
        if (handleServerError(message))
        {
            return true;
        }
        
        attemps++;
        
        //TODO: check attempts, and show error
        
        SyncTime();
    }
    
    return true;
}

void LoadingScene::InitProfile()
{
    // session
    attemps = 0;
    
    if(ServerConnection::getInstance().isSessionCreated())
    {
        LoadProfile();
    }
    else
    {
        CreateProfile();
    }
}

void LoadingScene::LoadProfile()
{
    auto message = new GetUserDataMessage();
    ServerConnection::getInstance().sendMessage(message, this, std::bind(&LoadingScene::onProfileLoaded, this, std::placeholders::_1));
    delete message;
}

void LoadingScene::CreateProfile()
{
    auto message = new CreateSessionMessage();
    ServerConnection::getInstance().sendMessage(message, this, std::bind(&LoadingScene::onSessionCreated, this, std::placeholders::_1));
    delete message;
}

bool LoadingScene::onProfileLoaded(ServerMessage * message)
{
    auto result = dynamic_cast<UserDataMessage*>(message);
    
    if (result)
    {
        if (migrateFromOldVersion)
        {
            PlayerProfile::getInstance().setPlayerID(result->getUserID());
            PlayerProfile::getInstance().Save();
            
            auto data = PlayerProfile::getInstance().getDataFile();
            
            if (data)
            {
                auto message = new UserDataMessage(PlayerProfile::getInstance().getPlayerID(), PlayerProfile::getInstance().getUserName(), PlayerProfile::getInstance().getLevel(), PlayerProfile::getInstance().getExperience(), PlayerProfile::getInstance().getCoins(), ::Platform::unixTime(), PlayerProfile::getInstance().getFBUserID(), data);
                ServerConnection::getInstance().sendMessage(message, this, std::bind(&LoadingScene::onDataSaved, this, std::placeholders::_1));
                delete message;
            }
        }
        else
        {
            if (PlayerProfile::getInstance().getTimestamp() < result->getTimestamp() || PlayerProfile::getInstance().getPlayerID() != result->getUserID())
            {
                if (result->getSaveFileData())
                {
                    //overwrite savefile and load it again!
                    PlayerProfile::getInstance().reinitWithData(result->getSaveFileData());
                }
            }
            
            //update values one by one
            PlayerProfile::getInstance().setUserName(result->getUserName());
            PlayerProfile::getInstance().setPlayerID(result->getUserID());
            PlayerProfile::getInstance().setLevel(result->getUserLevel());
            PlayerProfile::getInstance().setExperience(result->getUserExperience());
            PlayerProfile::getInstance().setCoins(result->getCoins());
            PlayerProfile::getInstance().setFBUserID(result->getFacebookID());
            PlayerProfile::getInstance().setEmail(result->getEmail());
            
            profileCreated = true;
        }
    }
    else
    {
        if (handleServerError(message))
        {
            return true;
        }
        
        attemps++;
        
        LoadProfile();
    }
    
    return true;
}

bool LoadingScene::onDataSaved(ServerMessage * message)
{
    auto result = dynamic_cast<GenericRequestResult*>(message);
    
    if (result)
    {
        log("Data saved on Server!");
        profileCreated = true;
    }
    
    return true;
}

bool LoadingScene::onSessionCreated(ServerMessage * message)
{
    auto result = dynamic_cast<NewSessionMessage*>(message);
    
    if (result)
    {
        ServerConnection::getInstance().setSessionID(result->getSession());
        attemps = 0;
        
        if (PlayerProfile::getInstance().getExperience() > 0 || PlayerProfile::getInstance().getFBUserID().empty() == false)
        {
            migrateFromOldVersion = true;
        }

        LoadProfile();
    }
    else
    {
        if (handleServerError(message))
        {
            return true;
        }
        
        attemps++;
        
        CreateProfile();
    }
    
    return true;
}

