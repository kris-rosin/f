//
//  CardSprite.cpp
//  2pb2
//
//  Created by liu on 6/2/17.
//
//

#include "CardSprite.hpp"

#include "CapsaDefine.h"

Size CardSprite::m_cardSize = Size(0.0f, 0.0f);

CardSprite* CardSprite::create(int cardNumber)
{
    CardSprite *card = new (std::nothrow) CardSprite();
    if (card && card->initWithFile("ui/Cards/card.png"))
    {
        card->setCardNumber(cardNumber);
        card->autorelease();
        return card;
    }
    CC_SAFE_DELETE(card);
    return nullptr;
}

int CardSprite::getCardNumber()
{
    return m_cardNumber;
}

void CardSprite::setCardNumber(int cardNumber)
{
    m_cardNumber = cardNumber;
    
    int cardWeight = m_cardNumber / 4;
    int cardType = m_cardNumber % 4;
    m_cardWeight = cardWeight;
    m_cardSuit = cardType;
    
    std::string cards[] = {"3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A", "2"};
    std::string suitStrings[] = {"D", "C", "H", "S"};
    
    std::stringstream ssCardFile;
    ssCardFile << "ui/Cards/" << cards[cardWeight] << suitStrings[cardType] << ".png";
    
    m_spFront = Sprite::create(ssCardFile.str());
    addChild(m_spFront);
    m_spFront->setPosition(getContentSize() * 0.5f);
    
    if (m_cardSize.width == 0.0f)
    {
        m_cardSize = getContentSize();
    }
}

int CardSprite::getCardWeight()
{
    return m_cardWeight;
}

int CardSprite::getCardSuit()
{
    return m_cardSuit;
}

Size CardSprite::getCardSize()
{
    return m_cardSize;
}

void CardSprite::show(bool value)
{
    m_spFront->setVisible(value);
}

void CardSprite::setSelected(bool selected)
{
    m_selected = selected;
}

bool CardSprite::isSelected()
{
    return m_selected;
}
