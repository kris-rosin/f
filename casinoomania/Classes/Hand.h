//
//  Hand.h
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 07.03.17.
//
//

#pragma once

#include <vector>

#include "Shoe.h"
#include "CardObject.h"

class Hand
{
public:
    enum HandStatus
    {
        BLACKJACK = 0,
        BUST      = 1,
        VALUE21   = 2,
        OPTIONS   = 3
    };
protected:
    std::vector<CardObject> cards;
    
    bool stand;
    bool ace_splitted;
    bool splitted;
public:
    Hand();
    
    bool CanSplit();
    bool HasAce(bool withClosedCards = false);
    bool HasTwoAces();
    bool CanHit();
    bool CanSurrender();
    void Hit(CardObject);
    bool FirstOfTwoCardsIsAce();
    bool isSplitted(bool withAces = false);
    
    void setCards(std::vector<CardObject> new_cards);
    
    std::vector<int> getTotal(bool withClosedCards = false);
    
    HandStatus GetStatus();
};
