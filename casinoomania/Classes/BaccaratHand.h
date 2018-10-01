//
//  BaccaratHand.h
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 28.03.17.
//
//

#pragma once

#include "HandView.h"

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class BaccaratHand : public HandView
{
protected:
    BaccaratHand();
    
    bool init() override;
    
    std::string getPointsString() override;
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
    static BaccaratHand * create();
    
    int getPoints();
};
