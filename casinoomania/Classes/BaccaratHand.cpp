//
//  BaccaratHand.cpp
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 28.03.17.
//
//

#include "BaccaratHand.h"

USING_NS_CC;

BaccaratHand::BaccaratHand()
{
    
}

BaccaratHand * BaccaratHand::create()
{
    BaccaratHand * pRet = new BaccaratHand();
    if (pRet && pRet->init())
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

bool BaccaratHand::init()
{
    if (!HandView::init())
        return false;
    
    //remove BJ features
    if (betNode && betNode->getParent())
    {
        betNode->getParent()->removeFromParent();
        betNode = payoutNode = insuranceNode = nullptr;
    }
    
    return true;
}

int BaccaratHand::getPoints()
{
    int total = 0;
    
    for (auto &card: cards)
    {
        total += card.BaccaratValue();
    }
    
    return total % 10;
}

std::string BaccaratHand::getPointsString()
{
    return StringUtils::format("%d", getPoints());
}
