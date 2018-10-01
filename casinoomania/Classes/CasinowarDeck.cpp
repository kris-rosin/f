//
//  CasinowarDeck.cpp
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 10.04.17.
//
//

#include "CasinowarDeck.h"

#include <vector>

CasinowarDeck::CasinowarDeck()
{
    setDecksNumber(6);
}

void CasinowarDeck::setDecksNumber(int decksNumber)
{
    if (decksNumber != 6)
    {
        printf("%s", "Error: attempt set wrong decks number in shoe");
        return;
    }
        
    cards.clear();
    
    Deck standardDeck;
    
    for (int i = 0; i < decksNumber; i++)
    {
        cards.insert(cards.end(), standardDeck.cards.begin(), standardDeck.cards.end());
    }
    
    Shuffle();
}

bool CasinowarDeck::CanPlay()
{
    return (cards.size() >= 5);
}
