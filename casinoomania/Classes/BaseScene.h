#pragma once

#include "cocos2d.h"

class ServerMessage;

class BaseScene : public cocos2d::Layer
{
protected:
    bool handleServerError(ServerMessage * message);
    
    void SyncData(int coinsChange, unsigned int expChange, unsigned int levelChange);
    
    bool onDataSync(ServerMessage * message);
public:
    virtual bool init();
    void increasePlayerValues(int coinsChange, unsigned int experienceChange, bool levelUp);
    
    static BaseScene * findBaseScene();
};
