//
//  CardObject.cpp
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 06.03.17.
//
//

#include "CardObject.h"

CardObject CardObject::NO_CARD = CardObject(CardSuite::CLUBS, CardValue::UNKNOWN);

CardObject::CardObject(CardSuite suite, CardValue value) : suite(suite), value(value), opened(false)
{
    
}

CardObject::CardObject(int suite, int value) : opened(false)
{
    this->suite = static_cast<CardSuite>(suite);
    this->value = static_cast<CardValue>(value);
}

CardObject::CardValue CardObject::getValue()
{
    return value;
}

CardObject::CardSuite CardObject::getSuite()
{
    return suite;
}

int CardObject::BlackJackValue()
{
    if (value < 11) // 2...10
    {
        return value;
    }
    else if (value == 14) //ACE
    {
        return 1;
    }
    else // all portrets
    {
        return 10;
    }
}

int CardObject::BaccaratValue()
{
    if (value < 11) // 2...10
    {
        return value;
    }
    else if (value == 14) //ACE
    {
        return 1;
    }
    else // all portrets
    {
        return 0;
    }
}

int CardObject::CasinowarValue()
{
    return value;
}

std::string CardObject::valueToString(CardValue value)
{
    if (value <= TEN)
    {
        char buff[32];
        snprintf(buff, 32, "%d", value);
        
        return buff;
    }
    
    switch (value)
    {
        case ACE:
            return "A";
            break;
        case JACK:
            return "J";
            break;
        case QUEEN:
            return "Q";
            break;
        case KING:
            return "K";
            break;
        default:
            break;
    }
    
    return "";
}

std::string CardObject::suiteToString(CardSuite suite)
{
    switch (suite)
    {
        case CLUBS:
            return "C";
        case DIAMONDS:
            return "D";
        case SPADES:
            return "S";
        case HEARTS:
            return "H";
        default:
            return "";
    }
}

std::string CardObject::getCardCode()
{
    return valueToString(value) + suiteToString(suite);
}
