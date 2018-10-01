//
//  CapsaPlayerNode.cpp
//  DovemobiCasino
//
//  Created by liu on 9/21/17.
//
//

#include "CapsaPlayerNode.hpp"

#include "CapsaDefine.h"
#include "CapsaPlayer.hpp"
#include "App42Define.h"
#include "App42API.h"

bool CapsaPlayerNode::init()
{
    if (!Sprite::initWithFile("ui/Capsa/placeholder.png"))
        return false;
    
    Sprite *spProgress = Sprite::create("ui/Capsa/blue_ring.png");
    m_progressTimer = ProgressTimer::create(spProgress);
    m_progressTimer->setType(ProgressTimer::Type::RADIAL);
    addChild(m_progressTimer, -1);
    m_progressTimer->setPosition(getContentSize() * 0.5f);
    
    m_spTurn = Sprite::create("ui/Capsa/turnactive.png");
    addChild(m_spTurn);
    m_spTurn->setPosition(getContentSize() * 0.5f);
    setActiveTurn(false);
    
    Sprite *namePanel = Sprite::create("ui/Capsa/nameholder.png");
    addChild(namePanel);
    namePanel->setPosition(getContentSize().width * 0.5f, 0);
 
    m_lblPlayerName = Label::createWithSystemFont("", "arial", namePanel->getContentSize().height * 0.8f);
    m_lblPlayerName->setDimensions(namePanel->getContentSize().width * 0.8f, namePanel->getContentSize().height * 0.8f);
    m_lblPlayerName->setOverflow(Label::Overflow::SHRINK);
    m_lblPlayerName->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    namePanel->addChild(m_lblPlayerName);
    m_lblPlayerName->setPosition(namePanel->getContentSize() * 0.5f);
    
    m_goldLevelNode = Node::create();
    addChild(m_goldLevelNode);
    
    Sprite * spLevelPanel = Sprite::create("ui/Capsa/level_placeholder.png");
    m_lblLevel = Label::createWithTTF("", THIN_FONT_FILE, spLevelPanel->getContentSize().height * 0.8f);
    spLevelPanel->addChild(m_lblLevel);
    m_lblLevel->setPosition(spLevelPanel->getContentSize() * 0.5f);
    m_lblLevel->setColor(Color3B::BLACK);
    m_goldLevelNode->addChild(spLevelPanel);
    spLevelPanel->setAnchorPoint(Vec2(0.0f, 0.0f));
    spLevelPanel->setPosition(0.0f, spLevelPanel->getContentSize().height * 0.1f);
    
    Sprite * spGoldPanel = Sprite::create("ui/Capsa/coin_placeholder.png");
    m_lblGold = Label::createWithTTF("", THIN_FONT_FILE, spGoldPanel->getContentSize().height * 0.8f);
    Sprite *spGoldIcon = Sprite::create("ui/Capsa/gold_icon.png");
    spGoldPanel->addChild(spGoldIcon);
    spGoldIcon->setAnchorPoint(Vec2(0.0f, 0.5f));
    spGoldIcon->setPosition(0, spGoldPanel->getContentSize().height * 0.5f);
    spGoldPanel->addChild(m_lblGold);
    m_lblGold->setAnchorPoint(Vec2(0.0f, 0.5f));
    m_lblGold->setPosition(spGoldIcon->getContentSize().width * 1.2f, spGoldPanel->getContentSize().height * 0.5f);
    m_lblGold->setColor(Color3B::YELLOW);
    m_goldLevelNode->addChild(spGoldPanel);
    spGoldPanel->setAnchorPoint(Vec2(0.0f, 1.0f));
    spGoldPanel->setPosition(Vec2(0.0f, -spLevelPanel->getPositionY()));
    
    m_goldLevelNode->setAnchorPoint(Vec2(0.0f, 0.5f));
    m_goldLevelNode->setPosition(getContentSize().width * 0.9f, getContentSize().height * 0.5f);
    
    showPlayerInfo(false);
    
    return true;
}

void CapsaPlayerNode::setPlayerName(const std::string &playerName)
{
    m_playerName = playerName;
    std::string visibleName = base64_decode(playerName);
    if (IS_CONTAIN_FBUSER_PRE(visibleName))
    {
        std::string fbName = visibleName.substr(0, FBUSER_PRE_POS(visibleName));
        m_lblPlayerName->setString(fbName);
    }
    else
        m_lblPlayerName->setString(visibleName);
}

std::string CapsaPlayerNode::getPlayerName()
{
    return m_playerName;
}

void CapsaPlayerNode::setActiveTurn(bool turn)
{
    m_spTurn->setVisible(turn);
    
    updateTurnTimer(turn);
}

void CapsaPlayerNode::updateTurnTimer(bool turn)
{
    if (turn)
    {
        m_time = 0.0f;
        schedule(schedule_selector(CapsaPlayerNode::onTimer), 1.0f);
    }
    else
    {
        m_progressTimer->setPercentage(0.0f);
        unscheduleAllCallbacks();
    }
}

void CapsaPlayerNode::setMaxTime(float maxTime)
{
    m_maxTime = maxTime;
}

void CapsaPlayerNode::onTimer(float dt)
{
    m_time++;
    if (m_time > m_maxTime)
    {
        updateTurnTimer(false);
        return;
    }
    
    float closedPercent = m_time / m_maxTime;
    std::string files[] = {
        "ui/Capsa/blue_ring.png",
        "ui/Capsa/yellow_ring.png",
        "ui/Capsa/red_ring.png"
    };
    int fileIndex = closedPercent > 0.8f ? 2 : 1;
    if (closedPercent < 0.5f)
        fileIndex = 0;
    m_progressTimer->getSprite()->setTexture(files[fileIndex]);
    m_progressTimer->setPercentage(closedPercent * 100);
}

void CapsaPlayerNode::showPlayerInfo(bool visible)
{
    m_goldLevelNode->setVisible(visible);
}

void CapsaPlayerNode::updatePlayer(const CapsaPlayer & player, bool onlyPicture)
{
    if (!onlyPicture)
    {
        std::stringstream ssLevel;
        ssLevel << "Lv. " << player.level;
        m_lblLevel->setString(ssLevel.str());
    
        std::stringstream ssGold;
        if (player.gold > 1000)
            ssGold << (int)(player.gold / 1000) << " K";
        else
            ssGold << player.gold;
        m_lblGold->setString(ssGold.str());
        showPlayerInfo(true);
    }
    
    if (player.pictureURL.length())
        loadFromURL(player.pictureURL.c_str());
}

void CapsaPlayerNode::loadFromURL(const char *url)
{
    if (m_loadedPicture)
        return;
    
    retain();
    
    HttpRequest* request = new (std::nothrow) HttpRequest();
    request->setUrl(url);
    request->setRequestType(HttpRequest::Type::GET);
    request->setResponseCallback(CC_CALLBACK_2(CapsaPlayerNode::onLoadedFromURL, this));
    HttpClient::getInstance()->send(request);
    request->release();
}

void CapsaPlayerNode::onLoadedFromURL(HttpClient *sender, HttpResponse *response)
{
    if (!response->isSucceed())
    {
        release();
        return;
    }
    
    std::vector<char> *buffer = response->getResponseData();
    Image * image = new  Image ();
    image-> initWithImageData ( reinterpret_cast<const unsigned char*>(&(buffer->front())), buffer->size());
    if (image->getData())
    {
        Texture2D * texture = new Texture2D();
        texture->initWithImage(image);
        Sprite * sprite = Sprite::createWithTexture (texture);
        if (sprite)
        {
            adjustChildSprite(sprite);
        }
    }
    
    m_loadedPicture = true;
    
    release();
}

void CapsaPlayerNode::adjustChildSprite(cocos2d::Sprite * child)
{
    if (m_child)
    {
        m_child->removeFromParentAndCleanup(true);
        m_child = NULL;
    }
    
    float kRatio = 0.75f;
    child->setScale(getContentSize().width / child->getContentSize().width * kRatio, getContentSize().height /  child->getContentSize().height * kRatio);
    child->setPosition(getContentSize() * 0.5f);
    
    DrawNode * stencil = DrawNode::create();
    stencil->drawSolidCircle(child->getPosition(), getContentSize().width * 0.5f * kRatio, 0.0f, 2 << 10, Color4F::GREEN);
    
    ClippingNode * clipper = ClippingNode::create();
    clipper->setStencil(stencil);
    
    clipper->addChild(child);
    
    addChild(clipper);
    m_child = clipper;
}
