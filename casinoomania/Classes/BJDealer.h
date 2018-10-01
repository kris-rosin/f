//
//  BJDealer.h
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 14.03.17.
//
//

#pragma once

#include "DealerHand.h"
#include "BJPlayer.h"

class BJDealer : public BJPlayer
{
private:
    BJDealer();
    
    bool init() override;
public:
    static BJDealer * create();
    
    void StartHand() override;
    bool isDealer() override;
    bool CanSplitHand(int handID) override;
    bool CanSplitHand(HandView * hand) override;
    
    virtual bool isInsuranceCase() override;
};
