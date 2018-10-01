//
//  ScrollBar.cpp
//
//  Created by Stanislav Zheronkin on 23.10.15.
//  Copyright (c) 2015 CedeExotics Studio. All rights reserved.
//

#include "ScrollBar.h"

USING_NS_CC;

ScrollBar::ScrollBar() : _target(nullptr), scrollThumb(nullptr)
{
}

ScrollBar::~ScrollBar()
{
    _target = nullptr;
}

ScrollBar * ScrollBar::create(ui::ScrollView * targetControl, std::string backgroundSprite, std::string thumbSprite)
{
    ScrollBar * result = new ScrollBar();
    
    if(result && result->initWithScrollView(targetControl, backgroundSprite, thumbSprite) )
    {
        result->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(result);
    }
    
    return result;
}

bool ScrollBar::initWithScrollView(ui::ScrollView * targetControl, std::string backgroundSprite, std::string thumbSprite)
{
    if (!Node::init())
        return false;
    
    _target = targetControl;
    
    //scrollbar
    ui::Scale9Sprite * scrollBar = ui::Scale9Sprite::create(backgroundSprite);
    scrollBar->setAnchorPoint(Vec2::ZERO);
    scrollBar->setCascadeOpacityEnabled(true);
    scrollBar->setPreferredSize(cocos2d::Size(scrollBar->getContentSize().width, targetControl->getContentSize().height));
    this->addChild(scrollBar);
    
    
    scrollThumb = Sprite::create(thumbSprite);
    scrollThumb->setAnchorPoint(Vec2(0.5f, 1.0f));
    scrollThumb->setPosition(Vec2(scrollBar->getContentSize().width / 2, scrollBar->getContentSize().height));
    scrollBar->addChild(scrollThumb);
    
    setCascadeOpacityEnabled(true);
    
    scheduleUpdate();
    
    return true;
}

void ScrollBar::update(float delta)
{
    if (_target == nullptr)
        return;
    
    if (_target->getLayoutSize().height + 1 > _target->getInnerContainerSize().height)
    {
        this->setVisible(false);
        return;
    }
    
    this->setVisible(true);
    
    float bar_lenght = _target->getLayoutSize().height - scrollThumb->getContentSize().height;
    float i = bar_lenght * _target->getInnerContainerPosition().y / (_target->getInnerContainerSize().height - _target->getLayoutSize().height);
    
    i = clampf(i, -bar_lenght, 0);
    
    scrollThumb->setPosition(Vec2(scrollThumb->getParent()->getContentSize().width / 2, _target->getLayoutSize().height - i - bar_lenght));
}

