//
//  HandView.h
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 20.03.17.
//
//

#pragma once

#include "HandView.h"
#include "Hand.h"
#include "CoinToken.h"

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class HandView : public cocos2d::Node, public Hand
{
public:
    enum HandComparisonResult
    {
        WON,
        FAILED,
        EQUALS
    };
protected:
    cocos2d::Vector<cocos2d::Sprite*> cardSprites;
    
    cocos2d::Node * cardsRoot;
    cocos2d::ui::Scale9Sprite * pointsView;
    cocos2d::Label * pointsLabel;
    
    CoinToken * betNode;
    CoinToken * insuranceNode;
    CoinToken * payoutNode;
    
    cocos2d::Size contentSize;
protected:
    HandView();
    
    bool init();
    
    void setPoints(std::string pointsString);
    virtual std::string getPointsString();
    
    bool onTokenButtonPressed(CoinToken::CoinTokenEvent event, CoinToken * token);
public:
    static HandView * create();
    
    HandView * Split();
    
    void Hit(CardObject card, float delay, std::function<void()> completionCallback = nullptr, bool forceHidden = false);
    
    virtual bool isDealer() { return false; };
    
    void openCard(int cardID);
    
    void showPoints(bool value);
    
    void addBet(CoinToken * token, bool animated = true);
    bool addInsurance();
    void takeBet(float delay);
    void takeHalfBet(float delay);
    void takeInsurance(float delay);
    int  grabAllTokens(float delay, bool animate = true);
    void removeCards(float delay, bool animated = true);
    
    int getBet();
    int getInsurance();
    
    void addPayout(CoinToken * token, float delay);
    
    HandComparisonResult compareTo(HandView * otherHand);
    
    cocos2d::Size & getContentSize();
    
    //dealer related
    virtual void Play(Shoe & shoe) {};
};
