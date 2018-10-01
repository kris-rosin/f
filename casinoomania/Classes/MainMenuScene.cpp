#include "MainMenuScene.h"
#include "AudioManager.h"

#include "PlayerProfile.h"
#include "TopBar.h"
#include "BottomBar.h"

#include "LoadingSplash.h"

#include "SlotsSelectThemeScene.h"
#include "RouletteSelectThemeScene.h"
#include "BJSelectThemeScene.h"
#include "BaccaratSelectThemeScene.h"
#include "CasinowarThemeScene.h"

#include "RatemeAlert.h"
#include "FacebookAddFriendsPopup.h"
#include "SignupView.h"

#include "Features/mConstants.h"

USING_NS_CC;

int MainMenuScene::openedTimes = 0;

Scene * MainMenuScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = MainMenuScene::create();
    scene->addChild(layer);

    return scene;
}

MainMenuScene::MainMenuScene() : userName(nullptr)
{
    openedTimes++;
}

MainMenuScene::~MainMenuScene()
{
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

bool MainMenuScene::init()
{
    if ( !BaseScene::init() )
    {
        return false;
    }
    
    //hackyhack_rate_me
//    if (PlayerProfile::getInstance().getLevel() < 8) {
//        if (PlayerProfile::getInstance().getOpenCountApp() % CYCLE_RATEME_POPUP == 0 &&
//            PlayerProfile::getInstance().getOpenCountApp() != 0) {
//            PlayerProfile::getInstance().setOpenCountApp(0);
//            RatemeAlert::create(MCONSTANTS_TEXT_RATEME)->show(this);
//        }
//    } else if (PlayerProfile::getInstance().getOpenCountApp() % (CYCLE_RATEME_POPUP * 3) == 0 &&
//               PlayerProfile::getInstance().getOpenCountApp() != 0) {
//        PlayerProfile::getInstance().setOpenCountApp(0);
//        RatemeAlert::create(MCONSTANTS_TEXT_RATEME)->show(this);
//    }
    
//    RatemeAlert::create(MCONSTANTS_TEXT_RATEME)->show(this);
    
    auto winSize = Director::getInstance()->getWinSize();
    auto visSize = Director::getInstance()->getVisibleSize();
    auto visOrig = Director::getInstance()->getVisibleOrigin();
    
    //background
    auto background = Sprite::create("ui/MainMenu/main_background.png");
    background->setPosition(winSize / 2);
    addChild(background);
    
    //logo
    auto logo = Sprite::create("ui/MainMenu/casino_logo.png");
    addChild(logo);
    logo->setPosition(Vec2(winSize.width / 2, visOrig.y + visSize.height - 135));
    
    //topbar
    auto topbar = TopBar::create(nullptr);
    topbar->setPosition(Size(winSize.width / 2, winSize.height));
    addChild(topbar, 90);
    
    //bottombar
    auto bottombar = BottomBar::create();
    bottombar->setPosition(Size(winSize.width / 2, 0));
    bottombar->setScale(Director::getInstance()->getVisibleSize().width / bottombar->getContentSize().width);
    addChild(bottombar, 90);
    
    //scroll area
    int margin = 15;
    int insertPosition = margin;
    
    auto scrollList = ui::ScrollView::create();
    scrollList->setAnchorPoint(Vec2::ZERO);
    scrollList->setContentSize(Size(visSize.width, 456));
    scrollList->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    scrollList->setPosition(Vec2(visOrig.x, 140));
    scrollList->setScrollBarEnabled(false);
    addChild(scrollList);
    
    //slots
    auto slots = ui::Button::create("ui/MainMenu/slots_icon.png", "ui/MainMenu/slots_icon.png");
    slots->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    slots->setPosition(Vec2(insertPosition, scrollList->getContentSize().height / 2));
    scrollList->getInnerContainer()->addChild(slots);
    insertPosition += slots->getContentSize().width + margin;
    slots->setName("slots");
    slots->addTouchEventListener(std::bind(&MainMenuScene::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    
    //roulette
    auto roulette = ui::Button::create("ui/MainMenu/roulette_icon.png", "ui/MainMenu/roulette_icon.png");
    roulette->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    roulette->setPosition(Vec2(insertPosition, scrollList->getContentSize().height / 2));
    scrollList->getInnerContainer()->addChild(roulette);
    insertPosition += roulette->getContentSize().width + margin;
    roulette->setName("roulette");
    roulette->addTouchEventListener(std::bind(&MainMenuScene::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    
    //blackjack
    auto blackjack = ui::Button::create("ui/MainMenu/blackjack_icon.png", "ui/MainMenu/blackjack_icon.png");
    blackjack->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    blackjack->setPosition(Vec2(insertPosition, scrollList->getContentSize().height / 2));
    scrollList->getInnerContainer()->addChild(blackjack);
    insertPosition += blackjack->getContentSize().width + margin;
    blackjack->setName("blackjack");
    blackjack->addTouchEventListener(std::bind(&MainMenuScene::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    
    //baccarat
    auto baccarat = ui::Button::create("ui/MainMenu/baccarat_icon.png", "ui/MainMenu/baccarat_icon.png");
    baccarat->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    baccarat->setPosition(Vec2(insertPosition, scrollList->getContentSize().height / 2));
    scrollList->getInnerContainer()->addChild(baccarat);
    insertPosition += baccarat->getContentSize().width + margin;
    baccarat->setName("baccarat");
    baccarat->addTouchEventListener(std::bind(&MainMenuScene::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    
    //casino war
    auto casinowar = ui::Button::create("ui/MainMenu/casinowar_icon.png", "ui/MainMenu/casinowar_icon.png");
    casinowar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    casinowar->setPosition(Vec2(insertPosition, scrollList->getContentSize().height / 2));
    scrollList->getInnerContainer()->addChild(casinowar);
    insertPosition += casinowar->getContentSize().width + margin;
    casinowar->setName("casinowar");
    casinowar->addTouchEventListener(std::bind(&MainMenuScene::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    
#ifdef CAPSA_IN_DEVELOPING
    //capsa
    auto capsa = ui::Button::create("ui/MainMenu/capsa_icon.png", "ui/MainMenu/capsa_icon.png");
    capsa->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    capsa->setPosition(Vec2(insertPosition, scrollList->getContentSize().height / 2));
    scrollList->getInnerContainer()->addChild(capsa);
    insertPosition += capsa->getContentSize().width + margin;
    capsa->setName("capsa");
    capsa->addTouchEventListener(std::bind(&MainMenuScene::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
#endif
    
    scrollList->setInnerContainerSize(Size(std::max(insertPosition, (int)scrollList->getContentSize().width), scrollList->getContentSize().height));

    return true;
}

void MainMenuScene::onEnter()
{
    Layer::onEnter();
    
    AudioManager::getInstance().PlayBackgroundMusic("music/BackgroundMusic/Home");
}

void MainMenuScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    
    auto launches = PlayerProfile::getInstance().getOpenCountApp();
    
    if (openedTimes == 1)
    {
        FacebookAddFriendsPopup::create(MCONSTANTS_TEXT_RATEME)->show(this);
    }
    if (openedTimes == 4)
    {
        RatemeAlert::create(MCONSTANTS_TEXT_RATEME)->show(this);
    }
    
    if ( ( launches + 8) % 10 == 0 )
    {
        if (PlayerProfile::getInstance().getEmail().empty() && PlayerProfile::getInstance().getFBUserID().empty())
        {
            SignupView::create()->show(this);
        }
    }
}

void MainMenuScene::onButtonPressed(Ref * sender, ui::Widget::TouchEventType event)
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

void MainMenuScene::ProcessButtonAction(cocos2d::ui::Button * button)
{
    if (button->getName() == "slots")
    {
        auto nextScene = LoadingSplash::createScene(LoadingSplash::SLOTS_THEME);
        Director::getInstance()->replaceScene(nextScene);
    }
    else if (button->getName() == "roulette")
    {
        auto nextScene = LoadingSplash::createScene(LoadingSplash::ROULETTE_THEME);
        Director::getInstance()->replaceScene(nextScene);
    }
    else if (button->getName() == "blackjack")
    {
        auto nextScene = LoadingSplash::createScene(LoadingSplash::BLACKJACK_THEME);
        Director::getInstance()->replaceScene(nextScene);
    }
    else if (button->getName() == "baccarat")
    {
        auto nextScene = LoadingSplash::createScene(LoadingSplash::BACCARAT_THEME);
        Director::getInstance()->replaceScene(nextScene);
    }
    else if (button->getName() == "casinowar")
    {
        auto nextScene = LoadingSplash::createScene(LoadingSplash::CASINOWAR_THEME);
        Director::getInstance()->replaceScene(nextScene);
    }
#ifdef CAPSA_IN_DEVELOPING
    else if (button->getName() == "capsa")
    {
        auto nextScene = LoadingSplash::createScene(LoadingSplash::CAPSA_THEME);
        Director::getInstance()->replaceScene(nextScene);
    }
#endif
}
