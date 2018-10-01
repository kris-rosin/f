//
//  BaccaratDeck.h
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 28.03.17.
//
//

#pragma once

#include <list>

#include "Deck.h"

class BaccaratDeck : public Deck
{
    int decksnum;
    
    bool reshuffled;
public:
    BaccaratDeck(int decksNumber);
    
    void setDecksNumber(int decksNumber);
    
    bool CanPlay();
    
    virtual CardObject DrawCard() override;
};
