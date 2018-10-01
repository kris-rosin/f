#include "BaccaratGameScene.h"

#include "LoadingSplash.h"
#include "PlayerProfile.h"
#include "CoinsPanel.h"
#include "ExperiencePanel.h"

#include "CongratulationsView.h"
#include "AudioManager.h"
#include "SettingsView.h"
#include "AndroidCompatibility.h"

#include "CardObject.h"
#include "Deck.h"

#include "Features/AdmobChartboost.h"
#include "Features/mConstants.h"

#include "RatemeAlert.h"

USING_NS_CC;

BaccaratGameScene * BaccaratGameScene::create(std::string theme)
{
    BaccaratGameScene * pRet = new(std::nothrow) BaccaratGameScene();
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

Scene * BaccaratGameScene::createScene(std::string theme)
{
    auto scene = Scene::create();
    
    auto layer = BaccaratGameScene::create(theme);
    scene->addChild(layer);

    return scene;
}

BaccaratGameScene::BaccaratGameScene() : deck(nullptr), player(nullptr), dealer(nullptr), historyOpened(false), historyRoot(nullptr), gameStarted(false)
{
    
}

bool BaccaratGameScene::init(std::string theme)
{
    if ( !BaseScene::init() )
    {
        return false;
    }
    
    currentTheme = theme;
    currentChapter = BaccaratConfig::themeNameToChapter(currentTheme);
    
    auto winSize = Director::getInstance()->getWinSize();
    //auto visSize = Director::getInstance()->getVisibleSize();
    auto visOrig = Director::getInstance()->getVisibleOrigin();

    background = Sprite::create("ui/Baccarat/background.png");
    background->setPosition(winSize / 2);
    this->addChild(background, 0);
    
    // game elements
    player = BaccaratHand::create();
    dealer = BaccaratHand::create();
    
    background->addChild(player);
    background->addChild(dealer);
    
    player->setPosition(Vec2(winSize.width / 2 - 200, 423));
    dealer->setPosition(Vec2(winSize.width / 2 + 200, 423));
    
    //logo
    auto logo = Sprite::create(StringUtils::format("ui/Cities/%s/logo.png", currentTheme.c_str()));
    logo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    logo->setPosition(Vec2(visOrig.x + 15, background->getContentSize().height - 120));
    background->addChild(logo);
    
    topbar = TopBar::create(std::bind(&BaccaratGameScene::onBackButtonPressed, this));
    topbar->setPosition(Size(background->getContentSize().width / 2, background->getContentSize().height));
    background->addChild(topbar, 100);
    
    //controls
    controlsNode = Node::create();
    controlsNode->setPosition(Vec2(background->getContentSize().width / 2, 40));
    background->addChild(controlsNode);
    
    std::vector<Node*> hudItemsToArrange;
    float total_width = 0;
    
    clearButton = ui::Button::create("ui/Baccarat/clear_button.png", "ui/Baccarat/clear_button_pressed.png", "ui/Baccarat/clear_button.png");
    clearButton->setName("clear");
    clearButton->addTouchEventListener(std::bind(&BaccaratGameScene::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    hudItemsToArrange.push_back(clearButton);
    actionButtons.pushBack(clearButton);
    controlsNode->addChild(clearButton);
    
    auto dealButton = ui::Button::create("ui/Baccarat/deal_button.png", "ui/Baccarat/deal_button_pressed.png", "ui/Baccarat/deal_button.png");
    dealButton->setName("deal");
    dealButton->addTouchEventListener(std::bind(&BaccaratGameScene::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    hudItemsToArrange.push_back(dealButton);
    actionButtons.pushBack(dealButton);
    controlsNode->addChild(dealButton);
    
    rebetButton = ui::Button::create("ui/Baccarat/rebet_button.png", "ui/Baccarat/rebet_button_pressed.png", "ui/Baccarat/rebet_button.png");
    rebetButton->setName("rebet");
    rebetButton->addTouchEventListener(std::bind(&BaccaratGameScene::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    hudItemsToArrange.push_back(rebetButton);
    actionButtons.pushBack(rebetButton);
    controlsNode->addChild(rebetButton);
    
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
    currentBets = {
        { PLAYER, BaccaratBet(PLAYER, Rect(419, 165, 176, 141), 1.00f) },
        { BANK  , BaccaratBet(BANK  , Rect(771, 165, 176, 141), 0.95f) },
        { TIE   , BaccaratBet(TIE   , Rect(595, 165, 176, 141), 8.00f) }
    };
    
    // bet buttons
    betPanel = Node::create();
    background->addChild(betPanel);
    betPanel->setPosition(Director::getInstance()->getVisibleOrigin()); // in left bottom corner
    
    auto betArray = BaccaratConfig::BetValues;
    
    for (int i = 0; i < 4; i++)
    {
        int index = currentChapter - 1 + i;
        
        if (index < BaccaratConfig::BetValues.size())
        {
            auto token = CoinToken::create(betArray[index], CoinToken::CoinTokenType::BUTTON, (CoinToken::CoinTokenSkin)i, nullptr);
            token->setCallback(std::bind(&BaccaratGameScene::onTokenAction, this, std::placeholders::_1, std::placeholders::_2));
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
    openHistory->addTouchEventListener(std::bind(&BaccaratGameScene::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    historyTab->addChild(openHistory);
    
    historyRoot = Node::create();
    historyRoot->setContentSize(Size(65, 490));
    historyRoot->setPosition(Vec2(47, 17));
    historyRoot->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    historyTab->addChild(historyRoot);

    return true;
}

void BaccaratGameScene::onEnter()
{
    Layer::onEnter();

    PlayBackgroundMusic();
}

void BaccaratGameScene::PlayBackgroundMusic()
{
    AudioManager::getInstance().PlayBackgroundMusic("music/BackgroundMusic/Ingame");
}

void BaccaratGameScene::enableInput(bool enable)
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
        button->setOpacity(enable ? 255 : 127);
    }
    
    for (auto &button: currentBets)
    {
        if (button.second.token)
            button.second.token->setEnabled(false);
    }
}

void BaccaratGameScene::onButtonPressed(Ref * sender, ui::Widget::TouchEventType event)
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

void BaccaratGameScene::onBackButtonPressed()
{
    auto nextScene = LoadingSplash::createScene(LoadingSplash::BACCARAT_THEME);
    Director::getInstance()->replaceScene(nextScene);
}

void BaccaratGameScene::ProcessButtonAction(cocos2d::ui::Button * button)
{
    AudioManager::getInstance().PlaySoundEffect("music/SlotsSound/TapBt");
    
    if (button->getName() == "history")
    {
        if (historyOpened)
        {
            //close
            historyTab->stopAllActions();
            
            float posX = Director::getInstance()->getVisibleOrigin().x + Director::getInstance()->getVisibleSize().width;
            historyTab->runAction(Sequence::create(EaseSineInOut::create( MoveTo::create(0.25f, Vec2(posX, historyTab->getPositionY() ))), CallFunc::create([button]
                                                                                                                                                            {
                                                                                                                                                                button->setEnabled(true);
                                                                                                                                                            }), NULL));
        }
        else
        {
            //open
            historyTab->stopAllActions();
            
            float posX = Director::getInstance()->getVisibleOrigin().x + Director::getInstance()->getVisibleSize().width - historyTab->getContentSize().width + historyTab->getAnchorPointInPoints().x;
            historyTab->runAction(Sequence::create(EaseSineInOut::create( MoveTo::create(0.25f, Vec2(posX, historyTab->getPositionY() ))), CallFunc::create([button]
                                                                                                                                                            {
                                                                                                                                                                button->setEnabled(true);
                                                                                                                                                            }), NULL));
        }
        
        historyOpened = !historyOpened;
        button->setEnabled(false);
    }
    else if (button->getName() == "deal")
    {
        //check bets
        int totalBet = 0;
        
        for (auto &bet: currentBets)
        {
            if (bet.second.token)
            {
                totalBet += bet.second.token->getTotalValue();
                
                previousBets[bet.first] = bet.second.token->getTotalValue();
            }
            else
            {
                previousBets[bet.first] = 0;
            }
        }
        
        if (totalBet == 0)
        {
            showPlaceBet();
            return;
        }
        
        if (totalBet > PlayerProfile::getInstance().getCoins())
        {
            showNotEnoughView();
            return;
        }
        
        PlayerProfile::getInstance().statsAddBetAmount(GAME_BACCARAT, totalBet);
        PlayerProfile::getInstance().addPiggyBankCoins(PIGGY_BANK_PROCENT * totalBet);
        
        unsigned int XP = PlayerProfile::getInstance().getExperienceForCoins(totalBet);
        
        auto isLevelUP = PlayerProfile::getInstance().isExperienceEnoughForLevelUp(XP);
        if (isLevelUP)
        {
            auto currLevel = PlayerProfile::getInstance().getLevel();
            totalBet -= (currLevel + 1) * PlayerProfile::getInstance().getLevelUpBonus();
            
            CongratulationsView::create(CongratulationsView::CongratulationType::LEVELUP, nullptr)->show(this);
        }
        
        increasePlayerValues(-totalBet, XP, isLevelUP);
        
        //hide controls
        enableInput(false);
        
        newRound();
    }
    else if (button->getName() == "clear")
    {
        button->setEnabled(false);
        rebetButton->setEnabled(true);
        
        for (auto &bet: currentBets)
        {
            if (bet.second.token)
            {
                auto token = bet.second.token;
                
                Vec2 target(background->getContentSize().width / 2, -100);
                float time = 0.5f * token->getPosition().distance(target) / 500;
                
                token->runAction(Sequence::create(MoveTo::create(time, target),
                                                  CallFunc::create([] { AudioManager::getInstance().PlaySoundEffect("music/Sounds/effect"); }),
                                                  RemoveSelf::create(), NULL));
                
                bet.second.token = nullptr;
            }
        }
    }
    else if (button->getName() == "rebet")
    {
        button->setEnabled(false);
        clearButton->setEnabled(true);
        
        for (auto &bet: currentBets)
        {
            if (bet.second.token) // first remove existing
            {
                auto token = bet.second.token;
                
                Vec2 target(background->getContentSize().width / 2, -100);
                float time = 0.5f * token->getPosition().distance(target) / 500;
                
                token->runAction(Sequence::create(MoveTo::create(time, target),
                                                  CallFunc::create([] { AudioManager::getInstance().PlaySoundEffect("music/Sounds/effect"); }),
                                                  RemoveSelf::create(), NULL));
                
                bet.second.token = nullptr;
            }
            
            //place new if needed
            auto itr = previousBets.find(bet.first);
            if (itr != previousBets.end() && itr->second > 0)
            {
                // create new bet
                auto token = CoinToken::create(itr->second, CoinToken::CoinTokenType::STACK, CoinToken::CoinTokenSkin::BLACK, betButtons.front());
                background->addChild(token);
                
                Vec2 target(bet.second.area.origin + bet.second.area.size / 2);
                
                token->setPosition(Vec2(background->getContentSize().width / 2, -100));
                token->setCallback(std::bind(&BaccaratGameScene::onTokenAction, this, std::placeholders::_1, std::placeholders::_2));
                
                float time = 0.5f * token->getPosition().distance(target) / 500;
                
                token->runAction(Sequence::create(MoveTo::create(time, target),
                                                  CallFunc::create([] { AudioManager::getInstance().PlaySoundEffect("music/Sounds/effect"); }), NULL));
                
                bet.second.token = token;
            }
        }
    }
}

#pragma mark Core Methods

void BaccaratGameScene::newRound()
{
    
    if (deck == nullptr || !deck->CanPlay())
    {
        delete deck;
        
        deck = new BaccaratDeck(8);
    }
    
    float totalDelay = 0;
    float firstPlayerCardDelay = 0;
    float firstDealerCardDelay = 0;
    
    gameStarted = true;
    
    player->Hit(deck->DrawCard(), totalDelay);
    firstPlayerCardDelay = totalDelay += 0.5f;
    
    dealer->Hit(deck->DrawCard(), totalDelay);
    firstDealerCardDelay = totalDelay += 0.5f;
    
    player->Hit(deck->DrawCard(), totalDelay);
    totalDelay += 0.5f;
    
    dealer->Hit(deck->DrawCard(), totalDelay);
    totalDelay += 0.5f;
    
    runAction(Sequence::create(DelayTime::create(firstPlayerCardDelay), CallFunc::create([this]{ player->showPoints(true); }),
                               DelayTime::create(firstDealerCardDelay - firstPlayerCardDelay), CallFunc::create([this]{ dealer->showPoints(true); }),
                               DelayTime::create(totalDelay - firstDealerCardDelay), CallFunc::create(std::bind(&BaccaratGameScene::playRound, this)), nullptr));
}

void BaccaratGameScene::playRound()
{
    int playerValue = player->getPoints();
    int bankerValue = dealer->getPoints();
    
    if (playerValue >= 8 || bankerValue >= 8)
    {
        // HERE, NO 3rd Card. It will be decided immediately!
        checkWinOrLose(playerValue, bankerValue);
    }
    else
    {
        // POSSIBILITY 3rd Card, Player stands on value 6 or 7, so check if Player value is 0 - 5
        int playerCard3 = 0;
        
        if (playerValue <= 5)
        {
            auto card   = deck->DrawCard();
            playerCard3 = card.BaccaratValue();
            
            player->Hit(card, 0, [this, bankerValue, playerCard3]
            {
                // on Hit completion
                bool dealerShouldDrawCard = false;
                
                if (bankerValue <= 2)
                {
                    dealerShouldDrawCard = true;
                }
                else if ( bankerValue == 3 && playerCard3 != 8)
                {
                    dealerShouldDrawCard = true;
                }
                else if ( bankerValue == 4 && playerCard3 >= 2 && playerCard3 <= 7)
                {
                    dealerShouldDrawCard = true;
                }
                else if ( bankerValue == 5 && playerCard3 >= 4 && playerCard3 <= 7)
                {
                    dealerShouldDrawCard = true;
                }
                else if ( bankerValue == 6 && playerCard3 >= 6 && playerCard3 <= 7)
                {
                    dealerShouldDrawCard = true;
                }
                
                if (dealerShouldDrawCard)
                {
                    dealer->Hit(deck->DrawCard(), 0, [this]
                    {
                        // on Hit completion
                        checkWinOrLose(player->getPoints(), dealer->getPoints());
                    });
                }
                else
                {
                    checkWinOrLose(player->getPoints(), dealer->getPoints());
                }
            });
        }
        else
        {
            if (bankerValue <= 5)
            {
                dealer->Hit(deck->DrawCard(), 0, [this]
                {
                    // on Hit completion
                    checkWinOrLose(player->getPoints(), dealer->getPoints());
                });
            }
            else
            {
                checkWinOrLose(player->getPoints(), dealer->getPoints());
            }
            
            return;
        }
    }
}

void BaccaratGameScene::checkWinOrLose(int playerValue, int bankerValue)
{
    BaccaratBetType result;
    
    Sprite * message = nullptr;
    std::string soundEffect;
    
    if (playerValue > bankerValue)
    {
        result = PLAYER;
        message = Sprite::create("ui/Baccarat/player_win.png");
        soundEffect = "music/Sounds/player_win";
    }
    else if (bankerValue > playerValue)
    {
        result = BANK;
        message = Sprite::create("ui/Baccarat/banker_win.png");
        soundEffect = "music/Sounds/banker_win";
    }
    else
    {
        result = TIE;
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
    
    runAction(Sequence::createWithTwoActions(DelayTime::create(1.5f), CallFunc::create(std::bind(&BaccaratGameScene::cleanTable, this, result))));
}

void BaccaratGameScene::cleanTable(BaccaratBetType result)
{
    int totalWin = 0;
    int coinsChange = 0;
    int totalLoose = 0;
    
    // take tokens
    for (auto &bet: currentBets)
    {
        if (bet.second.token)
        {
            auto token = bet.second.token;
            
            if (bet.second.type == result) //payout case
            {
                int payout = bet.second.payout * token->getTotalValue();
                
                totalWin += payout;
                coinsChange += payout + token->getTotalValue();
                
                //token animations
                bet.second.token = nullptr;
                
                Vec2 target(background->getContentSize().width / 2, -100);
                float time = 0.5f * token->getPosition().distance(target) / 500;
                
                token->runAction(Sequence::create(MoveTo::create(time, target),
                                                  CallFunc::create([] { AudioManager::getInstance().PlaySoundEffect("music/Sounds/effect"); }),
                                                  RemoveSelf::create(), NULL));
            }
            else
            {
                Vec2 target;
                
                if (result == TIE) //return case
                {
                    coinsChange += token->getTotalValue();
                    target = Vec2(background->getContentSize().width / 2, -100);
                }
                else //fail case
                {
                    totalLoose += token->getTotalValue();
                    target = Vec2(background->getContentSize().width / 2, background->getContentSize().height + 100);
                }
                
                bet.second.token = nullptr;
                
                float time = 0.25f * token->getPosition().distance(target) / 500;
                token->runAction(Sequence::create(MoveTo::create(time, target),
                                                  CallFunc::create([] { AudioManager::getInstance().PlaySoundEffect("music/Sounds/effect"); }),
                                                  RemoveSelf::create(), NULL));
            }
        }
    }
    
    player->removeCards(1.5f);
    dealer->removeCards(1.5f);
    
    unsigned int XP = 0;
    
    //give experience
    if (totalWin > 0)
    {
        //stats
        PlayerProfile::getInstance().statsAddWinAmount(GAME_BACCARAT, totalWin);
        XP += PlayerProfile::getInstance().getExperienceForCoins(totalWin);
    }
    if (totalLoose > 0)
    {
        XP += PlayerProfile::getInstance().getExperienceForCoins(totalLoose);
    }
    
    auto isLevelUP = PlayerProfile::getInstance().isExperienceEnoughForLevelUp(XP);
    if (isLevelUP)
    {
        auto currLevel = PlayerProfile::getInstance().getLevel();
        coinsChange += (currLevel + 1) * PlayerProfile::getInstance().getLevelUpBonus();
        
        CongratulationsView::create(CongratulationsView::CongratulationType::LEVELUP, nullptr)->show(this);
    }
    
    //commit results
    if (coinsChange != 0 || XP != 0 || isLevelUP)
    {
        increasePlayerValues(coinsChange, XP, isLevelUP);
    }
    
    gameStarted = false;
    
    PlayerProfile::getInstance().Save();
    
    //hackyhack_ads
    CCLOG("hackyhack: BaccaratGameScene.cpp --> round is finished!");
    spinningCount = (spinningCount + 1) % CYCLE_INTERSTITIAL_BACCARAT;
    if (spinningCount == 0) {
        AdmobChartboost::shared()->showInterstitialAdmob();
    } else if (spinningCount == CYCLE_SPIN_RATEME) {
        long hasShown = PlayerProfile::getInstance().getShowRateWhenSpinning(SPIN_BACCARAT);
        if (hasShown == 0) {
            PlayerProfile::getInstance().setShowRateWhenSpinning(SPIN_BACCARAT, 1);
            RatemeAlert::create(MCONSTANTS_TEXT_RATEME)->show(this);
        }
    }
    
    runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create(std::bind(&BaccaratGameScene::enableInput, this, true)), NULL));
}

#pragma Service

void BaccaratGameScene::showPlaceBet()
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

void BaccaratGameScene::showNotEnoughView()
{
    auto view = Sprite::create("ui/Slots/NotEnoughGold.png");
    view->setOpacity(0);
    view->runAction(Sequence::create(FadeIn::create(0.2f), DelayTime::create(1.3f), FadeOut::create(0.2f), RemoveSelf::create(), NULL));
    view->setPosition(getContentSize() / 2);
    
    addChild(view, 100);
}

void BaccaratGameScene::rebuildHistory()
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
            case TIE:
                label->setColor(Color3B::GREEN);
                label->setHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
                label->setString("T");
                break;
            case BANK:
                label->setColor(Color3B::RED);
                label->setHorizontalAlignment(cocos2d::TextHAlignment::RIGHT);
                label->setString("B");
                break;
            case PLAYER:
                label->setColor(Color3B::WHITE);
                label->setHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
                label->setString("P");
                break;
        }
        
        historyRoot->addChild(label);
        
        posY += label->getContentSize().height;
        
        if (posY + label->getContentSize().height > historyRoot->getContentSize().height)
            break;
    }
}

bool BaccaratGameScene::onTokenAction(CoinToken::CoinTokenEvent event, CoinToken * token)
{
    if (event == CoinToken::CoinTokenEvent::PLACED)
    {
        Vec2 point = token->convertToWorldSpaceAR(Vec2::ZERO); //background space equals world space
        
        for (auto &item: currentBets)
        {
            if (item.second.area.containsPoint(point))
            {
                if (token)
                {
                    if (item.second.token)
                    {
                        // bet exists on that place, stack them
                        item.second.token->addToken(token->getTotalValue(), token->getSkin(), token->getSourceToken());
                        token->stopAllActions();
                        token->removeFromParent();
                    }
                    else
                    {
                        // create new bet
                        token->retain();
                        token->stopAllActions();
                        token->removeFromParentAndCleanup(false);
                        
                        background->addChild(token);
                        
                        token->release();
                        token->setPosition(item.second.area.origin + item.second.area.size / 2);
                        token->setType(CoinToken::CoinTokenType::STACK);
                        token->setCallback(std::bind(&BaccaratGameScene::onTokenAction, this, std::placeholders::_1, std::placeholders::_2));
                        
                        
                        item.second.token = token;
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
            BaccaratBet * betItem = nullptr;
            
            for (auto &item: currentBets)
            {
                if (item.second.token == token)
                {
                    betItem = &item.second;
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

