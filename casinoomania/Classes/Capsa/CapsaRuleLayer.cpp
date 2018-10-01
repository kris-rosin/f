//
//  CapsaRuleLayer.cpp
//  DovemobiCasino
//
//  Created by Frank on 11/8/17.
//
//

#include "CapsaRuleLayer.hpp"

#include "CapsaDefine.h"
#include "TopBar.h"

USING_NS_CC;

#define N_LISTNAME_COUNT  16
static const char* g_szListNames[] =
{
    "Straight Flush", "Four of a kind", "Card Big 2 Left", "1 Card Left",
    "2 Card Left", "3 Card Left", "4 Card Left", "5 Card Left",
    "6 Card Left", "7 Card Left", "8 Card Left", "9 Card Left",
    "10 Card Left", "11 Card Left", "12 Card Left", "13 Card Left"
};

static const char* g_szFontName = "fonts/arial.ttf";

Scene * CapsaRuleLayer::createScene()
{
    auto scene = Scene::create();
    
    auto layer = CapsaRuleLayer::create();
    scene->addChild(layer);
    
    return scene;
}

CapsaRuleLayer::CapsaRuleLayer()
{
    
}

CapsaRuleLayer::~CapsaRuleLayer()
{
    
}

bool CapsaRuleLayer::init()
{
    if (!Layer::init())
        return false;
    
    auto winSize = Director::getInstance()->getWinSize();
    auto visSize = Director::getInstance()->getVisibleSize();
    
    m_rScaleX = visSize.width/1336;
    m_rScaleY = visSize.height/768;
    
    auto spBack = Sprite::create("ui/Capsa/rules/capsa_rule_bk.png");
    spBack->setPosition(winSize * 0.5f);
    this->addChild(spBack);
    
    auto spTitle = Sprite::create("ui/Capsa/rules/capsa_rule_title.png");
    spTitle->setPosition(Vec2(winSize.width * 0.62f, winSize.height * 0.77f));
    this->addChild(spTitle);
    
    static const char* szConfirmFile = "ui/Capsa/rules/capsa_rule_btn_confirm.png";
    m_btConfirm = MenuItemImage::create(szConfirmFile, szConfirmFile,
                            CC_CALLBACK_1(CapsaRuleLayer::onClickConfirm, this));
    m_btConfirm->setPosition(winSize.width * 0.62f, winSize.height * 0.11f);
    
    auto menu = Menu::create(m_btConfirm, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    auto spMask = Sprite::create("ui/Capsa/rules/capsa_rule_scrollboard.png");
    spMask->setPosition(winSize.width * 0.62f, winSize.height * 0.44f);
    this->addChild(spMask);
    
    auto spListName = Sprite::create("ui/Capsa/rules/capsa_rule_name.png");
    spListName->setPosition(winSize.width * 0.41f, winSize.height * 0.65f);
    this->addChild(spListName);
    
    auto spListPoint = Sprite::create("ui/Capsa/rules/capsa_rule_point.png");
    spListPoint->setPosition(winSize.width * 0.82f, winSize.height * 0.65f);
    this->addChild(spListPoint);
    
    //topbar
    auto topbar = TopBar::create(std::bind(&CapsaRuleLayer::onBack, this));
    topbar->setPosition(Size(WIN_SIZE.width / 2, WIN_SIZE.height));
    topbar->setScaleX(1 / m_rScaleX);
    topbar->setScaleY(1 / m_rScaleY);
    addChild(topbar, 90);
    
    // scroll view
    
    Size contentSize(spMask->getContentSize().width - 40,
                     spMask->getContentSize().height - 100);

    m_pScrollView = ui::ScrollView::create();
    m_pScrollView->setContentSize(contentSize);
    m_pScrollView->setPosition(Vec2(spMask->getPosition().x - contentSize.width/2,
                                 spMask->getPosition().y - contentSize.height/2 - 20));
    m_pScrollView->setScrollBarWidth(4);
    m_pScrollView->setScrollBarPositionFromCorner(Vec2(2, 2));
    m_pScrollView->setScrollBarColor(Color3B::GRAY);
    m_pScrollView->setGlobalZOrder(0);
    
    this->addChild(m_pScrollView, 1);
    
    m_pScrollView->setInnerContainerSize(Size(contentSize.width, 50*(N_LISTNAME_COUNT + 1)));
    m_pScrollView->scrollToTop(2.f, true);
    
    drawScrollViewContents();
    
    this->setScaleX(m_rScaleX);
    this->setScaleY(m_rScaleY);
    
    return true;
}

void CapsaRuleLayer::onBack()
{
    removeFromParentAndCleanup(true);
}

void CapsaRuleLayer::onClickConfirm(cocos2d::Ref *sender)
{
    
    
}


void CapsaRuleLayer::drawScrollViewContents()
{
    static const char* szPercent       = "ui/Capsa/rules/capsa_rule_point_plate.png";
    static const char* szSlideBall     = "ui/Capsa/rules/capsa_rule_slideball.png";
    static const char* szSlideBar      = "ui/Capsa/rules/capsa_rule_slidebar.png";
    static const char* szSlideProgbar  = "ui/Capsa/rules/capsa_rule_slideprogbar.png";
    
    float rY = m_pScrollView->getInnerContainerSize().height - 20;
    
    for (int i = 0; i < N_LISTNAME_COUNT; i++, rY -= 50)
    {
        //draw label names
        auto lbName = Label::createWithTTF(g_szListNames[i], g_szFontName, 24);
        lbName->setPosition(20, rY);
        lbName->setAnchorPoint(Vec2(0, 0.5));
        m_pScrollView->addChild(lbName);
        
        auto lbSlideNo1 = Label::createWithTTF("1", g_szFontName, 24);
        lbSlideNo1->setPosition(240, rY);
        m_pScrollView->addChild(lbSlideNo1);
        
        const char* strNo10 = "10";
        if (i < 3)
            strNo10 = "50";
        
        auto lbSlideNo10 = Label::createWithTTF(strNo10, g_szFontName, 24);
        lbSlideNo10->setPosition(550, rY);
        lbSlideNo10->setAnchorPoint(Vec2(0, 0.5));
        m_pScrollView->addChild(lbSlideNo10);
        
        auto spPercentBk = Sprite::create(szPercent);
        spPercentBk->setPosition(630, rY);
        m_pScrollView->addChild(spPercentBk);
        
        auto lbPercent = Label::createWithTTF("1", g_szFontName, 24);
        lbPercent->setPosition(spPercentBk->getPosition());
        lbPercent->setTag(i + 1000);
        m_pScrollView->addChild(lbPercent);
        
        // create slider
        ui::Slider* pSlider = ui::Slider::create();
        
        pSlider->loadBarTexture(szSlideBar);
        pSlider->loadSlidBallTextures(szSlideBall, szSlideBall, "");
        pSlider->loadProgressBarTexture(szSlideProgbar);
        
        pSlider->setScale9Enabled(true);
        pSlider->setCapInsets(Rect(0, 0, 0, 0));
        pSlider->setContentSize(Size(250.0f, 19));
        pSlider->setPosition(Vec2(400, rY));
        pSlider->setTag(i+100);

        pSlider->addEventListener(CC_CALLBACK_2(CapsaRuleLayer::onSliderEvent, this));
        
        pSlider->setScaleY(m_rScaleX);
        
        m_pScrollView->addChild(pSlider);
    }
}


void CapsaRuleLayer::onSliderEvent(Ref *pSender, ui::Slider::EventType type)
{
    if (type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
    {
        ui::Slider* pSlider = dynamic_cast<ui::Slider*>(pSender);
        
        for (int i = 0; i < N_LISTNAME_COUNT; i++)
        {
            if (pSlider->getTag() == 100+i)
            {
                log("You pressed %dth item.", i);
                
                Label* lbChanged = (Label*)m_pScrollView->getChildByTag(1000+i);
                
                int nPercent = (pSlider->getPercent()/10);
                if (i < 3)
                    nPercent = (pSlider->getPercent()/2);
                
                if (nPercent == 0)
                    nPercent = 1;
                
                lbChanged->setString(__String::createWithFormat("%d", nPercent)->getCString());
            }
        }
    }
}
