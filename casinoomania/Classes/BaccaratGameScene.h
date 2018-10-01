#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "BaseScene.h"
#include "CoinToken.h"
#include "BaccaratConfig.h"
#include "BaccaratDeck.h"
#include "BaccaratHand.h"
#include "TopBar.h"

#include <map>

class BaccaratGameScene : public BaseScene
{
private:
    enum BaccaratBetType
    {
        PLAYER,
        BANK,
        TIE
    };
    
    struct BaccaratBet
    {
        BaccaratBetType type;
        float payout;
        cocos2d::Rect area;
        
        CoinToken * token;
        
        BaccaratBet(BaccaratBetType type, cocos2d::Rect area, float payout) : type(type), area(area), payout(payout), token(nullptr)
        {
        }
    };
private:
    cocos2d::ui::Button * rebetButton;
    cocos2d::ui::Button * clearButton;
    cocos2d::Node       * controlsNode;
    cocos2d::Node       * betPanel;
    cocos2d::Sprite     * background;
    
    cocos2d::Node       * historyTab;
    cocos2d::Node       * historyRoot;
    
    cocos2d::Vector<CoinToken*> betButtons;
    cocos2d::Vector<cocos2d::ui::Button*> actionButtons;
    
    BaccaratHand * player;
    BaccaratHand * dealer;
    BaccaratDeck * deck;
    
    TopBar * topbar;
    
    std::map<BaccaratBetType, BaccaratBet> currentBets;
    std::map<BaccaratBetType, int> previousBets;
    std::list<BaccaratBetType> resultsHistory;
    
    std::string currentTheme;
    int   currentChapter;
    bool  historyOpened;
    bool  gameStarted;
private:
    BaccaratGameScene();
 
    void onButtonPressed(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType event);
    void ProcessButtonAction(cocos2d::ui::Button * button);
    void enableInput(bool enable);
    
    //service
    void PlayBackgroundMusic();
    void showPlaceBet();
    void showNotEnoughView();
    void rebuildHistory();
    
    //callbacks
    bool onTokenAction(CoinToken::CoinTokenEvent event, CoinToken * token);
    void onBackButtonPressed();
public:
    static cocos2d::Scene * createScene(std::string theme);

    virtual bool init(std::string theme);
    virtual void onEnter();
    
    static BaccaratGameScene * create(std::string theme);
    
    void cleanTable(BaccaratBetType result);
    
    // General Logic
    void newRound();
    void playRound();
    void checkWinOrLose(int playerValue, int bankervalue);
};
