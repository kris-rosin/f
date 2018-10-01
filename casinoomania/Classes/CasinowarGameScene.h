#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "BaseScene.h"
#include "CoinToken.h"
#include "CasinowarConfig.h"
#include "CasinowarDeck.h"
#include "CasinowarHand.h"
#include "TopBar.h"

#include <map>

class CasinowarGameScene : public BaseScene
{
private:
    enum CasinowarBetType
    {
        BET,
        RAISE,
        TIE
    };
    
    enum CasinowarResultType
    {
        WIN,
        LOSS,
        SURRENDER,
        WIN_TIE
    };
    
    struct CasinowarBet
    {
        CasinowarBetType type;
        float payout;
        cocos2d::Rect area;
        
        CoinToken * token;
        
        CasinowarBet() : token(nullptr) {};
        CasinowarBet(CasinowarBetType type, cocos2d::Rect area, float payout) : type(type), area(area), payout(payout), token(nullptr)
        {
        }
    };
private:
    cocos2d::ui::Button * rebetButton;
    cocos2d::ui::Button * clearButton;
    cocos2d::ui::Button * warButton;
    cocos2d::ui::Button * surrenderButton;
    cocos2d::Node       * controlsNode;
    cocos2d::Node       * betPanel;
    cocos2d::Sprite     * background;
    
    cocos2d::Node       * historyTab;
    cocos2d::Node       * historyRoot;
    
    cocos2d::Vector<CoinToken*> betButtons;
    cocos2d::Vector<cocos2d::ui::Button*> actionButtons;
    
    CasinowarHand * player;
    CasinowarHand * dealer;
    CasinowarDeck * deck;
    
    CasinowarBet raiseBet;
    CasinowarBet normalBet;
    CasinowarBet tieBet;
    
    TopBar * topbar;
    
    std::map<CasinowarBetType, int> previousBets;
    std::list<CasinowarResultType> resultsHistory;
    
    int  totalWin;
    bool warStarted;
    bool gameStarted;
    
    std::string currentTheme;
    int   currentChapter;
    bool  historyOpened;
private:
    CasinowarGameScene();
 
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
    
    static CasinowarGameScene * create(std::string theme);
    
    // General Logic
    void newRound();
    void playRound();
    void startWar();
    void checkWinOrLose(int playerValue, int bankervalue);
    void cleanTable(CasinowarResultType result);
};
