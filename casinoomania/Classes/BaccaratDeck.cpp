//
//  BaccaratDeck.cpp
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 28.03.17.
//
//

#include "BaccaratDeck.h"

#include <vector>

BaccaratDeck::BaccaratDeck(int decksNumber) : decksnum(8), reshuffled(false)
{
    setDecksNumber(decksNumber);
}

void BaccaratDeck::setDecksNumber(int decksNumber)
{
    if (decksNumber < 1 || decksNumber > 8)
    {
        printf("%s", "Error: attempt set wrong decks number in shoe");
        return;
    }
    
    decksnum = decksNumber;
        
    cards.clear();
    
    Deck standardDeck;
    
    for (int i = 0; i < decksNumber; i++)
    {
        cards.insert(cards.end(), standardDeck.cards.begin(), standardDeck.cards.end());
    }
    
    Shuffle();
}

bool BaccaratDeck::CanPlay()
{
    return (cards.size() >= 6);
}


CardObject BaccaratDeck::DrawCard()
{
    if (!reshuffled && cards.size() <= decksnum * 52 / 2)
    {
        Shuffle();
        reshuffled = true;
    }
    
    return Deck::DrawCard();
}
