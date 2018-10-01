//
//  CapsaThemeScene.hpp
//  DovemobiCasino
//
//  Created by liu on 9/6/17.
//
//

#ifndef CapsaThemeScene_hpp
#define CapsaThemeScene_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class CapsaThemeScene : public cocos2d::Layer
{
private:
    
private:
    CapsaThemeScene();
    virtual ~CapsaThemeScene();
    
    void onButtonPressed(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType event);
    void ProcessButtonAction(cocos2d::ui::Button * button);
    
    void onBackButtonPressed();
public:
    static cocos2d::Scene * createScene();
    
    virtual bool init();
    virtual void onEnter();
    
    CREATE_FUNC(CapsaThemeScene);
};

#endif /* CapsaThemeScene_hpp */
