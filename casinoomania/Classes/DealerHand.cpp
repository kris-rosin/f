//
//  DealerHand.cpp
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 09.03.17.
//
//

#include "DealerHand.h"

#include "PlayerProfile.h"

DealerHand::DealerHand()
{
    
}

DealerHand * DealerHand::create()
{
    DealerHand * pRet = new DealerHand();
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

bool DealerHand::init()
{
    if (!HandView::init())
        return false;
    
    return true;
}

bool DealerHand::ShouldHit()
{
    bool hitsoft17 = PlayerProfile::getInstance().getUIntValue("BlackJack_Soft17", 1);
    
    auto totals = getTotal();
    
    if(hitsoft17)
    {
        if(totals.size() == 2 && (totals[1] < 18))
        {
            return true;
        }
        else if(totals.size() == 1 && (totals[0] < 17))
        {
            return true;
        }
        
        return false;
    }
    else
    {
        return (totals.back() < 17);
    }
}

void DealerHand::Play(Shoe &shoe)
{
    while(ShouldHit())
    {
        Hit(shoe.DrawCard(), 0);
    }
    
    stand = true;
}
