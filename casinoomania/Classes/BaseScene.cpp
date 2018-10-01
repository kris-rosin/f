#include "BaseScene.h"

#include "ServerConnection.h"
#include "ServerMessage.h"
#include "MessageAlert.h"
#include "PlayerProfile.h"
#include "LoadingScene.h"

USING_NS_CC;

bool BaseScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

void BaseScene::increasePlayerValues(int coinsChange, unsigned int experienceChange, bool levelUp)
{
    PlayerProfile::getInstance().addCoins(coinsChange);
    PlayerProfile::getInstance().addExperience(experienceChange);
    
    SyncData(coinsChange, experienceChange, levelUp ? 1 : 0);
}

void BaseScene::SyncData(int coinsChange, unsigned int expChange, unsigned int levelChange)
{
    auto message = new SyncMoneyChange(coinsChange, expChange, levelChange);
    ServerConnection::getInstance().sendMessage(message, this, std::bind(&BaseScene::onDataSync, this, std::placeholders::_1));
    delete message;
}

bool BaseScene::onDataSync(ServerMessage * message)
{
    auto result = dynamic_cast<SyncMoneyResult*>(message);
    
    if (result)
    {
        PlayerProfile::getInstance().setCoins(result->getCoins());
        PlayerProfile::getInstance().setExperience(result->getExperience());
        PlayerProfile::getInstance().setLevel(result->getLevel());
        PlayerProfile::getInstance().Save();
    }
    else if (handleServerError(message))
    {
        return true;
    }
    
    return true;
}

bool BaseScene::handleServerError(ServerMessage * message)
{
    auto error = dynamic_cast<ErrorMessage*>(message);
    
    if (error)
    {
        switch (error->getType())
        {
            case ErrorMessageTypeMultipleSessions:
            {
                MessageAlert::create("Game is running on other device. Try again later.", false, "Try Again", [this]()
                {
                    Director::getInstance()->replaceScene(LoadingScene::createScene());
                })->show(this);
                
                return true;
            }
                break;
                
            case ErrorMessageTypeNoConnection:
            {
                MessageAlert::create("No Internel connection. Game requires Internet connection. Please, connect to Internet and try again.", false, "Try Again", [this]()
                {
                    Director::getInstance()->replaceScene(LoadingScene::createScene());
                })->show(this);
                
                return true;
            }
                break;
            default:
                break;
        }
    }
    
    return false;
}

BaseScene * BaseScene::findBaseScene()
{
    auto scene = Director::getInstance()->getRunningScene();
    
    if (scene)
    {
        for (auto &item: scene->getChildren())
        {
            auto base = dynamic_cast<BaseScene*>(item);
            
            if (base)
            {
                return base;
            }
        }
    }
    
    return nullptr;
}
