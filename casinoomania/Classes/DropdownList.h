//
//  DropdownList.h
//
//  Created by Stanislav Zheronkin on 18.11.16.
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class DropdownList : public cocos2d::Node
{
private:
    enum STATE_TYPE
    {
        STATE_CLOSED = 0,
        STATE_OPENED,
        STATE_LOCKED
    };
private:
    cocos2d::ui::Button * _openButton;
    cocos2d::ui::Button * _closeButton;
    cocos2d::Sprite     * _expandedSprite;
    cocos2d::ui::Layout * container;
    cocos2d::Label      * _label;
    
    cocos2d::ui::ScrollView * _scroll;
    
    STATE_TYPE state;
private:
    DropdownList();
    bool init(std::string normal, std::string opened, std::string disabled);
public:
    static DropdownList * create(std::string normal, std::string opened, std::string disabled);
    
    void setLabel(std::string text, std::string font, float size);
    void addItemToList(cocos2d::Node * item);
    void setLabelText(std::string text);
    void setEnabled(bool enable);
    
    void expand();
    void close();
    void toggle();
};
