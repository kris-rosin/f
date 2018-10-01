//
//  FreeSpinView.cpp
//
//  Created by Stanislav Zheronkin on 12.12.16.
//
//

#include "FreeSpinView.h"
#include "AudioManager.h"

USING_NS_CC;

FreeSpinView::FreeSpinView() : totalTime(0), currentFrame(1), preloaded(false)
{
    
}

FreeSpinView * FreeSpinView::create(int amount, std::function< void() > callback)
{
    FreeSpinView * pRet = new FreeSpinView();
    if (pRet && pRet->init(amount, callback))
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

bool FreeSpinView::init(int amount, std::function< void() > callback)
{
    if (!Node::init())
        return false;
    
    this->callback = callback;
    
    setCascadeOpacityEnabled(true);
    
    auto background = Sprite::create("ui/FreeSpin/back.png");
    background->setAnchorPoint(Vec2::ZERO);
    addChild(background);
    setContentSize(background->getContentSize());
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    //blocker
    auto block = ui::Button::create();
    block->setScale9Enabled(true);
    block->setContentSize(Director::getInstance()->getWinSize());
    block->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    block->setPosition(getContentSize() / 2);
    addChild(block, -1);
    
    animation = Sprite::create("ui/FreeSpin/free_spin0001.png");
    animation->setPosition(Vec2(background->getContentSize().width / 2, background->getContentSize().height * 0.4f));
    background->addChild(animation);
    
    auto label = Label::createWithTTF(StringUtils::format("%i Free Spins", amount), "fonts/Berlin.ttf", 50);
    label->setTextColor(Color4B(255, 220, 85, 255));
    label->setPosition(Vec2(background->getContentSize().width / 2, background->getContentSize().height * 0.8f));
    background->addChild(label);
    
    scheduleUpdate();
    
    return true;
}

void FreeSpinView::update(float deltaTime)
{
    totalTime += deltaTime;
    
    int frame = (totalTime - 2) / 0.1f;
    
    if (frame > currentFrame && frame <= 96)
    {
        preloaded = false;
        auto newSprite = Sprite::create(StringUtils::format("ui/FreeSpin/free_spin%04d.png", frame));
        animation->getParent()->addChild(newSprite);
        newSprite->setPosition(animation->getPosition());
        animation->removeFromParentAndCleanup(true);
        animation = newSprite;
        
        //remove old texture
        Director::getInstance()->getTextureCache()->removeTextureForKey(StringUtils::format("ui/FreeSpin/free_spin%04d.png", currentFrame));
        
        currentFrame = frame;
    }
    else if (frame > 96)
    {
        unscheduleUpdate();
        hide();
    }
    else if (frame == currentFrame && frame < 96 && preloaded == false)
    {
        Director::getInstance()->getTextureCache()->addImage(StringUtils::format("ui/FreeSpin/free_spin%04d.png", frame + 1));
        preloaded = true;
    }
}

void FreeSpinView::show(cocos2d::Node * parent)
{
    parent->addChild(this, 100);
    
    setPosition(parent->getContentSize() / 2);
}

void FreeSpinView::hide()
{
    this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([this](){ if (callback) callback(); }), RemoveSelf::create(), NULL));
}
