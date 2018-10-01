#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "BaseScene.h"

class SlotsSelectThemeScene : public BaseScene
{
private:
    
private:
    SlotsSelectThemeScene();
    virtual ~SlotsSelectThemeScene();
    
    void onButtonPressed(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType event);
    void ProcessButtonAction(cocos2d::ui::Button * button);
    void onBackButtonPressed();
public:
    static cocos2d::Scene * createScene();

    virtual bool init();
    virtual void onEnter();
    
    CREATE_FUNC(SlotsSelectThemeScene);
};