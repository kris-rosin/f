//
//  SwitchControl.cpp
//
//  Created by Stanislav Zheronkin on 20.04.17.
//
//

#include "SwitchControl.h"

USING_NS_CC;

SwitchControl::SwitchControl()
{
    
}

SwitchControl * SwitchControl::create(std::string background, std::string thumb, std::function<void(bool, std::string)> callback, std::string name)
{
    SwitchControl * pRet = new SwitchControl();
    if (pRet && pRet->init(background, thumb, callback, name))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool SwitchControl::init(std::string background, std::string thumb, std::function<void(bool, std::string)> callback, std::string name)
{
    if (!Node::init())
        return false;
    
    this->name     = name;
    this->callback = callback;
    
    setCascadeOpacityEnabled(true);
    
    auto back = Sprite::create(background);
    back->setAnchorPoint(Vec2::ZERO);
    back->setCascadeOpacityEnabled(true);
    addChild(back);
    
    switcher = Sprite::create(thumb);
    switcher->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    back->addChild(switcher);
    
    auto eventListener = EventListenerTouchOneByOne::create();
    eventListener->setSwallowTouches(true);
    eventListener->onTouchBegan = [this](Touch * touch, Event * event)
    {
        if (this->getParent())
        {
            Vec2 point = this->getParent()->convertTouchToNodeSpace(touch);
            return this->getBoundingBox().containsPoint(point);
        }
        
        return false;
    };
    eventListener->onTouchEnded = [this](Touch * touch, Event * event)
    {
        this->switchState();
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, switcher);
    
    setContentSize(back->getContentSize());
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    setState(true, false);
    
    return true;
}

void SwitchControl::setState(bool switchedOn, bool animated /*= true*/)
{
    currentState = switchedOn;
    
    Vec2 targetPosition = switchedOn ? Vec2((getContentSize().width * 2 - switcher->getContentSize().width) / 2, getContentSize().height / 2) : switcher->getContentSize() / 2;
    
    if (animated)
    {
        switcher->stopAllActions();
        switcher->runAction(Sequence::create(EaseSineInOut::create(MoveTo::create(0.15f, targetPosition)), NULL));
    }
    else
    {
        switcher->setPosition(targetPosition);
    }
}

void SwitchControl::switchState()
{
    setState(!currentState, true);
    
    if (callback)
        callback(currentState, name);
}

bool SwitchControl::getState()
{
    return currentState;
}
