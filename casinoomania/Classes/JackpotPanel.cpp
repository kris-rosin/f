//
//  JackpotPanel.cpp
//
//  Created by Stanislav Zheronkin on 18.11.16.
//
//

#include "JackpotPanel.h"

#include "PlayerProfile.h"
#include "Utilities.h"

USING_NS_CC;

JackpotPanel::JackpotPanel() : label(nullptr)
{
    
}

JackpotPanel * JackpotPanel::create()
{
    JackpotPanel * pRet = new JackpotPanel();
    if (pRet && pRet->init())
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

bool JackpotPanel::init()
{
    if (!Node::init())
        return false;
    
    setCascadeOpacityEnabled(true);
    
    //coins panel
    auto panel = Sprite::create("ui/Slots/jackpot_panel.png");
    addChild(panel);
    panel->setAnchorPoint(Vec2::ZERO);
    
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(143, 34));
    layout->setAnchorPoint(Vec2::ZERO);
    layout->setPosition(Vec2(40, 10));
    layout->setClippingEnabled(true);
    panel->addChild(layout);
    
    label = Label::createWithTTF("0.", "fonts/Berlin.ttf", 28.0f);
    label->setColor(Color3B::BLACK);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    label->setPosition(Vec2( layout->getContentSize().width - 20, layout->getContentSize().height / 2));
    layout->addChild(label);
    
    //cents panel
    float itemSize = label->getContentSize().height;
    
    auto cents = Node::create();
    cents->setContentSize(Size(20, itemSize * 11));
    cents->setAnchorPoint(Vec2(0, 1.0f - 1.0f / 22.0f));
    cents->setPosition(Vec2( layout->getContentSize().width - 20, layout->getContentSize().height / 2));
    layout->addChild(cents);
    
    for (int i = 0; i < 11; i++)
    {
        auto tmpLavel = Label::createWithTTF(StringUtils::format("%d", i % 10), "fonts/Berlin.ttf", 28.0f);
        tmpLavel->setColor(Color3B::BLACK);
        tmpLavel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        tmpLavel->setPosition(Vec2( cents->getContentSize().width / 2, cents->getContentSize().height - itemSize * (0.5f + i) ));
        cents->addChild(tmpLavel);
    }
    
    cents->runAction(RepeatForever::create(Sequence::create(MoveBy::create(10.0f, Vec2(0, itemSize * 10)), Place::create(Vec2( layout->getContentSize().width - 20, layout->getContentSize().height / 2)), CallFunc::create([this](){ this->incrementValue(); }), NULL)));
    
    setContentSize(panel->getContentSize());
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    update();
    
    return true;
}

void JackpotPanel::update()
{
    label->setString(FormatWithCommas(PlayerProfile::getInstance().getSlotsJackpot() ).append("."));
}

void JackpotPanel::incrementValue()
{
    PlayerProfile::getInstance().addSlotsJackpot(1);
    
    update();
}

