//
//  ChestsView.cpp
//
//  Created by Stanislav Zheronkin on 04.12.16.
//
//

#include "ChestsView.h"
#include "AudioManager.h"
#include "PlayerProfile.h"
#include "AndroidCompatibility.h"
#include "Platform.h"
#include "Features/mConstants.h"
#include "BaseScene.h"

USING_NS_CC;

ChestsView::ChestsView()
{
    
}

ChestsView * ChestsView::create(BonusType type, std::function< void() > callback)
{
    ChestsView * pRet = new ChestsView();
    if (pRet && pRet->init(type, callback))
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

bool ChestsView::init(BonusType type, std::function< void() > callback)
{
    if (!Node::init())
        return false;
    
    this->type = type;
    this->callback = callback;
    
    setCascadeOpacityEnabled(true);
    
    auto background = Sprite::create("ui/Chests/bonus_chests_back.png");
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
    
    auto title = Sprite::create(type == BONUS ? "ui/Chests/character_bonus.png" : "ui/Chests/character_dailybonus.png");
    title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    title->setPosition(Vec2(background->getContentSize().width / 2, background->getContentSize().height - 20));
    background->addChild(title);
    
    auto label = Label::createWithTTF("Click to Unlock the Treasure Chest!", "fonts/arial.ttf", 30);
    label->setTextColor(Color4B::WHITE);
    label->setPosition(Vec2(background->getContentSize().width / 2, background->getContentSize().height * 0.8f));
    background->addChild(label);
    
    //chests
    chests.resize(3);
    buttons.resize(3);
    
    for (int i = 0; i < 3; i++)
    {
        auto chest = Sprite::create("ui/Chests/bonus_chests_in_1.png");
        chest->setPosition(Vec2((0.272f + 0.239f * i) * background->getContentSize().width, 0.51f * background->getContentSize().height));
        background->addChild(chest);
        chests[i] = chest;
        
        auto button = ui::Button::create();
        button->setScale9Enabled(true);
        button->setPosition(chest->getPosition());
        button->setAnchorPoint(chest->getAnchorPoint());
        button->setContentSize(Size(chest->getContentSize().width * 0.5f, chest->getContentSize().height));
        button->setTag(i);
        button->addClickEventListener(std::bind(&ChestsView::onChestOpen, this, std::placeholders::_1));
        background->addChild(button);
        buttons[i] = button;
    }
    
    return true;
}

//hackyhack_
void ChestsView::onChestOpen(cocos2d::Ref* sender)
{
    auto button = dynamic_cast<ui::Button*>(sender);
    
    PlayerProfile::getInstance().setUIntValue("DAILY_BONUS_TIME", Platform::unixTime());
    PlayerProfile::getInstance().Save();
    
    if (button)
    {
        AudioManager::getInstance().PlaySoundEffect("music/SlotsSound/ChestOpen");
        
        //lock all buttons
        for (int i = 0; i < 3; i++)
        {
            buttons[i]->setEnabled(false);
        }
        
        //result
        //hackyhack_publish - arc4random_uniform
//        int t = arc4random_uniform(5);
        int t = rand() % 5;
        int coins = 0;
        
        if(type == DAILY)
        {
            if(t == 0)
            {
                coins = BONUS_LEVEL_UP;
            }
            else if (t == 4)
            {
                coins = BONUS_LEVEL_UP * 4;
            }
            else
            {
                coins = BONUS_LEVEL_UP * 2;
            }
            
            //multiple by bonus
            coins *= PlayerProfile::getInstance().getDailyBonusMultiplier();
        }
        else
        {
            if(t == 0)
            {
                coins = 0;
            }
            else if (t == 4)
            {
                coins = BONUS_LEVEL_UP * 2;
            }
            else
            {
                coins = BONUS_LEVEL_UP;
            }
        }
        
        // multiple by level
        coins *= PlayerProfile::getInstance().getLevel();
        
        //animate chest
        Vector<cocos2d::SpriteFrame *> frames;
        
        if (coins > 0)
        {
            for (int i = 0; i < 8; i++)
            {
                auto framename = StringUtils::format("ui/Chests/bonus_chests_in_%d.png",  i + 1);
                auto texture = Director::getInstance()->getTextureCache()->addImage(framename);
                SpriteFrameCache::getInstance()->addSpriteFrame(SpriteFrame::create(framename, Rect(Vec2::ZERO, texture->getContentSizeInPixels())), framename);
                auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(framename);
                frames.pushBack(frame);
            }
        }
        else
        {
            for (int i = 0; i < 4; i++)
            {
                auto framename = StringUtils::format("ui/Chests/bonus_chests_empty_%d.png",  i + 1);
                auto texture = Director::getInstance()->getTextureCache()->addImage(framename);
                SpriteFrameCache::getInstance()->addSpriteFrame(SpriteFrame::create(framename, Rect(Vec2::ZERO, texture->getContentSizeInPixels())), framename);
                auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(framename);
                frames.pushBack(frame);
            }
        }
        
        auto animation = Animation::createWithSpriteFrames(frames, 0.07f);
        chests[button->getTag()]->runAction(Sequence::create(Animate::create(animation), DelayTime::create(1.0f), CallFunc::create(std::bind(&ChestsView::showResult, this, coins, button->getTag())), nullptr));
    }
}

void ChestsView::showResult(int coins, int chestID)
{
    if (coins > 0)
    {
        AudioManager::getInstance().PlaySoundEffect("music/SlotsSound/CoinDrop");
        
        auto scene = BaseScene::findBaseScene();
        if (scene)
        {
            scene->increasePlayerValues(coins, 0, false);
        }
    }
    else
    {
        AudioManager::getInstance().PlaySoundEffect("music/SlotsSound/EmptyBox");
    }
    
    Sprite * panel = Sprite::create("ui/Chests/getcoins.png");
    if (panel)
    {
        chests[chestID]->addChild(panel);
        panel->setPosition(Vec2(chests[chestID]->getContentSize() / 2) + Vec2(0, 50));
        panel->setScale(0.1f);
        panel->runAction(Sequence::create(ScaleTo::create(0.5f, 1.0f), CallFunc::create(std::bind(&ChestsView::hide, this)), NULL));
        
        // label
        auto * label = Label::createWithTTF(StringUtils::format("%d", coins), "fonts/Berlin.ttf", 86.0f);
        label->setColor(Color3B::WHITE);
        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        label->setPosition(Vec2(panel->getContentSize() / 2) + Vec2(0, 5));
        label->enableShadow(Color4B(Color3B::BLACK, 192), Size(4, -4));
        label->setDimensions(panel->getContentSize().width * 0.8f, 0);
        label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        label->enableWrap(false);
        label->setOverflow(cocos2d::Label::Overflow::SHRINK);
        panel->addChild(label);
    }
}

Scene * ChestsView::findScene(Node * node)
{
    auto scene = dynamic_cast<Scene*>(node);
    
    if (scene)
    {
        return scene;
    }
    
    return findScene(node->getParent());
}

void ChestsView::show(cocos2d::Node * parent)
{
    auto scene = findScene(parent);
    
    scene->addChild(this, 100);
    
    setPosition(scene->getContentSize() / 2);
}

void ChestsView::hide()
{
    this->runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([this](){ if (callback) callback(); }), RemoveSelf::create(), NULL));
}
