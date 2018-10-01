//
//  CasinowarDeck.h
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 10.04.17.
//
//

#pragma once

#include <list>

#include "Deck.h"

class CasinowarDeck : public Deck
{
private:
    void setDecksNumber(int decksNumber);
public:
    CasinowarDeck();
    
    bool CanPlay();
};
