//
//  CardObject.h
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 06.03.17.
//
//

#pragma once

#include <string>

class CardObject
{
public:
    enum CardSuite
    {
        CLUBS    = 0,
        DIAMONDS = 1,
        HEARTS   = 2,
        SPADES   = 3
    };
    
    enum CardValue
    {
        UNKNOWN = 0,
        TWO   =  2,
        THREE =  3,
        FOUR  =  4,
        FIVE  =  5,
        SIX   =  6,
        SEVEN =  7,
        EIGHT =  8,
        NINE  =  9,
        TEN   = 10,
        JACK  = 11,
        QUEEN = 12,
        KING  = 13,
        ACE   = 14
    };
    
protected:
    CardSuite suite;
    CardValue value;
    
    bool opened;
public:
    static CardObject NO_CARD;
public:
    CardObject(CardSuite suite, CardValue value);
    CardObject(int suite, int value);
    
    int BlackJackValue();
    int BaccaratValue();
    int CasinowarValue();
    
    CardValue getValue();
    CardSuite getSuite();
    
    static std::string valueToString(CardValue value);
    static std::string suiteToString(CardSuite suite);
    
    std::string getCardCode();
    
    bool isCardOpen() const { return opened; };
    void setCardOpen(bool value) { opened = value; };
};
