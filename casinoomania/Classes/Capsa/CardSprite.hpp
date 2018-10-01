//
//  CardSprite.hpp
//  2pb2
//
//  Created by liu on 6/2/17.
//
//

#ifndef CardSprite_hpp
#define CardSprite_hpp

#include "cocos2d.h"

USING_NS_CC;

class CardSprite : public Sprite
{
public:
    static CardSprite * create(int cardNumber);
    
    int                 getCardNumber();
    void                setCardNumber(int cardNumber);
    
    int                 getCardWeight();
    int                 getCardSuit();
    
    static Size         getCardSize();
    
    void                show(bool value);
    
    bool                isSelected();
    void                setSelected(bool selected);
    
private:
    int                 m_cardNumber;
    
    int                 m_cardWeight;
    int                 m_cardSuit;
    
    static Size         m_cardSize;
    
    Sprite *            m_spFront;
    
    bool                m_selected;
};

#endif /* CardSprite_hpp */
