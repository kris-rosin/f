#include "CasinowarGameScene.h"

#include "LoadingSplash.h"
#include "PlayerProfile.h"

#include "CongratulationsView.h"
#include "AudioManager.h"
#include "AndroidCompatibility.h"

#include "CardObject.h"
#include "Deck.h"

#include "Features/AdmobChartboost.h"
#include "Features/mConstants.h"

#include "RatemeAlert.h"

USING_NS_CC;

CasinowarGameScene * CasinowarGameScene::create(std::string theme)
{
    CasinowarGameScene * pRet = new(std::nothrow)CasinowarGameScene();
    if (pRet && pRet->init(theme))
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

Scene * CasinowarGameScene::createScene(std::string theme)
{
    auto scene = Scene::create();
    
    auto layer = CasinowarGameScene::create(theme);
    scene->addChild(layer);

    return scene;
}

CasinowarGameScene::CasinowarGameScene() : deck(nullptr), player(nullptr), dealer(nullptr), historyOpened(false), historyRoot(nullptr), gameStarted(false)
{
    
}

bool CasinowarGameScene::init(std::string theme)
{
    if ( !BaseScene::init() )
    {
        return false;
    }
    
    currentTheme = theme;
    currentChapter = CasinowarConfig::themeNameToChapter(currentTheme);
    
    auto winSize = Director::getInstance()->getWinSize();
    auto visOrig = Director::getInstance()->getVisibleOrigin();

    background = Sprite::create("ui/CasinoWar/casinowar_table.png");
    background->setPosition(winSize / 2);
    this->addChild(background, 0);
    
    // game elements
    player = CasinowarHand::create();
    dealer = CasinowarHand::create();
    
    background->addChild(player, 1);
    background->addChild(dealer);
    
    player->setPosition(Vec2(winSize.width / 2, 317));
    dealer->setPosition(Vec2(winSize.width / 2, 600));
    
    //topbar
    topbar = TopBar::create(std::bind(&CasinowarGameScene::onBackButtonPressed, this));
    topbar->setPosition(Size(background->getContentSize().width / 2, background->getContentSize().height));
    background->addChild(topbar, 100);
    
    //logo
    auto logo = Sprite::create(StringUtils::format("ui/Cities/%s/logo.png", currentTheme.c_str()));
    logo->setPosition(Vec2( visOrig.x + 15, background->getContentSize().height - 120));
    logo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    background->addChild(logo);
    
    //controls
    controlsNode = Node::create();
    controlsNode->setPosition(Vec2(background->getContentSize().width / 2, 40));
    background->addChild(controlsNode);
    
    std::vector<Node*> hudItemsToArrange;
    float total_width = 0;
    
    clearButton = ui::Button::create("ui/CasinoWar/clear_button.png", "ui/CasinoWar/clear_button.png", "ui/CasinoWar/clear_button.png");
    clearButton->setName("clear");
    clearButton->addTouchEventListener(std::bind(&CasinowarGameScene::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    hudItemsToArrange.push_back(clearButton);
    actionButtons.pushBack(clearButton);
    controlsNode->addChild(clearButton);
    
    auto dealButton = ui::Button::create("ui/CasinoWar/deal_button.png", "ui/CasinoWar/deal_button.png", "ui/CasinoWar/deal_button.png");
    dealButton->setName("deal");
    dealButton->addTouchEventListener(std::bind(&CasinowarGameScene::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    hudItemsToArrange.push_back(dealButton);
    actionButtons.pushBack(dealButton);
    controlsNode->addChild(dealButton);
    
    rebetButton = ui::Button::create("ui/CasinoWar/rebet_button.png", "ui/CasinoWar/rebet_button.png", "ui/CasinoWar/rebet_button.png");
    rebetButton->setName("rebet");
    rebetButton->addTouchEventListener(std::bind(&CasinowarGameScene::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    hudItemsToArrange.push_back(rebetButton);
    actionButtons.pushBack(rebetButton);
    controlsNode->addChild(rebetButton);
    
    // options
    surrenderButton = ui::Button::create("ui/CasinoWar/surrender_button.png", "ui/CasinoWar/surrender_button.png", "ui/CasinoWar/surrender_button.png");
    surrenderButton->setName("surrender");
    surrenderButton->addTouchEventListener(std::bind(&CasinowarGameScene::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    surrenderButton->setVisible(false);
    surrenderButton->setPositionX(-surrenderButton->getContentSize().width / 2);
    controlsNode->addChild(surrenderButton);
    
    warButton = ui::Button::create("ui/CasinoWar/war_button.png", "ui/CasinoWar/war_button.png", "ui/CasinoWar/war_button.png");
    warButton->setName("war");
    warButton->addTouchEventListener(std::bind(&CasinowarGameScene::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    warButton->setVisible(false);
    warButton->setPositionX(warButton->getContentSize().width / 2);
    controlsNode->addChild(warButton);
    
    //arrange items
    for (auto &item: hudItemsToArrange)
    {
        total_width += item->getContentSize().width;
    }
    
    float padding = 10;
    
    total_width += padding * (hudItemsToArrange.size() - 1);
    
    float posX = -total_width / 2;
    
    for (auto &item: hudItemsToArrange)
    {
        item->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        item->setPositionX(posX + item->getContentSize().width / 2);
        posX += padding + item->getContentSize().width;
    }
    
    // bet places
    normalBet = CasinowarBet(BET  , Rect(637,  86, 92, 92),  1.00f);
    raiseBet  = CasinowarBet(RAISE, Rect(767, 113, 92, 92),  1.00f);
    tieBet    = CasinowarBet(TIE  , Rect(505, 113, 92, 92), 10.00f);
    
    // bet buttons
    betPanel = Node::create();
    background->addChild(betPanel);
    betPanel->setPosition(Director::getInstance()->getVisibleOrigin()); // in left bottom corner
    
    auto betArray = CasinowarConfig::BetValues;
    
    for (int i = 0; i < 4; i++)
    {
        int index = currentChapter - 1 + i;
        
        if (index < CasinowarConfig::BetValues.size())
        {
            auto token = CoinToken::create(betArray[index], CoinToken::CoinTokenType::BUTTON, (CoinToken::CoinTokenSkin)i, nullptr);
            token->setCallback(std::bind(&CasinowarGameScene::onTokenAction, this, std::placeholders::_1, std::placeholders::_2));
            token->setPosition(Vec2(token->getContentSize().width * 0.6f, token->getContentSize().height * (0.6f + i)));
            betPanel->addChild(token);
            
            betButtons.pushBack(token);
        }
    }
    
    //history
    historyTab = Sprite::create("ui/Roulette/history_tab.png");
    historyTab->setAnchorPoint(Vec2(35 / historyTab->getContentSize().width, 0.5f));
    historyTab->setPosition(Vec2(Director::getInstance()->getVisibleSize().width + Director::getInstance()->getVisibleOrigin().x, 400));
    background->addChild(historyTab);
    
    auto openHistory = ui::Button::create();
    openHistory->setScale9Enabled(true);
    openHistory->setContentSize(Size(35, historyTab->getContentSize().height));
    openHistory->setAnchorPoint(Vec2::ZERO);
    openHistory->setName("history");
    openHistory->addTouchEventListener(std::bind(&CasinowarGameScene::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    historyTab->addChild(openHistory);
    
    historyRoot = Node::create();
    historyRoot->setContentSize(Size(65, 490));
    historyRoot->setPosition(Vec2(47, 17));
    historyRoot->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    historyTab->addChild(historyRoot);

    return true;
}

void CasinowarGameScene::onEnter()
{
    Layer::onEnter();

    PlayBackgroundMusic();
}

void CasinowarGameScene::PlayBackgroundMusic()
{
    AudioManager::getInstance().PlayBackgroundMusic("music/BackgroundMusic/Ingame");
}

void CasinowarGameScene::enableInput(bool enable)
{
    topbar->setEnabled(enable);
    
    betPanel->setVisible(enable);
    
    for (auto &button: betButtons)
    {
        button->setEnabled(enable);
    }
    
    for (auto &button: actionButtons)
    {
        button->setEnabled(enable);
        button->setVisible(enable);
    }
    
    if (normalBet.token)
        normalBet.token->setEnabled(false);
    if (tieBet.token)
        tieBet.token->setEnabled(false);
    
    surrenderButton->setVisible(false);
    warButton->setVisible(false);
}

void CasinowarGameScene::onButtonPressed(Ref * sender, ui::Widget::TouchEventType event)
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

void CasinowarGameScene::onBackButtonPressed()
{
    auto nextScene = LoadingSplash::createScene(LoadingSplash::CASINOWAR_THEME);
    Director::getInstance()->replaceScene(nextScene);
}

void CasinowarGameScene::ProcessButtonAction(cocos2d::ui::Button * button)
{
    AudioManager::getInstance().PlaySoundEffect("music/SlotsSound/TapBt");
    
    if (button->getName() == "surrender")
    {
        surrenderButton->setVisible(false);
        warButton->setVisible(false);
        
        cleanTable(SURRENDER);
    }
    else if (button->getName() == "war")
    {
        startWar();
    }
    else if (button->getName() == "history")
    {
        if (historyOpened)
        {
            //close
            historyTab->stopAllActions();
            
            float posX = Director::getInstance()->getVisibleOrigin().x + Director::getInstance()->getVisibleSize().width;
            historyTab->runAction(Sequence::create(EaseSineInOut::create( MoveTo::create(0.25f, Vec2(posX, historyTab->getPositionY() ))),
                                                   CallFunc::create([button] { button->setEnabled(true); }), NULL));
        }
        else
        {
            //open
            historyTab->stopAllActions();
            
            float posX = Director::getInstance()->getVisibleOrigin().x + Director::getInstance()->getVisibleSize().width - historyTab->getContentSize().width + historyTab->getAnchorPointInPoints().x;
            historyTab->runAction(Sequence::create(EaseSineInOut::create( MoveTo::create(0.25f, Vec2(posX, historyTab->getPositionY() ))),
                                                   CallFunc::create([button] { button->setEnabled(true); }), NULL));
        }
        
        historyOpened = !historyOpened;
        button->setEnabled(false);
    }
    else if (button->getName() == "deal")
    {
        //check bets
        int totalBet = 0;
        bool mainBet = false;
        
        if (normalBet.token)
        {
            mainBet = true;
            totalBet += normalBet.token->getTotalValue();
            previousBets[normalBet.type] = normalBet.token->getTotalValue();
        }
        else
        {
            previousBets[normalBet.type] = 0;
        }
        
        if (tieBet.token)
        {
            totalBet += tieBet.token->getTotalValue();
            previousBets[tieBet.type] = tieBet.token->getTotalValue();
        }
        else
        {
            previousBets[tieBet.type] = 0;
        }
        
        if (totalBet == 0 || !mainBet)
        {
            showPlaceBet();
            return;
        }
        
        if (totalBet > PlayerProfile::getInstance().getCoins())
        {
            showNotEnoughView();
            return;
        }
        
        PlayerProfile::getInstance().statsAddBetAmount(GAME_CASINO_WAR, totalBet);
        PlayerProfile::getInstance().addPiggyBankCoins(PIGGY_BANK_PROCENT * totalBet);
        
        //coins and experience
        int coinsChange = -totalBet;
        auto XP = PlayerProfile::getInstance().getExperienceForCoins(coinsChange);
        auto isLevelUp = PlayerProfile::getInstance().isExperienceEnoughForLevelUp(XP);
        
        if (isLevelUp)
        {
            auto currLevel = PlayerProfile::getInstance().getLevel();
            coinsChange += (currLevel + 1) * PlayerProfile::getInstance().getLevelUpBonus();
            
            CongratulationsView::create(CongratulationsView::CongratulationType::LEVELUP, nullptr)->show(this);
        }
        increasePlayerValues(coinsChange, XP, isLevelUp);
        
        //hide controls
        enableInput(false);
        
        newRound();
    }
    else if (button->getName() == "clear")
    {
        button->setEnabled(false);
        rebetButton->setEnabled(true);
        
        std::vector<CasinowarBet*> currentBets = {&normalBet, &tieBet, &raiseBet};
        
        for (auto &bet: currentBets)
        {
            if (bet->token)
            {
                auto token = bet->token;
                
                Vec2 target(background->getContentSize().width / 2, -100);
                float time = 0.5f * token->getPosition().distance(target) / 500;
                
                token->runAction(Sequence::create(MoveTo::create(time, target),
                                                  CallFunc::create([] { AudioManager::getInstance().PlaySoundEffect("music/Sounds/effect"); }),
                                                  RemoveSelf::create(), NULL));
                
                bet->token = nullptr;
            }
        }
    }
    else if (button->getName() == "rebet")
    {
        button->setEnabled(false);
        clearButton->setEnabled(true);
        
        std::vector<CasinowarBet*> currentBets = {&normalBet, &tieBet, &raiseBet};
        
        for (auto &bet: currentBets)
        {
            if (bet->token) // first remove existing
            {
                auto token = bet->token;
                
                Vec2 target(background->getContentSize().width / 2, -100);
                float time = 0.5f * token->getPosition().distance(target) / 500;
                
                token->runAction(Sequence::create(MoveTo::create(time, target),
                                                  CallFunc::create([] { AudioManager::getInstance().PlaySoundEffect("music/Sounds/effect"); }),
                                                  RemoveSelf::create(), NULL));
                
                bet->token = nullptr;
            }
            
            //place new if needed
            auto itr = previousBets.find(bet->type);
            if (itr != previousBets.end() && itr->second > 0)
            {
                // create new bet
                auto token = CoinToken::create(itr->second, CoinToken::CoinTokenType::STACK, CoinToken::CoinTokenSkin::BLACK, betButtons.front());
                background->addChild(token);
                
                Vec2 target(bet->area.origin + bet->area.size / 2);
                
                token->setPosition(Vec2(background->getContentSize().width / 2, -100));
                token->setCallback(std::bind(&CasinowarGameScene::onTokenAction, this, std::placeholders::_1, std::placeholders::_2));
                
                float time = 0.5f * token->getPosition().distance(target) / 500;
                
                token->runAction(Sequence::create(MoveTo::create(time, target),
                                                  CallFunc::create([] { AudioManager::getInstance().PlaySoundEffect("music/Sounds/effect"); }), NULL));
                
                bet->token = token;
            }
        }
    }
}

#pragma mark Core Methods

void CasinowarGameScene::newRound()
{
    totalWin = 0;
    warStarted  = false;
    gameStarted = true;
    
    if (deck == nullptr || !deck->CanPlay())
    {
        delete deck;
        
        deck = new CasinowarDeck();
    }
    
    float totalDelay = 0;
    
    auto card = deck->DrawCard();
    dealer->Hit(card, totalDelay);
    totalDelay += 0.5f;
    
    player->Hit(FORCE_TIE_CASE ? card : deck->DrawCard(), totalDelay);
    totalDelay += 0.5f;
    //totalDelay += 1.0f;
    
    runAction(Sequence::create(DelayTime::create(totalDelay), CallFunc::create(std::bind(&CasinowarGameScene::playRound, this)), nullptr));
}

void CasinowarGameScene::playRound()
{
    if (player->getPoints() != dealer->getPoints() || warStarted)
    {
        checkWinOrLose(player->getPoints(), dealer->getPoints());
    }
    else
    {
        // TIE case
        float totalTime = 0;
        
        // check side bet
        if (tieBet.token) //payout case
        {
            auto token = tieBet.token;
            
            int payout = tieBet.payout * token->getTotalValue();
            
            totalWin += payout;
            
            increasePlayerValues(payout + token->getTotalValue(), 0, false);
            
            //token animations
            tieBet.token = nullptr;
            
            Vec2 target(background->getContentSize().width / 2, -100);
            float time = 0.5f * token->getPosition().distance(target) / 500;
            
            token->runAction(Sequence::create(MoveTo::create(time, target),
                                              CallFunc::create([] { AudioManager::getInstance().PlaySoundEffect("music/Sounds/effect"); }),
                                              RemoveSelf::create(), NULL));
            
            totalTime += time;
        }
        
        // surrender or war?
        warButton->setVisible(true);
        surrenderButton->setVisible(true);
    }
    
}

void CasinowarGameScene::checkWinOrLose(int playerValue, int bankervalue)
{
    CasinowarResultType result = LOSS;
    
    Sprite * message = nullptr;
    std::string soundEffect;
    
    if (playerValue > bankervalue)
    {
        result = WIN;
        
        message = Sprite::create("ui/Baccarat/player_win.png");
        soundEffect = "music/Sounds/player_win";
    }
    else if (playerValue < bankervalue)
    {
        result = LOSS;
        
        message = Sprite::create("ui/Baccarat/banker_win.png");
        soundEffect = "music/Sounds/banker_win";
    }
    else if (warStarted) // and values are equal
    {
        result = WIN_TIE;
        
        message = Sprite::create("ui/Baccarat/tie.png");
        soundEffect = "music/Sounds/tie";
    }
    
    //update history
    resultsHistory.push_front(result);
    rebuildHistory();
    
    AudioManager::getInstance().PlaySoundEffect(soundEffect);
    
    background->addChild(message, 50);
    message->setOpacity(0);
    message->setScale(0.5f);
    message->setPosition(background->getContentSize() / 2);
    message->runAction(Sequence::create(EaseSineIn::create(Spawn::create(ScaleTo::create(0.25f, 1.0f), FadeIn::create(0.25f), NULL)),
                                        DelayTime::create(1.0f),
                                        EaseSineOut::create(FadeOut::create(0.12f)),
                                        RemoveSelf::create(), nullptr));
    
    runAction(Sequence::createWithTwoActions(DelayTime::create(1.5f), CallFunc::create(std::bind(&CasinowarGameScene::cleanTable, this, result))));
}

void CasinowarGameScene::startWar()
{
    float time = 0;
    
    if (warStarted == false)
    {
        if (normalBet.token == nullptr || PlayerProfile::getInstance().getCoins() < normalBet.token->getTotalValue())
        {
            showNotEnoughView();
            return;
        }
        
        //double bet
        PlayerProfile::getInstance().statsAddBetAmount(GAME_CASINO_WAR, normalBet.token->getTotalValue());
        PlayerProfile::getInstance().addPiggyBankCoins(PIGGY_BANK_PROCENT * normalBet.token->getTotalValue());
        
        //coins and experience
        int coinsChange = -normalBet.token->getTotalValue();
        auto XP = PlayerProfile::getInstance().getExperienceForCoins(coinsChange);
        auto isLevelUp = PlayerProfile::getInstance().isExperienceEnoughForLevelUp(XP);
        
        if (isLevelUp)
        {
            auto currLevel = PlayerProfile::getInstance().getLevel();
            coinsChange += (currLevel + 1) * PlayerProfile::getInstance().getLevelUpBonus();
            
            CongratulationsView::create(CongratulationsView::CongratulationType::LEVELUP, nullptr)->show(this);
        }
        increasePlayerValues(coinsChange, XP, isLevelUp);
        
        auto clone = CoinToken::create(normalBet.token->getTotalValue(), CoinToken::CoinTokenType::NORMAL, normalBet.token->getSkin(), nullptr);
        normalBet.token->getParent()->addChild(clone);
        clone->setLocalZOrder(normalBet.token->getLocalZOrder());
        clone->setPosition(background->getContentSize().width / 2, -100);
        raiseBet.token = clone;
        
        //animate
        Vec2 target(raiseBet.area.origin + raiseBet.area.size / 2);
        time = 0.5f * clone->getPosition().distance(target) / 500;
        clone->runAction(Sequence::create(MoveTo::create(time, target),
                                          CallFunc::create([] { AudioManager::getInstance().PlaySoundEffect("music/Sounds/effect"); }), NULL));
        
        warStarted = true;
    }
    
    warButton->setVisible(false);
    surrenderButton->setVisible(false);
    
    dealer->Hit(deck->DrawCard(), time, nullptr, true);
    dealer->Hit(deck->DrawCard(), time += 0.25f, nullptr, true);
    dealer->Hit(deck->DrawCard(), time += 0.25f, nullptr, true);
    
    auto card = deck->DrawCard();
    dealer->Hit(card, time += 0.25f);

    player->Hit(deck->DrawCard(), time += 0.25f, nullptr, true);
    player->Hit(deck->DrawCard(), time += 0.25f, nullptr, true);
    player->Hit(deck->DrawCard(), time += 0.25f, nullptr, true);
    player->Hit(FORCE_WAR_TIE ? card : deck->DrawCard(), time += 0.25f);
    
    time += 0.5f;
    
    runAction(Sequence::create(DelayTime::create(time), CallFunc::create(std::bind(&CasinowarGameScene::playRound, this)), nullptr));
}

void CasinowarGameScene::cleanTable(CasinowarResultType result)
{
    // take tokens
    std::vector<CasinowarBet*> currentBets = {&normalBet, &tieBet, &raiseBet};
    
    int coinsChange = 0;
    unsigned int XP = 0;
    
    for (auto &bet: currentBets)
    {
        if (bet->token)
        {
            auto token = bet->token;
            
            if (bet->type == BET && result == SURRENDER) //lose half stake
            {
                auto skin  = token->getSkin();
                auto value = token->getTotalValue() / 2;
                
                token->reset();
                token->addToken(value, skin, nullptr);
                
                auto clone = token->getTopTokenClone();
                token->getParent()->addChild(clone, token->getLocalZOrder());
                clone->setPosition(token->getPosition());
                
                coinsChange += token->getTotalValue();
                XP += PlayerProfile::getInstance().getExperienceForCoins(token->getTotalValue());
                
                // return half
                bet->token = nullptr;
                
                Vec2 target(background->getContentSize().width / 2, -100);
                float time = 0.5f * token->getPosition().distance(target) / 500;
                
                token->runAction(Sequence::create(MoveTo::create(time, target),
                                                  CallFunc::create([] { AudioManager::getInstance().PlaySoundEffect("music/Sounds/effect"); }),
                                                  RemoveSelf::create(), NULL));
                
                // lost half
                target = Vec2(background->getContentSize().width / 2, background->getContentSize().height + 100);
                
                time = 0.25f * clone->getPosition().distance(target) / 500;
                clone->runAction(Sequence::create(MoveTo::create(time, target),
                                                  CallFunc::create([] { AudioManager::getInstance().PlaySoundEffect("music/Sounds/effect"); }),
                                                  RemoveSelf::create(), NULL));
            }
            else if ((bet->type == BET   && (result == WIN_TIE || result == WIN)) ||
                     (bet->type == RAISE &&  result == WIN_TIE) ) // win BET stake or RAISE stake
            {
                int payout = bet->payout * token->getTotalValue();
                
                totalWin += payout;
                
                coinsChange += payout + token->getTotalValue(); // origanal bet + payout
                
                //token animations
                bet->token = nullptr;
                
                Vec2 target(background->getContentSize().width / 2, -100);
                float time = 0.5f * token->getPosition().distance(target) / 500;
                
                token->runAction(Sequence::create(MoveTo::create(time, target),
                                                  CallFunc::create([] { AudioManager::getInstance().PlaySoundEffect("music/Sounds/effect"); }),
                                                  RemoveSelf::create(), NULL));
            }
            else // lose all remaining stakes
            {
                Vec2 target = Vec2(background->getContentSize().width / 2, background->getContentSize().height + 100);
                
                XP += PlayerProfile::getInstance().getExperienceForCoins(bet->token->getTotalValue());
                
                bet->token = nullptr;
                
                float time = 0.25f * token->getPosition().distance(target) / 500;
                token->runAction(Sequence::create(MoveTo::create(time, target),
                                                  CallFunc::create([] { AudioManager::getInstance().PlaySoundEffect("music/Sounds/effect"); }),
                                                  RemoveSelf::create(), NULL));
            }
        }
    }
    
    player->removeCards(1.5f);
    dealer->removeCards(1.5f);
    
    //give experience
    if (totalWin > 0)
    {
        //stats
        PlayerProfile::getInstance().statsAddWinAmount(GAME_CASINO_WAR, totalWin);
        
        XP += PlayerProfile::getInstance().getExperienceForCoins(totalWin);
    }
    
    //coins and experience
    auto isLevelUp = PlayerProfile::getInstance().isExperienceEnoughForLevelUp(XP);
    
    if (isLevelUp)
    {
        auto currLevel = PlayerProfile::getInstance().getLevel();
        coinsChange += (currLevel + 1) * PlayerProfile::getInstance().getLevelUpBonus();
        
        CongratulationsView::create(CongratulationsView::CongratulationType::LEVELUP, nullptr)->show(this);
    }
    increasePlayerValues(coinsChange, XP, isLevelUp);
    
    gameStarted = false;
    
    PlayerProfile::getInstance().Save();

    //hackyhack_ads
    CCLOG("hackyhack: CasinowarGameScene.cpp --> round is finished!");
    spinningCount = (spinningCount + 1) % CYCLE_INTERSTITIAL_CASINOWAR;
    if (spinningCount == 0) {
        AdmobChartboost::shared()->showInterstitialAdmob();
    } else if (spinningCount == CYCLE_SPIN_RATEME) {
        long hasShown = PlayerProfile::getInstance().getShowRateWhenSpinning(SPIN_CASINO_WAR);
        if (hasShown == 0) {
            PlayerProfile::getInstance().setShowRateWhenSpinning(SPIN_CASINO_WAR, 1);
            RatemeAlert::create(MCONSTANTS_TEXT_RATEME)->show(this);
        }
    }

    
    runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create(std::bind(&CasinowarGameScene::enableInput, this, true)), NULL));
}

#pragma mark Service

void CasinowarGameScene::showPlaceBet()
{
    auto view = Sprite::create("ui/Roulette/result_back.png");
    view->setOpacity(0);
    view->runAction(Sequence::create(FadeIn::create(0.2f), DelayTime::create(1.3f), FadeOut::create(0.2f), RemoveSelf::create(), NULL));
    view->setPosition(getContentSize() / 2);
    view->setCascadeOpacityEnabled(true);
    
    auto label = Label::createWithTTF("Please make a bet!", "fonts/Berlin.ttf", 36.0f);
    label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    label->setPosition(view->getContentSize() / 2);
    label->setMaxLineWidth(view->getContentSize().width * 0.8f);
    view->addChild(label);
    
    addChild(view, 100);
}

void CasinowarGameScene::showNotEnoughView()
{
    auto view = Sprite::create("ui/Slots/NotEnoughGold.png");
    view->setOpacity(0);
    view->runAction(Sequence::create(FadeIn::create(0.2f), DelayTime::create(1.3f), FadeOut::create(0.2f), RemoveSelf::create(), NULL));
    view->setPosition(getContentSize() / 2);
    
    addChild(view, 100);
}

void CasinowarGameScene::rebuildHistory()
{
    historyRoot->removeAllChildren();
    
    float posY = 0;
    
    for (auto &item: resultsHistory)
    {
        auto label = Label::createWithTTF("0", "fonts/arial.ttf", 30);
        
        label->setDimensions(historyRoot->getContentSize().width, label->getContentSize().height);
        label->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        label->setPosition(Vec2(0, historyRoot->getContentSize().height - posY));
        
        switch (item)
        {
            case WIN:
                label->setColor(Color3B::GREEN);
                label->setHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
                label->setString("W");
                break;
            case LOSS:
                label->setColor(Color3B::RED);
                label->setHorizontalAlignment(cocos2d::TextHAlignment::RIGHT);
                label->setString("L");
                break;
            case SURRENDER:
                label->setColor(Color3B::WHITE);
                label->setHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
                label->setString("S");
                break;
            case WIN_TIE:
                label->setColor(Color3B::GREEN);
                label->setHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
                label->setString("T");
                break;
        }
        
        historyRoot->addChild(label);
        
        posY += label->getContentSize().height;
        
        if (posY + label->getContentSize().height > historyRoot->getContentSize().height)
            break;
    }
}

bool CasinowarGameScene::onTokenAction(CoinToken::CoinTokenEvent event, CoinToken * token)
{
    std::vector<CasinowarBet*> currentBets = {&normalBet, &tieBet, &raiseBet};
    
    if (event == CoinToken::CoinTokenEvent::PLACED)
    {
        Vec2 point = token->convertToWorldSpaceAR(Vec2::ZERO); //background space equals world space
        
        for (auto &item: currentBets)
        {
            if (item->type != CasinowarBetType::RAISE && item->area.containsPoint(point))
            {
                if (token)
                {
                    if (item->token)
                    {
                        // bet exists on that place, stack them
                        item->token->addToken(token->getTotalValue(), token->getSkin(), token->getSourceToken());
                        token->stopAllActions();
                        token->removeFromParent();
                    }
                    else
                    {
                        // create new bet
                        token->retain();
                        token->stopAllActions();
                        token->removeFromParentAndCleanup(false);
                        
                        background->addChild(token, 3); // above cards
                        
                        token->release();
                        token->setPosition(item->area.origin + item->area.size / 2);
                        token->setType(CoinToken::CoinTokenType::STACK);
                        token->setCallback(std::bind(&CasinowarGameScene::onTokenAction, this, std::placeholders::_1, std::placeholders::_2));
                        
                        
                        item->token = token;
                    }
                    
                    clearButton->setEnabled(true);
                    rebetButton->setEnabled(true);
                }
                
                return true;
            }
        }
    }
    else if (event == CoinToken::CoinTokenEvent::CANCELED)
    {
        if (token && !gameStarted)
        {
            CasinowarBet * betItem = nullptr;
            
            for (auto &item: currentBets)
            {
                if (item->token == token)
                {
                    betItem = item;
                    break;
                }
            }
            
            if (token->popToken())
            {
                //token self removed
                if (betItem)
                {
                    betItem->token = nullptr;
                }
            }
            
            return true;
        }
    }
    
    return false;
}

#pragma mark Debug

