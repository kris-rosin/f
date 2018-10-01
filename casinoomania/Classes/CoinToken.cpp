//
//  CoinToken.cpp
//
//  Created by Stanislav Zheronkin on 24.02.17.
//
//

#include "CoinToken.h"

#include "Utilities.h"
#include "AudioManager.h"

USING_NS_CC;

CoinToken::CoinToken() : coinsLabel(nullptr), coinsSprite(nullptr), dragingToken(nullptr), betPlace(-1), enabled(true), magnifier(nullptr), magnifierFrame(nullptr), scaleDragTokens(1.0f)
{
    
}

CoinToken * CoinToken::create(int initialValue, CoinTokenType type, CoinTokenSkin skin, CoinToken * source)
{
    CoinToken * pRet = new CoinToken();
    if (pRet && pRet->init(initialValue, type, skin, source))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool CoinToken::init(int initialValue, CoinTokenType type, CoinTokenSkin skin, CoinToken * source)
{
    if (!Node::init())
        return false;
    
    setCascadeOpacityEnabled(true);
    
    this->type = type;
    
    addToken(initialValue, skin, source);
    
    auto eventListener = EventListenerTouchOneByOne::create();
    
    eventListener->setSwallowTouches(true);
    
    eventListener->onTouchBegan = [this](Touch * touch, Event * event)
    {
        if (this->enabled == false || this->isVisible() == false)
            return false;
        
        if (this->type == BUTTON || this->type == STACK || this->type == PUSH_BUTTON)
        {
            Rect area = Rect(Vec2::ZERO, getContentSize());
            
            Vec2 point = convertTouchToNodeSpace(touch);
            
            if (area.containsPoint(point))
            {
                if (this->type == BUTTON)
                {
                    if (dragingToken)
                        dragingToken->removeFromParent();
                    
                    dragingToken = CoinToken::create(std::get<0>(this->coinsStack.front()), CoinTokenType::NORMAL, (CoinTokenSkin)std::get<1>(this->coinsStack.front()), this);
                    dragingToken->setPosition(point);
                    
                    this->addChild(dragingToken, 2);
                    
                    if (this->IsPhoneSizeDevice())
                    {
                        magnifier = RenderTexture::create(200, 200);
                        magnifier->setContentSize(Size(200, 200));
                        magnifier->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                        magnifier->setPosition(point + magnifier->getContentSize() / 2);
                        
                        magnifierFrame = Sprite::create("ui/Roulette/magnifier.png");
                        magnifierFrame->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                        
                        this->addChild(magnifier, 1);
                        this->addChild(magnifierFrame, 2);
                        
                        updateMagnifier(touch);
                    }
                }
                else if (this->type == PUSH_BUTTON)
                {
                    coinsSprite->setColor(Color3B(127, 127, 127));
                }
                return true;
            }
        }
        
        return false;
    };
    
    eventListener->onTouchMoved = [this](Touch * touch, Event * event)
    {
        if (this->type == BUTTON)
        {
            if (dragingToken)
            {
                Vec2 point = convertTouchToNodeSpace(touch);
                
                dragingToken->setPosition(point);
                
                if (magnifier)
                {
                    magnifier->setPosition(point + magnifier->getContentSize() / 2);
                    magnifierFrame->setPosition(point);
                    updateMagnifier(touch);
                }
                
                const float maxDistance = 100.0f;
                
                float scale = 1.0f - (1.0f - scaleDragTokens) * ( std::min((point - this->getContentSize() / 2).length(), maxDistance) / maxDistance);
                
                dragingToken->setScale(scale);
            }
        }
    };
    
    eventListener->onTouchEnded = [this](Touch * touch, Event * event)
    {
        if (this->type == BUTTON)
        {
            if (magnifier)
            {
                magnifier->removeFromParent();
                magnifier = nullptr;
                
                magnifierFrame->removeFromParent();
                magnifierFrame = nullptr;
            }
            
            if (dragingToken)
            {
                if (callback)
                {
                    if (callback(CoinTokenEvent::PLACED, dragingToken) == true)
                    {
                        dragingToken = nullptr;
                        AudioManager::getInstance().PlaySoundEffect("music/Sounds/effect");
                        
                        return;
                    }
                }
                
                dragingToken->runAction(Sequence::create(Spawn::createWithTwoActions(EaseSineOut::create(MoveTo::create(0.5f, this->getContentSize() / 2)),
                                                                                     ScaleTo::create(0.5f, 1.0f)), RemoveSelf::create(), NULL));
                dragingToken = nullptr;
            }
        }
        else if (this->type == STACK)
        {
            if (callback)
            {
                if (callback(CoinTokenEvent::CANCELED, this) == true)
                {
                    AudioManager::getInstance().PlaySoundEffect("music/Sounds/moneyGain");
                    return;
                }
            }
        }
        else if (this->type == PUSH_BUTTON)
        {
            if (callback)
            {
                callback(CoinTokenEvent::PUSHED, this);
            }
            
            coinsSprite->setColor(Color3B::WHITE);
        }
    };
    
    eventListener->onTouchCancelled = eventListener->onTouchEnded;
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);
    
    return true;
}

void CoinToken::updateMagnifier(Touch * touch)
{
    if (magnifier)
    {
        magnifier->setVisible(false);
        dragingToken->setVisible(false);
        magnifierFrame->setVisible(false);
        
        auto scene        = Director::getInstance()->getRunningScene();
        auto prevPosition = scene->getPosition();
        auto prevScale    = scene->getScale();
        auto prevAnchor   = scene->getAnchorPoint();
        
        Vec2 pos = scene->convertTouchToNodeSpace(touch);
        
        magnifier->begin();
        
        scene->setPosition(magnifier->getContentSize() / 2 + Size(-2.0f * pos.x, -2.0f * pos.y));
        scene->setScale(2.0f);
        scene->setAnchorPoint(Vec2::ZERO);
        scene->visit();
        
        magnifier->end();
        
        Director::getInstance()->getRenderer()->render();
        
        scene->setPosition(prevPosition);
        scene->setScale(prevScale);
        scene->setAnchorPoint(prevAnchor);
        
        magnifier->setVisible(true);
        dragingToken->setVisible(true);
        magnifierFrame->setVisible(true);
    }
}

void CoinToken::reset()
{
    coinsStack.clear();
    
    updateSkin(CoinToken::CoinTokenSkin::GREY);
}

int  CoinToken::getTotalValue()
{
    int result = 0;
    
    for (auto k: coinsStack)
    {
        result += std::get<0>(k);
    }
    
    return result;
}

CoinToken * CoinToken::getTopTokenClone()
{
    if (coinsStack.size() > 0)
    {
        auto result = CoinToken::create(std::get<0>(coinsStack.back()), CoinToken::CoinTokenType::NORMAL, (CoinTokenSkin)std::get<1>(coinsStack.back()), nullptr);
        
        return result;
    }
    
    return nullptr;
}

CoinToken::CoinTokenSkin CoinToken::getSkin()
{
    if (coinsStack.size() > 0)
    {
        return (CoinTokenSkin)std::get<1>(coinsStack.back());
    }
    
    return CoinTokenSkin::GREY;
}

CoinToken * CoinToken::getSourceToken()
{
    if (coinsStack.size() > 0)
    {
        return std::get<2>(coinsStack.back());
    }
    
    return nullptr;
}

void CoinToken::addToken(int value, CoinTokenSkin tokenSkin, CoinToken * source)
{
    if (tokenSkin < CoinTokenSkin::GREY)
        tokenSkin = CoinTokenSkin::GREY;
    if (tokenSkin > CoinTokenSkin::BLACK)
        tokenSkin = CoinTokenSkin::BLACK;
    
    coinsStack.push_back(std::make_tuple(value, (int)tokenSkin, source));
    
    updateSkin(tokenSkin);
}

bool CoinToken::popToken()
{
    if (coinsStack.size() == 1)
    {
        auto target = std::get<2>(coinsStack.back());
        
        if (target)
        {
            Vec2 point = target->convertToNodeSpace(convertToWorldSpaceAR(Vec2::ZERO));
            
            retain();
            removeFromParentAndCleanup(false);
            target->addChild(this, 1);
            setPosition(point);
            runAction(Sequence::create(Spawn::createWithTwoActions(EaseSineOut::create(MoveTo::create(0.5f, this->getContentSize() / 2)),
                                                                                 ScaleTo::create(0.5f, 1.0f)), RemoveSelf::create(), NULL));
            
            release();
        }
        else
        {
            removeFromParent();
        }
        
        return true;
    }
    else
    {
        auto target = std::get<2>(coinsStack.back());
        
        if (target)
        {
            Vec2 point = target->convertToNodeSpace(convertToWorldSpaceAR(Vec2::ZERO));
        
            CoinToken * flyBackToken = CoinToken::create(std::get<0>(coinsStack.back()), CoinToken::CoinTokenType::NORMAL, (CoinTokenSkin)std::get<1>(coinsStack.back()), target);
        
            target->addChild(flyBackToken, 1);
            flyBackToken->setPosition(point);
            flyBackToken->setScale(getScale());
            flyBackToken->runAction(Sequence::create(Spawn::createWithTwoActions(EaseSineOut::create(MoveTo::create(0.5f, flyBackToken->getContentSize() / 2)),
                                                                   ScaleTo::create(0.5f, 1.0f)), RemoveSelf::create(), NULL));
        }
        
        coinsStack.pop_back();
        
        updateSkin( (CoinTokenSkin)std::get<1>(coinsStack.back()) );
    }
    
    return false;
}

void CoinToken::updateSkin(CoinTokenSkin skin)
{
    if (coinsSprite)
    {
        coinsSprite->removeFromParent();
    }
    
    coinsSprite = Sprite::create(StringUtils::format("ui/Roulette/coin_blank%d.png", skin));
    coinsSprite->setAnchorPoint(Vec2(0.5f, 0.56f));
    addChild(coinsSprite);
    setContentSize(coinsSprite->getContentSize());
    coinsSprite->setPosition(getContentSize() / 2);
    
    if (!coinsLabel)
    {
        coinsLabel = Label::createWithBMFont("fonts/CoinNumbers.fnt", "0");
        coinsLabel->setColor(Color3B::BLACK);
        coinsLabel->enableWrap(false);
        coinsLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        coinsLabel->setDimensions(52, 35);
        coinsLabel->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        coinsLabel->setPosition(getContentSize() / 2);
        coinsLabel->setOverflow(cocos2d::Label::Overflow::SHRINK);
        addChild(coinsLabel, 1);
    }
    
    coinsLabel->setString(getCompactNumberString(getTotalValue()));
    
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
}

void CoinToken::setCallback(std::function<bool(CoinTokenEvent, CoinToken*)> callbackFunction)
{
    callback = callbackFunction;
}

void CoinToken::setType(CoinTokenType type)
{
    this->type = type;
}

std::string CoinToken::getCompactNumberString(unsigned int value)
{
    if (value >= 1000000000) //B
    {
        if (value % 1000000000 == 0)
        {
            return StringUtils::format("%dB", value / 1000000000);
        }
    }
    else if (value >= 1000000) //M
    {
        if (value % 1000000 == 0)
        {
            return StringUtils::format("%dM", value / 1000000);
        }
    }
    else if (value >= 1000) //K
    {
        if (value % 1000 == 0)
        {
            return StringUtils::format("%dK", value / 1000);
        }
    }
    
    return StringUtils::format("%d", value);
}

void CoinToken::setBetPlace(int placeSlot)
{
    betPlace = placeSlot;
}

int CoinToken::getBetPlace()
{
    return betPlace;
}

void CoinToken::setEnabled(bool value)
{
    enabled = value;
}

bool CoinToken::IsPhoneSizeDevice()
{
    cocos2d::Size screenSize = Director::getInstance()->getOpenGLView()->getFrameSize();
    
    int dpi = Device::getDPI();
    
    float _screenSize = sqrtf(powf(screenSize.width, 2.0f) + powf(screenSize.height, 2.0f)) / dpi;
    
    if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    {
        //iphone 6 plus hack, to prevent stupid 489 dpi
        if (screenSize.width == 2208 && dpi == 489)
        {
            _screenSize = 5.5f;
        }
    }
    
    log("Screen size: %f inches", _screenSize);
    
    return _screenSize < 6.5;
}
