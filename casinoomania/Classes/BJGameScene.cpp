#include "BJGameScene.h"

#include "LoadingSplash.h"
#include "PlayerProfile.h"

#include "CongratulationsView.h"
#include "AudioManager.h"
#include "AndroidCompatibility.h"

#include "CardObject.h"
#include "Deck.h"
#include "BJDealer.h"

#include "Features/AdmobChartboost.h"
#include "Features/mConstants.h"

#include "RatemeAlert.h"

USING_NS_CC;

BJGameScene * BJGameScene::create(std::string theme)
{
    BJGameScene * pRet = new(std::nothrow) BJGameScene();
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

Scene * BJGameScene::createScene(std::string theme)
{
    auto scene = Scene::create();
    
    auto layer = BJGameScene::create(theme);
    scene->addChild(layer);

    return scene;
}

BJGameScene::BJGameScene() : currentHand(-1), shoe(nullptr), currentPlayer(nullptr), currentState(NOT_STARTED), previousBet(0)
{
    
}

bool BJGameScene::init(std::string theme)
{
    if ( !BaseScene::init() )
    {
        return false;
    }
    
    currentTheme = theme;
    currentChapter = BJConfig::themeNameToChapter(currentTheme);
    
    auto winSize = Director::getInstance()->getWinSize();
    //auto visSize = Director::getInstance()->getVisibleSize();
    auto visOrig = Director::getInstance()->getVisibleOrigin();

    auto background = Sprite::create("ui/BlackJack/background.png");
    background->setPosition(winSize / 2);
    this->addChild(background, 0);
    
    // dealer strategy
    bool hitsoft17 = PlayerProfile::getInstance().getUIntValue("BlackJack_Soft17", 1);
    auto strategy  = Sprite::create(hitsoft17 ? "ui/BlackJack/soft17.png" : "ui/BlackJack/hard17.png");
    strategy->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    strategy->setPosition(Vec2(background->getContentSize().width / 2, 435));
    background->addChild(strategy);
    
    handSelector = Sprite::create("ui/BlackJack/glow_selection.png");
    handSelector->setBlendFunc(BlendFunc::ADDITIVE);
    handSelector->setVisible(false);
    handSelector->setColor(Color3B(255, 255, 200));
    handSelector->setScale(4);
    background->addChild(handSelector);
    
    //logo
    auto logo = Sprite::create(StringUtils::format("ui/Cities/%s/logo.png", currentTheme.c_str()));
    logo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    logo->setPosition(Vec2(visOrig.x + 15, background->getContentSize().height - 120));
    background->addChild(logo);
    
    topbar = TopBar::create(std::bind(&BJGameScene::onBackButtonPressed, this));
    topbar->setPosition(Size(background->getContentSize().width / 2, background->getContentSize().height));
    addChild(topbar, 100);
    
    controlsNode = Node::create();
    controlsNode->setPosition(Vec2(background->getContentSize().width / 2, 50));
    background->addChild(controlsNode);
    
    // bet buttons
    betPanel = Node::create();
    background->addChild(betPanel);
    betPanel->setPosition(Director::getInstance()->getVisibleOrigin()); // in left bottom corner
    
    auto betArray = BJConfig::BetValues;
    
    for (int i = 0; i < 4; i++)
    {
        int index = currentChapter - 1 + i;
        
        if (index < BJConfig::BetValues.size())
        {
            auto token = CoinToken::create(betArray[index], CoinToken::CoinTokenType::PUSH_BUTTON, (CoinToken::CoinTokenSkin)i, nullptr);
            token->setCallback(std::bind(&BJGameScene::onBetButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
            token->setPosition(Vec2(token->getContentSize().width * 0.6f, token->getContentSize().height * (0.6f + i)));
            betPanel->addChild(token);
            
            betButtons.pushBack(token);
        }
    }
    
    
    //auto start
    startNewGame();
    
    return true;
}

void BJGameScene::onEnter()
{
    Layer::onEnter();

    PlayBackgroundMusic();
}

void BJGameScene::PlayBackgroundMusic()
{
    AudioManager::getInstance().PlayBackgroundMusic("music/BackgroundMusic/Ingame");
}

void BJGameScene::enableInput(bool enable)
{
    topbar->setEnabled(enable);
    
    betPanel->setVisible(enable);
    
    for (auto &button: betButtons)
    {
        button->setEnabled(enable);
    }
}

void BJGameScene::onButtonPressed(Ref * sender, ui::Widget::TouchEventType event)
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

void BJGameScene::onBackButtonPressed()
{
    auto nextScene = LoadingSplash::createScene(LoadingSplash::BLACKJACK_THEME);
    Director::getInstance()->replaceScene(nextScene);
}

void BJGameScene::ProcessButtonAction(cocos2d::ui::Button * button)
{
    if (button->getName() == "deal")
    {
        for (auto &player: players)
        {
            // check bets
            for (auto &hand: player->getHands())
            {
                if (hand->isDealer() == false)
                {
                    if (hand->getBet() == 0)
                    {
                        showPlaceBet();
                        return;
                    }
                    
                    previousBet = hand->getBet();
                }
            }
            
            if (!player->isDealer())
            {
                if (player->TotalBet() > PlayerProfile::getInstance().getCoins() )
                {
                    showNotEnoughView();
                    return;
                }
                else
                {
                    PlayerProfile::getInstance().statsAddBetAmount(GAME_BLACK_JACK, player->TotalBet());
                    PlayerProfile::getInstance().addPiggyBankCoins(PIGGY_BANK_PROCENT * player->TotalBet());
                    
                    int coinsChange = -player->TotalBet();
                    auto XP = PlayerProfile::getInstance().getExperienceForCoins(coinsChange);
                    auto isLevelUP = PlayerProfile::getInstance().isExperienceEnoughForLevelUp(XP);
                    
                    if (isLevelUP)
                    {
                        auto currLevel = PlayerProfile::getInstance().getLevel();
                        coinsChange += (currLevel + 1) * PlayerProfile::getInstance().getLevelUpBonus();
                        
                        CongratulationsView::create(CongratulationsView::CongratulationType::LEVELUP, nullptr)->show(this);
                    }
                    
                    increasePlayerValues(coinsChange, XP, isLevelUP);
                }
            }
        }
        
        // hide controls
        controlsNode->removeAllChildren();
        
        enableInput(false);
        
        dealStartCards();
    }
    else if (button->getName() == "rebet")
    {
        for (auto &player: players)
        {
            // check bets
            for (auto &hand: player->getHands())
            {
                if (hand->isDealer() == false)
                {
                    hand->grabAllTokens(0, false);
                    
                    hand->addBet(CoinToken::create(previousBet, CoinToken::CoinTokenType::NORMAL, CoinToken::CoinTokenSkin::GREY, nullptr));
                }
            }
        }
    }
    else if (button->getName() == "hit")
    {
        controlsNode->removeAllChildren();
        
        playingHands.at(currentHand)->Hit(shoe->DrawCard(), 0, [this]
        {
            checkAndPay(playingHands.at(currentHand));
        });
    }
    else if (button->getName() == "stand")
    {
        controlsNode->removeAllChildren();
        
        nextHand();
    }
    else if (button->getName() == "insurance")
    {
        if (PlayerProfile::getInstance().getCoins() >= (0.5f + playingHands.at(currentHand)->getBet() / 2) )
        {
            playingHands.at(currentHand)->addInsurance();
            int coinsChange = - playingHands.at(currentHand)->getInsurance();
            PlayerProfile::getInstance().statsAddBetAmount(GAME_BLACK_JACK, playingHands.at(currentHand)->getInsurance());
            PlayerProfile::getInstance().addPiggyBankCoins(PIGGY_BANK_PROCENT * playingHands.at(currentHand)->getInsurance());
            
            auto XP = PlayerProfile::getInstance().getExperienceForCoins(playingHands.at(currentHand)->getInsurance());
            auto isLevelUP = PlayerProfile::getInstance().isExperienceEnoughForLevelUp(XP);
            
            if (isLevelUP)
            {
                auto currLevel = PlayerProfile::getInstance().getLevel();
                coinsChange += (currLevel + 1) * PlayerProfile::getInstance().getLevelUpBonus();
                
                CongratulationsView::create(CongratulationsView::CongratulationType::LEVELUP, nullptr)->show(this);
            }
            
            increasePlayerValues(coinsChange, XP, isLevelUP);
        
            controlsNode->removeAllChildren();
            
            //wait animations complete
            controlsNode->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f), CallFunc::create(std::bind(&BJGameScene::nextHand, this))));
        }
        else
        {
            showNotEnoughView();
        }
    }
    else if (button->getName() == "pass")
    {
        controlsNode->removeAllChildren();
        
        nextHand();
    }
    else if (button->getName() == "surrender")
    {
        controlsNode->removeAllChildren();
        
        surrenderHand(playingHands.at(currentHand));
    }
    else if (button->getName() == "equal_money")
    {
        controlsNode->removeAllChildren();
        
        equalmoneyHand(playingHands.at(currentHand));
    }
    else if (button->getName() == "double")
    {
        controlsNode->removeAllChildren();
        
        doubleHand(playingHands.at(currentHand));
    }
    else if (button->getName() == "split")
    {
        if (currentPlayer->CanPlaceBet( playingHands.at(currentHand)->getBet()))
        {
            if (currentPlayer->CanSplitHand(playingHands.at(currentHand)))
            {
                controlsNode->removeAllChildren();
                
                auto newHand = currentPlayer->SplitHand(playingHands.at(currentHand), *shoe);
                
                if (newHand)
                {
                    playingHands.insert(currentHand + 1, newHand);
                    
                    if (newHand->isSplitted(true))
                    {
                        currentHand++; //pass first hand
                        
                        //pass second hand
                        controlsNode->runAction(Sequence::createWithTwoActions(DelayTime::create(3.0f), CallFunc::create(std::bind(&BJGameScene::nextHand, this))));
                    }
                    else
                    {
                        //wait animations
                        controlsNode->runAction(Sequence::create(DelayTime::create(3.0f), CallFunc::create(std::bind(&BJGameScene::selectCurrentHand, this)),
                                                                 CallFunc::create(std::bind(&BJGameScene::ShowPossibleActions, this)), nullptr));
                    }
                }
                else
                {
                    ShowPossibleActions();
                }
            }
        }
        else
        {
            showNotEnoughView();
        }
    }
}

void BJGameScene::selectCurrentHand()
{
    if (currentHand < playingHands.size())
    {
        auto hand = playingHands.at(currentHand);
        
        auto pos = hand->convertToWorldSpace(Vec2::ZERO);
        handSelector->setPosition(pos);
        handSelector->setVisible(true);
    }
    else
    {
        handSelector->setVisible(false);
    }
}

void BJGameScene::ShowPossibleActions()
{
    controlsNode->removeAllChildren();
    
    // sprite name, action name
    std::map<BlackJackActions, std::tuple< std::string, std::string > > buttonsInfo = {
        { DEAL     , { "deal_button"     , "deal"      } },
        { REBET    , { "rebet_button"    , "rebet"     } },
        { HIT      , { "hit_button"      , "hit"       } },
        { STAND    , { "stand_button"    , "stand"     } },
        { SURRENDER, { "surrender_button", "surrender" } },
        { INSURANCE, { "yes_button"      , "insurance" } },
        { SPLIT    , { "split_button"    , "split"     } },
        { DOUBLE   , { "double_button"   , "double"    } },
        { INSURANCE_DENY, { "no_button"  , "pass"      } },
        { EQUAL_MONEY, { "equal_money_button", "equal_money"} }
    };
    
    float total_width = 0;
    float padding = 10;
    bool  show_insurance = false;
    
    for (auto action: getPossibleActions())
    {
        std::string spriteName = StringUtils::format("ui/BlackJack/%s.png", std::get<0>(buttonsInfo[action]).c_str());
                                                     
        auto button = ui::Button::create(spriteName, spriteName, spriteName);
        button->setName(std::get<1>(buttonsInfo[action]));
        button->addTouchEventListener(std::bind(&BJGameScene::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
        button->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        
        if (action == INSURANCE)
            show_insurance = true;
        
        controlsNode->addChild(button);
        
        total_width += button->getContentSize().width;
    }
    
    total_width += (controlsNode->getChildren().size() - 1) * padding;
    
    float xpos = -total_width / 2;
    
    for (auto &item: controlsNode->getChildren())
    {
        item->setPosition(Vec2( xpos, 0));
        
        xpos += item->getContentSize().width + padding;
    }
    
    if (show_insurance)
    {
        auto panel = Sprite::create("ui/BlackJack/insurance_panel.png");
        panel->setAnchorPoint(Vec2(0.5f, 0.33f));
        controlsNode->addChild(panel, -1);
    }
}

#pragma mark Core Methods

void BJGameScene::startNewGame()
{
    if (players.empty())
    {
        auto dealer = BJDealer::create();
        auto player = BJPlayer::create("You");
        
        addChild(player, 1);
        addChild(dealer);
        
        players.pushBack(player);
        players.pushBack(dealer); //dealer should be last player!
        
        auto winSize = Director::getInstance()->getWinSize();
        
        dealer->setPosition(Vec2(winSize.width / 2, winSize.height * 0.73f));
        player->setPosition(Vec2(winSize.width / 2, winSize.height * 0.45f));
        
        currentPlayer = player;
    }
    
    newRound();
    
}

void BJGameScene::newRound()
{
    for (auto &player: players)
    {
        player->FinishRound();
        player->StartHand();
    }
    
    if (shoe && !shoe->CanPlay())
    {
        delete shoe;
    }
    
    shoe = new Shoe(4);
    shoe->Shuffle();
    
    currentHand = -1;
    
    currentState = NOT_STARTED;
    
    enableInput(true);
    
    ShowPossibleActions();
}

void BJGameScene::dealStartCards()
{
    const float delayPerHand = 0.5f;
    float totalDelay = 0;
    
    for (int i = 0; i < 2; i ++)
    {
        for (auto &player: players)
        {
            for (auto &hand: player->getHands())
            {
                hand->runAction(Sequence::createWithTwoActions(DelayTime::create(totalDelay), CallFunc::create([hand, this, i]()
                {
                    if ((FORCE_INSURANCE_CASE || FORCE_DEALER_BJ) && hand->isDealer() && i == 0)
                    {
                        hand->Hit(CardObject(CardObject::CardSuite::CLUBS, CardObject::CardValue::ACE), 0);
                    }
                    else if (FORCE_DEALER_BJ && hand->isDealer() && i == 1)
                    {
                        hand->Hit(CardObject(CardObject::CardSuite::CLUBS, CardObject::CardValue::TEN), 0);
                    }
                    else if (FORCE_PLAYER_SPLIT && !hand->isDealer())
                    {
                        if (FORCE_ACE_SPLIT)
                        {
                            hand->Hit(CardObject(CardObject::CardSuite::CLUBS, CardObject::CardValue::ACE), 0);
                        }
                        else
                        {
                            hand->Hit(CardObject(CardObject::CardSuite::CLUBS, CardObject::CardValue::SEVEN), 0);
                        }
                    }
                    else if (FORCE_PLAYER_BJ && !hand->isDealer())
                    {
                        if (i == 0)
                        {
                            hand->Hit(CardObject(CardObject::CardSuite::CLUBS, CardObject::CardValue::KING), 0);
                        }
                        else
                        {
                            hand->Hit(CardObject(CardObject::CardSuite::CLUBS, CardObject::CardValue::ACE), 0);
                        }
                    }
                    else
                    {
                        hand->Hit(shoe->DrawCard(), 0);
                    }
                })));
                
                totalDelay += delayPerHand;
            }
        }
    }
    
    scheduleOnce(std::bind(&BJGameScene::onStateChanged, this, std::placeholders::_1, BlackJackGameStates::DEAL_CARDS), totalDelay, "StateChanged");
}

void BJGameScene::onStateChanged(float deltaTime, BlackJackGameStates state)
{
    currentState = state;
    
    switch (state)
    {
        case BlackJackGameStates::DEAL_CARDS:
        {
            playingHands.clear();
            
            for (auto &player: players)
            {
                for (auto &hand: player->getHands())
                {
                    hand->showPoints(true);
                    playingHands.pushBack(hand);
                }
            }
            
            if (getDealer()->isInsuranceCase() )
            {
                onStateChanged(0, BlackJackGameStates::ASK_INSURANCE);
            }
            else
            {
                onStateChanged(0, BlackJackGameStates::DEALER_CHECK_BLACKJACK);
            }
        }
            break;
            
        case BlackJackGameStates::ASK_INSURANCE:
        {
            currentHand = -1;
            nextHand();
        }
            break;
            
        case BlackJackGameStates::DEALER_CHECK_BLACKJACK:
        {
            if (getDealer()->getHands().at(0)->GetStatus() == Hand::BLACKJACK)
            {
                getDealer()->getHands().at(0)->openCard(1);
                
                //wait for animations completion
                runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create(std::bind(&BJGameScene::onRoundFinished, this)), NULL));
            }
            else
            {
                onStateChanged(0, ROUND);
            }
        }
            break;
            
        case BlackJackGameStates::ROUND:
        {
            currentHand = -1;
            nextHand();
        }
            break;
            
        default:
            break;
    }
}

void BJGameScene::onRoundFinished()
{
    float totalDelay = 0;
    
    auto dealerStatus = getDealer()->getHands().at(0)->GetStatus();
    
    // pay winners
    for (auto &player: players)
    {
        for (auto &hand: player->getHands())
        {
            if (hand->isDealer() == false)
            {
                if (dealerStatus == Hand::BLACKJACK) //pay insurance
                {
                    int insurance = hand->getInsurance();
                    
                    if (insurance > 0)
                    {
                        hand->addPayout(CoinToken::create(insurance, CoinToken::CoinTokenType::NORMAL, CoinToken::CoinTokenSkin::GREEN, nullptr), totalDelay);
                        totalDelay += 0.25f;
                    }
                }
                else
                {
                    hand->takeInsurance(totalDelay);
                    
                    if (hand->compareTo(getDealer()->getHands().at(0)) == HandView::WON)
                    {
                        if (hand->GetStatus() == Hand::BLACKJACK)
                        {
                            // 3 to 2 payout for BJ
                            hand->addPayout(CoinToken::create(hand->getBet() * 3 / 2, CoinToken::CoinTokenType::NORMAL, CoinToken::CoinTokenSkin::GREEN, nullptr), totalDelay);
                            totalDelay += 0.25f;
                        }
                        else
                        {
                            // 1 to 1 payout
                            hand->addPayout(CoinToken::create(hand->getBet(), CoinToken::CoinTokenType::NORMAL, CoinToken::CoinTokenSkin::GREEN, nullptr), totalDelay);
                            totalDelay += 0.25f;
                        }
                    }
                }
            }
        }
    }
    
    //take bets from loosers
    for (auto &player: players)
    {
        for (auto &hand: player->getHands())
        {
            if (hand->isDealer() == false)
            {
                if (hand->compareTo(getDealer()->getHands().at(0)) == HandView::FAILED) // take bets
                {
                    currentPlayer->totalLoose += hand->getBet();
                    
                    hand->takeBet(totalDelay);
                    totalDelay += 0.25f;
                }
                
                if (dealerStatus != Hand::BLACKJACK) // take insurance
                {
                    if (hand->getInsurance() > 0)
                    {
                        currentPlayer->totalLoose += hand->getInsurance();
                        
                        hand->takeInsurance(totalDelay);
                    }
                }
            }
        }
    }
    
    totalDelay += 2.0f;
    
    runAction(Sequence::create(DelayTime::create(totalDelay), CallFunc::create(std::bind(&BJGameScene::cleanTable, this)),
                               DelayTime::create(0.5f), CallFunc::create(std::bind(&BJGameScene::newRound, this)), NULL));
}

void BJGameScene::cleanTable()
{
    int coinsChange = 0;
    unsigned int XP = 0;
    
    //take tokens back to players
    for (auto &player: players)
    {
        for (auto &hand: player->getHands())
        {
            if (hand->isDealer() == false)
            {
                int money = hand->grabAllTokens(0);
                
                coinsChange += money;
                
                player->totalWin += money;
            }
            
            //take cards
            hand->removeCards(0);
        }
    }
    
    //give experience
    if (currentPlayer->totalWin > 0)
    {
        //stats
        PlayerProfile::getInstance().statsAddWinAmount(GAME_BLACK_JACK, currentPlayer->totalWin);
        
        XP += PlayerProfile::getInstance().getExperienceForCoins(currentPlayer->totalWin);
    }
    
    if (currentPlayer->totalLoose > 0)
    {
        //stats
        XP += PlayerProfile::getInstance().getExperienceForCoins(currentPlayer->totalLoose);
    }
    
    handSelector->setVisible(false);
    
    auto isLevelUP = PlayerProfile::getInstance().isExperienceEnoughForLevelUp(XP);
    if (isLevelUP)
    {
        auto currLevel = PlayerProfile::getInstance().getLevel();
        coinsChange += (currLevel + 1) * PlayerProfile::getInstance().getLevelUpBonus();
        
        CongratulationsView::create(CongratulationsView::CongratulationType::LEVELUP, nullptr)->show(this);
    }
    increasePlayerValues(coinsChange, XP, isLevelUP);
    
    PlayerProfile::getInstance().Save();
    
    //hackyhack_ads
    CCLOG("hackyhack: BJGameScene.cpp --> round is finished!");
    spinningCount = (spinningCount + 1) % CYCLE_INTERSTITIAL_BLACKJACK;
    if (spinningCount == 0) {
        AdmobChartboost::shared()->showInterstitialAdmob();
    } else if (spinningCount == CYCLE_SPIN_RATEME) {
        long hasShown = PlayerProfile::getInstance().getShowRateWhenSpinning(SPIN_BLACKJACK);
        if (hasShown == 0) {
            PlayerProfile::getInstance().setShowRateWhenSpinning(SPIN_BLACKJACK, 1);
            RatemeAlert::create(MCONSTANTS_TEXT_RATEME)->show(this);
        }
    }
}

void BJGameScene::checkAndPay(HandView * hand)
{
    if (!hand->isDealer())
    {
        switch (hand->GetStatus())
        {
            case Hand::BUST:
            {
                // take bet after 1 second delay
                runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create(std::bind(&BJGameScene::bustHand, this, hand)), NULL));
            }
                break;
                
            case Hand::BLACKJACK:
            case Hand::VALUE21:
            {
                // Next hand after 1 second delay
                runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create(std::bind(&BJGameScene::nextHand, this)), NULL));
            }
                break;
                
            case Hand::OPTIONS:
            {
                ShowPossibleActions();
            }
                break;
                
            default:
                break;
        }
    }
}

void BJGameScene::bustHand(HandView * hand)
{
    if (hand)
    {
        // take bet
        if (!hand->isDealer())
        {
            currentPlayer->totalLoose += hand->getBet();
            currentPlayer->totalLoose += hand->getInsurance();
        }
        
        hand->takeBet(0);
        hand->takeInsurance(0);
        hand->removeCards(0.5f);
    }
    
    // Next hand after 1 second delay
    runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create(std::bind(&BJGameScene::nextHand, this)), NULL));
}

void BJGameScene::surrenderHand(HandView * hand)
{
    if (hand)
    {
        // take bet
        hand->takeHalfBet(0);
        int money = hand->grabAllTokens(0);
        
        currentPlayer->totalWin += money;
        currentPlayer->totalLoose += money;
        
        increasePlayerValues(money, 0, false);
        
        hand->takeInsurance(0);
        hand->removeCards(0.5f);
    }
    
    // Next hand after 1 second delay
    runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create(std::bind(&BJGameScene::nextHand, this)), NULL));
}

void BJGameScene::doubleHand(HandView * hand)
{
    float totalDelay = 0;
    
    if (currentPlayer->CanPlaceBet(hand->getBet()))
    {
        if (hand)
        {
            increasePlayerValues(-hand->getBet(), 0, false);
            
            hand->addBet(CoinToken::create(hand->getBet(), CoinToken::CoinTokenType::NORMAL, CoinToken::CoinTokenSkin::GREEN, nullptr), 0);
            totalDelay += 0.5f;
            
            hand->Hit(shoe->DrawCard(), totalDelay);
            totalDelay += 0.5f;
        }
        
        // Next hand after 1 second delay
        totalDelay += 1.0f;
        runAction(Sequence::create(DelayTime::create(totalDelay), CallFunc::create(std::bind(&BJGameScene::nextHand, this)), NULL));
    }
    else
    {
        showNotEnoughView();
        ShowPossibleActions();
    }
}

void BJGameScene::equalmoneyHand(HandView * hand)
{
    float totalDelay = 0;
    
    if (hand)
    {
        hand->addPayout(CoinToken::create(hand->getBet(), CoinToken::CoinTokenType::NORMAL, CoinToken::CoinTokenSkin::GREEN, nullptr), 0);
        totalDelay += 0.5f;
        
        int money = hand->grabAllTokens(totalDelay);
        
        increasePlayerValues(money, 0, false);
        
        currentPlayer->totalWin += money;
        
        totalDelay += 0.5f;
        hand->removeCards(totalDelay);
    }
    
    // Next hand after 1 second delay
    totalDelay += 1.0f;
    runAction(Sequence::create(DelayTime::create(totalDelay), CallFunc::create(std::bind(&BJGameScene::nextHand, this)), NULL));
}

std::list<BlackJackActions> BJGameScene::getPossibleActions()
{
    std::list<BlackJackActions> result;
    
    if (currentHand > -1 && playingHands.size() > currentHand)
    {
        if (currentState == ASK_INSURANCE)
        {
            if (playingHands.at(currentHand)->GetStatus() == Hand::BLACKJACK)
            {
                result.push_back(BlackJackActions::EQUAL_MONEY);
                result.push_back(BlackJackActions::STAND);
            }
            else
            {
                result.push_back(BlackJackActions::INSURANCE_DENY);
                result.push_back(BlackJackActions::INSURANCE);
                result.push_back(BlackJackActions::SURRENDER); // "Early Surrender"
            }
        }
        else
        {
            result.push_back(BlackJackActions::STAND);
            
            if (playingHands.at(currentHand)->CanHit())
            {
                result.push_back(BlackJackActions::HIT);
            }
            
            if (playingHands.at(currentHand)->CanSplit())
            {
                if (currentPlayer->CanSplitHand(playingHands.at(currentHand)))
                {
                    result.push_back(BlackJackActions::SPLIT);
                }
            }
            
            result.push_back(BlackJackActions::DOUBLE);
            
            if (playingHands.at(currentHand)->CanSurrender())
            {
                result.push_back(BlackJackActions::SURRENDER);
            }
        }
    }
    else
    {
        result.push_back(BlackJackActions::DEAL);
        
        if (previousBet > 0)
        {
            result.push_back(BlackJackActions::REBET);
        }
    }
    
    return result;
}

BJPlayer * BJGameScene::getDealer()
{
    return players.back();
}

void BJGameScene::nextHand()
{
    currentHand++;
    
    if (currentHand < playingHands.size())
    {
        auto hand = playingHands.at(currentHand);
        
        selectCurrentHand();
        
        switch (currentState)
        {
            case ASK_INSURANCE:
            {
                if (hand->isDealer())
                {
                    onStateChanged(0, BlackJackGameStates::DEALER_CHECK_BLACKJACK);
                }
                else
                {
                    ShowPossibleActions();
                }
            }
                break;
                
            case ROUND:
            {
                if (hand->isDealer())
                {
                    hand->openCard(1); // open closed card
                    
                    if (isAllBust() || isAllBlackJack())
                    {
                        //wait for animations completion
                        runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create(std::bind(&BJGameScene::onRoundFinished, this)), NULL));
                    }
                    else
                    {
                        //wait for animations completion
                        runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create(std::bind(&BJGameScene::playDealer, this)), NULL));
                    }
                }
                else
                {
                    // do not process BUSTED, STAND, BJ and VALUE21 hands, because game is over for them
                    if (hand->GetStatus() == Hand::OPTIONS)
                    {
                        ShowPossibleActions();
                    }
                    else
                    {
                        nextHand();
                    }
                }
            }
                break;
                
            default:
                break;
        }
    }
}

void BJGameScene::playDealer()
{
    auto dealer = dynamic_cast<DealerHand*>(getDealer()->getHands().at(0));
    
    if (dealer)
    {
        if (dealer->ShouldHit())
        {
            dealer->Hit(shoe->DrawCard(), 0, std::bind(&BJGameScene::playDealer, this));
        }
        else
        {
            runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create(std::bind(&BJGameScene::onRoundFinished, this)), NULL));
        }
    }
}

bool BJGameScene::isAllBust()
{
    for (auto &player: players)
    {
        for (auto &hand: player->getHands())
        {
            if (hand->isDealer() == false)
            {
                if (hand->GetStatus() != Hand::BUST)
                {
                    return false;
                }
            }
        }
    }
    
    return true;
}

bool BJGameScene::isAllBlackJack()
{
    for (auto &player: players)
    {
        for (auto &hand: player->getHands())
        {
            if (hand->isDealer() == false)
            {
                if (hand->GetStatus() != Hand::BLACKJACK)
                {
                    return false;
                }
            }
        }
    }
    
    return true;
}

#pragma Service

void BJGameScene::showPlaceBet()
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

void BJGameScene::showNotEnoughView()
{
    auto view = Sprite::create("ui/Slots/NotEnoughGold.png");
    view->setOpacity(0);
    view->runAction(Sequence::create(FadeIn::create(0.2f), DelayTime::create(1.3f), FadeOut::create(0.2f), RemoveSelf::create(), NULL));
    view->setPosition(getContentSize() / 2);
    
    addChild(view, 100);
}

bool BJGameScene::onBetButtonPressed(CoinToken::CoinTokenEvent event, CoinToken * token)
{
    AudioManager::getInstance().PlaySoundEffect("music/SlotsSound/TapBt");
    
    if (currentPlayer == nullptr)
        return false;
    
    int totalBet = currentPlayer->TotalBet() + token->getTotalValue();
    
    if (PlayerProfile::getInstance().getCoins() < totalBet)
    {
        showNotEnoughView();
        return false;
    }
    
    for (auto &hand: currentPlayer->getHands())
    {
        hand->addBet(token, false);
    }
    
    return true;
}

#pragma mark Debug

