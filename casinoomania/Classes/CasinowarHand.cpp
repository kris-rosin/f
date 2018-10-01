//
//  CasinowarHand.cpp
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 10.04.17.
//
//

#include "CasinowarHand.h"

USING_NS_CC;

CasinowarHand::CasinowarHand()
{
    
}

CasinowarHand * CasinowarHand::create()
{
    CasinowarHand * pRet = new CasinowarHand();
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

bool CasinowarHand::init()
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

int CasinowarHand::getPoints()
{
    if (cards.size() > 0)
        return cards.back().CasinowarValue();
    
    return 0;
}
