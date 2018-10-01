//
//  Shoe.cpp
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 07.03.17.
//
//

#include "Shoe.h"

#include <vector>

Shoe::Shoe(int decksNumber) : decksnum(1)
{
    setDecksNumber(decksNumber);
}

void Shoe::setDecksNumber(int decksNumber)
{
    if (decksNumber < 1 || decksNumber > 6)
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
    
}

bool Shoe::CanPlay()
{
    return (cards.size() >= 26 * decksnum);
}

std::list<CardObject> Shoe::DrawHand()
{
    std::list<CardObject> result;
    
    result.push_back(DrawCard());
    result.push_back(DrawCard());
    
    return result;
}
