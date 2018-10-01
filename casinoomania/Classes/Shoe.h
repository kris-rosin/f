//
//  Shoe.h
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 07.03.17.
//
//

#pragma once

#include <list>

#include "Deck.h"

class Shoe : public Deck
{
    int decksnum;
public:
    Shoe(int decksNumber);
    
    void setDecksNumber(int decksNumber);
    
    bool CanPlay();
    
    std::list<CardObject> DrawHand();
};
