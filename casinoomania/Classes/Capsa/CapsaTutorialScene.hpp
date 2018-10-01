//
//  CapsaTutorialScene.hpp
//  DovemobiCasino-mobile
//
//  Created by Frank on 11/7/17.
//

#ifndef CapsaTutorialScene_hpp
#define CapsaTutorialScene_hpp

#include "cocos2d.h"
USING_NS_CC;

class CapsaTutorialScene : public cocos2d::Layer
{
private:
    CapsaTutorialScene();
    virtual ~CapsaTutorialScene();

// method
public:
    static cocos2d::Scene * createScene();
    
    virtual bool init();
    
    void onBack();
    void onClickLeft(Ref* sender);
    void onClickRight(Ref* sender);
    
    void setPageImage();
    void setLightImage();
    void upgradeButton();
    
    void update(float dt);
    
    CREATE_FUNC(CapsaTutorialScene);

// propertys
private:

    float       m_rScaleX;
    float       m_rScaleY;
    
    Sprite*     m_spPageImage;
    
    Sprite*     m_spPlayRuleLight;
    Sprite*     m_spHandRankLight;
    Sprite*     m_spSpecialRuleLight;
    Sprite*     m_spPlayHandLight;
    Sprite*     m_spHandSpecialLight;
    
    Sprite*     m_spHandRank;
    Sprite*     m_spSpecialRules;
    
    MenuItemImage*  m_btLeft;
    MenuItemImage*  m_btRight;
    
    ClippingRectangleNode* m_nodeClipper;
    
    int         m_nPageIndex;
    
    float       m_rClipperY;
};

#endif /* CapsaTutorialScene_hpp */
