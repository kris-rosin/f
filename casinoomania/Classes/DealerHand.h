//
//  DealerHand.h
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 09.03.17.
//
//

#pragma once

#include "HandView.h"

class DealerHand : public HandView
{
private:
    DealerHand();
    
    bool init();
public:
    static DealerHand * create();
    
    bool ShouldHit();
    void Play(Shoe & shoe);
    
    bool isDealer() { return true; };
};
