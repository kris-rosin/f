//
//  CoinsRainView.cpp
//
//  Created by Stanislav Zheronkin on 14.12.16.
//
//

#include "CoinsRainView.h"
#include "AudioManager.h"
#include "AndroidCompatibility.h"

USING_NS_CC;

CoinsRainView::CoinsRainView() : totalTime(0)
{
    
}

CoinsRainView * CoinsRainView::create(float multiple)
{
    CoinsRainView * pRet = new CoinsRainView();
    if (pRet && pRet->init(multiple))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool CoinsRainView::init(float multiple)
{
    if (!Node::init())
        return false;
    
    if (multiple < 1.0f)
    {
        coinsAmount = 9;
    }
    else if (multiple < 2.0f)
    {
        coinsAmount = 12;
    }
    else
    {
        coinsAmount = 24;
    }
    
    setContentSize(Director::getInstance()->getVisibleSize());
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    animate();
    scheduleUpdate();
    
    return true;
}

void CoinsRainView::show(cocos2d::Node * parent)
{
    AudioManager::getInstance().PlaySoundEffect("music/SlotsSound/BigCoins");
    parent->addChild(this, 102);
    
    setPosition(parent->getContentSize() / 2);
    
    animate();
}

void CoinsRainView::hide()
{
    unscheduleUpdate();
    removeFromParentAndCleanup(true);
}

void CoinsRainView::update(float deltaTime)
{
    totalTime += deltaTime;
    
    if (totalTime > 3.3f)
    {
        hide();
        return;
    }
    
    float alpha = totalTime > 3.0f ? 1.0f - (totalTime - 3.0f) / 0.3f : 1.0;
    int   step = Director::getInstance()->getVisibleSize().width / (coinsAmount + 1);
    
    for (int i = 0; i < coinsAmount; i++)
    {
        if (coins[i] == nullptr || (coins[i]->getPositionY() + coins[i]->getContentSize().height / 2) < 0 )
        {
            if (coins[i])
            {
                coins[i]->removeFromParentAndCleanup(true);
            }
            
            coins[i] = Sprite::create(StringUtils::format("effects/drop_coin_%d.png", (rand() % 3) + 1));
            addChild(coins[i]);
            coins[i]->setPosition(Vec2((i + 1) * step, Director::getInstance()->getWinSize().height + coins[i]->getContentSize().height / 2 ));
            //hackyhack_publish - arc4random_uniform
//            coinSpeed[i] = -80.0f * (7 + 4 * ( (int)arc4random_uniform(6) + 1) );
            coinSpeed[i] = -80.0f * (7 + 4 * ( (int)(rand() % 6) + 1) );
        }
        
        coins[i]->setPosition(coins[i]->getPosition() + Vec2(0, coinSpeed[i] * deltaTime));
        coins[i]->setOpacity(alpha * 255);
    }
}

void CoinsRainView::animate()
{
    coins.resize(coinsAmount, nullptr);
    coinSpeed.resize(coinsAmount, 0);
}
