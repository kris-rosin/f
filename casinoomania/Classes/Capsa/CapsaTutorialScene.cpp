//
//  CapsaTutorialScene.cpp
//  DovemobiCasino-mobile
//
//  Created by Frank on 11/7/17.
//

#include "CapsaTutorialScene.hpp"

#include "CapsaDefine.h"
#include "TopBar.h"

USING_NS_CC;

Scene * CapsaTutorialScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = CapsaTutorialScene::create();
    scene->addChild(layer);
    
    return scene;
}

CapsaTutorialScene::CapsaTutorialScene()
{
    m_nPageIndex = 1;
}

CapsaTutorialScene::~CapsaTutorialScene()
{
    
}

bool CapsaTutorialScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    const float rScalePosX = 0.62f;
    const float rScalePosY = 0.15f;
    
    auto winSize = Director::getInstance()->getWinSize();
    auto visSize = Director::getInstance()->getVisibleSize();
    
    Vec2 vecBottom(winSize.width * rScalePosX, winSize.height * rScalePosY);
    
    m_rScaleX = visSize.width/1336;
    m_rScaleY = visSize.height/768;
    
    auto spTutoBk = Sprite::create("ui/Capsa/tuto/capsa_tuto_bk.png");
    spTutoBk->setPosition(winSize / 2);
    this->addChild(spTutoBk);
    
    auto spBar = Sprite::create("ui/Capsa/tuto/capsa_tuto_bar.png");
    spBar->setPosition(vecBottom.x, vecBottom.y);
    spBar->setScaleY(m_rScaleX);
    this->addChild(spBar);
    
    
    auto spPlayRules = Sprite::create("ui/Capsa/tuto/capsa_tuto_Play_Rules_highlight.png");
    spPlayRules->setPosition(vecBottom.x - 155, winSize.height*0.1);
    this->addChild(spPlayRules);
    
    m_spHandRank = Sprite::create("ui/Capsa/tuto/capsa_tuto_Hand_Rank.png");
    m_spHandRank->setPosition(vecBottom.x, winSize.height*0.1);
    this->addChild(m_spHandRank);
    
    m_spSpecialRules = Sprite::create("ui/Capsa/tuto/capsa_tuto_Special_Rules.png");
    m_spSpecialRules->setPosition(vecBottom.x + 155, winSize.height*0.1);
    this->addChild(m_spSpecialRules);

    
    const char* szHighlineName = "ui/Capsa/tuto/capsa_tuto_highlight_line.png";
    
    m_spPlayHandLight = Sprite::create(szHighlineName);
    m_spPlayHandLight->setPosition(vecBottom.x - 74, vecBottom.y);
    this->addChild(m_spPlayHandLight);
    
    m_spHandSpecialLight = Sprite::create(szHighlineName);
    m_spHandSpecialLight->setPosition(vecBottom.x + 74, vecBottom.y);
    this->addChild(m_spHandSpecialLight);

    const char* szHighlightName = "ui/Capsa/tuto/capsa_tuto_highlight.png";

    m_spPlayRuleLight = Sprite::create(szHighlightName);
    m_spPlayRuleLight->setPosition(vecBottom.x - 155, vecBottom.y);
    m_spPlayRuleLight->setScaleY(m_rScaleX);
    this->addChild(m_spPlayRuleLight);
    
    m_spHandRankLight = Sprite::create(szHighlightName);
    m_spHandRankLight->setPosition(vecBottom.x, vecBottom.y);
    m_spHandRankLight->setScaleY(m_rScaleX);
    this->addChild(m_spHandRankLight);
    
    m_spSpecialRuleLight = Sprite::create(szHighlightName);
    m_spSpecialRuleLight->setPosition(vecBottom.x + 155, vecBottom.y);
    m_spSpecialRuleLight->setScaleY(m_rScaleX);
    this->addChild(m_spSpecialRuleLight);
    
    m_spPlayHandLight->setVisible(false);
    m_spHandRankLight->setVisible(false);
    m_spHandSpecialLight->setVisible(false);
    m_spSpecialRuleLight->setVisible(false);
    
    // create menu button image
    
    m_btLeft = MenuItemImage::create("ui/Capsa/tuto/capsa_tuto_back.png",
                    "ui/Capsa/tuto/capsa_tuto_back.png",
                    CC_CALLBACK_1(CapsaTutorialScene::onClickLeft, this));
    m_btLeft->setPosition(winSize.width*0.36, vecBottom.y);
    m_btLeft->setScaleY(m_rScaleX);
    
    m_btRight = MenuItemImage::create("ui/Capsa/tuto/capsa_tuto_next.png",
                    "ui/Capsa/tuto/capsa_tuto_next.png",
                    CC_CALLBACK_1(CapsaTutorialScene::onClickRight, this));
    m_btRight->setPosition(winSize.width*0.88, vecBottom.y);
    m_btRight->setScaleY(m_rScaleX);
    
    auto menu = Menu::create(m_btLeft, m_btRight, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    m_btLeft->setVisible(false);

    // add background
    
    m_nodeClipper = ClippingRectangleNode::create();
    m_nodeClipper->setClippingRegion(Rect(winSize.width * 0.36,
                                    winSize.height * 0.72,
                                    winSize.width*(0.9 - 0.36),
                                    winSize.height * (0.72 - rScalePosY)));
    m_nodeClipper->setAnchorPoint(Vec2(0, 1));
    this->addChild(m_nodeClipper);
    
    m_spPageImage = Sprite::create("ui/Capsa/tuto/capsa_tuto_page1.png");
    m_spPageImage->setPosition(winSize.width * 0.36, winSize.height * 0.68);
    m_spPageImage->setAnchorPoint(Vec2(0, 1));
    
    m_nodeClipper->addChild(m_spPageImage);
    
    //topbar
    auto topbar = TopBar::create(std::bind(&CapsaTutorialScene::onBack, this));
    topbar->setPosition(Size(WIN_SIZE.width / 2, WIN_SIZE.height));
    topbar->setScaleX(1 / m_rScaleX);
    topbar->setScaleY(1 / m_rScaleY);
    addChild(topbar, 90);

    this->setScaleX(m_rScaleX);
    this->setScaleY(m_rScaleY);
 
    this->schedule(schedule_selector(CapsaTutorialScene::update), 0.01);
    
    return true;
}

void CapsaTutorialScene::onBack()
{
    removeFromParentAndCleanup(true);
}

void CapsaTutorialScene::onClickLeft(cocos2d::Ref *sender)
{
    m_nPageIndex -= 1;
    if (m_nPageIndex < 1)
    {
        m_nPageIndex = 1;
        return;
    }
    
    setPageImage();
    setLightImage();
    upgradeButton();
}

void CapsaTutorialScene::onClickRight(cocos2d::Ref *sender)
{
    m_nPageIndex += 1;
    if (m_nPageIndex > 15)
    {
        m_nPageIndex = 15;
        return;
    }
    
    setPageImage();
    setLightImage();
    upgradeButton();
}


void CapsaTutorialScene::update(float dt)
{
    auto winSize = Director::getInstance()->getWinSize();
    
    m_rClipperY += 3;
    if (m_rClipperY >= winSize.height*(0.72 - 0.15))
    {
        m_rClipperY = winSize.height*(0.72 - 0.15);
    }
    m_nodeClipper->setClippingRegion(Rect(winSize.width * 0.36,
                                    winSize.height * 0.72 - m_rClipperY,
                                    winSize.width*(0.9 - 0.36),
                                    winSize.height * (0.72 - 0.15)));
}

void CapsaTutorialScene::setPageImage()
{
    __String* szFileName = __String::createWithFormat("ui/Capsa/tuto/capsa_tuto_page%d.png", m_nPageIndex);
    m_spPageImage->setTexture(szFileName->getCString());
    
    auto winSize = Director::getInstance()->getWinSize();
    if (m_nPageIndex == 10)
    {
        m_spPageImage->setPosition(winSize.width * 0.39, winSize.height * 0.465);
        m_spPageImage->setAnchorPoint(Vec2(0, 0.5));
    }
    else
    {
        m_spPageImage->setPosition(winSize.width * 0.36, winSize.height * 0.68);
        m_spPageImage->setAnchorPoint(Vec2(0, 1));
    }
    
    m_rClipperY = 0;
}

void CapsaTutorialScene::setLightImage()
{
    if (m_nPageIndex < 9)
    {
        m_spPlayHandLight->setVisible(false);
        m_spHandRankLight->setVisible(false);
        m_spHandRank->setTexture("ui/Capsa/tuto/capsa_tuto_Hand_Rank.png");
    }
    else
    {
        m_spPlayHandLight->setVisible(true);
        m_spHandRankLight->setVisible(true);
        m_spHandRank->setTexture("ui/Capsa/tuto/capsa_tuto_Hand_Rank_highlight.png");
    }
    
    if (m_nPageIndex < 11)
    {
        m_spHandSpecialLight->setVisible(false);
        m_spSpecialRuleLight->setVisible(false);
        m_spSpecialRules->setTexture("ui/Capsa/tuto/capsa_tuto_Special_Rules.png");
    }
    else
    {
        m_spHandSpecialLight->setVisible(true);
        m_spSpecialRuleLight->setVisible(true);
        m_spSpecialRules->setTexture("ui/Capsa/tuto/capsa_tuto_Special_Rules_highlight.png");
    }
}

void CapsaTutorialScene::upgradeButton()
{
    if (m_nPageIndex == 1)
        m_btLeft->setVisible(false);
    else
        m_btLeft->setVisible(true);
    
    if (m_nPageIndex == 15)
        m_btRight->setVisible(false);
    else
        m_btRight->setVisible(true);
}


