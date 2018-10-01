//
//  ExperiencePanel.cpp
//
//  Created by Stanislav Zheronkin on 18.11.16.
//
//

#include "ExperiencePanel.h"

#include "PlayerProfile.h"

USING_NS_CC;

ExperiencePanel::ExperiencePanel() : levelLabel(nullptr), level_bar_fill(nullptr)
{
    
}

ExperiencePanel * ExperiencePanel::create()
{
    ExperiencePanel * pRet = new ExperiencePanel();
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

bool ExperiencePanel::init()
{
    if (!Node::init())
        return false;
    
    setCascadeOpacityEnabled(true);
    
    //coins panel
    auto level_bar = Sprite::create("ui/MainMenu/level_bar.png");
    addChild(level_bar);
    level_bar->setAnchorPoint(Vec2::ZERO);
    
    levelLabel = Label::createWithTTF("0", "fonts/Berlin.ttf", 22.0f);
    levelLabel->setColor(Color3B::BLACK);
    levelLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    levelLabel->setPosition(Vec2(27, level_bar->getContentSize().height / 2));
    level_bar->addChild(levelLabel, 3);
    
    level_bar_fill = ProgressTimer::create(Sprite::create("ui/MainMenu/level_bar_fill.png"));
    level_bar->addChild(level_bar_fill);
    level_bar_fill->setAnchorPoint(Vec2::ZERO);
    level_bar_fill->setType(cocos2d::ProgressTimer::Type::BAR);
    level_bar_fill->setMidpoint(Vec2::ZERO);
    level_bar_fill->setBarChangeRate(Vec2(1, 0));
    level_bar_fill->setPercentage(0);
    
    auto star_overlay = Sprite::create("ui/MainMenu/bar_star_icon.png");
    star_overlay->setAnchorPoint(Vec2::ZERO);
    level_bar->addChild(star_overlay, 2);
    
    setContentSize(level_bar->getContentSize());
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    update(0);
    
    schedule(std::bind(&ExperiencePanel::update, this, std::placeholders::_1), 0.5f, "COINS_TIMER");
    
    return true;
}

void ExperiencePanel::update(float deltaTime)
{
    levelLabel->setString(StringUtils::format("%d", PlayerProfile::getInstance().getLevel()));
    
    const int leftMargin = 28;
    const int rightMargin = 9;
    
    float startProcent = leftMargin / level_bar_fill->getContentSize().width;
    float midProcent = (level_bar_fill->getContentSize().width - rightMargin - leftMargin) / level_bar_fill->getContentSize().width;
    
    level_bar_fill->setPercentage(100.0f * (startProcent + midProcent * PlayerProfile::getInstance().getLevelProgress()));
}
