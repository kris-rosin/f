//
//  CoinToken.h
//
//  Created by Stanislav Zheronkin on 24.02.17.
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include <list>

class CoinToken : public cocos2d::Node
{
public:
    enum CoinTokenType
    {
        NORMAL,
        BUTTON,
        PUSH_BUTTON,
        STACK
    };
    
    enum CoinTokenSkin
    {
        GREY,
        GREEN,
        RED,
        BLACK
    };
    
    enum CoinTokenEvent
    {
        PLACED,
        CANCELED,
        PUSHED
    };
private:
    CoinTokenType type;
    
    std::list< std::tuple< int, int, CoinToken* > > coinsStack;
    
    std::function<bool(CoinTokenEvent, CoinToken*)> callback;
    
    cocos2d::Label  * coinsLabel;
    cocos2d::Sprite * coinsSprite;
    cocos2d::Sprite * magnifierFrame;
    
    cocos2d::RenderTexture * magnifier;
    
    CoinToken * dragingToken;
    
    int betPlace;
    
    bool  enabled;
    float scaleDragTokens;
private:
    CoinToken();
    
    void updateSkin(CoinTokenSkin skin);
    
    bool init(int initialValue, CoinTokenType type, CoinTokenSkin skin, CoinToken * source);
    
    void updateMagnifier(cocos2d::Touch * touch);
    
    bool IsPhoneSizeDevice();
public:
    static CoinToken * create(int initialValue, CoinTokenType type, CoinTokenSkin skin, CoinToken * source);
    
    void reset();
    int  getTotalValue();
    void addToken(int value, CoinTokenSkin tokenSkin, CoinToken * source);
    bool popToken();
    
    void setBetPlace(int placeSlot);
    int  getBetPlace();
    
    void setEnabled(bool value);
    void setScaleForDragingTokens(float value) { scaleDragTokens = value; };
    
    CoinToken * getSourceToken();
    
    void setType(CoinTokenType type);
    CoinTokenSkin getSkin();
    
    void setCallback(std::function<bool(CoinTokenEvent, CoinToken*)>);
    
    std::string getCompactNumberString(unsigned int value);
    
    CoinToken * getTopTokenClone();
};
