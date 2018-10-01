//
//  PayoutsView.cpp
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 22.11.16.
//
//

#include "PayoutsView.h"
#include "SlotsConfig.h"
#include "AudioManager.h"


USING_NS_CC;

PayoutsView::PayoutsView()
{
    
}

PayoutsView * PayoutsView::create(std::string theme, int lines)
{
    PayoutsView * pRet = new PayoutsView();
    if (pRet && pRet->init(theme, lines))
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

bool PayoutsView::init(std::string theme, int lines)
{
    if (!Node::init())
        return false;
    
    currentLines = lines;
    currentTheme = theme;
    
    setCascadeOpacityEnabled(true);
    
    auto background = Sprite::create("ui/Slots/paytable.png");
    background->setCascadeOpacityEnabled(true);
    addChild(background, 1);
    setContentSize(background->getContentSize());
    
    block = ui::Button::create();
    block->setScale9Enabled(true);
    block->setContentSize(Director::getInstance()->getWinSize() * 2);
    addChild(block, -2);
    
    auto left_arrow = ui::Button::create("ui/Slots/left_arrow.png");
    left_arrow->setZoomScale(0);
    left_arrow->setName("left");
    left_arrow->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    left_arrow->setPosition(Vec2(10, background->getContentSize().height / 2));
    left_arrow->addTouchEventListener(std::bind(&PayoutsView::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    background->addChild(left_arrow);
    
    auto right_arrow = ui::Button::create("ui/Slots/right_arrow.png");
    right_arrow->setZoomScale(0);
    right_arrow->setName("right");
    right_arrow->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    right_arrow->setPosition(Vec2(background->getContentSize().width - 10, background->getContentSize().height / 2));
    right_arrow->addTouchEventListener(std::bind(&PayoutsView::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    background->addChild(right_arrow);
    
    auto close_button = ui::Button::create("ui/Slots/close_button.png");
    close_button->setZoomScale(0);
    close_button->setName("close");
    close_button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    close_button->setPosition(Vec2(background->getContentSize().width - 10, background->getContentSize().height - 10));
    close_button->addTouchEventListener(std::bind(&PayoutsView::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    background->addChild(close_button);
    
    //page view
    list = ui::PageView::create();
    list->setContentSize(Size(background->getContentSize().width - left_arrow->getContentSize().width * 2 - 10 * 4, background->getContentSize().height - 40.0f));
    list->setCascadeOpacityEnabled(true);
    list->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    list->setPosition(background->getContentSize() / 2);
    list->setDirection(ui::ScrollView::Direction::HORIZONTAL);

    background->addChild(list, 2);
    
    BuildPages();
    
    return true;
}

void PayoutsView::show(cocos2d::Node * parent)
{
    Vec2 point = parent->convertToNodeSpace(Director::getInstance()->getWinSize() / 2);
    
    this->setPosition(point);
    this->setOpacity(0);
    this->runAction(FadeIn::create(0.3f) );
    
    parent->addChild(this, 10000);
}

void PayoutsView::hide()
{
    this->runAction(Sequence::create(EaseSineIn::create(FadeOut::create(0.3f)), RemoveSelf::create(), nullptr));
    block->setEnabled(false);
}

void PayoutsView::BuildPages()
{
    list->removeAllPages();
    
    //page #1
    auto page1 = ui::Layout::create();
    page1->setContentSize(list->getContentSize());
    page1->setCascadeOpacityEnabled(true);
    list->addPage(page1);
    
    //title
    auto title = Sprite::create("ui/Slots/payouts_title.png");
    title->setPosition(Vec2( page1->getContentSize().width / 2, page1->getContentSize().height - 25.0f));
    page1->addChild(title);
    
    
    float margineY = 50.0f;
    float stepX = page1->getContentSize().width / 5;
    float stepY = (page1->getContentSize().height - margineY) / 2;
    float lineSpacing = 25;
    
    for (int i = 0; i < 10; i++) // first 10 elements
    {
        auto item = Sprite::create(StringUtils::format("ui/Slots/%s/items/%s.png", currentTheme.c_str(), SlotsConfig::payouts_items[i].c_str() ));
        item->setCascadeOpacityEnabled(true);
        page1->addChild(item);
        
        int yPos = page1->getContentSize().height - margineY - stepY * (i / 5) - 75.0f;
        item->setPosition(Vec2(stepX * 0.5f + (i % 5) * stepX, yPos));
        yPos = -lineSpacing;
        
        for (int k = 5; k > 1; k--)
        {
            int amount = SlotsConfig::payouts[k - 2][i];
            
            if (amount > 0)
            {
                bool createLabel = true;
                
                if (k == 5 && i == 0) // jackpot
                {
                    auto badge = Sprite::create("ui/Slots/win_badge.png");
                    item->addChild(badge);
                    badge->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
                    badge->setPosition(Vec2(item->getContentSize().width, 0));
                    
                    auto jackpot = Sprite::create("ui/Slots/jackpot_badge.png");
                    item->addChild(jackpot);
                    jackpot->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
                    jackpot->setPosition(Vec2(0, -5));
                    
                    yPos = jackpot->getPositionY() - jackpot->getContentSize().height - lineSpacing;
                    createLabel = false;
                }
                
                if (createLabel)
                {
                    auto label = Label::createWithTTF(StringUtils::format("%d", k), "fonts/Berlin.ttf", 26.0f);
                    label->enableBold();
                    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                    label->setPosition(10, yPos);
                    item->addChild(label);
                    
                    auto desc = Label::createWithTTF(StringUtils::format("%d", amount), "fonts/Berlin.ttf", 24.0f);
                    desc->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                    desc->setPosition(35, yPos);
                    item->addChild(desc);
                    
                    yPos = label->getPositionY() - lineSpacing;
                }
                
            }
        }
    }
    
    //page #2
    auto page2 = ui::Layout::create();
    page2->setContentSize(list->getContentSize());
    page2->setCascadeOpacityEnabled(true);
    list->addPage(page2);
    list->setCurrentPageIndex(0);
    
    stepX = page2->getContentSize().width / 2;
    
    int yPos = page2->getContentSize().height - margineY;
    
    //wild
    auto wild = Sprite::create("ui/Slots/wild_badge.png");
    wild->setPosition(Vec2( stepX * 0.5f, yPos));
    page2->addChild(wild);
    
    //bonus
    auto bonus = Sprite::create("ui/Slots/bonus_badge.png");
    bonus->setPosition(Vec2( stepX * 1.5f, yPos));
    page2->addChild(bonus);
    
    int wildItemID  = SlotsConfig::itemNameToID("Wild");
    int bonusItemID = SlotsConfig::itemNameToID("Bonus");
    
    if (wildItemID > -1)
    {
        yPos = page2->getContentSize().height - margineY - 125.0f;
        
        auto item = Sprite::create(StringUtils::format("ui/Slots/%s/items/%s.png", currentTheme.c_str(), SlotsConfig::payouts_items[wildItemID].c_str() ));
        item->setCascadeOpacityEnabled(true);
        item->setPosition(Vec2(stepX * 0.5f, yPos));
        page2->addChild(item);
        
        yPos = -lineSpacing;
        
        for (int k = 5; k > 1; k--)
        {
            int amount = SlotsConfig::payouts[k - 2][wildItemID];
            
            if (amount > 0)
            {
                auto label = Label::createWithTTF(StringUtils::format("%d", k), "fonts/Berlin.ttf", 26.0f);
                label->enableBold();
                label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                label->setPosition(10, yPos);
                item->addChild(label);
                
                auto desc = Label::createWithTTF(StringUtils::format("%d", amount), "fonts/Berlin.ttf", 24.0f);
                desc->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                desc->setPosition(35, yPos);
                item->addChild(desc);
                
                yPos = label->getPositionY() - lineSpacing;
            }
        }
        
        auto note = Label::createWithTTF("Wild symbols count as every other symbol except Bonus.", "fonts/Berlin.ttf", 28.0f);
        note->setDimensions(stepX * 0.8f, 0);
        note->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        note->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
        note->setPosition(Vec2(item->getContentSize().width / 2, -200));
        item->addChild(note);
    }
    
    if (bonusItemID > -1)
    {
        yPos = page2->getContentSize().height - margineY - 125.0f;
        
        auto item = Sprite::create(StringUtils::format("ui/Slots/%s/items/%s.png", currentTheme.c_str(), SlotsConfig::payouts_items[bonusItemID].c_str() ));
        item->setCascadeOpacityEnabled(true);
        item->setPosition(Vec2(stepX * 1.5f, yPos));
        page2->addChild(item);
        
        auto note = Label::createWithTTF("Get 3 or more bonus symbols to play a bonus game!", "fonts/Berlin.ttf", 28.0f);
        note->setDimensions(stepX * 0.8f, 0);
        note->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        note->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
        note->setPosition(Vec2(item->getContentSize().width / 2, -200));
        item->addChild(note);
    }
    
    //page #3
    auto page3 = ui::Layout::create();
    page3->setContentSize(list->getContentSize());
    page3->setCascadeOpacityEnabled(true);
    list->addPage(page3);
    
    {
    
        //title
        auto title = Sprite::create("ui/Slots/payouts_title.png");
        title->setPosition(Vec2( page3->getContentSize().width / 2, page3->getContentSize().height - 25.0f));
        page3->addChild(title);
        
        auto note = Label::createWithTTF("payouts awarded from left to right reel", "fonts/Berlin.ttf", 28.0f);
        note->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        note->setPosition(Vec2(page3->getContentSize().width / 2, page3->getContentSize().height - 75.0f));
        page3->addChild(note);
        
        int stepsW = 5;
        int stepsH = 4;
        
        if (currentLines == 20)
        {
            stepX = page3->getContentSize().width / stepsW;
            stepY = (page3->getContentSize().height - 120.0f) / stepsH;
        }
        else
        {
            stepsW = 6;
            stepsH = 5;
            stepX =  page3->getContentSize().width / stepsW;
            stepY = (page3->getContentSize().height - 120.0f) / stepsH;
        }
        
        for (int i = 0; i < currentLines; i++)
        {
            Vec2 point = Vec2(stepX * (0.5f + (i % stepsW)), page3->getContentSize().height - 120 - stepY * (0.5f + (i / stepsW)));
            
            auto item = Node::create();
            item->setCascadeOpacityEnabled(true);
            item->setPosition(point);
            page3->addChild(item);
            
            auto number = Label::createWithTTF(StringUtils::format("%d", i + 1), "fonts/Berlin.ttf", 28.0f);
            number->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            number->setPosition(Vec2(0, 50.0f));
            item->addChild(number);
            
            Color3B color = Color3B(SlotsConfig::lines_colors[i][0], SlotsConfig::lines_colors[i][1], SlotsConfig::lines_colors[i][2]);
            
            //generate scheme
            for (int x = 0; x < 5; x++)
            {
                for (int y = 0; y < 3; y++)
                {
                    auto box = Sprite::create("ui/Slots/box.png");
                    item->addChild(box);
                    
                    if (SlotsConfig::conditions[i][x] == (y - 1))
                    {
                        box->setColor(color);
                    }
                    else
                    {
                        box->setColor(Color3B::GRAY);
                    }
                    box->setPosition(Vec2((x - 2) * box->getContentSize().width, -(y - 1) * box->getContentSize().height));
                }
            }
        }
    }
}


void PayoutsView::onButtonPressed(Ref * sender, ui::Widget::TouchEventType event)
{
    auto button = dynamic_cast<ui::Button*>(sender);
    
    if (event == ui::Widget::TouchEventType::BEGAN)
    {
        AudioManager::getInstance().PlaySoundEffect("music/SlotsSound/TapBt");
        button->setColor(Color3B(178, 178, 178));
    }
    else if (event == ui::Widget::TouchEventType::ENDED)
    {
        button->setColor(Color3B::WHITE);
        
        ProcessButtonAction(button);
    }
    else if (event == ui::Widget::TouchEventType::CANCELED)
    {
        button->setColor(Color3B::WHITE);
    }
}

void PayoutsView::ProcessButtonAction(cocos2d::ui::Button * button)
{
    if (button->getName() == "left")
    {
        list->scrollToPage(list->getCurrentPageIndex() - 1);
    }
    else if (button->getName() == "right")
    {
        list->scrollToPage(list->getCurrentPageIndex() + 1);
    }
    else if (button->getName() == "close")
    {
        button->setEnabled(false);
        hide();
    }
}
