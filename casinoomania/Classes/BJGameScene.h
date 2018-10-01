#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "BaseScene.h"
#include "BJConfig.h"
#include "BJPlayer.h"
#include "Shoe.h"
#include "CoinToken.h"
#include "TopBar.h"

#include <list>

class BJGameScene : public BaseScene
{
    enum BlackJackGameStates
    {
        NOT_STARTED = 0,
        DEAL_CARDS,
        ASK_INSURANCE,
        DEALER_CHECK_BLACKJACK,
        ROUND
    };
private:
    cocos2d::Node   * controlsNode;
    cocos2d::Node   * betPanel;
    cocos2d::Sprite * handSelector;
    
    cocos2d::Vector<CoinToken*> betButtons;
    cocos2d::Vector<BJPlayer*>  players;
    cocos2d::Vector<HandView*>  playingHands;
    
    int currentHand;
    BJPlayer * currentPlayer;
    Shoe     * shoe;
    TopBar   * topbar;
    
    BlackJackGameStates currentState;
    
    std::string currentTheme;
    int currentChapter;
    int previousBet;
private:
    BJGameScene();
 
    void onButtonPressed(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType event);
    void ProcessButtonAction(cocos2d::ui::Button * button);
    void enableInput(bool enable);
    
    void bustHand(HandView * hand);
    void surrenderHand(HandView * hand);
    void equalmoneyHand(HandView * hand);
    void doubleHand(HandView * hand);
    
    //service
    void PlayBackgroundMusic();
    bool onBetButtonPressed(CoinToken::CoinTokenEvent event, CoinToken * token);
    void showPlaceBet();
    void showNotEnoughView();
    void selectCurrentHand();
    
    void onBackButtonPressed();
public:
    static cocos2d::Scene * createScene(std::string theme);

    virtual bool init(std::string theme);
    virtual void onEnter();
    
    static BJGameScene * create(std::string theme);
    
    void ShowPossibleActions();
    void cleanTable();
    
    // General Logic
    void startNewGame();
    void newRound();
    
    void dealStartCards();
    void playDealer();
    
    void checkAndPay(HandView * hand);
    
    void onStateChanged(float deltaTime, BlackJackGameStates state);
    void onRoundFinished();
    
    void nextHand();
    
    std::list<BlackJackActions> getPossibleActions();
    
    BJPlayer * getDealer();
    
    bool isAllBust();
    bool isAllBlackJack();
};
