#include "RouletteGameScene.h"

#include "LoadingSplash.h"
#include "PlayerProfile.h"

#include "CongratulationsView.h"
#include "CongratulationsView.h"
#include "AudioManager.h"
#include "AndroidCompatibility.h"

#include "Features/AdmobChartboost.h"
#include "Features/mConstants.h"

#include "RatemeAlert.h"

USING_NS_CC;

const float RouletteGameScene::ballStartDistance     = 230;
const float RouletteGameScene::ballEndDistance       = 135;
const float RouletteGameScene::ballDeflectorDistance = 217;

RouletteGameScene * RouletteGameScene::create(std::string theme)
{
    RouletteGameScene * pRet = new(std::nothrow) RouletteGameScene();
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

Scene * RouletteGameScene::createScene(std::string theme)
{
    auto scene = Scene::create();
    
    auto layer = RouletteGameScene::create(theme);
    scene->addChild(layer);

    return scene;
}

RouletteGameScene::RouletteGameScene() : wheelSpinning(false), spinSpeed(0), ballSpeed(0), ballRotation(0), currentBetValue(0), ballRollingSound(-1), lastWinCoins(0),
historyOpened(false), historyRoot(nullptr), lastBetCoins(0)
{
    
}

bool RouletteGameScene::init(std::string theme)
{
    if ( !BaseScene::init() )
    {
        return false;
    }
    
    currentTheme = theme;
    currentChapter = RouletteConfig::themeNameToChapter(currentTheme);
    
    auto winSize = Director::getInstance()->getWinSize();
    auto visSize = Director::getInstance()->getVisibleSize();
    auto visOrig = Director::getInstance()->getVisibleOrigin();

    auto background = Sprite::create(StringUtils::format("ui/Cities/%s/back.png", currentTheme.c_str()));
    background->setPosition(winSize / 2);
    this->addChild(background, 0);
    
    //logo
    auto logo = Sprite::create(StringUtils::format("ui/Cities/%s/logo.png", currentTheme.c_str()));
    logo->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    logo->setPosition(Vec2(background->getContentSize().width / 2, background->getContentSize().height - 120));
    background->addChild(logo);
    
    topbar = TopBar::create(std::bind(&RouletteGameScene::onBackButtonPressed, this));
    topbar->setPosition(Size(background->getContentSize().width / 2, background->getContentSize().height));
    background->addChild(topbar, 100);
    
    //table
    tableNode = Node::create();
    tableNode->setPosition(background->getContentSize() / 2);
    background->addChild(tableNode);
    
    table = Sprite::create("ui/Roulette/roulette_bettable.png");
    tableNode->addChild(table);
    
    //wheel
    wheelNode = Node::create();
    tableNode->addChild(wheelNode);
    
    auto base = Sprite::create("ui/Roulette/roulette_spin_back.png");
    wheelNode->addChild(base);
    
    wheel = Sprite::create("ui/Roulette/roulette_spin.png");
    wheelNode->addChild(wheel, 1);
    
    ball = Sprite::create("ui/Roulette/roulette_ball.png");
    ball->setVisible(false);
    wheelNode->addChild(ball, 2);
    
    //arrange wheel and table
    float table_padding = 40;
    float tableWidth = base->getContentSize().width + table->getContentSize().width + table_padding;
    
    wheelNode->setPositionX(- (tableWidth - base->getContentSize().width) / 2);
    table->setPositionX( (tableWidth - table->getContentSize().width) / 2);
    
    betModeTablePosition  = background->getContentSize().width / 2 - table->getPositionX();
    spinModeTablePosition = background->getContentSize().width / 2 + (tableWidth - visSize.width) / 2 + table_padding;
    
    tableNode->setPositionX(betModeTablePosition);
    
    //controls panel
    controlsPanel = ui::Scale9Sprite::create("ui/Roulette/buttons/bar_back.png");
    controlsPanel->setPreferredSize(Size(visSize.width, controlsPanel->getSprite()->getContentSize().height ));
    controlsPanel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    controlsPanel->setPosition(Vec2(getContentSize().width / 2, visOrig.y));
    background->addChild(controlsPanel, 8);
    
    for (int i = 0; i <= 3; i++)
    {
        CoinToken * betButton = CoinToken::create(RouletteConfig::BetValues[currentChapter + i - 1], CoinToken::CoinTokenType::BUTTON, static_cast<CoinToken::CoinTokenSkin>(i), nullptr);
        betButton->setPosition(Vec2(controlsPanel->getContentSize().height / 2 + i * betButton->getContentSize().width * 1.1f, controlsPanel->getContentSize().height / 2));
        betButton->setScaleForDragingTokens(0.5f);
        
        controlsPanel->addChild(betButton, 1);
        betButton->setCallback(std::bind(&RouletteGameScene::onTokenAction, this, std::placeholders::_1, std::placeholders::_2));
        betButtons.push_back(betButton);
    }
    
    //spin
    spinButton = ui::Button::create("ui/Roulette/buttons/spin_button.png", "ui/Roulette/buttons/spin_button.png", "ui/Roulette/buttons/spin_button.png");
    spinButton->setZoomScale(0);
    spinButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    spinButton->setPosition(Vec2(controlsPanel->getContentSize().width, controlsPanel->getContentSize().height / 2));
    spinButton->setName("spin");
    spinButton->addTouchEventListener(std::bind(&RouletteGameScene::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    controlsPanel->addChild(spinButton);
    
    float remain_width = controlsPanel->getContentSize().width - spinButton->getContentSize().width - betButtons.back()->getPositionX() - betButtons.back()->getContentSize().width * 0.55f;
    
    //clear
    clearButton = ui::Button::create("ui/Roulette/buttons/small_button.png", "ui/Roulette/buttons/small_button.png", "ui/Roulette/buttons/small_button.png");
    clearButton->setScale9Enabled(true);
    clearButton->setContentSize(Size(remain_width / 2, clearButton->getRendererNormal()->getSprite()->getContentSize().height));
    clearButton->setZoomScale(0);
    clearButton->setPosition(Vec2(spinButton->getPositionX() - spinButton->getContentSize().width - clearButton->getContentSize().width / 2, 37));
    clearButton->setName("clear");
    clearButton->addTouchEventListener(std::bind(&RouletteGameScene::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    controlsPanel->addChild(clearButton);
    
    auto clearLabel = Label::createWithBMFont("fonts/bluefont.fnt", "CLEAR");
    clearLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    clearLabel->setPosition(clearButton->getContentSize().width * 0.5f, clearButton->getContentSize().height * 0.6f);
    clearButton->addChild(clearLabel);
    
    //win rate
    winrateButton = ui::Button::create("ui/Roulette/buttons/small_button.png", "ui/Roulette/buttons/small_button.png", "ui/Roulette/buttons/small_button.png");
    winrateButton->setScale9Enabled(true);
    winrateButton->setContentSize(clearButton->getContentSize());
    winrateButton->setZoomScale(0);
    winrateButton->setPosition(clearButton->getPosition() - Vec2(clearButton->getContentSize().width, 0));
    winrateButton->setName("winrate");
    winrateButton->addTouchEventListener(std::bind(&RouletteGameScene::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    controlsPanel->addChild(winrateButton);
    
    rateLabel = Label::createWithBMFont("fonts/bluefont.fnt", "0%");
    rateLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    rateLabel->setPosition(winrateButton->getContentSize().width * 0.5f, winrateButton->getContentSize().height * 0.6f);
    winrateButton->addChild(rateLabel);
    
    // total bet and win
    auto totalBetAndWin = ui::Scale9Sprite::create("ui/Roulette/buttons/text_field.png");
    totalBetAndWin->setPreferredSize(Size(remain_width, totalBetAndWin->getSprite()->getContentSize().height));
    totalBetAndWin->setPosition(Vec2(spinButton->getPositionX() - spinButton->getContentSize().width - totalBetAndWin->getContentSize().width / 2, 89));
    controlsPanel->addChild(totalBetAndWin);
    
    //line
    auto line = Sprite::create("ui/Roulette/buttons/line.png");
    line->setPosition(totalBetAndWin->getContentSize() / 2);
    totalBetAndWin->addChild(line);
    
    //bet
    auto betLabel = Sprite::create("ui/Roulette/buttons/bet_label.png");
    betLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    betLabel->setPosition(Vec2(0, totalBetAndWin->getContentSize().height / 2));
    totalBetAndWin->addChild(betLabel);
    
    //win
    auto winLabel = Sprite::create("ui/Roulette/buttons/win_label.png");
    winLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    winLabel->setPosition(Vec2(totalBetAndWin->getContentSize().width / 2, totalBetAndWin->getContentSize().height / 2));
    totalBetAndWin->addChild(winLabel);
    
    //totalbet
    totalBetLabel = Label::createWithTTF("$0", "fonts/Berlin.ttf", 28.0f);
    totalBetLabel->setColor(Color3B::WHITE);
    totalBetLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    totalBetLabel->setPosition(Vec2(betLabel->getContentSize().width * 1.1, totalBetAndWin->getContentSize().height * 0.5f));
    totalBetAndWin->addChild(totalBetLabel);
    
    //totalwin
    totalWinLabel = Label::createWithTTF("$0", "fonts/Berlin.ttf", 28.0f);
    totalWinLabel->setColor(Color3B::WHITE);
    totalWinLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    totalWinLabel->setPosition(Vec2(winLabel->getPositionX() + winLabel->getContentSize().width * 1.1, totalBetAndWin->getContentSize().height * 0.5f));
    totalBetAndWin->addChild(totalWinLabel);
    
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
    openHistory->addTouchEventListener(std::bind(&RouletteGameScene::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    historyTab->addChild(openHistory);
    
    historyRoot = Node::create();
    historyRoot->setContentSize(Size(65, 490));
    historyRoot->setPosition(Vec2(47, 17));
    historyRoot->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    historyTab->addChild(historyRoot);
    
    //dolly
    dolly = Sprite::create("ui/Roulette/dolly.png");
    dolly->setVisible(false);
    dolly->setAnchorPoint(Vec2(0.5f, 0.25f));
    table->addChild(dolly, 1);
    
    onBetChanged();
    UpdateRate();
    
    // Debug
    //placeAllBets();
    
    return true;
}

void RouletteGameScene::startSpin()
{
    
    //hackyhack_publish - arc4random_uniform
//    spinSpeed = 0.5f + 0.01f * arc4random_uniform(100) * 0.3f; //rotations per second (0.5 ... 0.8)
    spinSpeed = 0.5f + 0.01f * (rand() % 100) * 0.3f; //rotations per second (0.5 ... 0.8)
    
    log("Speed: %f", spinSpeed);
    
    //hackyhack_publish - arc4random_uniform
//    ballSpeed = 1.0f + 0.01f * arc4random_uniform(100) * 0.5f; //rotations per second (1.0 ... 1.5)
    ballSpeed = 1.0f + 0.01f * (rand() % 100) * 0.5f; //rotations per second (1.0 ... 1.5)
    
    log("Ball Speed: %f", ballSpeed);
    
    ballDistance = ballStartDistance;
    ballKnocked = false;
    ballStopped = false;
    
    ball->setPosition(Vec2(0, ballDistance));
    ball->setVisible(true);
    
    ballRollingSound = AudioManager::getInstance().PlaySoundEffect("music/Sounds/ball_rolling", true);
    
    scheduleUpdate();
}

void RouletteGameScene::update(float deltaTime)
{
    if (spinSpeed > 0 || ballSpeed > 0)
    {
        float speedAcc = spinSpeed;
        
        if (speedAcc > 0)
        {
            spinSpeed -= 0.05f * deltaTime;
            speedAcc = (speedAcc + spinSpeed) / 2; //average spin speed
            
            wheel->setRotation(wheel->getRotation() - 360.0f * speedAcc * deltaTime);
        }
        
        // ball animation
        speedAcc = ballSpeed;
        
        if (!ballStopped)
        {
            ballSpeed -= 0.20f * deltaTime;
            speedAcc = (speedAcc + ballSpeed) / 2; //average spin speed
            
            ballRotation += 360.0f * speedAcc * deltaTime;
            
            float zspeed = ballSpeed / 0.75f - 0.5f;
            
            ballDistance = ballDistance + zspeed * 200.0f * deltaTime;
            ballDistance = clampf(ballDistance, ballEndDistance, ballStartDistance);
            
            if (!ballKnocked && ballDistance <= ballDeflectorDistance)
            {
                float sector = fmodf(ballRotation, 90);
                
                if (sector > 43 && sector < 47) // 4 degrees area
                {
                    ballRotation += ( 45.0f - sector);
                    ballKnocked = true;
                    ballSpeed *= 0.5f;
                }
            }
            
            if (fabsf( ballDistance - ballEndDistance ) < 5 )
            {
                //ball stopped!
                ballStopped = true;
                
                float ballPosition = fmodf(ballRotation - wheel->getRotation(), 360.0f);
                if (ballPosition < 0)
                    ballPosition += 360.0f;
                
                lastWinSector = (ballPosition / 360.0f * 37.0f + 0.5f);
                
                // debug
                if (FORCE_WIN_NUMBER)
                {
                    for (int i = 0; i < RouletteConfig::SectorNumbers.size(); i++)
                    {
                        if (RouletteConfig::SectorNumbers[i] == WIN_NUMBER_VALUE)
                        {
                            lastWinSector = i;
                            break;
                        }
                    }
                }
                
                ballRotation = lastWinSector * 360.0f / 37.0f;
                ballDistance = ballEndDistance;
                
                AudioManager::getInstance().PlaySoundEffect("music/Sounds/ball_stopped");
                
                onSpinEnded();
            }
            
            float radians = ballRotation * 3.14f / 180;
            ball->setPosition(Vec2( sinf(radians) * ballDistance, cosf(radians) * ballDistance ));
        }
        else
        {
            float radians = fmodf(ballRotation + wheel->getRotation(), 360.0f) * 3.14f / 180;
            ball->setPosition(Vec2( sinf(radians) * ballDistance, cosf(radians) * ballDistance ));
        }
    }
    else
    {
        wheel->setRotation(fmodf(wheel->getRotation(), 360.0f));
        unscheduleUpdate();
    }
}

void RouletteGameScene::onEnter()
{
    Layer::onEnter();

    PlayBackgroundMusic();
}

void RouletteGameScene::PlayBackgroundMusic()
{
    AudioManager::getInstance().PlayBackgroundMusic("music/BackgroundMusic/Ingame");
}

void RouletteGameScene::enableInput(bool enable)
{
    topbar->setEnabled(enable);
    
    spinButton->setEnabled(enable);
    spinButton->setOpacity(enable ? 255 : 127);
    
    clearButton->setEnabled(enable);
    clearButton->setOpacity(enable ? 255 : 127);
    
    winrateButton->setEnabled(enable);
    winrateButton->setOpacity(enable ? 255 : 127);
    
    for (auto &item: betButtons)
    {
        item->setEnabled(enable);
        item->setOpacity(enable ? 255 : 127);
    }
}

void RouletteGameScene::onButtonPressed(Ref * sender, ui::Widget::TouchEventType event)
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

void RouletteGameScene::onBackButtonPressed()
{
    if (ballRollingSound != -1)
        AudioManager::getInstance().StopSoundEffect(ballRollingSound);
    
    auto nextScene = LoadingSplash::createScene(LoadingSplash::ROULETTE_THEME);
    Director::getInstance()->replaceScene(nextScene);
}

void RouletteGameScene::ProcessButtonAction(cocos2d::ui::Button * button)
{
    if (button->getName() == "spin")
    {
        beforeSpin();
    }
    else if (button->getName() == "clear")
    {
        clearBets();
    }
    else if (button->getName() == "winrate")
    {
        showRates();
    }
    else if (button->getName() == "history")
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
    else if (button->getName() == "CloseResults")
    {
        button->setEnabled(false);
        
        if (button->getUserObject())
        {
            auto view = dynamic_cast<Sprite*>(button->getUserObject());
            if (view)
            {
                view->runAction(Sequence::create(FadeOut::create(0.2f), RemoveSelf::create(), NULL));
            }
        }
    }
    else if (button->getName() == "CloseRates")
    {
        button->setEnabled(false);
        
        if (button->getUserObject())
        {
            auto view = dynamic_cast<Sprite*>(button->getUserObject());
            if (view)
            {
                view->runAction(Sequence::create(FadeOut::create(0.2f), RemoveSelf::create(), NULL));
            }
        }
    }
    else if (button->getName() == "ResetRates")
    {
        button->setEnabled(false);
        
        std::string varNameWon   = StringUtils::format("RouletteChapter%d_won"  , currentChapter);
        std::string varNameBet   = StringUtils::format("RouletteChapter%d_bets" , currentChapter);
        std::string varNameGames = StringUtils::format("RouletteChapter%d_games", currentChapter);
        
        PlayerProfile::getInstance().setLongValue(varNameWon,   0);
        PlayerProfile::getInstance().setLongValue(varNameBet,   0);
        PlayerProfile::getInstance().setUIntValue(varNameGames, 0);
        
        UpdateRate();
        
        if (button->getUserObject())
        {
            auto view = dynamic_cast<Sprite*>(button->getUserObject());
            if (view)
            {
                view->runAction(Sequence::create(FadeOut::create(0.2f), RemoveSelf::create(), NULL));
            }
        }
    }
}

void RouletteGameScene::showNotEnoughView()
{
    auto view = Sprite::create("ui/Slots/NotEnoughGold.png");
    view->setOpacity(0);
    view->runAction(Sequence::create(FadeIn::create(0.2f), DelayTime::create(1.3f), FadeOut::create(0.2f), RemoveSelf::create(), NULL));
    view->setPosition(getContentSize() / 2);
    
    addChild(view, 100);
}

void RouletteGameScene::showPlaceBet()
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

void RouletteGameScene::showRates()
{
    auto view = Sprite::create("ui/Roulette/result_back.png");
    view->setOpacity(0);
    view->runAction(FadeIn::create(0.2f));
    view->setPosition(getContentSize() / 2);
    view->setCascadeOpacityEnabled(true);
    
    std::string varNameWon   = StringUtils::format("RouletteChapter%d_won"  , currentChapter);
    std::string varNameBet   = StringUtils::format("RouletteChapter%d_bets" , currentChapter);
    std::string varNameGames = StringUtils::format("RouletteChapter%d_games", currentChapter);
    
    long procent = PlayerProfile::getInstance().getLongValue(varNameWon) * 100 / std::max((long)1, PlayerProfile::getInstance().getLongValue(varNameBet, 1));
    
    std::vector< std::tuple< std::string, std::string > > values = {
        {"Games Played:"      , StringUtils::format("%d"  , PlayerProfile::getInstance().getUIntValue(varNameGames)) },
        {"Amount Won:"        , StringUtils::format("$%ld", PlayerProfile::getInstance().getLongValue(varNameWon) ) },
        {"Amount Bet:"        , StringUtils::format("$%ld", PlayerProfile::getInstance().getLongValue(varNameBet) ) },
        {"Amount Return Rate:", StringUtils::format("%ld%%", procent) },
    };
    
    auto label = Label::createWithTTF("Rate of Return", "fonts/Berlin.ttf", 36.0f);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    label->setPosition(view->getContentSize().width * 0.5f, view->getContentSize().height * 0.85f);
    view->addChild(label);
    
    int counter = 0;
    
    for (auto item: values)
    {
        auto labelTitle = Label::createWithTTF(std::get<0>(item), "fonts/Berlin.ttf", 28.0f);
        labelTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelTitle->setPosition(view->getContentSize().width * 0.15f, view->getContentSize().height * (0.70f - 0.1f * counter) );
        view->addChild(labelTitle);
        
        auto labelValue = Label::createWithTTF(std::get<1>(item), "fonts/Berlin.ttf", 28.0f);
        labelValue->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        labelValue->setPosition(view->getContentSize().width * 0.85f, view->getContentSize().height * (0.70f - 0.1f * counter) );
        view->addChild(labelValue);
        
        counter++;
    }
    
    // close button
    auto closeButton = ui::Button::create("ui/Roulette/close_button.png", "ui/Roulette/close_button.png", "ui/Roulette/close_button.png");
    closeButton->setZoomScale(0);
    closeButton->setPosition(Vec2((view->getContentSize().width - closeButton->getContentSize().width * 2) / 3 + closeButton->getContentSize().width / 2,
                                  view->getContentSize().height * 0.15f));
    closeButton->setName("CloseRates");
    closeButton->setUserObject(view);
    closeButton->setCascadeOpacityEnabled(true);
    closeButton->addTouchEventListener(std::bind(&RouletteGameScene::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    view->addChild(closeButton);
    
//    auto labelClose = Label::createWithTTF("Close", "fonts/Berlin.ttf", 36.0f);
//    labelClose->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    labelClose->setPosition(closeButton->getContentSize().width / 2, closeButton->getContentSize().height * 0.60f);
//    closeButton->addChild(labelClose);
    
    // reset button
    auto resetButton = ui::Button::create("ui/Roulette/reset_button.png", "ui/Roulette/reset_button.png", "ui/Roulette/reset_button.png");
    resetButton->setZoomScale(0);
    resetButton->setPosition(Vec2(view->getContentSize().width - (view->getContentSize().width - closeButton->getContentSize().width * 2) / 3 - closeButton->getContentSize().width / 2,
                                  view->getContentSize().height * 0.15f));
    resetButton->setName("ResetRates");
    resetButton->setUserObject(view);
    resetButton->setCascadeOpacityEnabled(true);
    resetButton->addTouchEventListener(std::bind(&RouletteGameScene::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    view->addChild(resetButton);
    
//    auto labelReset = Label::createWithTTF("Reset", "fonts/Berlin.ttf", 36.0f);
//    labelReset->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    labelReset->setPosition(resetButton->getContentSize().width / 2, resetButton->getContentSize().height * 0.60f);
//    resetButton->addChild(labelReset);
    
    // enable modal behaviour
    auto eventListener = EventListenerTouchOneByOne::create();
    eventListener->setSwallowTouches(true);
    
    eventListener->onTouchBegan = [](Touch * touch, Event * event) {
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, view);
    
    addChild(view, 100);
}

void RouletteGameScene::showResults(int winNumber, int currentBet, int currentWin)
{
    lastBetCoins = currentBet;
    
    auto view = Sprite::create("ui/Roulette/result_back.png");
    view->setOpacity(0);
    view->runAction(Sequence::createWithTwoActions(DelayTime::create(4.0f), FadeIn::create(0.2f)));
    view->setPosition(getContentSize() / 2);
    view->setCascadeOpacityEnabled(true);
    
    auto label = Label::createWithTTF("Spin Value:", "fonts/Berlin.ttf", 36.0f);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    label->setPosition(view->getContentSize().width * 0.15f, view->getContentSize().height * 0.8f);
    view->addChild(label);
    
    auto labelBets = Label::createWithTTF("Bets:", "fonts/Berlin.ttf", 36.0f);
    labelBets->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    labelBets->setPosition(view->getContentSize().width * 0.15f, view->getContentSize().height * 0.60f);
    view->addChild(labelBets);
    
    auto labelPayout = Label::createWithTTF("Payout:", "fonts/Berlin.ttf", 36.0f);
    labelPayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    labelPayout->setPosition(view->getContentSize().width * 0.15f, view->getContentSize().height * 0.40f);
    view->addChild(labelPayout);
    
    auto labelNumber = Label::createWithTTF(StringUtils::format("%d", winNumber), "fonts/Berlin.ttf", 36.0f);
    labelNumber->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    labelNumber->setPosition(view->getContentSize().width * 0.85f, view->getContentSize().height * 0.8f);
    view->addChild(labelNumber);
    
    auto labelBetValue = Label::createWithTTF(StringUtils::format("%d", currentBet), "fonts/Berlin.ttf", 36.0f);
    labelBetValue->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    labelBetValue->setPosition(view->getContentSize().width * 0.85f, view->getContentSize().height * 0.60f);
    view->addChild(labelBetValue);
    
    auto labelWinValue = Label::createWithTTF(StringUtils::format("%d", currentWin), "fonts/Berlin.ttf", 36.0f);
    labelWinValue->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    labelWinValue->setPosition(view->getContentSize().width * 0.85f, view->getContentSize().height * 0.40f);
    view->addChild(labelWinValue);
    
    auto closeButton = ui::Button::create("ui/Roulette/ok_button.png", "ui/Roulette/ok_button.png", "ui/Roulette/ok_button.png");
    closeButton->setZoomScale(0);
    closeButton->setPosition(Vec2(view->getContentSize().width / 2, view->getContentSize().height * 0.15f));
    closeButton->setName("CloseResults");
    closeButton->setUserObject(view);
    closeButton->setCascadeOpacityEnabled(true);
    closeButton->addTouchEventListener(std::bind(&RouletteGameScene::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    view->addChild(closeButton);
    
    auto labelClose = Label::createWithTTF("Close", "fonts/Berlin.ttf", 36.0f);
    labelClose->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    labelClose->setPosition(closeButton->getContentSize().width / 2, closeButton->getContentSize().height * 0.60f);
    closeButton->addChild(labelClose);
    
    // enable modal behaviour
    auto eventListener = EventListenerTouchOneByOne::create();
    eventListener->setSwallowTouches(true);
    
    eventListener->onTouchBegan = [](Touch * touch, Event * event) {
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, view);
    
    addChild(view, 100);
    
    //hackyhack_ads
    CCLOG("hackyhack: RouletteGameScene.cpp --> round is finished!");
    spinningCount = (spinningCount + 1) % CYCLE_INTERSTITIAL_ROULETTE;
    if (spinningCount == 0) {
        AdmobChartboost::shared()->showInterstitialAdmob();
    } else if (spinningCount == CYCLE_SPIN_RATEME) {
        long hasShown = PlayerProfile::getInstance().getShowRateWhenSpinning(SPIN_ROULETTE);
        if (hasShown == 0) {
            PlayerProfile::getInstance().setShowRateWhenSpinning(SPIN_ROULETTE, 1);
            RatemeAlert::create(MCONSTANTS_TEXT_RATEME)->show(this);
        }
    }

}

void RouletteGameScene::beforeSpin()
{
    //check bet is placed
    if (currentBetValue == 0)
    {
        showPlaceBet();
        return;
    }
    
    //check money enough
    if (PlayerProfile::getInstance().getCoins() < currentBetValue)
    {
        showNotEnoughView();
        return;
    }
    
    enableInput(false);
    
    //take bet
    PlayerProfile::getInstance().statsAddBetAmount(GAME_ROULETTE, currentBetValue);
    PlayerProfile::getInstance().addPiggyBankCoins(PIGGY_BANK_PROCENT * currentBetValue);
    
    int coinsChange = -currentBetValue;
    
    auto XP = PlayerProfile::getInstance().getExperienceForCoins(coinsChange);
    bool isLevelUP = PlayerProfile::getInstance().isExperienceEnoughForLevelUp(XP);
    
    if (isLevelUP)
    {
        auto currLevel = PlayerProfile::getInstance().getLevel();
        
        coinsChange += (currLevel + 1) * PlayerProfile::getInstance().getLevelUpBonus();
        CongratulationsView::create(CongratulationsView::CongratulationType::LEVELUP, nullptr)->show(this);
    }
    
    increasePlayerValues(coinsChange, XP, isLevelUP);
    
    
    moveTableToSpin();
}

void RouletteGameScene::onBetChanged()
{
    currentBetValue = 0;
    
    for (auto &item: currentBets)
    {
        currentBetValue += item.second->getTotalValue();
    }
    
    totalBetLabel->setString(StringUtils::format("$%d", currentBetValue));
}

void RouletteGameScene::onSpinEnded()
{
    AudioManager::getInstance().StopSoundEffect(ballRollingSound);
    ballRollingSound = -1;
    
    //update statistics
    std::string varName = StringUtils::format("RouletteChapter%d_games", currentChapter);
    PlayerProfile::getInstance().setUIntValue(varName, PlayerProfile::getInstance().getUIntValue(varName) + 1);
    
    varName = StringUtils::format("RouletteChapter%d_bets", currentChapter);
    PlayerProfile::getInstance().setLongValue(varName, PlayerProfile::getInstance().getLongValue(varName) + currentBetValue);
    
    log("Win sector is: %d", lastWinSector);
    
    lastWinNumber = RouletteConfig::SectorNumbers[lastWinSector % RouletteConfig::SectorNumbers.size()];
    
    log("Win number is: %d", lastWinNumber);
    
    CheckWinConditions();
    
    clearBets();
    
    enableInput(true);
    
    moveTableToBet();
    
    PlayerProfile::getInstance().Save();
}

void RouletteGameScene::setWinAmountLabel(int amount, float time)
{
    int result = std::min(amount, (int)(time * amount));
    
    if (amount < 20) // show without animation
    {
        result = amount;
    }
    
    totalWinLabel->setString(StringUtils::format("$%d", result));
    totalWinLabel->stopAllActions();
    
    if (result == amount)
        return;
    
    totalWinLabel->runAction(Sequence::create(DelayTime::create(0.016f), CallFunc::create([this, amount, time]()
    {
        setWinAmountLabel(amount, time + 0.016f);
    }), NULL));
}

bool RouletteGameScene::onTokenAction(CoinToken::CoinTokenEvent event, CoinToken * token)
{
    if (event == CoinToken::CoinTokenEvent::PLACED)
    {
        Vec2 world = token->convertToWorldSpaceAR(Vec2::ZERO);
        Vec2 point = table->convertToNodeSpace(world);
        
        for (int i = 0; i < RouletteConfig::BettablePoints.size(); i++)
        {
            Rect area = RouletteConfig::BettablePoints[i].area;
            area.origin.y = table->getContentSize().height - area.origin.y;
            
            if (area.containsPoint(point))
            {
                if (token)
                {
                    auto itr = currentBets.find(i);
                    
                    if (itr != currentBets.end())
                    {
                        // bet exists on that place, stack them
                        itr->second->addToken(token->getTotalValue(), token->getSkin(), token->getSourceToken());
                        token->stopAllActions();
                        token->removeFromParent();
                    }
                    else
                    {
                        // create new bet
                        token->retain();
                        token->stopAllActions();
                        token->removeFromParentAndCleanup(false);
                        table->addChild(token);
                        token->release();
                        token->setPosition(area.origin + area.size / 2);
                        token->setType(CoinToken::CoinTokenType::STACK);
                        token->setCallback(std::bind(&RouletteGameScene::onTokenAction, this, std::placeholders::_1, std::placeholders::_2));
                        
                        token->setBetPlace(i);
                        
                        currentBets[i] = token;
                    }
                }
                
                onBetChanged();
                
                return true;
            }
        }
    }
    else if (event == CoinToken::CoinTokenEvent::CANCELED)
    {
        if (token)
        {
            auto itr = currentBets.find(token->getBetPlace());
            
            if (token->popToken())
            {
                if (itr != currentBets.end())
                {
                    currentBets.erase(itr);
                }
            }
            
            onBetChanged();
            
            return true;
        }
    }
    
    return false;
}

#pragma mark Core Methods

void RouletteGameScene::CheckWinConditions()
{
    int result = 0;
    
    for (auto &item: currentBets)
    {
        for (auto value: RouletteConfig::BettablePoints[item.first].values)
        {
            if (value == lastWinNumber)
            {
                result += item.second->getTotalValue() * (1 + RouletteConfig::BetRates.at(RouletteConfig::BettablePoints[item.first].type));
            }
        }
    }
    
    //move dolly
    auto item = RouletteConfig::getNumberInfo(lastWinNumber);
    if (item.area.equals(Rect::ZERO) == false)
    {
        item.area.origin.y = table->getContentSize().height - item.area.origin.y;
        dolly->setPosition(item.area.origin + item.area.size / 2);
        dolly->setVisible(true);
    }
    
    //update history
    resultsHistory.push_front(HistoryItem(lastWinNumber, RouletteConfig::getNumberColor(lastWinNumber) ));
    rebuildHistory();
    
    if (result > 0)
    {
        log("User won: $%d", result);
        
        //statistics
        std::string varName = StringUtils::format("RouletteChapter%d_won" , currentChapter);
        PlayerProfile::getInstance().setLongValue(varName, PlayerProfile::getInstance().getLongValue(varName) + result);
        
        int coinsChange = result;
        
        auto XP = PlayerProfile::getInstance().getExperienceForCoins(coinsChange);
        bool isLevelUP = PlayerProfile::getInstance().isExperienceEnoughForLevelUp(XP);
        
        if (isLevelUP)
        {
            auto currLevel = PlayerProfile::getInstance().getLevel();
            
            coinsChange += (currLevel + 1) * PlayerProfile::getInstance().getLevelUpBonus();
            CongratulationsView::create(CongratulationsView::CongratulationType::LEVELUP, nullptr)->show(this);
        }
        
        increasePlayerValues(coinsChange, XP, isLevelUP);
        
        UpdateRate();
    }
    
    lastWinCoins = result;
    
    setWinAmountLabel(result);
    
    if (lastWinCoins > 0)
        PlayerProfile::getInstance().statsAddWinAmount(GAME_ROULETTE, lastWinCoins);
    
    showResults(lastWinNumber, currentBetValue, result);
}

void RouletteGameScene::UpdateRate()
{
    //statistics
    std::string varName    = StringUtils::format("RouletteChapter%d_won" , currentChapter);
    std::string varNameBet = StringUtils::format("RouletteChapter%d_bets", currentChapter);
    
    int procent = PlayerProfile::getInstance().getLongValue(varName) * 100 / std::max((long)1, PlayerProfile::getInstance().getLongValue(varNameBet, 1));
    rateLabel->setString(StringUtils::format("%d%%", procent));
}

void RouletteGameScene::clearBets()
{
    for (auto &item: currentBets)
    {
        item.second->removeFromParent();
    }
    
    currentBets.clear();
    
    onBetChanged();
}

void RouletteGameScene::moveTableToBet()
{
    tableNode->stopAllActions();
    tableNode->runAction(Sequence::create(DelayTime::create(4.0f), EaseSineInOut::create(MoveTo::create(0.7f, Vec2(betModeTablePosition, tableNode->getPositionY()))), nullptr));
}

void RouletteGameScene::moveTableToSpin()
{
    tableNode->stopAllActions();
    tableNode->runAction(Sequence::create(EaseSineInOut::create(MoveTo::create(0.7f, Vec2(spinModeTablePosition, tableNode->getPositionY()))),
                                          CallFunc::create(std::bind(&RouletteGameScene::startSpin, this)), nullptr) );
}

void RouletteGameScene::rebuildHistory()
{
    historyRoot->removeAllChildren();
    
    float posY = 0;
    
    for (auto &item: resultsHistory)
    {
        auto label = Label::createWithTTF(StringUtils::format("%d", item.number), "fonts/Berlin.ttf", 30);
        
        label->setDimensions(historyRoot->getContentSize().width, label->getContentSize().height);
        label->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        label->setPosition(Vec2(0, historyRoot->getContentSize().height - posY));
        
        switch (item.color)
        {
            case RouletteConfig::ZERO:
                label->setColor(Color3B::GREEN);
                label->setHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
                break;
            case RouletteConfig::BLACK:
                label->setColor(Color3B::WHITE);
                label->setHorizontalAlignment(cocos2d::TextHAlignment::RIGHT);
                break;
            case RouletteConfig::RED:
                label->setColor(Color3B::RED);
                label->setHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
                break;
        }
        
        historyRoot->addChild(label);
        
        posY += label->getContentSize().height;
        
        if (posY + label->getContentSize().height > historyRoot->getContentSize().height)
            break;
    }
}

#pragma mark Debug

void RouletteGameScene::placeAllBets()
{
    int counter = 0;
    for (auto &item: RouletteConfig::BettablePoints)
    {
        CoinToken * token = CoinToken::create(counter, CoinToken::CoinTokenType::STACK, CoinToken::CoinTokenSkin::GREY, nullptr);
        token->setScale(0.33f);
        
        Rect area = item.area;
        area.origin.y = table->getContentSize().height - area.origin.y;
        
        token->setPosition(area.origin + area.size / 2);
        table->addChild(token);
        
        counter++;
    }
}
