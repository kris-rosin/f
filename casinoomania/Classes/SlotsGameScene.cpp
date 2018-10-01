#include "SlotsGameScene.h"
#include "AudioManager.h"

#include "LoadingSplash.h"
#include "PlayerProfile.h"
#include "CoinsPanel.h"
#include "PayoutsView.h"
#include "ExperiencePanel.h"
#include "JackpotPanel.h"
#include "CongratulationsView.h"
#include "CoinsRainView.h"

#include "ChestsView.h"
#include "WheelView.h"
#include "FreeSpinView.h"

#include "SlotsConfig.h"

#include "AndroidCompatibility.h"
#include "ServerConnection.h"

#include "Features/AdmobChartboost.h"
#include "Features/mConstants.h"
#include "RatemeAlert.h"



USING_NS_CC;

SlotsGameScene * SlotsGameScene::create(std::string theme, int lines)
{
    SlotsGameScene * pRet = new(std::nothrow) SlotsGameScene();
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

Scene * SlotsGameScene::createScene(std::string theme, int lines)
{
    auto scene = Scene::create();
    
    auto layer = SlotsGameScene::create(theme, lines);
    scene->addChild(layer);

    return scene;
}

SlotsGameScene::SlotsGameScene() : controlsPanel(nullptr), currentLinesSelected(1), currentBetSelected(0), currentTotalBet(0), wheelSpinning(false),
autoSpinsRemains(0), autoSpinInProgress(false), winAnimationInProgress(false), bScheduleBonus(false), currentTotalWin(0), currentFreeSpins(0), freeSpinInProgress(false),
bonusViewShown(false), currentBetIndex(0)
{
    
}

bool SlotsGameScene::init(std::string theme, int lines)
{
    if ( !BaseScene::init() )
    {
        return false;
    }
    
    currentLines = lines;
    currentTheme = theme;
    
    currentChapter = SlotsConfig::themeNameToChapter(currentTheme);
    
    auto winSize = Director::getInstance()->getWinSize();
    auto visSize = Director::getInstance()->getVisibleSize();
    auto visOrig = Director::getInstance()->getVisibleOrigin();

    auto background = Sprite::create(StringUtils::format("ui/Slots/%s/background.png", currentTheme.c_str()));
    background->setPosition(winSize / 2);
    this->addChild(background, 0);
    
    auto frame = Sprite::create(StringUtils::format("ui/Slots/%s/frame.png", currentTheme.c_str()));
    frame->setPosition(winSize / 2);
    background->addChild(frame);
    
    //columns
    wheels.resize(5);
    
    for (int i = 0; i < 5; i++)
    {
        auto item = Sprite::create(StringUtils::format("ui/Slots/%s/column.png", currentTheme.c_str()));
        //item->setPosition(Vec2(frame->getContentSize().width / 2 + (i - 2) * SlotsConfig::wheel_items_width, frame->getContentSize().height / 2));
        //frame->addChild(item);
        
        auto panel = ui::Layout::create();
        panel->setContentSize(item->getContentSize());
        panel->setPosition(Vec2(frame->getContentSize().width / 2 - SlotsConfig::wheel_items_width / 2 + (i - 2) * SlotsConfig::wheel_items_width,
                                frame->getContentSize().height / 2 - item->getContentSize().height / 2));
        frame->addChild(panel, 2);
        
        createWheel(i, panel);
    }
    
    topbar = TopBar::create(std::bind(&SlotsGameScene::onBackButtonPressed, this));
    topbar->setPosition(Size(background->getContentSize().width / 2, background->getContentSize().height));
    background->addChild(topbar, 100);
    
    //
    auto hudPanel = Node::create();
    hudPanel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    hudPanel->setPosition(Vec2(getContentSize().width / 2, visOrig.y + visSize.height));
    hudPanel->setContentSize(Size(980, 80.0f));
    background->addChild(hudPanel, 8);
    
    //jackpot
    jackpotPanel = JackpotPanel::create();
    jackpotPanel->setPosition(Vec2(background->getContentSize().width / 2, background->getContentSize().height - 120));
    background->addChild(jackpotPanel, 99);
    
    //controls panel
    auto proto = Sprite::create("ui/Slots/buttons/panel_back.png");
    controlsPanel = ui::Scale9Sprite::create("ui/Slots/buttons/panel_back.png");
    controlsPanel->setPreferredSize(Size(Director::getInstance()->getVisibleSize().width, proto->getContentSize().height));
    controlsPanel->setAnchorPoint(Vec2::ZERO);
    controlsPanel->setPosition(Director::getInstance()->getVisibleOrigin());
    background->addChild(controlsPanel, 8);
    
    std::list<Node*> itemsToArrange;
    
    //spin
    spinButton = ui::Button::create("ui/Slots/buttons/spin_button.png", "ui/Slots/buttons/spin_button.png", "ui/Slots/buttons/spin_button.png");
    spinButton->setZoomScale(0);
    spinButton->setPositionY(controlsPanel->getContentSize().height / 2);
    spinButton->setName("spin");
    spinButton->addTouchEventListener(std::bind(&SlotsGameScene::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    controlsPanel->addChild(spinButton);
    itemsToArrange.push_back(spinButton);
    
    //#### total bet & max win #####
    float baseLine = controlsPanel->getContentSize().width - spinButton->getContentSize().width - 4 * 5; // 4 x spacers
    
    std::string textField = "ui/Slots/buttons/slots_text_field.png";
    
    auto fieldProto = Sprite::create(textField);
    auto totalBetAndWin = ui::Scale9Sprite::create(textField);
    totalBetAndWin->setPreferredSize(Size(0.6f * baseLine, fieldProto->getContentSize().height));
    totalBetAndWin->setPositionY(100);
    controlsPanel->addChild(totalBetAndWin);
    itemsToArrange.push_front(totalBetAndWin);
    
    auto betLabel = Sprite::create("ui/Slots/buttons/label_totalbet.png");
    betLabel->setPosition(Vec2(0, totalBetAndWin->getContentSize().height / 2));
    betLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    totalBetAndWin->addChild(betLabel);
    
    auto delimeter = Sprite::create("ui/Slots/buttons/vertical_line.png");
    delimeter->setPosition(totalBetAndWin->getContentSize() / 2);
    totalBetAndWin->addChild(delimeter);
    
    totalBetLabel = Label::createWithTTF("$0", "fonts/Berlin.ttf", 28.0f);
    totalBetLabel->setColor(Color3B::WHITE);
    totalBetLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    totalBetLabel->setPosition(betLabel->getPosition() + Vec2(betLabel->getContentSize().width + 5, 0));
    totalBetAndWin->addChild(totalBetLabel);
    
    auto winLabel = Sprite::create("ui/Slots/buttons/label_win.png");
    winLabel->setPosition(totalBetAndWin->getContentSize() / 2);
    winLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    totalBetAndWin->addChild(winLabel);
    
    totalWinLabel = Label::createWithTTF("$0", "fonts/Berlin.ttf", 28.0f);
    totalWinLabel->setColor(Color3B::WHITE);
    totalWinLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    totalWinLabel->setPosition(winLabel->getPosition() + Vec2(winLabel->getContentSize().width + 5, 0));
    totalBetAndWin->addChild(totalWinLabel);
    
    //pay table
    std::string buttonNormal  = "ui/Slots/buttons/slots_button_normal.png";
    std::string buttonPressed = "ui/Slots/buttons/slots_button_selected.png";
    
    auto btnProto = Sprite::create(buttonNormal);
    
    paytable = ui::Button::create(buttonNormal, buttonPressed, buttonNormal);
    paytable->setScale9Enabled(true);
    paytable->setContentSize(Size((totalBetAndWin->getContentSize().width - 5) / 2, btnProto->getContentSize().height));
    paytable->setZoomScale(0);
    paytable->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    paytable->setPosition(Vec2(0, -4));
    paytable->setName("paytable");
    paytable->addTouchEventListener(std::bind(&SlotsGameScene::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    totalBetAndWin->addChild(paytable);
    
    auto paytable_label = Sprite::create("ui/Slots/buttons/label_paytable.png");
    paytable_label->setPosition(paytable->getContentSize() / 2);
    paytable->addChild(paytable_label);
    
    //max bet
    maxLinesButton = ui::Button::create(buttonNormal, buttonPressed, buttonNormal);
    maxLinesButton->setScale9Enabled(true);
    maxLinesButton->setContentSize(Size((totalBetAndWin->getContentSize().width - 5) / 2, btnProto->getContentSize().height));
    maxLinesButton->setZoomScale(0);
    maxLinesButton->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    maxLinesButton->setPosition(Vec2(totalBetAndWin->getContentSize().width, -4));
    maxLinesButton->setName("max_lines");
    maxLinesButton->addTouchEventListener(std::bind(&SlotsGameScene::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    totalBetAndWin->addChild(maxLinesButton);
    
    auto maxbet_label = Sprite::create("ui/Slots/buttons/label_maxbet.png");
    maxbet_label->setPosition(maxLinesButton->getContentSize() / 2);
    maxLinesButton->addChild(maxbet_label);
    
    //bets
    auto betsField = ui::Scale9Sprite::create(textField);
    betsField->setPreferredSize(Size(0.20f * baseLine, fieldProto->getContentSize().height));
    betsField->setPositionY(100);
    controlsPanel->addChild(betsField);
    itemsToArrange.push_front(betsField);
    
    auto betsFieldLabel = Sprite::create("ui/Slots/buttons/label_bet.png");
    betsFieldLabel->setPosition(Vec2(0, betsField->getContentSize().height / 2));
    betsFieldLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    betsField->addChild(betsFieldLabel);
    
    selectedBetLabel = Label::createWithTTF("$0", "fonts/Berlin.ttf", 28.0f);
    selectedBetLabel->setColor(Color3B::WHITE);
    selectedBetLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    selectedBetLabel->setPosition(betsFieldLabel->getPosition() + Vec2(betsFieldLabel->getContentSize().width + 5, 0));
    betsField->addChild(selectedBetLabel);
    
    std::string minusImage = "ui/Slots/buttons/left_button.png" ;
    std::string plusImage  = "ui/Slots/buttons/right_button.png";
    
    auto minusProto = Sprite::create(minusImage);
    auto plusProto  = Sprite::create(plusImage);
    
    {
        auto linesMinus = ui::Button::create(minusImage, minusImage, minusImage);
        auto linesPlus  = ui::Button::create(plusImage , plusImage , plusImage );
        
        linesMinus->setScale9Enabled(true);
        linesPlus->setScale9Enabled(true);
        
        linesMinus->setCapInsets(Rect(minusProto->getContentSize().width * 0.5f, minusProto->getContentSize().height * 0.5f, minusProto->getContentSize().width * 0.25f, 1));
        linesPlus->setCapInsets( Rect( plusProto->getContentSize().width * 0.25f, plusProto->getContentSize().height * 0.5f,  plusProto->getContentSize().width * 0.25f, 1));
        
        linesMinus->setContentSize(Size(betsField->getContentSize().width / 2, minusProto->getContentSize().height));
        linesPlus->setContentSize( Size(betsField->getContentSize().width / 2,  plusProto->getContentSize().height));
        
        linesMinus->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        linesPlus->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT);
        
        linesMinus->setPosition(Vec2(betsField->getContentSize().width / 2, -4));
        linesPlus->setPosition( Vec2(betsField->getContentSize().width / 2, -4));
        
        betsField->addChild(linesMinus);
        betsField->addChild(linesPlus);
        
        linesMinus->setName("bets_minus");
        linesPlus->setName( "bets_plus");
        linesMinus->addTouchEventListener(std::bind(&SlotsGameScene::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
        linesPlus->addTouchEventListener( std::bind(&SlotsGameScene::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
        
        auto linesMinusSign = Sprite::create("ui/Slots/buttons/minus_sign.png");
        auto linesPlusSign  = Sprite::create("ui/Slots/buttons/plus_sign.png" );
        
        linesMinusSign->setPosition(linesMinus->getContentSize() / 2);
        linesPlusSign->setPosition(  linesPlus->getContentSize() / 2);
        
        linesMinus->addChild(linesMinusSign);
        linesPlus->addChild(linesPlusSign);
    }
    
    //lines
    auto linesField = ui::Scale9Sprite::create(textField);
    linesField->setPreferredSize(Size(0.20f * baseLine, fieldProto->getContentSize().height));
    linesField->setPositionY(100);
    controlsPanel->addChild(linesField);
    itemsToArrange.push_front(linesField);
    
    auto linesFieldLabel = Sprite::create("ui/Slots/buttons/label_lines.png");
    linesFieldLabel->setPosition(Vec2(0, linesField->getContentSize().height / 2));
    linesFieldLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    linesField->addChild(linesFieldLabel);
    
    selectedLinesLabel = Label::createWithTTF("1", "fonts/Berlin.ttf", 28.0f);
    selectedLinesLabel->setColor(Color3B::WHITE);
    selectedLinesLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    selectedLinesLabel->setPosition(linesFieldLabel->getPosition() + Vec2(linesFieldLabel->getContentSize().width + 5, 0));
    linesField->addChild(selectedLinesLabel);
    
    {
        auto linesMinus = ui::Button::create(minusImage, minusImage, minusImage);
        auto linesPlus  = ui::Button::create(plusImage , plusImage , plusImage );
        
        linesMinus->setScale9Enabled(true);
        linesPlus->setScale9Enabled(true);
        
        linesMinus->setCapInsets(Rect(minusProto->getContentSize().width * 0.5f, minusProto->getContentSize().height * 0.5f, minusProto->getContentSize().width * 0.25f, 1));
        linesPlus->setCapInsets( Rect( plusProto->getContentSize().width * 0.25f, plusProto->getContentSize().height * 0.5f,  plusProto->getContentSize().width * 0.25f, 1));
        
        linesMinus->setContentSize(Size(linesField->getContentSize().width / 2, minusProto->getContentSize().height));
        linesPlus->setContentSize( Size(linesField->getContentSize().width / 2,  plusProto->getContentSize().height));
        
        linesMinus->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        linesPlus->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT);
        
        linesMinus->setPosition(Vec2(linesField->getContentSize().width / 2, -4));
        linesPlus->setPosition( Vec2(linesField->getContentSize().width / 2, -4));
        
        linesField->addChild(linesMinus);
        linesField->addChild(linesPlus);
        
        linesMinus->setName("lines_minus");
        linesPlus->setName("lines_plus");
        linesMinus->addTouchEventListener(std::bind(&SlotsGameScene::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
        linesPlus->addTouchEventListener( std::bind(&SlotsGameScene::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
        
        auto linesMinusSign = Sprite::create("ui/Slots/buttons/minus_sign.png");
        auto linesPlusSign  = Sprite::create("ui/Slots/buttons/plus_sign.png" );
        
        linesMinusSign->setPosition(linesMinus->getContentSize() / 2);
        linesPlusSign->setPosition(  linesPlus->getContentSize() / 2);
        
        linesMinus->addChild(linesMinusSign);
        linesPlus->addChild(linesPlusSign);
    }
    
    //arrange items
    float total_width = 0;
    for (auto &item: itemsToArrange)
    {
        total_width += item->getContentSize().width;
    }
    
    float padding = (controlsPanel->getContentSize().width - total_width) / std::max((int)1, (int)itemsToArrange.size() - 1);
    float posX = 0;
    for (auto &item: itemsToArrange)
    {
        item->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        item->setPositionX(posX + item->getContentSize().width / 2);
        
        posX += padding + item->getContentSize().width;
    }
    
    //game field init
    winBoxesPanel = Node::create();
    winBoxesPanel->setPosition(frame->getContentSize() / 2);
    frame->addChild(winBoxesPanel, 5);
    initWinBoxes(winBoxesPanel);
    
    winLinesPanel = Node::create();
    winLinesPanel->setPosition(frame->getContentSize() / 2);
    frame->addChild(winLinesPanel, 6);
    
    lineButtonsPanel = Node::create();
    lineButtonsPanel->setPosition(frame->getContentSize() / 2);
    frame->addChild(lineButtonsPanel, 7);
    initLinesButtons();
    
    //autospins
    autoSpinCounter = Sprite::create("ui/Slots/spins_counter.png");
    autoSpinCounter->setPosition(Vec2(680.0f, 600.0f));
    autoSpinCounter->setVisible(false);
    
    autospinsLabel = Label::createWithTTF("", "fonts/Berlin.ttf", 36);
    autospinsLabel->setTextColor(Color4B::WHITE);
    autospinsLabel->setPosition(Vec2(autoSpinCounter->getContentSize().width / 2, autoSpinCounter->getContentSize().height * 0.6f));
    autoSpinCounter->addChild(autospinsLabel);
    addChild(autoSpinCounter, 8);
    
    //freespins
    freeSpinCounter = Sprite::create("ui/Slots/spins_counter.png");
    freeSpinCounter->setPosition(Vec2(680.0f, 600.0f));
    freeSpinCounter->setVisible(false);
    
    freespinsLabel = Label::createWithTTF("", "fonts/Berlin.ttf", 36);
    freespinsLabel->setTextColor(Color4B::WHITE);
    freespinsLabel->setPosition(Vec2(freeSpinCounter->getContentSize().width / 2, freeSpinCounter->getContentSize().height * 0.6f));
    freeSpinCounter->addChild(freespinsLabel);
    addChild(freeSpinCounter, 8);
    
    //adjust game field
    float visibleHeight = visSize.height - topbar->getContentSize().height - controlsPanel->getContentSize().height;
    frame->setPositionY(controlsPanel->getContentSize().height + visibleHeight / 2);
    
    int screenHeightIndex = visSize.height * 16 / visSize.width;
    if (screenHeightIndex < 12) // wide screen
    {
        frame->setScale(visibleHeight / 456.0f);
        frame->setPositionY(frame->getPositionY() + 12);
    }
    
    //select max lines
    currentLinesSelected = SlotsConfig::lines;
    //select middle bet
    auto bets = SlotsConfig::getBetsForLevel(PlayerProfile::getInstance().getLevel(), currentChapter);
    currentBetIndex = bets.size() * 0.5f + 0.5f;
    currentBetSelected = bets[currentBetIndex];
    
    onBetChanged();
    
    //clear lines for first time
    hideWinLines();
    updateLinesButtons(currentLines);
    
    wheelScrollLastPositions.resize(5, 0.0f);
    wheelsPositions.resize(5, 24);
    wheelAnimated.resize(5, false);
    
    //lightUpAllWinLinesTo(currentLines);
    //DebugShowAllWinConditions();
    
    scheduleUpdate();
    
    return true;
}

void SlotsGameScene::update(float deltaTime)
{
    updateWheels(deltaTime);
}

void SlotsGameScene::updateWheels(float deltaTime)
{
    if (wheelSpinning == false)
        return;
    
    const float itemSize  = SlotsConfig::wheel_items_height;
    const float spinSpeed = 8000.0f;
    
    auto finish = CallFunc::create([this](){ wheelSpinning = false; this->CheckWinConditions(); });
    
    //animate wheels
    for (int i = 0; i < 5; i++)
    {
        if (wheelAnimated[i])
        {
            //update position
            wheels[i]->setPositionY(wheels[i]->getPositionY() - deltaTime * spinSpeed * (wheelsScrollAmount[i] > SlotsConfig::wheel_items_amount / 2 ? 1.0f : 0.5f) );
            
            float delta = wheels[i]->getPositionY() - wheelScrollLastPositions[i];
            
            if ( fabsf(delta) > itemSize)
            {
                int itemsScrolled = static_cast<int>(fabsf(delta) / itemSize);
                
                for (int m = 0; m < itemsScrolled; m++)
                {
                    wheelsPositions[i]--;
                    if (wheelsPositions[i] <= 0)
                    {
                        wheelsPositions[i] += SlotsConfig::wheel_items_amount;
                        
                        wheels[i]->setPositionY(delta + itemSize);
                        wheelScrollLastPositions[i] = 0;
                    }
                    else
                    {
                        wheelScrollLastPositions[i] += -itemSize;
                    }
                    
                    wheelsScrollAmount[i]--;
                    
                    if (wheelsScrollAmount[i] == 0)
                    {
                        wheelAnimated[i] = false;
                        wheels[i]->setPositionY(wheelScrollLastPositions[i]); //fix position
                        
                        
                        
                        //stop animation and sound
                        wheels[i]->stopAllActions();
                        wheels[i]->runAction(Sequence::create(EaseSineOut::create( MoveTo::create(0.3f, wheels[i]->getPosition() + Vec2(0, -itemSize / 2)) ),
                                                              CallFunc::create([this](){ AudioManager::getInstance().PlaySoundEffect("music/SlotsSound/FinishedSpin"); }),
                                                              EaseSineIn::create( MoveTo::create(0.3f,  wheels[i]->getPosition() )), (i == 4) ? finish : NULL, NULL));
                        break;
                    }
                }
            }
        }
    }
}

void SlotsGameScene::onEnter()
{
    Layer::onEnter();

    PlayBackgroundMusic();
}

void SlotsGameScene::PlayBackgroundMusic()
{
    AudioManager::getInstance().PlayBackgroundMusic(StringUtils::format("music/BackgroundMusic/%s", currentTheme.c_str()));
}

void SlotsGameScene::enableInput(bool enable)
{
    for (int i = 0; i < lineButtons.size(); i++)
    {
        lineButtons[i]->setEnabled(enable);
        lineButtons[i]->setOpacity(enable && i < currentLinesSelected  ? 255 : 127);
    }
    
    topbar->setEnabled(enable);
    
    paytable->setEnabled(enable);
    paytable->setOpacity(enable ? 255 : 127);
    
    spinButton->setEnabled(enable);
    spinButton->setOpacity(enable ? 255 : 127);
    
    maxLinesButton->setEnabled(enable);
    maxLinesButton->setOpacity(enable ? 255 : 127);
}

void SlotsGameScene::onButtonPressed(Ref * sender, ui::Widget::TouchEventType event)
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

void SlotsGameScene::onBackButtonPressed()
{
    auto nextScene = LoadingSplash::createScene(LoadingSplash::SLOTS_THEME);
    Director::getInstance()->replaceScene(nextScene);
}

void SlotsGameScene::ProcessButtonAction(cocos2d::ui::Button * button)
{
    if (button->getName() == "spin")
    {
        startSpin();
    }
    else if (button->getName() == "paytable")
    {
        auto popup = PayoutsView::create(currentTheme, currentLines);
        popup->show(this);
    }
    else if (button->getName() == "max_lines")
    {
        currentLinesSelected = currentLines;
        onBetChanged();
        startSpin();
    }
    else if (button->getName() == "lines_selector")
    {
        currentLinesSelected = button->getTag();
        onBetChanged();
    }
    else if (button->getName() == "lines_minus")
    {
        if (currentLinesSelected > 1)
        {
            currentLinesSelected--;
        }
        onBetChanged();
    }
    else if (button->getName() == "lines_plus")
    {
        if (currentLinesSelected < currentLines)
        {
            currentLinesSelected++;
        }
        onBetChanged();
    }
    else if (button->getName() == "bets_minus")
    {
        auto bets = SlotsConfig::getBetsForLevel(PlayerProfile::getInstance().getLevel(), currentChapter);
        
        if (currentBetIndex > 0)
        {
            currentBetIndex--;
        }
        
        currentBetSelected = bets[currentBetIndex];
        onBetChanged();
    }
    else if (button->getName() == "bets_plus")
    {
        auto bets = SlotsConfig::getBetsForLevel(PlayerProfile::getInstance().getLevel(), currentChapter);
        
        if (currentBetIndex < bets.size() - 1)
        {
            currentBetIndex++;
        }
        
        currentBetSelected = bets[currentBetIndex];
        onBetChanged();
    }
}

void SlotsGameScene::showNotEnoughView()
{
    auto view = Sprite::create("ui/Slots/NotEnoughGold.png");
    view->setOpacity(0);
    view->runAction(Sequence::create(FadeIn::create(0.5f), DelayTime::create(1.0f), RemoveSelf::create(), NULL));
    view->setPosition(getContentSize() / 2);
    
    addChild(view, 100);
}

void SlotsGameScene::startSpin()
{
    //hackyhack_ads
    CCLOG("hackyhack: SlotsGameScene.cpp --> round is finished!");
    spinningCount = (spinningCount + 1) % CYCLE_INTERSTITIAL_SLOTS;
    if (spinningCount == 0) {
        AdmobChartboost::shared()->showInterstitialAdmob();
    } else if (spinningCount == CYCLE_SPIN_RATEME) {
        long hasShown = PlayerProfile::getInstance().getShowRateWhenSpinning(SPIN_SLOTS);
        if (hasShown == 0) {
            PlayerProfile::getInstance().setShowRateWhenSpinning(SPIN_SLOTS, 1);
            RatemeAlert::create(MCONSTANTS_TEXT_RATEME)->show(this);
        }
    }
    
    if (!freeSpinInProgress)
    {
        if (currentTotalBet > PlayerProfile::getInstance().getCoins())
        {
            showNotEnoughView();
            
            autoSpinInProgress = false;
            autoSpinCounter->setVisible(false);
            
            return;
        }
        else
        {
            PlayerProfile::getInstance().statsAddBetAmount(GAME_SLOTS, currentTotalBet);
            PlayerProfile::getInstance().addPiggyBankCoins(PIGGY_BANK_PROCENT * currentTotalBet);
            
            increasePlayerValues(-currentTotalBet, 0, false);
        }
    }
    
    enableInput(false);
    hideWinLines();
    
    freeSpinCounter->setVisible(freeSpinInProgress);
    if (freeSpinInProgress)
    {
        autoSpinCounter->setVisible(false);
        freespinsLabel->setString(StringUtils::format("%d Free Spin%s", currentFreeSpins, currentFreeSpins == 1 ? "" : "s"));
    }
    else
    {
        autoSpinCounter->setVisible(autoSpinInProgress);
        if (autoSpinInProgress)
        {
            autospinsLabel->setString(StringUtils::format("%d Spin%s", autoSpinsRemains, autoSpinsRemains == 1 ? "" : "s"));
        }
    }
    
    SpinTo("random", "random", "random", "random", "random");
}

void SlotsGameScene::autoSpin(int spinsAmount)
{
    autoSpinInProgress = true;
    
    autoSpinsRemains = spinsAmount;
    
    startSpin();
}

void SlotsGameScene::handlePendingCalls()
{
    if (pendingCalls.empty())
    {
        onSpinEnded();
        return;
    }
    
    auto callback = pendingCalls.front();
    pendingCalls.pop_front();
    
    callback();
}

void SlotsGameScene::handleBonus()
{
    bScheduleBonus = true;
    CongratulationsView::create(CongratulationsView::CongratulationType::BONUS, std::bind(&SlotsGameScene::handlePendingCalls, this))->show(this);
}

void SlotsGameScene::handleBigWin()
{
    CongratulationsView::create(CongratulationsView::CongratulationType::BIGWIN, std::bind(&SlotsGameScene::handlePendingCalls, this))->show(this);
}

void SlotsGameScene::handleLevelUp()
{
    CongratulationsView::create(CongratulationsView::CongratulationType::LEVELUP, std::bind(&SlotsGameScene::handlePendingCalls, this))->show(this);
}

void SlotsGameScene::onBetChanged()
{
    currentTotalBet = currentBetSelected * currentLinesSelected;
    
    selectedBetLabel->setString(  StringUtils::format("$%d", currentBetSelected));
    selectedLinesLabel->setString(StringUtils::format("%d" , currentLinesSelected));
    totalBetLabel->setString(StringUtils::format("$%d", currentTotalBet));
    
    updateLinesButtons(currentLinesSelected);
    
    hideWinLines();
    
    lightUpAllWinLinesTo(currentLinesSelected);
}

void SlotsGameScene::showBonusGame()
{
    bScheduleBonus = false;
    bonusViewShown = true;
    
    if(currentTotalWin > 0)
    {
        //hackyhack_publish - arc4random_uniform
//        int random = arc4random_uniform(3);
        int random = rand() % 3;
        
        //hackyhack_test_force
        if (FORCE_SLOTS_BONUS_FREE_SPINS) {
            CCLOG("hackyhack: FORCE_SLOTS_BONUS_FREE_SPINS");
            random = 0;
        } else if (FORCE_SLOTS_BONUS_WHEEL) {
            CCLOG("hackyhack: FORCE_SLOTS_BONUS_WHEEL");
            random = 1;
        } else if (FORCE_SLOTS_BONUS_CHEST) {
            CCLOG("hackyhack: FORCE_SLOTS_BONUS_CHEST");
            random = 2;
        }
        
        switch (random)
        {
            case 0:
            {
                //hackyhack_publish - arc4random_uniform
//                int rnd = arc4random_uniform(5);
                int rnd = rand() % 5;
                int times = 3;
                
                if(rnd < 2)
                {
                    times = 1;
                }
                else if (rnd < 4)
                {
                    times = 2;
                }
                
                currentFreeSpins += times;
                FreeSpinView::create(times, std::bind(&SlotsGameScene::onBonusViewEnded, this, 0))->show(this);
            }
                break;
            case 1:
            {
                WheelView::create(1, currentTotalWin, std::bind(&SlotsGameScene::onBonusViewEnded, this, std::placeholders::_1))->show(this);
            }
                break;
            case 2:
            {
                ChestsView::create(ChestsView::BonusType::BONUS, std::bind(&SlotsGameScene::onBonusViewEnded, this, 0))->show(this);
            }
                break;
                
            default:
                break;
        }
    }
    else
    {
        //hackyhack_publish - arc4random_uniform
//        int random = arc4random_uniform(2);
        int random = rand() % 2;
        
        log("%d", random);
        
        switch (random)
        {
            case 0:
            {
                currentFreeSpins += FREE_SPINS;
                FreeSpinView::create(1, std::bind(&SlotsGameScene::onBonusViewEnded, this, 0))->show(this);
            }
                break;
                
            case 1:
            {
                ChestsView::create(ChestsView::BonusType::BONUS, std::bind(&SlotsGameScene::onBonusViewEnded, this, 0))->show(this);
            }
                break;
                
            default:
                break;
        }
    }
}

void SlotsGameScene::onSpinEnded()
{
    //hackyhack_slots --> onSpinEnded()
    if (bonusViewShown)
        return;
    
    if (winAnimationInProgress)
        return;
    
    if (pendingCalls.empty() == false)
        return;
    
    if (bScheduleBonus)
    {
        showBonusGame();
        return;
    }
    
    //handle freespin
    if (freeSpinInProgress)
    {
        currentFreeSpins--;
        freeSpinInProgress = currentFreeSpins > 0;
        freeSpinCounter->setVisible(freeSpinInProgress);
        autoSpinCounter->setVisible(false);
    }
    
    //auto start freespin
    if (currentFreeSpins > 0)
    {
        freeSpinInProgress = true;
        freeSpinCounter->setVisible(true);
        autoSpinCounter->setVisible(false);
    }
    
    //handle autospin
    if (autoSpinInProgress && !freeSpinInProgress)
    {
        autoSpinsRemains--;
        autoSpinInProgress = autoSpinsRemains > 0;
        autoSpinCounter->setVisible(autoSpinInProgress);
    }
    
    if (autoSpinInProgress || freeSpinInProgress)
    {
        startSpin();
    }
    
    PlayerProfile::getInstance().Save();
    
    enableInput(!autoSpinInProgress && !freeSpinInProgress);
}

void SlotsGameScene::onBonusViewEnded(int result)
{
    if (result > 0)
    {
        increasePlayerValues(result, 0, false);
    }
    
    bonusViewShown = false;
    onSpinEnded();
}

void SlotsGameScene::setWinAmountLabel(int amount, float time)
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

#pragma mark Core Methods

void SlotsGameScene::SpinTo(std::string w1, std::string w2, std::string w3, std::string w4, std::string w5)
{
    if (w1 == "random")
    {
        //hackyhack_publish - arc4random_uniform
//        w1 = SlotsConfig::wheel_items[0][arc4random_uniform( static_cast<unsigned int>(SlotsConfig::wheel_items[0].size()))];
        w1 = SlotsConfig::wheel_items[0][rand() % ( static_cast<unsigned int>(SlotsConfig::wheel_items[0].size()))];
    }
    
    if (w2 == "random")
    {
        //hackyhack_publish - arc4random_uniform
//        w2 = SlotsConfig::wheel_items[1][arc4random_uniform( static_cast<unsigned int>(SlotsConfig::wheel_items[1].size()))];
        w2 = SlotsConfig::wheel_items[1][rand() % ( static_cast<unsigned int>(SlotsConfig::wheel_items[1].size()))];
    }
    
    if (w3 == "random")
    {
        //hackyhack_publish - arc4random_uniform
//        w3 = SlotsConfig::wheel_items[2][arc4random_uniform( static_cast<unsigned int>(SlotsConfig::wheel_items[2].size()))];
        w3 = SlotsConfig::wheel_items[2][rand() % ( static_cast<unsigned int>(SlotsConfig::wheel_items[2].size()))];
    }
    
    if (w4 == "random")
    {
        //hackyhack_publish - arc4random_uniform
//        w4 = SlotsConfig::wheel_items[3][arc4random_uniform( static_cast<unsigned int>(SlotsConfig::wheel_items[3].size()))];
        w4 = SlotsConfig::wheel_items[3][rand() % ( static_cast<unsigned int>(SlotsConfig::wheel_items[3].size()))];
    }
    
    if (w5 == "random")
    {
        //hackyhack_publish - arc4random_uniform
//        w5 = SlotsConfig::wheel_items[4][arc4random_uniform( static_cast<unsigned int>(SlotsConfig::wheel_items[4].size()))];
        w5 = SlotsConfig::wheel_items[4][rand() % ( static_cast<unsigned int>(SlotsConfig::wheel_items[4].size()))];
    }
    
    log("Current pattern: %s, %s, %s, %s, %s", w1.c_str(), w2.c_str(), w3.c_str(), w4.c_str(), w5.c_str());
    
    //randomize wheels positions
    wheelsScrollAmount.resize(5, 0);

    wheelsScrollAmount[0] = SlotsConfig::getRandomPositionOfSymbolOnWheel(w1, 0);
    wheelsScrollAmount[1] = SlotsConfig::getRandomPositionOfSymbolOnWheel(w2, 1);
    wheelsScrollAmount[2] = SlotsConfig::getRandomPositionOfSymbolOnWheel(w3, 2);
    wheelsScrollAmount[3] = SlotsConfig::getRandomPositionOfSymbolOnWheel(w4, 3);
    wheelsScrollAmount[4] = SlotsConfig::getRandomPositionOfSymbolOnWheel(w5, 4);
    
    float prevRotation = 3; // SlotsConfig::wheel_items_amount;
    
    for (int i = 0; i < 5; i++)
    {
        log("Value in: %d", wheelsScrollAmount[i]);
        
        int scrollDelta = wheelsPositions[i] - wheelsScrollAmount[i] - 1;
        
        while (scrollDelta < 0)
        {
            scrollDelta += SlotsConfig::wheel_items_amount;
        }
        
        int prevDelta = prevRotation;
        while (prevDelta > 0)
        {
            prevDelta -= SlotsConfig::wheel_items_amount;
        }
        
        wheelsScrollAmount[i] = scrollDelta + prevRotation - prevDelta;
        
        if (wheelsScrollAmount[i] < prevRotation)
            wheelsScrollAmount[i] += SlotsConfig::wheel_items_amount;
        
        prevRotation = wheelsScrollAmount[i];
    }
    
    wheelScrollLastPositions.resize(5);
    wheelAnimated.resize(5);
    
    for (int i = 0; i < 5; i++)
    {
        wheelScrollLastPositions[i] = wheels[i]->getPositionY();
        wheelAnimated[i] = true;
    }
    
    AudioManager::getInstance().PlayBackgroundMusic("music/SlotsSound/Spining");
    
    wheelSpinning = true;
}

void SlotsGameScene::CheckWinConditions()
{
    int  currentBonus = 0;
    bool bJackpot = false;
    bool isBonus  = false;
    unsigned int jackpot_value = 0;
    
    currentTotalWin = 0;
    
    winLines.clear();
    winLines.reserve(10);
    
    PlayBackgroundMusic();
    
    //build matrix
    std::vector< std::vector< std::string > > winMatrix;
    winMatrix.resize(5);
    
    for (int i = 0; i < 5; i++)
    {
        winMatrix[i].resize(3);
        
        for (int k = 0; k < 3; k++)
        {
            int pos = wheelsPositions[i] - (3 - k);
            winMatrix[i][k] = SlotsConfig::wheel_items[i][pos < 0 ? 24 + pos : pos];
            
            //count bonuses
            if (winMatrix[i][k] == "Bonus")
                currentBonus++;
        }
    }
    
    isBonus = currentBonus >= 3;
    
    for (int i = 0; i < currentLinesSelected; i++)
    {
        std::vector< std::string > currentLine;
        currentLine.resize(5);
        
        for (int k = 0; k < 5; k++)
        {
            currentLine[k] = winMatrix[k][SlotsConfig::conditions[i][k] + 1];
        }
        
        for (int same_items = 5; same_items > 1; same_items--)
        {
            currentLine.resize(same_items);
            std::map< std::string, int > matches;
            
            for (int m = 0; m < same_items; m++)
            {
                matches[currentLine[m]] = m;
            }
            
            if (matches.size() == 1) //all symbols are same
            {
                if (same_items == 5 && matches.find("Diamond") != matches.end()) //jackpot
                {
                    if (!bJackpot) //only one jackpot is available!
                    {
                        log("Win: *** JACKPOT ***");
                        jackpot_value   = PlayerProfile::getInstance().grabSlotsJackpot();
                        currentTotalWin += jackpot_value;
                        bJackpot = true;
                        
                        winLines.push_back(std::make_pair(i, same_items) );
                    }
                }
                else // all items match
                {
                    log("Win: *** %d items line ***", same_items);
                    auto payout = SlotsConfig::payouts[same_items - 2][SlotsConfig::itemNameToID(matches.begin()->first)];
                    currentTotalWin += payout;
                    
                    if (payout > 0)
                        winLines.push_back(std::make_pair(i, same_items) );
                }
                break; //do not go deeper
            }
            else if (matches.size() == 2 && matches.find("Wild") != matches.end()) // all items match with universal items
            {
                matches.erase(matches.find("Wild"));
                
                log("Win: *** %d items line (with Wilds) ***", same_items);
                auto payout = SlotsConfig::payouts[same_items - 2][SlotsConfig::itemNameToID(matches.begin()->first)];
                currentTotalWin += payout;
                
                if (payout > 0)
                    winLines.push_back(std::make_pair(i, same_items) );
                
                break; //do not go deeper
            }
        }
    }
    
    // calculate win amount
    currentTotalWin = currentTotalWin * currentBetSelected;
    setWinAmountLabel(currentTotalWin);
    
    if (currentTotalWin > PlayerProfile::getInstance().getLongValue("MaxWin", 0))
    {
        PlayerProfile::getInstance().setLongValue("MaxWin", currentTotalWin);
    }
    
    // win
    bool won = false;
    
    if (currentTotalWin > 0 && winLines.empty() == false)
    {
        log("Total Win Amount: %d", currentTotalWin);
        won = true;
        
        PlayerProfile::getInstance().incrementWonCounter();
        PlayerProfile::getInstance().statsAddWinAmount(GAME_SLOTS, currentTotalWin);
        
        lightUpWinConditions();
        
        CoinsRainView::create((float)currentTotalWin / currentTotalBet)->show(this);
    }
    
    // jackpot
    if (bJackpot)
    {
        auto item = Sprite::create("ui/Slots/jackpots_big.png");
        item->setPosition(jackpotPanel->getPosition());
        item->setCascadeOpacityEnabled(true);
        item->setScale(jackpotPanel->getContentSize().width / item->getContentSize().width);
        
        auto label = Label::createWithTTF(StringUtils::format("$%d", jackpot_value), "fonts/Berlin.ttf", 80.0f);
        label->setPosition(item->getContentSize().width / 2, item->getContentSize().height * 0.4f);
        label->setTextColor(Color4B::BLACK);
        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        item->addChild(label);
        
        item->runAction(Sequence::create(Spawn::createWithTwoActions(EaseSineIn::create(ScaleTo::create(0.3f, 1.0f)),
                                                                     EaseSineIn::create(MoveTo::create(0.3f, Vec2(getContentSize().width / 2, getContentSize().height * 0.75f)))),
                                         DelayTime::create(2.0f), FadeOut::create(0.3f), RemoveSelf::create(), NULL));
        
        addChild(item, 101);
    }
    
    // 2x bet amount is the "BIG WIN"!
    if (currentTotalWin >= 2.0f * currentTotalBet)
    {
        pendingCalls.push_back(std::bind(&SlotsGameScene::handleBigWin, this));
    }
    
    //experience
    unsigned int XP = PlayerProfile::getInstance().getExperienceForCoins(currentTotalWin);
    bool isLevelUp = PlayerProfile::getInstance().isExperienceEnoughForLevelUp(XP);
    
    //level up
    //hackyhack_bonus_level
    if (isLevelUp)
    {
        auto currLevel = PlayerProfile::getInstance().getLevel();
        
        currentTotalWin += (currLevel + 1) * PlayerProfile::getInstance().getLevelUpBonus();
        pendingCalls.push_back(std::bind(&SlotsGameScene::handleLevelUp, this));
    }
    
    // give coins, experience and level
    increasePlayerValues(currentTotalWin, XP, isLevelUp);

    //hackyhack_test_force
    if (FORCE_SLOTS_BONUS_FREE_SPINS || FORCE_SLOTS_BONUS_CHEST || FORCE_SLOTS_BONUS_WHEEL) {
        isBonus = true;
    }

    if (isBonus)
    {
        pendingCalls.push_back(std::bind(&SlotsGameScene::handleBonus, this));
    }
    
    handlePendingCalls();
}

#pragma mark WinBoxes

void SlotsGameScene::initWinBoxes(cocos2d::Node * parent)
{
    winBoxes.clear();
    parent->removeAllChildren();
    
    winBoxes.resize(5);
    
    for (int x = 0; x < 5; x++)
    {
        winBoxes[x].resize(3);
        
        for (int y = 0; y < 3; y++)
        {
            winBoxes[x][y] = Sprite::create("ui/Slots/win_box.png");
            winBoxes[x][y]->setPosition(Vec2((x - 2) * SlotsConfig::wheel_items_width, (y - 1) * SlotsConfig::wheel_items_height));
            winBoxes[x][y]->setVisible(false);
            parent->addChild(winBoxes[x][y]);
        }
    }
}

void SlotsGameScene::lightUpWinBoxes(int condition, cocos2d::Color3B color, int values /* = 5*/)
{
    auto boxes = SlotsConfig::conditions[condition];
    
    for (int x = 0; x < 5; x++)
    {
        for (int y = 0; y < 3; y++)
        {
            bool show = (1 - boxes[x] == y) && (x < values);
            winBoxes[x][y]->setVisible(show);
            winBoxes[x][y]->setColor(color);
        }
    }
    
    hideWinLines();
    lightUpWinLines(condition, color, false);
}

void SlotsGameScene::hideWinBoxes()
{
    for (int x = 0; x < 5; x++)
    {
        for (int y = 0; y < 3; y++)
        {
            winBoxes[x][y]->setVisible(false);
        }
    }
    
    hideWinLines();
}

#pragma mark WinLines

ui::Scale9Sprite * SlotsGameScene::createWinLine(Vec2 from, Vec2 to, Color3B color)
{
    auto  proto = Sprite::create("ui/Slots/win_line.png");
    float marginX = proto->getContentSize().height * 0.5f;
    
    auto line = ui::Scale9Sprite::create(Rect(marginX, proto->getContentSize().height / 2, proto->getContentSize().width - marginX * 2, 1), "ui/Slots/win_line.png");

    //scale
    float distance = from.distance(to);
    line->setPreferredSize(Size(distance + marginX * 2, proto->getContentSize().height));
    
    //rotation
    float angle = MATH_RAD_TO_DEG( (to - from).getAngle() );
    line->setRotation(-angle);
    
    line->setColor(color);
    line->setAnchorPoint(Vec2( marginX / line->getContentSize().width, 0.5f));
    line->setPosition(from);
    
    return line;
}

void SlotsGameScene::lightUpWinLines(int condition, cocos2d::Color3B color, bool withHandles /*= true*/)
{
    auto boxes   = SlotsConfig::conditions[condition];
    auto config  = SlotsConfig::linebuttons[condition];
    
    bool right   = std::get<2>(config);
    bool shifted = std::get<1>(config);
    
    auto pos     = getLineButtonPosition(condition);
    
    int straightStart = 1;
    int straightEnd   = 1;
    
    std::pair< Vec2, Vec2> startSegment;
    std::pair< Vec2, Vec2> endSegment;
    
    //invert Y of boxes
    for (int i = 0; i < 5; i++)
        boxes[i] = - boxes[i];
    
    //detect straight lines
    for (int i = 1; i < 5; i++)
    {
        if (boxes[0] != boxes[i])
            break;
        
        ++straightStart;
    }
    
    for (int i = 1; i < 5; i++)
    {
        if (boxes[4] != boxes[4 - i])
            break;
        
        ++straightEnd;
    }
    
    float shift  = pos.y - (right ? (boxes[4] * SlotsConfig::wheel_items_height) : (boxes[0] * SlotsConfig::wheel_items_height));
    float endShift = shift;
    
    if (right) // no shift for non-straight right-aligment lines
    {
        bool straight = boxes[3] == boxes[4];
        if (!straight)
            shift = 0;
        
        shift += 10;
    }
    
    //no shift if without handles
    if (!withHandles)
    {
        shift = endShift = 0;
    }
    
    // start segment
    {
        float handleShift  = (withHandles && !right ? -0.35f : 0);
        float handleShiftR = (withHandles &&  right ?  0.35f : 0) + 0.5f;
        
        Vec2 startPoint = Vec2(( handleShift - 0.5f + (shifted && !right ? -0.5f : 0) - 2) * SlotsConfig::wheel_items_width,
                               boxes[0] * SlotsConfig::wheel_items_height + shift);
        
        Vec2 endPoint   = Vec2( (( straightStart == 5 ? handleShiftR : 0 ) + (straightStart - 1 - 2)) * SlotsConfig::wheel_items_width,
                               boxes[0] * SlotsConfig::wheel_items_height + shift);
        
        startSegment.first  = startPoint;
        startSegment.second = endPoint;
    }
    
    // end segment
    if (straightStart < 5)
    {
        int m = 5 - straightEnd;
        float handleShift = (withHandles && right ? 0.35f : 0);
        
        Vec2 startPoint = Vec2( (m - 2) * SlotsConfig::wheel_items_width,
                               boxes[m] * SlotsConfig::wheel_items_height + endShift);
        
        Vec2 endPoint   = Vec2(( handleShift + 0.5f + (withHandles && shifted && right ? 0.5f : 0) + 4 - 2) * SlotsConfig::wheel_items_width,
                               boxes[4] * SlotsConfig::wheel_items_height + endShift);
        
        endSegment.first  = startPoint;
        endSegment.second = endPoint;
    }
    
    //middle
    for (int x = straightStart; x <= 5 - straightEnd; x++)
    {
        // regular segment
        Vec2 startPoint = Vec2((x - 1 - 2) * SlotsConfig::wheel_items_width, boxes[x - 1] * SlotsConfig::wheel_items_height + shift);
        Vec2 endPoint   = Vec2((x     - 2) * SlotsConfig::wheel_items_width, boxes[x    ] * SlotsConfig::wheel_items_height + shift);
        
        //extending
        if (x == straightStart)
        {
            startPoint = startPoint + Vec2( -SlotsConfig::wheel_items_width, (boxes[x - 1] - boxes[x]) * SlotsConfig::wheel_items_height );
            startSegment.second += Vec2(SlotsConfig::wheel_items_width, 0);
        }
        if (x == 5 - straightEnd)
        {
            endPoint = endPoint + Vec2( SlotsConfig::wheel_items_width, (boxes[x] - boxes[x - 1]) * SlotsConfig::wheel_items_height  );
            endSegment.first += Vec2(-SlotsConfig::wheel_items_width, 0);
        }
        
        //cross
        if (x == straightStart)
        {
            Vec2 intersect = Vec2::getIntersectPoint(startPoint, endPoint, startSegment.first, startSegment.second);
            startSegment.second = intersect;
            startPoint = intersect;
        }
        if (x == 5 - straightEnd)
        {
            Vec2 intersect = Vec2::getIntersectPoint(startPoint, endPoint, endSegment.first, endSegment.second);
            endSegment.first = intersect;
            endPoint = intersect;
        }
        
        auto line = createWinLine(startPoint, endPoint, color);
        
        winLinesPanel->addChild(line);
    }
    
    //create start and end lines
    auto startline = createWinLine(startSegment.first, startSegment.second, color);
    winLinesPanel->addChild(startline);
    
    if (straightStart < 5)
    {
        auto endline = createWinLine(endSegment.first, endSegment.second, color);
        winLinesPanel->addChild(endline);
    }
}

void SlotsGameScene::hideWinLines()
{
    winLinesPanel->removeAllChildren();
}

void SlotsGameScene::lightUpAllWinLinesTo(int endCondition)
{
    hideWinLines();

    for (int i = 0; i < endCondition; i++)
    {
        auto rgb = SlotsConfig::lines_colors[i];
        Color3B color(rgb[0], rgb[1], rgb[2]);
        lightUpWinLines(i, color);
    }
}

void SlotsGameScene::lightUpWinConditions(int index /*= 0*/)
{
    winAnimationInProgress = true;
    
    hideWinLines();
    
    if (index >= winLines.size())
    {
        winAnimationInProgress = false;
        onSpinEnded();
        
        return;
    }
    
    winBoxesPanel->stopAllActions();
    winBoxesPanel->runAction(Sequence::createWithTwoActions( Repeat::create(Sequence::create(CallFunc::create([this, index]()
                                                                                                              {
                                                                                                                  auto item = winLines[index];
                                                                                                                  auto rgb = SlotsConfig::lines_colors[item.first];
                                                                                                                  Color3B color(rgb[0], rgb[1], rgb[2]);
                                                                                                                  this->lightUpWinBoxes(item.first, color, item.second);
                                                                                                                  
                                                                                                                  std::string effect = "music/SlotsSound/TheLines";
                                                                                                                  if (item.second > 3)
                                                                                                                      effect = StringUtils::format("music/SlotsSound/%s_%d", currentTheme.c_str(), item.second);
                                                                                                                  
                                                                                                                  AudioManager::getInstance().PlaySoundEffect(effect);
                                                                                                              }),
                                                                                             DelayTime::create(0.5f),
                                                                                             CallFunc::create(std::bind(&SlotsGameScene::hideWinBoxes, this)),
                                                                                             DelayTime::create(0.5f),
                                                                                             NULL), winLines.size() > 1 ? 1 : 2), CallFunc::create([this, index]()
                                                                                                                         {
                                                                                                                             this->lightUpWinConditions(index + 1);
                                                                                                                         })));
}

#pragma mark LinesButtons

void SlotsGameScene::initLinesButtons()
{
    lineButtons.clear();
    lineButtons.resize(currentLines);
    lineButtonsPanel->removeAllChildren();
    
    for (int i = 0; i < currentLines; i++)
    {
        std::string path = StringUtils::format("ui/Slots/%s/line_buttons/line_button_%d.png", currentTheme.c_str(), i + 1);
        auto item = ui::Button::create(path, path, path);
        item->setZoomScale(0);
        item->setName("lines_selector");
        item->setTag(i + 1);
        item->addTouchEventListener(std::bind(&SlotsGameScene::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
        item->setOpacity(127);
        item->setPosition(getLineButtonPosition(i));

        int order = std::get<0>( SlotsConfig::linebuttons[i] );
        lineButtonsPanel->addChild(item, currentLines / 2 - order);
        lineButtons[i] = item;
        
        auto label = Label::createWithTTF(StringUtils::format("%d", i + 1), "fonts/Berlin.ttf", 24.0f);
        label->setColor(Color3B::BLACK);
        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        label->setPosition(Vec2(item->getContentSize().width * 0.5f, item->getContentSize().height * 0.5f));
        item->addChild(label);
    }
}

Vec2 SlotsGameScene::getLineButtonPosition(int condition)
{
    auto config  = SlotsConfig::linebuttons[condition];
    int  order   = std::get<0>(config);
    bool shifted = std::get<1>(config);
    bool right   = std::get<2>(config);
    int  step    = (currentLines == 20 ? 44 : 30);
    
    return Vec2(440.0f * (right ? 1 : -1) + (right ? 1 : -1) * (shifted ? 1 : 0) * 45.0f, -0.25f * ( currentLines - 1 ) * step + (order - 1) * step);
}

void SlotsGameScene::updateLinesButtons(int target)
{
    for (int i = 0; i < lineButtons.size(); i++)
    {
        lineButtons[i]->setOpacity(i >= target ? 127 : 255);
    }
}

#pragma mark - Wheels

void SlotsGameScene::createWheel(int wheel, cocos2d::ui::Layout * parent)
{
    const float itemSize = SlotsConfig::wheel_items_height;
    
    parent->removeAllChildren();
    parent->setClippingEnabled(true);
    parent->setClippingType(cocos2d::ui::Layout::ClippingType::STENCIL);
    parent->setLayoutType(cocos2d::ui::Layout::Type::ABSOLUTE);
    
    wheels[wheel] = Node::create();
    parent->addChild(wheels[wheel]);
    
    wheels[wheel]->setContentSize(Size(itemSize, itemSize * (SlotsConfig::wheel_items_amount + 3)));
    wheels[wheel]->setAnchorPoint(Vec2::ZERO);
    
    float lastBGHeight = wheels[wheel]->getContentSize().height;
    
    //background
    do
    {
        auto item = Sprite::create(StringUtils::format("ui/Slots/%s/column.png", currentTheme.c_str()));
        item->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        item->setPosition(Vec2(0, lastBGHeight));
        wheels[wheel]->addChild(item);
        
        lastBGHeight -= item->getContentSize().height;
    } while (lastBGHeight > 0);
    
    for (int i = 0; i < SlotsConfig::wheel_items[wheel].size() + 3; i++)
    {
        int m = (i - 3);
        if (m < 0)
            m += SlotsConfig::wheel_items_amount;
        
        m = m % SlotsConfig::wheel_items[wheel].size();
        auto item = Sprite::create(StringUtils::format("ui/Slots/%s/items/%s.png", currentTheme.c_str(), SlotsConfig::wheel_items[wheel][m].c_str() ));
        
        item->setPosition(Vec2(wheels[wheel]->getContentSize().width / 2, wheels[wheel]->getContentSize().height - (0.5f + i) * itemSize));
        wheels[wheel]->addChild(item);
    }
}

#pragma mark - Debug Methods

void SlotsGameScene::DebugShowAllWinConditions(int condition /* = 0*/)
{
    if (condition >= currentLines)
        return;
    
    winBoxesPanel->stopAllActions();
    winBoxesPanel->runAction(Sequence::createWithTwoActions( Repeat::create(Sequence::create(CallFunc::create([this, condition]()
    {
        Color3B color = Color3B(SlotsConfig::lines_colors[condition][0], SlotsConfig::lines_colors[condition][1], SlotsConfig::lines_colors[condition][2]);
        this->lightUpWinBoxes(condition, color);
    }),
                                              DelayTime::create(0.5f),
                                              CallFunc::create(std::bind(&SlotsGameScene::hideWinBoxes, this)),
                                              DelayTime::create(0.5f),
                                                                                             NULL), 3), CallFunc::create([this, condition]()
    {
        this->DebugShowAllWinConditions(condition + 1);
    })));
}
