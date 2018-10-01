//
//  Big2PokerModel.hpp
//  2pb2
//
//  Created by liu on 7/28/17.
//
//

#ifndef Big2PokerModel_hpp
#define Big2PokerModel_hpp

#include "cocos2d.h"
USING_NS_CC;

class CardSprite;
class Big2PokerModel
{
public:
    enum Big2PokerCardPairInfo
    {
        None,
        Single,
        Pair,
        Triple,
        Straight,
        Flush,
        FullHouse,
        Bomb,
        StraightFlush,
        PairTypeCount
    };
public:
    Big2PokerModel(__Array * arrCards);
    
    void                    setCard(__Array *arrCards);
    
    Big2PokerCardPairInfo   getCardPairInfo();
    int                     getCardCount();
    
    int                     getHighestCardWeight();
    int                     getCardSuit();
    
    bool                    canGo(Big2PokerModel *otherModel = NULL);
    
    static
    Big2PokerCardPairInfo   getHigherCardPairFromArray(__Array *arrCards);
    
    static
    std::string             getPairTitle(int);
    
private:
    Big2PokerCardPairInfo   m_cardPairInfo;
    
    int                     m_highestCardWeight;
    int                     m_cardSuit;
    
    int                     m_cardCount;
};

#endif /* PokerModel_hpp */
