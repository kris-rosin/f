//
//  BJDealer.cpp
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 14.03.17.
//
//

#include "BJDealer.h"
#include "PlayerProfile.h"
#include "BJConfig.h"

BJDealer::BJDealer()
{
    
}

BJDealer * BJDealer::create()
{
    BJDealer * pRet = new BJDealer();
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

bool BJDealer::init()
{
    if (!BJPlayer::init("Dealer"))
        return false;
    
    return true;
}

void BJDealer::StartHand()
{
    auto newHand = DealerHand::create();
    
    addChild(newHand);
    
    hands.pushBack(newHand);
}

bool BJDealer::isDealer()
{
    return true;
}

bool BJDealer::CanSplitHand(int handID)
{
    return false;
}

bool BJDealer::CanSplitHand(HandView * hand)
{
    return false;
}

bool BJDealer::isInsuranceCase()
{
    return hands.at(0)->FirstOfTwoCardsIsAce();
}
