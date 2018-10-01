//
//  ProcessLayer.cpp
//  2pb2
//
//  Created by liu on 6/9/17.
//
//

#include "ProcessLayer.hpp"

#include "CapsaDefine.h"

USING_NS_CC;

void ProcessLayer::setTitle(const std::string &title)
{
    m_lblTitle->setString(title);
}

bool ProcessLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !LayerColor::init())
    {
        return false;
    }
    
    setColor(Color3B::BLACK);
    setOpacity(50);
    
    m_lblTitle = Label::createWithTTF(" ", MAIN_FONT_FILE, WIN_SIZE.height * 0.05f);
    //m_lblTitle->setColor(Color3B::BLACK);
    addChild(m_lblTitle);
    m_lblTitle->setPosition(WIN_SIZE * 0.5f);
    
    return true;
}

void ProcessLayer::onEnter()
{
    LayerColor::onEnter();
    
    setupTouch();
}

void ProcessLayer::onExit()
{
    getEventDispatcher()->removeEventListenersForTarget(this);
    LayerColor::onExit();
}

void ProcessLayer::setupTouch()
{
    auto pListener = EventListenerTouchOneByOne::create();
    pListener->setSwallowTouches(true);
    
    pListener->onTouchBegan = [] (cocos2d::Touch* pThouch, cocos2d::Event* pEvent)
    {
        return true;
    };
    
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(pListener, this);
}
