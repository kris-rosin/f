//
//  ShopView.cpp
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 03.04.17.
//
//

#include "ShopView.h"
#include "AudioManager.h"
#include "Features/mConstants.h"
#include "Features/PaymentManager.h"
#include "Features/AdmobChartboost.h"
#include "Utilities.h"
#include "AndroidCompatibility.h"
#include "PlayerProfile.h"
#include "ShareManager/ShareManager.h"
#include "BaseScene.h"

USING_NS_CC;

ShopView::ShopView() : activeTab(nullptr)
{
    
}

ShopView * ShopView::create()
{
    ShopView * pRet = new ShopView();
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

bool ShopView::init()
{
    if (!Node::init())
        return false;
    
    setCascadeOpacityEnabled(true);
    
    //setup modal behaviour
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [](Touch * touch, Event * event) { return true; };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto background = Sprite::create("ui/Shop/shop_back.png");
    background->setCascadeOpacityEnabled(true);
    addChild(background, 1);
    setContentSize(background->getContentSize());
    
    auto close_button = ui::Button::create("ui/MainMenu/close_button.png");
    close_button->setZoomScale(0);
    close_button->setName("close");
    close_button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    close_button->setPosition(background->getContentSize() - Size(10, 10));
    close_button->addTouchEventListener(std::bind(&ShopView::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    background->addChild(close_button);
    
    // ***********
    //  coins tab
    // ***********
    auto coinsTab = Sprite::create("ui/Shop/shop_coins.png");
    coinsTab->setCascadeOpacityEnabled(true);
    coinsTab->setAnchorPoint(Vec2::ZERO);
    background->addChild(coinsTab);
    
    Tabs.pushBack(coinsTab);
    
    auto coinsButton = ui::Button::create();
    coinsButton->setScale9Enabled(true);
    coinsButton->setContentSize(Size(168, 74));
    coinsButton->setPosition(Vec2(101, 570));
    coinsButton->addClickEventListener(std::bind(&ShopView::onTabClicked, this, std::placeholders::_1));
    coinsTab->addChild(coinsButton);
    
    auto rootNode1 = Node::create();
    rootNode1->setCascadeOpacityEnabled(true);
    rootNode1->setTag(42);
    coinsTab->addChild(rootNode1);
    
    auto table = ui::ScrollView::create();
    table->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
    table->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    table->setContentSize(Size(900, 380));
    table->setPosition(Vec2(57, 110));
    table->setScrollBarEnabled(false);
    table->setCascadeOpacityEnabled(true);
    rootNode1->addChild(table);
    
    //buy button
    auto buybutton = ui::Button::create("ui/Shop/buycoins_normal.png", "ui/Shop/buycoins_normal.png");
    buybutton->setPosition(Vec2(background->getContentSize().width / 2, table->getPositionY() / 2));
    buybutton->setName("buybutton");
    buybutton->addTouchEventListener(std::bind(&ShopView::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    rootNode1->addChild(buybutton);
    
    selector = Sprite::create("ui/Shop/shop_selector.png");
    selector->setVisible(false);
    rootNode1->addChild(selector);
    
    //free coins
    auto freecoins = ui::Button::create("ui/Shop/getfreecoins_normal.png", "ui/Shop/getfreecoins_normal.png");
    freecoins->setPosition(Vec2(table->getPositionX() + table->getContentSize().width, table->getPositionY() / 2));
    freecoins->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    freecoins->setName("freecoins");
    freecoins->addTouchEventListener(std::bind(&ShopView::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    rootNode1->addChild(freecoins);

    //facebook coins
    auto fbcoins = ui::Button::create("ui/Shop/getfreecoins_facebook.png", "ui/Shop/getfreecoins_facebook.png");
    fbcoins->setPosition(Vec2(0 + table->getPositionX()/2, table->getPositionY() / 2));
    fbcoins->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    fbcoins->setName("fb_coins");
    fbcoins->addTouchEventListener(std::bind(&ShopView::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    rootNode1->addChild(fbcoins);
    
    //IAPs definitions
    //                       IAP_ID, coins, bonus %, price , tag (0, 1 - best price, 2 - most popular)
    //hackyhack_publish
    std::vector<std::tuple<std::string, int, int, std::string, int> > iaps = {
        { IAP1_STRING_ID, IAP1_ORI_VALUE, IAP1_ORI_PERCENTAGE, IAP1_PRICE, 1},
        { IAP2_STRING_ID, IAP2_ORI_VALUE, IAP2_ORI_PERCENTAGE, IAP2_PRICE, 0},
        { IAP3_STRING_ID, IAP3_ORI_VALUE, IAP3_ORI_PERCENTAGE, IAP3_PRICE, 2},
        { IAP4_STRING_ID, IAP4_ORI_VALUE, IAP4_ORI_PERCENTAGE, IAP4_PRICE, 0},
        { IAP5_STRING_ID, IAP5_ORI_VALUE, IAP5_ORI_PERCENTAGE, IAP5_PRICE, 0},
        { IAP6_STRING_ID, IAP6_ORI_VALUE, IAP6_ORI_PERCENTAGE, IAP6_PRICE, 0}
    };
    
    auto proto = Sprite::create("ui/Shop/shop_purple_strip.png");
    table->setInnerContainerSize(Size(table->getContentSize().width, std::max(table->getContentSize().height, iaps.size() * proto->getContentSize().height)));
    
    float yPos = table->getInnerContainerSize().height - proto->getContentSize().height / 2;
    
    for (auto &item: iaps)
    {
        std::string button_back = (std::get<4>(item) > 0 ? "ui/Shop/shop_purple_strip.png" : "ui/Shop/shop_blue_strip.png");
        auto button = ui::Button::create(button_back, button_back);
        button->setName(std::get<0>(item));
        button->setCascadeOpacityEnabled(true);
        button->addTouchEventListener(std::bind(&ShopView::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
        button->setPosition(Vec2(button->getContentSize().width / 2, yPos));
        table->getInnerContainer()->addChild(button);
        
        yPos -= button->getContentSize().height;
        
        //badges
        if (std::get<4>(item) > 0)
        {
            auto badge = Sprite::create(std::get<4>(item) == 1 ? "ui/Shop/shop_best_value.png" : "ui/Shop/shop_most_popular.png");
            badge->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            button->addChild(badge);
            
            badge->setPosition(Vec2(button->getContentSize().width + 10, button->getContentSize().height / 2));
        }
        
        auto coinIcon2 = Sprite::create("ui/Shop/coin.png");
        coinIcon2->setPosition(Vec2(70, button->getContentSize().height * 0.54f));
        button->addChild(coinIcon2);
        
        /*auto coins = Label::createWithTTF( FormatWithCommas(std::get<1>(item)), "fonts/Berlin.ttf", 30);
        coins->setColor(Color3B::WHITE);
        coins->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        coins->setPosition(Vec2(90, button->getContentSize().height * 0.54f));
        button->addChild(coins);*/
        
        /*auto plusSign = Label::createWithTTF( "+", "fonts/Berlin.ttf", 30);
        plusSign->setColor(Color3B::WHITE);
        plusSign->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        plusSign->setPosition(Vec2(210, button->getContentSize().height * 0.54f));
        button->addChild(plusSign);*/
        
        /*auto procents = Label::createWithTTF(StringUtils::format("%d%%", std::get<2>(item)), "fonts/Berlin.ttf", 30);
        procents->setColor(Color3B::WHITE);
        procents->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        procents->setPosition(Vec2(283, button->getContentSize().height * 0.54f));
        button->addChild(procents);*/
        
        /*auto equalsSign = Label::createWithTTF( "=", "fonts/Berlin.ttf", 30);
        equalsSign->setColor(Color3B::WHITE);
        equalsSign->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        equalsSign->setPosition(Vec2(345, button->getContentSize().height * 0.54f));
        button->addChild(equalsSign);
        
        auto coinIcon = Sprite::create("ui/Shop/coin.png");
        coinIcon->setPosition(Vec2(370, button->getContentSize().height * 0.54f));
        button->addChild(coinIcon);*/
        
        int totalCoins = std::get<1>(item) + std::get<1>(item) / 100 * std::get<2>(item);
        
        auto total = Label::createWithTTF( FormatWithCommas(totalCoins), "fonts/gadugi.ttf", 30);
        total->setColor(Color3B::WHITE);
        total->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        total->setPosition(Vec2(90, button->getContentSize().height * 0.54f));
        button->addChild(total);
        
        auto price = Label::createWithTTF(std::get<3>(item), "fonts/gadugi.ttf", 30);
        price->setColor(Color3B::WHITE);
        price->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        price->setPosition(Vec2(700-10, button->getContentSize().height * 0.54f));
        button->addChild(price);
        
        auto usd = Label::createWithTTF("USD", "fonts/gadugi.ttf", 30);
        usd->setColor(Color3B(192, 192, 192));
        usd->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        usd->setPosition(Vec2(705-10, button->getContentSize().height * 0.54f));
        button->addChild(usd);
    }
    
    // ***********
    //  vip tab
    // ***********
    auto vipTab = Sprite::create("ui/Shop/shop_vip.png");
    vipTab->setAnchorPoint(Vec2::ZERO);
    vipTab->setCascadeOpacityEnabled(true);
    background->addChild(vipTab);
    
    Tabs.pushBack(vipTab);
    
    auto vipButton = ui::Button::create();
    vipButton->setScale9Enabled(true);
    vipButton->setContentSize(Size(168, 74));
    vipButton->setPosition(Vec2(263, 570));
    vipButton->addClickEventListener(std::bind(&ShopView::onTabClicked, this, std::placeholders::_1));
    vipTab->addChild(vipButton);
    
    auto rootNode2 = Node::create();
    rootNode2->setCascadeOpacityEnabled(true);
    rootNode2->setTag(42);
    vipTab->addChild(rootNode2);
    
    // info buttons
    auto infoVIP = ui::Button::create("ui/Shop/btn_info.png");
    infoVIP->setZoomScale(0);
    infoVIP->setPosition(Vec2(443, 406));
    infoVIP->setName("info_vip");
    infoVIP->addTouchEventListener(std::bind(&ShopView::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    rootNode2->addChild(infoVIP);
    
    auto infoVVIP = ui::Button::create("ui/Shop/btn_info.png");
    infoVVIP->setZoomScale(0);
    infoVVIP->setPosition(Vec2(749, 406));
    infoVVIP->setName("info_vvip");
    infoVVIP->addTouchEventListener(std::bind(&ShopView::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    rootNode2->addChild(infoVVIP);
    
    // subscription buttons
    for (int i = 0; i < 3; i++)
    {
        auto subbtn = ui::Button::create("ui/Shop/btn_vip_day.png");
        subbtn->setZoomScale(0);
        subbtn->setName(StringUtils::format("vip_days_%d", std::get<0>(vipPrices[i])) );
        subbtn->addTouchEventListener(std::bind(&ShopView::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
        subbtn->setPosition(Vec2(339, 241 - i * 51));
        rootNode2->addChild(subbtn);
        
        auto days = Label::createWithTTF(StringUtils::format("%d Days", std::get<0>(vipPrices[i])), "fonts/gadugi.ttf", 22.0f);
        days->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        days->setPosition(Vec2(21, subbtn->getContentSize().height * 0.5f));
        subbtn->addChild(days);
        
        auto price = Label::createWithTTF(std::get<1>(vipPrices[i]), "fonts/gadugi.ttf", 22.0f);
        price->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        price->setPosition(Vec2(115, subbtn->getContentSize().height * 0.5f));
        subbtn->addChild(price);
        
        
        auto subbtn2 = ui::Button::create("ui/Shop/btn_vip_day.png");
        subbtn2->setZoomScale(0);
        subbtn2->setName(StringUtils::format("vvip_days_%d", std::get<0>(vvipPrices[i])) );
        subbtn2->addTouchEventListener(std::bind(&ShopView::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
        subbtn2->setPosition(Vec2(645, 241 - i * 51));
        rootNode2->addChild(subbtn2);
        
        auto days2 = Label::createWithTTF(StringUtils::format("%d Days", std::get<0>(vvipPrices[i])), "fonts/gadugi.ttf", 22.0f);
        days2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        days2->setPosition(Vec2(21, subbtn2->getContentSize().height * 0.5f));
        subbtn2->addChild(days2);
        
        auto price2 = Label::createWithTTF(std::get<1>(vvipPrices[i]), "fonts/gadugi.ttf", 22.0f);
        price2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        price2->setPosition(Vec2(115, subbtn2->getContentSize().height * 0.5f));
        subbtn2->addChild(price2);
    }
    
    // ***********
    //  pig tab
    // ***********
    {
        auto pigTab = Sprite::create("ui/Shop/shop_pig.png");
        pigTab->setAnchorPoint(Vec2::ZERO);
        pigTab->setCascadeOpacityEnabled(true);
        background->addChild(pigTab);
        
        Tabs.pushBack(pigTab);
        
        auto pigButton = ui::Button::create();
        pigButton->setScale9Enabled(true);
        pigButton->setContentSize(Size(168, 74));
        pigButton->setPosition(Vec2(433, 570));
        pigButton->addClickEventListener(std::bind(&ShopView::onTabClicked, this, std::placeholders::_1));
        pigTab->addChild(pigButton);
        
        auto rootNode3 = Node::create();
        rootNode3->setCascadeOpacityEnabled(true);
        rootNode3->setTag(42);
        pigTab->addChild(rootNode3);
        
        auto breakButton = ui::Button::create("ui/Shop/break_button.png");
        breakButton->setZoomScale(0);
        breakButton->setCascadeOpacityEnabled(true);
        breakButton->setPosition(Vec2(pigTab->getContentSize().width / 2, 91));
        breakButton->setName("break_pig");
        breakButton->addTouchEventListener(std::bind(&ShopView::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
        rootNode3->addChild(breakButton);
        
        auto breakLabel = Label::createWithTTF(StringUtils::format("Break now for %s", IAP_PIGGY_BANK_PRICE.c_str()), "fonts/Berlin.ttf", 28.0f);
        breakLabel->setPosition(Vec2(0, 5) + breakButton->getContentSize() / 2);
        breakLabel->setColor(Color3B::WHITE);
        breakLabel->enableWrap(false);
        breakLabel->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        breakLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        breakLabel->setOverflow(cocos2d::Label::Overflow::SHRINK);
        breakLabel->setDimensions(breakButton->getContentSize().width * 0.8f, 0);
        breakButton->addChild(breakLabel);
        
        auto hammer = Sprite::create("ui/Shop/hammer.png");
        hammer->setPosition(Vec2(10, breakButton->getContentSize().height / 2 + 5));
        breakButton->addChild(hammer);
        
        auto coin = Sprite::create("ui/Shop/animated_coin.png");
        coin->setPosition(Vec2(breakButton->getContentSize().width + 2, breakButton->getContentSize().height + 5));
        coin->setCascadeOpacityEnabled(true);
        breakButton->addChild(coin);
        
        auto spark = Sprite::create("ui/Shop/spark.png");
        spark->setPosition(coin->getContentSize() / 2);
        coin->addChild(spark);
        setSparkBlink(spark);
        
        auto savedLabel = Sprite::create("ui/Shop/saved_coins.png");
        savedLabel->setPosition(Vec2(pigTab->getContentSize().width / 2, 218));
        rootNode3->addChild(savedLabel);
        
        auto savedCoins = PlayerProfile::getInstance().getUIntValue("PIGGY_BANK_COINS", 0);
        coinsLabel = Label::createWithBMFont("fonts/pigbank.fnt", FormatWithCommas(savedCoins));
        coinsLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        coinsLabel->setPosition(Vec2(pigTab->getContentSize().width / 2, 218));
        rootNode3->addChild(coinsLabel);
    }
    
    // Defaults
    activeTab = coinsTab;
    activeTab->setLocalZOrder(100);
    
    return true;
}

void ShopView::show(cocos2d::Node * parent)
{
    auto scene = findScene(parent);
    
    if (scene)
    {
        Vec2 point = scene->convertToNodeSpace(Director::getInstance()->getWinSize() / 2);
        
        this->setPosition(point);
        this->setOpacity(0);
        this->runAction(FadeIn::create(0.3f) );
        
        scene->addChild(this, 10000);
    }
}

void ShopView::hide()
{
    this->runAction(Sequence::create(EaseSineIn::create(FadeOut::create(0.3f)), RemoveSelf::create(), nullptr));
}

Scene * ShopView::findScene(Node * node)
{
    auto scene = dynamic_cast<Scene*>(node);
    
    if (scene)
    {
        return scene;
    }
    
    return findScene(node->getParent());
}

void ShopView::onButtonPressed(Ref * sender, ui::Widget::TouchEventType event)
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

void ShopView::ProcessButtonAction(cocos2d::ui::Button * button)
{
    if (button->getName() == "close")
    {
        button->setEnabled(false);
        hide();
    }
    else if (button->getName() == "freecoins")
    {
        log("Free button pressed!");
        AdmobChartboost::shared()->playRewardedVideo(0);
    }
    else if (button->getName() == "fb_coins")
    {
        log("Facebook button pressed!");
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        ShareManager::getInstance().shareURLFacebook("https://www.facebook.com/casinoomania", std::bind(&ShopView::onSharingCompleted, this, std::placeholders::_1));
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        ShareManager::getInstance().shareURLFacebook("https://www.facebook.com/casinoomania", std::bind(&ShopView::onSharingCompleted, this, std::placeholders::_1));
#endif

    }
    else if (button->getName() == "buybutton")
    {
        if (selectedProductID.empty() == false)
        {
            log("Attempt to buy inApp: %s", selectedProductID.c_str());
            
            PaymentManager::shared()->buyFeature(selectedProductID.c_str());
        }
    }
    else if (button->getName() == "break_pig")
    {
        log("Attempt to buy inApp: %s", IAP_PIGGY_BANK_ID.c_str());
        PaymentManager::shared()->buyFeatureCallback(IAP_PIGGY_BANK_ID.c_str(), [this](int value, std::string productID)
        {
            if (IAP_PIGGY_BANK_ID == productID)
            {
                //refresh label
                coinsLabel->setString(FormatWithCommas(PlayerProfile::getInstance().getUIntValue("PIGGY_BANK_COINS", 0)));
            }
        });
    }
    else if (button->getName() == "info_vip")
    {
        showVipInfo(button, false);
    }
    else if (button->getName() == "info_vvip")
    {
        showVipInfo(button, true);
    }
    else if (button->getName() == "vip_days_3")
    {
        PaymentManager::shared()->buyFeature(IAP_VIP1_ID.c_str());
    }
    else if (button->getName() == "vip_days_7")
    {
        PaymentManager::shared()->buyFeature(IAP_VIP2_ID.c_str());
    }
    else if (button->getName() == "vip_days_14")
    {
        PaymentManager::shared()->buyFeature(IAP_VIP3_ID.c_str());
    }
    else if (button->getName() == "vvip_days_3")
    {
        PaymentManager::shared()->buyFeature(IAP_VVIP1_ID.c_str());
    }
    else if (button->getName() == "vvip_days_7")
    {
        PaymentManager::shared()->buyFeature(IAP_VVIP2_ID.c_str());
    }
    else if (button->getName() == "vvip_days_14")
    {
        PaymentManager::shared()->buyFeature(IAP_VVIP3_ID.c_str());
    }
    else
    {
        selectedProductID = button->getName();
        
        selector->retain();
        selector->removeFromParent();
        button->addChild(selector);
        selector->release();
        selector->setVisible(true);
        selector->setPosition(Vec2(30, button->getContentSize().height / 2 + 2));
    }
}

void ShopView::onSharingCompleted(bool success)
{
    if (success)
    {
        log("Success!");
        //TODO: put here reward
        auto scene = BaseScene::findBaseScene();
        if (scene)
        {
            scene->increasePlayerValues(SHARE_FACEBOOK_REWARD_COIN, 0, false);
        }

        return;
    }
    
    log("Fail!");
}

void ShopView::onTabClicked(Ref * sender)
{
    auto button = dynamic_cast<ui::Button*>(sender);
    
    if (button && activeTab != button->getParent())
    {
        activeTab = button->getParent();
        
        int i = 0;
        for (auto &tab : Tabs)
        {
            tab->setLocalZOrder(tab == activeTab ? 100 : i);
            
            auto rootNode = tab->getChildByTag(42);
            if (rootNode)
            {
                rootNode->setVisible(tab == activeTab);
            }
            
            i++;
        }
    }
}

void ShopView::setSparkBlink(Sprite * spark)
{
    //hackyhack_publish - arc4random_uniform
//    float scaleUpTime  = 0.05f + 0.2f * 0.01f * arc4random_uniform(100);
//    float scaleUpScale = 1.00f + 1.2f * 0.01f * arc4random_uniform(100);
    float scaleUpTime  = 0.05f + 0.2f * 0.01f * (rand() % 100);
    float scaleUpScale = 1.00f + 1.2f * 0.01f * (rand() % 100);
    
    spark->runAction(Sequence::create(ScaleTo::create(scaleUpTime, scaleUpScale), ScaleTo::create(scaleUpTime, 1.0f), CallFunc::create(std::bind(&ShopView::setSparkBlink, this, spark)), NULL));
}

void ShopView::showVipInfo(Node * sender, bool supervip)
{
    auto panel = Sprite::create(supervip ? "ui/Shop/vvip_info.png" : "ui/Shop/vip_info.png");
    panel->setAnchorPoint(Vec2(supervip ? 1.0f - 0.066f : 0.066f, 1.0f));
    panel->setPosition(sender->convertToWorldSpace(sender->getContentSize() / 2));
    
    auto touchArea = ui::Button::create();
    touchArea->setScale9Enabled(true);
    touchArea->setAnchorPoint(Vec2::ZERO);
    touchArea->setContentSize(Director::getInstance()->getWinSize()); // full screen
    touchArea->addTouchEventListener([touchArea](Ref * sender, ui::Widget::TouchEventType event)
    {
        if (event == ui::Widget::TouchEventType::ENDED)
            touchArea->removeFromParent();
    });
    
    touchArea->addChild(panel);
    touchArea->setPosition(this->convertToNodeSpace(Vec2::ZERO));
    this->addChild(touchArea, 100);
    
    // text info
    std::string headerTemplate = (supervip ? "%d days VVIP - %s" : "%d days VIP - %s");
    
    std::vector<std::string> lines =
    {
        "i. %dx experience gaining",
        "ii. %d coins for every level increment",
        "iii. Bonus daily multiplied by level x%d"
    };
    
    auto values = supervip ? vvipBonuses : vipBonuses;
    auto prices = supervip ? vvipPrices  : vipPrices;
    
    for (int i = 0; i < 3; i++)
    {
        std::string header = StringUtils::format(headerTemplate.c_str(), std::get<0>(prices[i]), std::get<1>(prices[i]).c_str() );
        
        auto labelH = Label::createWithTTF(header, "fonts/Berlin.ttf", 20.0f);
        labelH->setColor(Color3B::WHITE);
        labelH->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelH->setPosition(Vec2(60, 280 - i * 96));
        panel->addChild(labelH);
        
        for (int k = 0; k < lines.size(); k++)
        {
            std::string subline = StringUtils::format(lines[k].c_str(), std::get<0>(values), std::get<1>(values) , std::get<2>(values) );
            
            auto labelS = Label::createWithTTF(subline, "fonts/Berlin.ttf", 20.0f);
            labelS->setColor(Color3B::WHITE);
            labelS->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            labelS->setPosition(Vec2(76, 280 - i * 96 - (k + 1) * 18));
            panel->addChild(labelS);
        }
    }
}
