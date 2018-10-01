//
//  CapsaInviteList.cpp
//  DovemobiCasino-mobile
//
//  Created by Frank on 11/8/17.
//

#include "CapsaInviteList.hpp"

#include "CapsaInviteUserManager.hpp"
#include "CapsaInviteUser.hpp"

USING_NS_CC;
using namespace cocos2d::ui;

Scene * CapsaInviteList::createScene()
{
    auto scene = Scene::create();
    
    auto layer = CapsaInviteList::create();
    scene->addChild(layer);
    
    return scene;
}

CapsaInviteList::CapsaInviteList()
{
    m_fSearchState = false; //if false, Lobby. if true, friends.
}

CapsaInviteList::~CapsaInviteList()
{
    
}

bool CapsaInviteList::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto winSize = Director::getInstance()->getWinSize();
    
    LayerColor *backgroundLayer = LayerColor::create(Color4B(0,0,0,100));
    this->addChild(backgroundLayer, -1);
    
    auto spbk = Sprite::create("ui/Capsa/invite/capsa_invite_pop_bk.png");
    spbk->setPosition(winSize / 2);
    addChild(spbk);
    
    auto spTitle = Sprite::create("ui/Capsa/invite/capsa_invite_pop_title.png");
    spTitle->setPosition(Vec2(winSize.width/2, winSize.height*0.8));
    addChild(spTitle);
    
    m_pBtnLobby = MenuItemImage::create("ui/Capsa/invite/capsa_invite_pop_btn_lobby_on.png",
                                     "ui/Capsa/invite/capsa_invite_pop_btn_lobby_on.png",
                                     CC_CALLBACK_1(CapsaInviteList::onClickLobby, this));
    m_pBtnLobby->setPosition(winSize.width*0.5 - 100, winSize.height*0.7);
    
    m_pBtnFriends = MenuItemImage::create("ui/Capsa/invite/capsa_invite_pop_btn_friends_off.png",
                                      "ui/Capsa/invite/capsa_invite_pop_btn_friends_off.png",
                                      CC_CALLBACK_1(CapsaInviteList::onClickFriends, this));
    m_pBtnFriends->setPosition(winSize.width*0.5 + 100, winSize.height*0.7);
    
    m_pBtnClose = MenuItemImage::create("ui/Capsa/invite/capsa_invite_pop_btn_close.png",
                                      "ui/Capsa/invite/capsa_invite_pop_btn_close.png",
                                      CC_CALLBACK_1(CapsaInviteList::onClickClose, this));
    m_pBtnClose->setPosition(winSize.width*0.8, winSize.height*0.87);
    
    auto menu = Menu::create(m_pBtnLobby, m_pBtnFriends, m_pBtnClose, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    auto spMask = Sprite::create("ui/Capsa/invite/capsa_invite_pop_mask.png");
    spMask->setPosition(winSize.width / 2, winSize.height * 0.4);
    addChild(spMask);
    
    Size contentSize(spMask->getContentSize().width - 40,
                     spMask->getContentSize().height - 40);
    
    m_pScrollView = ui::ScrollView::create();
    
    m_pScrollView->setContentSize(contentSize);
    m_pScrollView->setPosition(Vec2(spMask->getPosition().x - contentSize.width/2,
                                 spMask->getPosition().y - contentSize.height/2));
    
    m_pScrollView->setScrollBarWidth(4);
    m_pScrollView->setScrollBarPositionFromCorner(Vec2(2, 2));
    m_pScrollView->setScrollBarColor(Color3B::GRAY);
    m_pScrollView->setGlobalZOrder(0);
    
    this->addChild(m_pScrollView, 1);
    
    CapsaInviteUserManager::shard()->generateInviteUsers();
    redrawUsers();
    
    return true;
}


void CapsaInviteList::onClickLobby(cocos2d::Ref *sender)
{
    m_pBtnLobby->setNormalImage(Sprite::create("ui/Capsa/invite/capsa_invite_pop_btn_lobby_on.png"));
    m_pBtnFriends->setNormalImage(Sprite::create("ui/Capsa/invite/capsa_invite_pop_btn_friends_off.png"));
    
    m_fSearchState = false;

    CapsaInviteUserManager* pManager = CapsaInviteUserManager::shard();
    pManager->resetInviteUsers();
    pManager->generateInviteUsers();
    redrawUsers();
}

void CapsaInviteList::onClickFriends(cocos2d::Ref *sender)
{
    m_pBtnLobby->setNormalImage(Sprite::create("ui/Capsa/invite/capsa_invite_pop_btn_lobby_off.png"));
    m_pBtnFriends->setNormalImage(Sprite::create("ui/Capsa/invite/capsa_invite_pop_btn_friends_on.png"));
    
    m_fSearchState = true;
    
    CapsaInviteUserManager* pManager = CapsaInviteUserManager::shard();
    pManager->resetInviteUsers();
    pManager->generateInviteUsers();
    redrawUsers();
}

void CapsaInviteList::onClickClose(cocos2d::Ref *sender)
{
    this->removeFromParent();
}

#pragma MARK - draw users
void CapsaInviteList::drawUsers(bool isFlag)
{
    CapsaInviteUserManager::shard()->addToParent(m_pScrollView, isFlag, m_rScrollVH);
}

void CapsaInviteList::redrawUsers()
{
    int nUsers = 0;
    if (m_fSearchState)
        nUsers = CapsaInviteUserManager::shard()->getFriendUsers();
    else
        nUsers = CapsaInviteUserManager::shard()->getLobbyUsers();
    
    m_rScrollVH = (nUsers / 4 + 1)*170 - 150;
    m_pScrollView->setInnerContainerSize(Size(m_pScrollView->getContentSize().width,
                                           m_rScrollVH));
    m_pScrollView->scrollToTop(2.0, true);
    
    drawUsers(m_fSearchState);
}


