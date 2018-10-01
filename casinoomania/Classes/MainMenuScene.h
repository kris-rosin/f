#pragma once

#include "cocos2d.h"
#include "BaseScene.h"
#include "ui/CocosGUI.h"

class MainMenuScene : public BaseScene
{
private:
    cocos2d::Label * userName;
    
    static int openedTimes;
private:
    MainMenuScene();
    virtual ~MainMenuScene();
    
    void onButtonPressed(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType event);
    void ProcessButtonAction(cocos2d::ui::Button * button);
    
    virtual void onEnterTransitionDidFinish() override;
public:
    static cocos2d::Scene * createScene();

    virtual bool init() override;
    virtual void onEnter() override;
    
    CREATE_FUNC(MainMenuScene);
};
