//
//  PokerModel.cpp
//  2pb2
//
//  Created by liu on 7/28/17.
//
//

#include "Big2PokerModel.hpp"

#include "CardSprite.hpp"

#define MAX_CARD_WEIGHT 13

std::string gCardPairTitle[] =
{
    "",
    "Single",
    "Pair",
    "Triple",
    "Straight",
    "Flush",
    "Full House",
//    "Bomb",
    "Four of a kind",
    "StraightFlush",
};

Big2PokerModel::Big2PokerModel(__Array * arrCards)
{
    setCard(arrCards);
}

void Big2PokerModel::setCard(cocos2d::__Array *arrCards)
{
    m_cardPairInfo = None;
    ssize_t nCount = arrCards->count();
    m_cardCount = (int)nCount;
    if (nCount == 1)
    {
        m_cardPairInfo = Single;
        CardSprite *card = (CardSprite *)arrCards->getObjectAtIndex(0);
        m_highestCardWeight = card->getCardWeight();
        m_cardSuit = card->getCardSuit();
    }
    else if (nCount == 2 || nCount == 3)
    {
        bool isSame = true;
        for (int i = 1; i < arrCards->count(); i++)
        {
            CardSprite *card = (CardSprite *)arrCards->getObjectAtIndex(i);
            CardSprite *prevCard = (CardSprite *)arrCards->getObjectAtIndex(i - 1);
            if (card->getCardWeight() != prevCard->getCardWeight())
            {
                isSame = false;
                break;
            }
        }
        
        if (isSame)
        {
            m_cardPairInfo = nCount == 2 ? Pair : Triple;
            m_cardSuit = 0;
            for (int i = 0; i < arrCards->count(); i++)
            {
                CardSprite *card = (CardSprite *)arrCards->getObjectAtIndex(i);
                if (i == 0)
                    m_highestCardWeight = card->getCardWeight();
                m_cardSuit = MAX(m_cardSuit, card->getCardSuit());
            }
        }
    }
    else if (nCount == 5)
    {
        int cardCountForWeight[MAX_CARD_WEIGHT];
        for (int i = 0; i < MAX_CARD_WEIGHT; i++)
            cardCountForWeight[i] = 0;
        
        int highestWeight = 0;
        int highestSuit = 0;
        int maxCardNumber = 0;
        for (int i = 0; i < arrCards->count(); i++)
        {
            CardSprite *card = (CardSprite *)arrCards->getObjectAtIndex(i);
            cardCountForWeight[card->getCardWeight()]++;
            if (card->getCardNumber() > maxCardNumber)
            {
                maxCardNumber = card->getCardNumber();
                highestWeight = card->getCardWeight();
                highestSuit = card->getCardSuit();
            }
        }
        
        int maxSameCount = 0;
        int cardTypeNum = 0;
        int highestCardOfMaxSame = 0;
        for (int i = 0; i < MAX_CARD_WEIGHT; i++)
        {
            if (cardCountForWeight[i] > maxSameCount)
            {
                maxSameCount = cardCountForWeight[i];
                highestCardOfMaxSame = i;
            }
            if (cardCountForWeight[i])
                cardTypeNum++;
        }
        
        if (maxSameCount == 4)
        {
            m_cardPairInfo = Bomb;
            m_highestCardWeight = highestCardOfMaxSame;
            return;
        }
        else if (maxSameCount == 3 && cardTypeNum == 2)
        {
            m_cardPairInfo = FullHouse;
            m_highestCardWeight = highestCardOfMaxSame;
            return;
        }
        
        bool flush = true;
        for (int i = 1; i < arrCards->count(); i++)
        {
            CardSprite *card = (CardSprite *)arrCards->getObjectAtIndex(i);
            CardSprite *prevCard = (CardSprite *)arrCards->getObjectAtIndex(i - 1);
            if (card->getCardSuit() != prevCard->getCardSuit())
            {
                flush = false;
                break;
            }
        }
        
        int continuousCount = 0;
        for (int i = 0; i < MAX_CARD_WEIGHT; i++)
        {
            int prevIndex = i - 1;
            if (prevIndex < 0)
                prevIndex = MAX_CARD_WEIGHT - 1;
            if (cardCountForWeight[i] && cardCountForWeight[prevIndex])
                continuousCount++;
            else
                continuousCount = 0;
            
            if (continuousCount == 4)
                break;
        }
        
        bool straight = continuousCount == 4;
        if (straight || flush)
        {
            m_highestCardWeight = highestWeight;
            m_cardSuit = highestSuit;
            
            if (straight && flush)
                m_cardPairInfo = StraightFlush;
            else if (flush)
                m_cardPairInfo = Flush;
            else
                m_cardPairInfo = Straight;
        }
    }
}

Big2PokerModel::Big2PokerCardPairInfo Big2PokerModel::getCardPairInfo()
{
    return m_cardPairInfo;
}

int Big2PokerModel::getCardCount()
{
    return m_cardCount;
}

int Big2PokerModel::getHighestCardWeight()
{
    return m_highestCardWeight;
}

int Big2PokerModel::getCardSuit()
{
    return m_cardSuit;
}

bool Big2PokerModel::canGo(Big2PokerModel *otherModel)
{
    if (m_cardPairInfo == None)
        return false;
    
    if (!otherModel)
        return true;
    
    if (otherModel->getCardCount() != getCardCount())
        return false;
    
    Big2PokerCardPairInfo otherCardPairInfo = otherModel->getCardPairInfo();
    if (m_cardPairInfo > otherCardPairInfo)
    {
        return true;
    }
    else if (m_cardPairInfo == otherCardPairInfo)
    {
        if (getHighestCardWeight() > otherModel->getHighestCardWeight())
            return true;
        else if (getHighestCardWeight() == otherModel->getHighestCardWeight())
        {
            if (getCardSuit() > otherModel->getCardSuit())
                return true;
        }
    }
    return false;
}

Big2PokerModel::Big2PokerCardPairInfo Big2PokerModel::getHigherCardPairFromArray(__Array *arrCards)
{
    ssize_t nCount = arrCards->count();
    if (nCount < 2)
    {
        return None;
    }
    
    if (nCount >= 2 && nCount < 5)
    {
        bool isSame = true;
        for (int i = 1; i < arrCards->count(); i++)
        {
            CardSprite *card = (CardSprite *)arrCards->getObjectAtIndex(i);
            CardSprite *prevCard = (CardSprite *)arrCards->getObjectAtIndex(i - 1);
            if (card->getCardWeight() != prevCard->getCardWeight())
            {
                isSame = false;
                break;
            }
        }
        
        if (isSame)
        {
            return nCount == 4 ? Bomb : (nCount == 3 ? Triple : Pair);
        }
    }
    else if (nCount == 5)
    {
        int cardCountForWeight[MAX_CARD_WEIGHT];
        for (int i = 0; i < MAX_CARD_WEIGHT; i++)
            cardCountForWeight[i] = 0;
        
        for (int i = 0; i < arrCards->count(); i++)
        {
            CardSprite *card = (CardSprite *)arrCards->getObjectAtIndex(i);
            cardCountForWeight[card->getCardWeight()]++;
        }
        
        int maxSameCount = 0;
        int cardTypeNum = 0;
        for (int i = 0; i < MAX_CARD_WEIGHT; i++)
        {
            if (cardCountForWeight[i] > maxSameCount)
                maxSameCount = cardCountForWeight[i];
            if (cardCountForWeight[i])
                cardTypeNum++;
        }
        
        if (maxSameCount == 3 && cardTypeNum == 2)
        {
            return FullHouse;
        }
        
        bool flush = true;
        for (int i = 1; i < arrCards->count(); i++)
        {
            CardSprite *card = (CardSprite *)arrCards->getObjectAtIndex(i);
            CardSprite *prevCard = (CardSprite *)arrCards->getObjectAtIndex(i - 1);
            if (card->getCardSuit() != prevCard->getCardSuit())
            {
                flush = false;
                break;
            }
        }
        
        int continuousCount = 0;
        for (int i = 0; i < MAX_CARD_WEIGHT; i++)
        {
            int prevIndex = i - 1;
            if (prevIndex < 0)
                prevIndex = MAX_CARD_WEIGHT - 1;
            if (cardCountForWeight[i] && cardCountForWeight[prevIndex])
                continuousCount++;
            else
                continuousCount = 0;
            
            if (continuousCount == 4)
                break;
        }
        
        bool straight = continuousCount == 4;
        if (straight || flush)
        {
            if (straight && flush)
                return StraightFlush;
            else if (flush)
                return Flush;
            else
                return Straight;
        }
    }
    return None;
}

std::string Big2PokerModel::getPairTitle(int cardPair)
{
    return gCardPairTitle[cardPair];
}

