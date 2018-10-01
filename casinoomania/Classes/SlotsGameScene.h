#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "BaseScene.h"
#include "JackpotPanel.h"
#include "TopBar.h"
#include "ServerMessage.h"

class SlotsGameScene : public BaseScene
{
private:
    cocos2d::ui::Button * paytable;
    cocos2d::ui::Button * maxLinesButton;
    cocos2d::ui::Button * spinButton;
    
    cocos2d::Label * totalBetLabel;
    cocos2d::Label * totalWinLabel;
    cocos2d::Label * selectedLinesLabel;
    cocos2d::Label * selectedBetLabel;
    cocos2d::Label * autospinsLabel;
    cocos2d::Label * freespinsLabel;
    
    cocos2d::ui::Scale9Sprite * controlsPanel;
    
    cocos2d::Node  * winBoxesPanel;
    cocos2d::Node  * winLinesPanel;
    cocos2d::Node  * lineButtonsPanel;
    cocos2d::Node  * autoSpinCounter;
    cocos2d::Node  * freeSpinCounter;
    
    TopBar * topbar;
    
    JackpotPanel * jackpotPanel;
    
    std::vector< std::vector<cocos2d::Sprite*> > winBoxes;
    std::vector< cocos2d::ui::Button*> lineButtons;
    std::vector< cocos2d::Node* > wheels;
    std::vector< int > wheelsPositions;
    std::vector< int > wheelsScrollAmount;
    std::vector< float > wheelScrollLastPositions;
    std::vector< bool > wheelAnimated;
    std::vector< std::pair<int, int > > winLines; // array of pairs: condition ID, matched conditions
    
    std::list< std::function<void()> > pendingCalls;
    
    std::string currentTheme;
    int currentChapter;
    int currentLines;
    
    int currentBetSelected;
    int currentBetIndex;
    int currentLinesSelected;
    int currentTotalBet;
    int currentTotalWin;
    
    int autoSpinsRemains;
    int currentFreeSpins;
    
    bool wheelSpinning; // ?
    bool autoSpinInProgress;
    bool freeSpinInProgress;
    bool winAnimationInProgress;
    bool bScheduleBonus;
    bool bonusViewShown;
private:
    SlotsGameScene();
    
    void initWinBoxes(cocos2d::Node * parent);
    void lightUpWinBoxes(int condition, cocos2d::Color3B color, int values = 5);
    void hideWinBoxes();
    
    cocos2d::ui::Scale9Sprite * createWinLine(cocos2d::Vec2 from, cocos2d::Vec2 to, cocos2d::Color3B color);
    void lightUpWinLines(int condition, cocos2d::Color3B color, bool withHandles = true);
    void lightUpAllWinLinesTo(int endCondition);
    void lightUpWinConditions(int index = 0);
    void hideWinLines();
    
    void onButtonPressed(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType event);
    void ProcessButtonAction(cocos2d::ui::Button * button);
    void enableInput(bool enable);
    
    void initLinesButtons();
    void updateLinesButtons(int target);
    cocos2d::Vec2 getLineButtonPosition(int condition);
    
    void createWheel(int wheel, cocos2d::ui::Layout* parent);
    void updateWheels(float deltaTime);
    void onBetChanged();
    void onSpinEnded();
    void onBonusViewEnded(int result);
    void onBackButtonPressed();
    
    //main
    void startSpin();
    void autoSpin(int spinsAmount);
    void SpinTo(std::string w1, std::string w2, std::string w3, std::string w4, std::string w5);
    void CheckWinConditions();
    void handleLevelUp();
    void handleBigWin();
    void handleBonus();
    void handlePendingCalls();
    void showBonusGame();
    void showNotEnoughView();
    
    //service
    void PlayBackgroundMusic();
    void setWinAmountLabel(int amount, float time = 0);
    
    //debug methods
    void DebugShowAllWinConditions(int condition = 0);
public:
    static cocos2d::Scene * createScene(std::string theme, int lines);

    virtual bool init(std::string theme, int lines);
    virtual void update(float deltaTime);
    virtual void onEnter();
    
    static SlotsGameScene * create(std::string theme, int lines);
};
