//
//  SharePhotoPopup.h
//
//  Created by Stanislav Zheronkin on 22.08.17.
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class SharePhotoPopup : public cocos2d::Node
{
private:
    cocos2d::RenderTexture * _photo;
private:
    SharePhotoPopup();
    
    bool init(cocos2d::RenderTexture * screenshot);
    void hide();
    
    void onButtonPressed(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType event);
    void ProcessButtonAction(cocos2d::ui::Widget * button);
    
    cocos2d::Scene * findScene(cocos2d::Node * node);
public:
    static SharePhotoPopup * create(cocos2d::RenderTexture * screenshot);
    
    void show(cocos2d::Node * parent);
};
