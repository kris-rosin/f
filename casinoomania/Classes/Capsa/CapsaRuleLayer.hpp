//
//  CapsaRuleLayer.hpp
//  DovemobiCasino
//
//  Created by Frank on 11/8/17.
//
//

#ifndef CapsaRuleLayer_hpp
#define CapsaRuleLayer_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class CapsaRuleLayer : public cocos2d::Layer
{
private:
    CapsaRuleLayer();
    virtual ~CapsaRuleLayer();
    
public:
    static cocos2d::Scene * createScene();
    
    virtual bool init();

    void onBack();

    void onClickConfirm(Ref* sender);

    void onSliderEvent(Ref* sender, ui::Slider::EventType type);

    void drawScrollViewContents();
    
    CREATE_FUNC(CapsaRuleLayer);

private:
    
    ui::ScrollView* m_pScrollView;
    
    MenuItemImage*  m_btConfirm;

    float m_rScaleX;
    float m_rScaleY;
};

#endif /* CapsaRuleLayer_hpp */
