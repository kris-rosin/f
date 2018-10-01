//
//  Deck.h
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 06.03.17.
//
//

#pragma once

#include <list>

#include "CardObject.h"

class Deck
{
    friend class Shoe;
    friend class BaccaratDeck;
    friend class CasinowarDeck;
protected:
    std::list<CardObject> cards;
public:
    Deck();
    Deck(std::list<CardObject> cards);
    
    void Sort();
    void Shuffle();
    
    virtual CardObject DrawCard();
    void PushCard(CardObject card);
};
