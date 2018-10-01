//
//  CapsaCreateRoomLayer.cpp
//  DovemobiCasino
//
//  Created by Admin on 10/19/17.
//
//

#include "CapsaCreateRoomLayer.hpp"

#include "CapsaDefine.h"
#include "GameHandler.hpp"


bool CapsaCreateRoomLayer::init()
{
    if (!Layer::init())
        return false;

    DrawNode *drawNode = DrawNode::create();
    addChild(drawNode);
    drawNode->drawSolidRect(Vec2::ZERO, WIN_SIZE, Color4F(0, 0, 0, 0.5f));
    
    
    Sprite *spDialog = Sprite::create("ui/Capsa/create_room.png");
    addChild(spDialog);
    spDialog->setPosition(Vec2(WIN_SIZE * 0.5f));
    Size dialogSize = spDialog->getContentSize();

    m_btnCreate = ui::Button::create("ui/Capsa/capsalobby_roomcreate_2.png");
    spDialog->addChild(m_btnCreate);
    
    m_btnRule = ui::Button::create("ui/Capsa/capsalobby_roomrule.png");
    spDialog->addChild(m_btnRule);

    m_btnCreate->setPosition(POS_SIZE(dialogSize, Vec2(0.5f, 0.1f)));
    m_btnRule->setPosition(POS_SIZE(dialogSize, Vec2(0.8f, 0.1f)));
    
    ui::Button *btnClose = ui::Button::create("ui/Capsa/close_button.png");
    spDialog->addChild(btnClose);
    btnClose->addClickEventListener(CC_CALLBACK_0(CapsaCreateRoomLayer::onCloseDialog, this));
    btnClose->setAnchorPoint(Vec2(1.0f, 1.0f));
    btnClose->setPosition(dialogSize);
    
    m_btnHelp = ui::Button::create("ui/Capsa/help.png");
    spDialog->addChild(m_btnHelp);
    m_btnHelp->setPosition(POS_SIZE(dialogSize, Vec2(0.9f, 0.725f)));

    float posXRatio = 0.69f;
    std::stringstream ssCoin;
    PlayerProfile profile = PlayerProfile::getInstance();
    int deposite[] = CITY_DEPOSITE;
    int cityIndex = GameHandler::getInstance()->getSelectedCityIndex();
    m_deposite = deposite[cityIndex];
    ssCoin << m_deposite << " coins";
    Label *lblCoin  = Label::createWithTTF(ssCoin.str(), MAIN_FONT_FILE, dialogSize.height * 0.05f);
    spDialog->addChild(lblCoin);
    lblCoin->setPosition(POS_SIZE(dialogSize, Vec2(posXRatio, 0.55f)));
    
    MenuItemImage *itemNormalSpeed = MenuItemImage::create("ui/Capsa/normal_speed.png", "");
    Sprite *speedTap = Sprite::create("ui/Capsa/speed_tap.png");
    speedTap->setAnchorPoint(Vec2(1.0f, 0.5f));
    speedTap->setPosition(itemNormalSpeed->getContentSize().width, itemNormalSpeed->getContentSize().height * 0.5f);
    itemNormalSpeed->addChild(speedTap);
    
    
    MenuItemImage *itemFastSpeed = MenuItemImage::create("ui/Capsa/fast_speed.png", "");
    speedTap = Sprite::create("ui/Capsa/speed_tap.png");
    speedTap->setAnchorPoint(Vec2(0, 0.5f));
    speedTap->setPosition(0, itemFastSpeed->getContentSize().height * 0.5f);
    itemFastSpeed->addChild(speedTap);
    
    m_toggleSpeed = MenuItemToggle::createWithCallback(CC_CALLBACK_0(CapsaCreateRoomLayer::onToggleSpeed, this), itemNormalSpeed, itemFastSpeed, NULL);
    
    MenuItemImage *itemMaxPlayer2 = MenuItemImage::create("ui/Capsa/maxplayer_2.png", "");
    MenuItemImage *itemMaxPlayer3 = MenuItemImage::create("ui/Capsa/maxplayer_3.png", "");
    MenuItemImage *itemMaxPlayer4 = MenuItemImage::create("ui/Capsa/maxplayer_4.png", "");
    m_toggleMaxPlayers = MenuItemToggle::createWithCallback(CC_CALLBACK_0(CapsaCreateRoomLayer::onToggleMaxPlayers, this), itemMaxPlayer2, itemMaxPlayer3, itemMaxPlayer4, NULL);
    m_maxPlayers = 2;
    
    MenuItemImage *itemPrivateOff = MenuItemImage::create("ui/Capsa/private_uncheck.png", "");
    MenuItemImage *itemPrivateOn = MenuItemImage::create("ui/Capsa/private_check.png", "");
    m_togglePrivateRoom = MenuItemToggle::createWithCallback(CC_CALLBACK_0(CapsaCreateRoomLayer::onTogglePrivateRoom, this), itemPrivateOff, itemPrivateOn, NULL);
    
    Vec2 posDialog = spDialog->getBoundingBox().origin;
    
    m_toggleSpeed->setPosition(posDialog + POS_SIZE(dialogSize, Vec2(posXRatio, 0.423f)));
    m_toggleMaxPlayers->setPosition(posDialog + POS_SIZE(dialogSize, Vec2(posXRatio, 0.305f)));
    m_togglePrivateRoom->setPosition(posDialog + POS_SIZE(dialogSize, Vec2(0.33f, 0.1f)));
    
    Menu *menu = Menu::create(m_toggleSpeed, m_toggleMaxPlayers, m_togglePrivateRoom, NULL);
    addChild(menu);
    menu->setPosition(Vec2::ZERO);
    
    return true;
}
void CapsaCreateRoomLayer::onEnter()
{
    Layer::onEnter();
    
    setupTouch();
}

void CapsaCreateRoomLayer::setupTouch()
{
    auto pListener = EventListenerTouchOneByOne::create();
    pListener->setSwallowTouches(true);
    
    pListener->onTouchBegan = [] (cocos2d::Touch* pThouch, cocos2d::Event* pEvent)
    {
        return true;
    };
    
//    pListener->onTouchEnded = [this, pListener](Touch * touch, Event * event)
//    {
//        if (this->getParent())
//        {
//            auto point = this->getParent()->convertTouchToNodeSpace(touch);
//            if (this->getBoundingBox().containsPoint(point) == false)
//            {
//                this->onCloseDialog();
//            }
//        }
//    };

    getEventDispatcher()->addEventListenerWithSceneGraphPriority(pListener, this);
}

void CapsaCreateRoomLayer::setCallback(const ui::Widget::ccWidgetClickCallback &callbackCreate,
                                       const ui::Widget::ccWidgetClickCallback &callbackRule,
                                       const ui::Widget::ccWidgetClickCallback &callbackHelp)
{
    m_btnCreate->addClickEventListener(callbackCreate);
    m_btnRule->addClickEventListener(callbackRule);
    m_btnHelp->addClickEventListener(callbackHelp);
}

void CapsaCreateRoomLayer::onCloseDialog()
{
    removeFromParentAndCleanup(true);
}

int CapsaCreateRoomLayer::getMaxPlayers()
{
    return m_maxPlayers;
}

bool CapsaCreateRoomLayer::isFastSpeed()
{
    return m_isFastSpeed;
}

bool CapsaCreateRoomLayer::isPrivateRoom()
{
    return m_isPrivateRoom;
}

int CapsaCreateRoomLayer::getDeposite()
{
    return m_deposite;
}

void CapsaCreateRoomLayer::onToggleSpeed()
{
    m_isFastSpeed = m_toggleSpeed->getSelectedIndex();
}

void CapsaCreateRoomLayer::onToggleMaxPlayers()
{
    m_maxPlayers = m_toggleMaxPlayers->getSelectedIndex() + 2;
}

void CapsaCreateRoomLayer::onTogglePrivateRoom()
{
    m_isPrivateRoom = m_togglePrivateRoom->getSelectedIndex();
}
