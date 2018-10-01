//
//  CasinowarHand.h
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 10.04.17.
//
//

#pragma once

#include "HandView.h"

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class CasinowarHand : public HandView
{
protected:
    CasinowarHand();
    
    bool init() override;
private:
    void addBet(CoinToken * token, bool animated = true) {};
    bool addInsurance() { return false; };
    void takeBet(float delay) {};
    void takeHalfBet(float delay) {};
    void takeInsurance(float delay) {};
    int  grabAllTokens(float delay) { return 0; };
    int  getBet() { return 0; };
    int  getInsurance() { return 0; };
    void addPayout(CoinToken * token, float delay) {};
public:
    static CasinowarHand * create();
    
    int getPoints();
};
