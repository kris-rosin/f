//
//  BJPlayer.h
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 10.03.17.
//
//

#pragma once

#include <vector>
#include <string>
#include "HandView.h"
#include "Shoe.h"

#include "cocos2d.h"

class BJPlayer : public cocos2d::Node
{
    friend class BJGame;
protected:
    std::string Name;
    
    cocos2d::Vector<HandView*> hands;
    
    BJPlayer();
    
    bool init(std::string name);
public:
    int totalWin;
    int totalLoose;
public:
    static BJPlayer * create(std::string name);
    
    bool CanPlaceBet(int bet);
    void FinishRound();
    
    virtual void StartHand();
    virtual bool isDealer();
    virtual bool CanSplitHand(int handID);
    virtual bool CanSplitHand(HandView * hand);
    
    HandView * SplitHand(int handID, Shoe & shoe);
    HandView * SplitHand(HandView * hand, Shoe & shoe);
    
    long TotalBet(bool includingInsurance = false);
    
    cocos2d::Vector<HandView*> & getHands();
    
    //dealer related helpers
    virtual bool isInsuranceCase() { return false; };
};
