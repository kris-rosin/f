//
//  Hand.cpp
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 07.03.17.
//
//

#include "Hand.h"

Hand::Hand() : stand(false), splitted(false), ace_splitted(false)
{
    
}

bool Hand::FirstOfTwoCardsIsAce()
{
    return cards.size() == 2 && cards[0].getValue() == CardObject::CardValue::ACE;
}

bool Hand::CanSplit()
{
    return (cards.size() == 2 && (cards[0].getValue() == cards[1].getValue()));
    
    //modified condition, card should be exactly same, to use standart rules use code: cards[0].BlackJackValue() == cards[1].BlackJackValue()
}

bool Hand::HasTwoAces()
{
    return (cards.size() == 2 && cards[0].getValue() == CardObject::CardValue::ACE && (cards[0].getValue() == cards[1].getValue()));
}

bool Hand::HasAce(bool withClosedCards /*= false*/)
{
    for (auto card: cards)
    {
        if (card.getValue() == CardObject::CardValue::ACE && (card.isCardOpen() || withClosedCards) )
            return true;
    }
    
    return false;
}

bool Hand::CanHit()
{
    auto totals = getTotal();
    
    if(totals.size() == 2)
    {
        return true;
    }
    else
    {
        return (totals[0] < 21);
    }
}

std::vector<int> Hand::getTotal(bool withClosedCards /*= false*/)
{
    int total1 = 0;
    
    for (auto card: cards)
    {
        if (card.isCardOpen() || withClosedCards)
            total1 += card.BlackJackValue();
    }
    
    if (total1 < 12 && HasAce())
    {
        int total2 = total1 + 10;
        
        if (stand)
        {
            return {total2}; //in place vector creation
        }
        else
        {
            return {total1, total2};
        }
    }
    else
    {
        return {total1};
    }
}

void Hand::Hit(CardObject card)
{
    cards.push_back(card);
}

void Hand::setCards(std::vector<CardObject> new_cards)
{
    cards = new_cards;
}

bool Hand::CanSurrender()
{
    return cards.size() == 2 && splitted == false;
}

Hand::HandStatus Hand::GetStatus()
{
    if (cards.size() == 0)
        return BUST;
    
    if(cards.size() == 2 && !splitted && HasAce(true) && (cards[0].BlackJackValue() == 10 || cards[1].BlackJackValue() == 10))
    {
        return BLACKJACK;
    }
    
    auto total = getTotal(true);
    
    if(total[0] > 21)
        return BUST;
    
    if(total[0] == 21 || (total.size() == 2 && total[1] == 21))
        return VALUE21;
    
    return OPTIONS;
}

bool Hand::isSplitted(bool withAces /*= false*/)
{
    return splitted && (withAces ? ace_splitted : true);
}
