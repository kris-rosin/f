#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "BaseScene.h"
#include "CoinToken.h"
#include "RouletteConfig.h"

#include "TopBar.h"

#include <map>

class RouletteGameScene : public BaseScene
{
private:
    struct HistoryItem
    {
        int number;
        RouletteConfig::NumberColor color;
        
        HistoryItem(int number, RouletteConfig::NumberColor color) : number(number), color(color) {};
    };
private:
    std::map< int, CoinToken * > currentBets;
    
    std::vector< CoinToken * > betButtons;
    
    std::list<HistoryItem> resultsHistory;
    
    cocos2d::ui::Button * spinButton;
    cocos2d::ui::Button * clearButton;
    cocos2d::ui::Button * winrateButton;
    
    cocos2d::Node  * historyRoot;
    cocos2d::Node  * wheelNode;
    cocos2d::Node  * tableNode;
    cocos2d::Node  * wheel;
    cocos2d::Node  * ball;
    cocos2d::Node  * table;
    cocos2d::Node  * historyTab;
    cocos2d::Node  * dolly;
    
    cocos2d::Label * totalWinLabel;
    cocos2d::Label * totalBetLabel;
    cocos2d::Label * rateLabel;
    
    cocos2d::ui::Scale9Sprite * controlsPanel;
    
    TopBar * topbar;
    
    float betModeTablePosition;
    float spinModeTablePosition;
    
    std::string currentTheme;
    int currentChapter;
    
    int currentBetValue;
    int lastWinSector;
    int lastWinNumber;
    int lastWinCoins;
    int lastBetCoins;

    int ballRollingSound;
    
    //
    float spinSpeed;
    float ballSpeed;
    float ballRotation;
    float ballDistance;
    bool  wheelSpinning;
    bool  ballKnocked;
    bool  ballStopped;
    bool  historyOpened;
    
    static const float ballStartDistance;
    static const float ballEndDistance;
    static const float ballDeflectorDistance;
private:
    RouletteGameScene();
 
    void onButtonPressed(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType event);
    void ProcessButtonAction(cocos2d::ui::Button * button);
    void enableInput(bool enable);
    
    void onBetChanged();
    void onSpinEnded();
    
    //main
    void beforeSpin();
    void startSpin();
    void CheckWinConditions();
    void showNotEnoughView();
    void showPlaceBet();
    void showRates();
    void showResults(int winNumber, int currentBet, int currentWin);
    void clearBets();
    void moveTableToBet();
    void moveTableToSpin();
    
    //service
    void PlayBackgroundMusic();
    void setWinAmountLabel(int amount, float time = 0);
    void UpdateRate();
    void rebuildHistory();
    
    //callbacks
    bool onTokenAction(CoinToken::CoinTokenEvent event, CoinToken * token);
    void onBackButtonPressed();
    
    //debug
    void placeAllBets();
public:
    static cocos2d::Scene * createScene(std::string theme);

    virtual bool init(std::string theme);
    virtual void update(float deltaTime);
    virtual void onEnter();
    
    static RouletteGameScene * create(std::string theme);
};
