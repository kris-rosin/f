//
//  Deck.cpp
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 06.03.17.
//
//

#include "Deck.h"

#include <vector>
#include "AndroidCompatibility.h"
#include <cstdlib>

Deck::Deck()
{
    for (int suite = CardObject::CardSuite::CLUBS; suite <= CardObject::CardSuite::SPADES; suite++)
    {
        for (int value = CardObject::CardValue::TWO; value <= CardObject::CardValue::ACE; value++)
        {
            cards.push_back(CardObject(suite, value));
        }
    }
}

Deck::Deck(std::list<CardObject> cards) : cards(cards)
{
    
}

CardObject Deck::DrawCard()
{
    if (cards.size() == 0)
    {
        printf("%s", "Error: No cards in Deck!");
        return CardObject::NO_CARD;
    }
    
    auto card = cards.front();
    cards.pop_front();
    
    return card;
}

void Deck::PushCard(CardObject card)
{
    cards.push_front(card);
}

void Deck::Sort()
{
    cards.sort([](CardObject & first, CardObject & second)
    {
        if (first.getValue() != second.getValue())
        {
            return first.getValue() < second.getValue();
        }
        else if (first.getSuite() != second.getSuite())
        {
            return first.getSuite() < second.getSuite();
        }
        
        return true;
    });
}

void Deck::Shuffle()
{
    int n = cards.size();
    //hackyhack_publish - arc4random_uniform
//    int n_shuffles = arc4random_uniform(8) + 4;
    int n_shuffles = (rand() % 8) + 4;
    
    std::vector<CardObject> cardsArray(cards.begin(), cards.end());
    
    for (int k = 0; k < n_shuffles; k++)
    {
        for (int i = 0; i < n; i++)
        {
            //hackyhack_publish - arc4random_uniform
//            int r = arc4random_uniform(n - i) + i;
            int r = (rand() % (n - i)) + i;
            
            auto prevCard = cardsArray[i];
            cardsArray[i] = cardsArray[r];
            cardsArray[r] = prevCard;
        }
    }
    
    cards.clear();
    cards.insert(cards.begin(), cardsArray.begin(), cardsArray.end());
}
