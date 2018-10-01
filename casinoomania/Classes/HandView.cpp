//
//  HandView.cpp
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 20.03.17.
//
//

#include "HandView.h"
#include "PlayerProfile.h"
#include "AudioManager.h"
#include "BaseScene.h"

USING_NS_CC;

HandView::HandView() : cardsRoot(nullptr), betNode(nullptr), insuranceNode(nullptr), payoutNode(nullptr)
{
    
}

HandView * HandView::create()
{
    HandView * pRet = new HandView();
    if (pRet && pRet->init())
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

bool HandView::init()
{
    if (!Node::init())
        return false;
    
    cardsRoot = Node::create();
    addChild(cardsRoot);
    
    // Points View
    pointsView = ui::Scale9Sprite::create("ui/BlackJack/points.png");
    pointsLabel= Label::createWithTTF("0", "fonts/Berlin.ttf", 20.0f);
    pointsView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    pointsLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    pointsView->addChild(pointsLabel);
    cardsRoot->addChild(pointsView, 2);
    pointsView->setVisible(false);
    
    // Bet & insurance token stacks
    auto bet_field = Sprite::create("ui/BlackJack/bet_field.png");
    bet_field->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    //bet_field->setScale(1.0f);
    bet_field->setPosition(Vec2(0, -95));
    addChild(bet_field);
    
    betNode       = CoinToken::create(0, CoinToken::CoinTokenType::STACK, CoinToken::CoinTokenSkin::GREY, nullptr);
    insuranceNode = CoinToken::create(0, CoinToken::CoinTokenType::STACK, CoinToken::CoinTokenSkin::GREY, nullptr);
    payoutNode    = CoinToken::create(0, CoinToken::CoinTokenType::STACK, CoinToken::CoinTokenSkin::GREY, nullptr);
    
    betNode->setCallback(std::bind(&HandView::onTokenButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    insuranceNode->setCallback(std::bind(&HandView::onTokenButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    
    bet_field->addChild(betNode);
    bet_field->addChild(insuranceNode);
    bet_field->addChild(payoutNode);
    
    betNode->setPosition(bet_field->getContentSize() / 2);
    insuranceNode->setPosition(Vec2(bet_field->getContentSize().width, bet_field->getContentSize().height * 0.5f));
    payoutNode->setPosition(Vec2(0, bet_field->getContentSize().height * 0.5f));
    
    betNode->setVisible(false);
    insuranceNode->setVisible(false);
    payoutNode->setVisible(false);
    
    bet_field->setVisible(!isDealer());
    
    return true;
}

HandView * HandView::Split()
{
    auto newHand = HandView::create();
    
    splitted = true;
    
    if(HasAce())
        ace_splitted = true;
    
    newHand->addBet(betNode);
    newHand->splitted = true;
    newHand->ace_splitted = ace_splitted;
    
    //last card transfer to new hand
    newHand->setCards({ cards.back() });
    
    Vec2 endPos(cardSprites.back()->getContentSize().width * 0.5f, 0);
    cardSprites.back()->removeFromParentAndCleanup(false);
    newHand->cardSprites.pushBack(cardSprites.back());
    newHand->cardsRoot->addChild(cardSprites.back());
    
    cards.pop_back();
    cardSprites.popBack();
    
    //adjust cards positions
    cardsRoot->runAction(EaseSineInOut::create(MoveTo::create(0.12f, -endPos)));
    newHand->cardsRoot->runAction(EaseSineInOut::create(MoveTo::create(0.12f, -endPos)));
    //cardSprites.back()->runAction(EaseSineInOut::create(MoveTo::create(0.12f, endPos)));
    newHand->cardSprites.back()->runAction(EaseSineInOut::create(MoveTo::create(0.12f, endPos)));
    
    return newHand;
}

void HandView::Hit(CardObject card, float delay, std::function<void()> completionCallback /*= nullptr*/, bool forceHidden /*= false*/)
{
    Hand::Hit(card);
    
    int cardID = cards.size() - 1;
    
    auto sprite = Sprite::create("ui/Cards/card.png");
    auto shadow = Sprite::create("ui/Cards/card_shadow.png");
    shadow->setOpacity(164);
    shadow->setVisible(false);
    
    cardsRoot->addChild(shadow);
    cardsRoot->addChild(sprite);
    
    cardSprites.pushBack(sprite);
    
    auto winSize = Director::getInstance()->getWinSize();
    Vec2 startPos(winSize.width / 2, winSize.height + sprite->getContentSize().height / 2);
    Vec2 endPos( sprite->getContentSize().width * (0.5f + 0.4f * cardID), sprite->getContentSize().width * 0.01f * cardID);
    
    startPos = convertToNodeSpace(startPos);
    sprite->setPosition(startPos);
    shadow->setPosition(startPos + Vec2(20, -50));
    
    float time = startPos.length() / 500 * 0.25f;
    auto  actions = Vector<FiniteTimeAction*>();
    
    auto delayAction = DelayTime::create(delay);
    
    actions.pushBack(delayAction);
    actions.pushBack(CallFunc::create([]{ AudioManager::getInstance().PlaySoundEffect("music/Sounds/sendcard"); }));
    actions.pushBack(MoveTo::create(time, endPos));
    
    //animate shadow
    shadow->runAction(Sequence::create(delayAction, Show::create(), MoveTo::create(time, endPos), RemoveSelf::create(), nullptr));
    
    if ( (isDealer() && cards.size() == 2) == false && !forceHidden) //do not open second dealer's card
    {
        
        actions.pushBack(CallFunc::create([this, cardID](){ openCard(cardID); }));
    }
    
    // adjust hand position
    actions.pushBack(CallFunc::create([this, sprite, cardID]
    {
        contentSize = Size(sprite->getContentSize().width * (1.0f + 0.4f * cardID), sprite->getContentSize().height * (1.0f + 0.01f * cardID));
        
        cardsRoot->runAction(EaseSineInOut::create(MoveTo::create(0.15f, Vec2(-contentSize.width / 2, 0))));
        
        pointsView->setPosition(Vec2(0, sprite->getContentSize().height / 2));
    }));
    
    if (completionCallback)
    {
        actions.pushBack(DelayTime::create(0.5f));
        actions.pushBack(CallFunc::create(completionCallback));
    }
    
    sprite->runAction(Sequence::create(actions));
}

void HandView::openCard(int cardID)
{
    if (cardID >= 0 && cardID < cards.size())
    {
        CardObject & card = cards[cardID];
        
        if (!card.isCardOpen())
        {
            card.setCardOpen(true);
            
            auto  actions = Vector<FiniteTimeAction*>();
            
            const float halfFlipTime = 0.12f;
            
            actions.pushBack(EaseSineIn::create(Spawn::create(ScaleTo::create(halfFlipTime, 0.01f, 1.0f),
                                                              SkewTo::create(halfFlipTime , 0, -25),
                                                              MoveBy::create(halfFlipTime , Vec2(0, 50)), nullptr)));
            
            auto sprite   = cardSprites.at(cardID);
            auto cardFace = Sprite::create(StringUtils::format("ui/Cards/%s.png", card.getCardCode().c_str()));
            auto cardFaceFrame = cardFace->getSpriteFrame();
            cardFaceFrame->retain();
            sprite->setUserObject(cardFaceFrame); //face sprite frame
            sprite->setLocalZOrder(1);
            
            //shadow setup
            auto shadow = Sprite::create("ui/Cards/card_shadow.png");
            shadow->setOpacity(164);
            shadow->setPosition(sprite->getPosition());
            cardsRoot->addChild(shadow);
            
            auto flipCard = [sprite, shadow]
            {
                auto frame = dynamic_cast<SpriteFrame*>(sprite->getUserObject());
                if (frame)
                {
                    sprite->setSpriteFrame(frame);
                    sprite->setSkewY(25);
                    shadow->setSkewY(12);
                    frame->release();
                    sprite->setUserObject(nullptr);
                }
            };
            
            actions.pushBack(CallFunc::create(flipCard));
            actions.pushBack(EaseSineOut::create(Spawn::create(ScaleTo::create(halfFlipTime, 1.0f, 1.0f),
                                                               SkewTo::create(halfFlipTime , 0, 0),
                                                               MoveBy::create(halfFlipTime , Vec2(0, -50)), nullptr)));
            
            actions.pushBack(CallFuncN::create([](Node * node)
            {
                node->setLocalZOrder(0);
            }));
            
            sprite->runAction(Sequence::create(actions));
            
            //shadow animation
            shadow->runAction(Sequence::create(Spawn::create(ScaleTo::create(halfFlipTime, 0.01f, 1.0f),
                                                             SkewTo::create(halfFlipTime , 0, -12),
                                                             MoveBy::create(halfFlipTime , Vec2(10, -25)), NULL),
                                               Spawn::create(ScaleTo::create(halfFlipTime, 1.0f, 1.0f),
                                                             SkewTo::create(halfFlipTime , 0, 0),
                                                             MoveBy::create(halfFlipTime , Vec2(-10, 25)), NULL),
                                               RemoveSelf::create(), nullptr));
            
            setPoints(getPointsString());
        }
    }
}

void HandView::showPoints(bool value)
{
    if (pointsView)
        pointsView->setVisible(value);
}

void HandView::setPoints(std::string pointsString)
{
    if (pointsLabel && pointsView)
    {
        pointsLabel->setString(pointsString);
        pointsView->setPreferredSize(pointsLabel->getContentSize() + Size(30, 30));
        pointsView->setContentSize(pointsView->getPreferredSize());
        pointsLabel->setPosition(Vec2(-3, 3) + pointsView->getContentSize() / 2);
    }
}

std::string HandView::getPointsString()
{
    auto totals = getTotal();
    
    if (totals.size() == 1)
        return StringUtils::format("%d", totals[0]);
    
    return StringUtils::format("%d/%d", totals[0], totals[1]);
}

void HandView::addBet(CoinToken * token, bool animated /*= true*/)
{
    auto startPoint = betNode->getParent()->convertToNodeSpace(Vec2(convertToWorldSpace(Vec2::ZERO).x, - token->getContentSize().height / 2));
    auto newToken   = token->getTopTokenClone();
    
    newToken->setPosition(startPoint);
    betNode->getParent()->addChild(newToken);
    betNode->addToken( token->getTotalValue(), token->getSkin(), nullptr);
    
    //animation
    newToken->runAction(Sequence::create(MoveTo::create(0.25f, betNode->getPosition()), CallFuncN::create([this](Node * node)
     {
         AudioManager::getInstance().PlaySoundEffect("music/Sounds/effect");
         betNode->setVisible(true);
         node->removeFromParent();
         
     }), NULL));
}

bool HandView::addInsurance()
{
    if (getInsurance() == 0)
    {
        int value = (0.5f + getBet() / 2);
        
        auto token = CoinToken::create(value, CoinToken::CoinTokenType::NORMAL, CoinToken::CoinTokenSkin::GREEN, nullptr);
        
        auto startPoint = insuranceNode->getParent()->convertToNodeSpace(Vec2(convertToWorldSpace(Vec2::ZERO).x, - token->getContentSize().height / 2));
        
        token->setPosition(startPoint);
        insuranceNode->getParent()->addChild(token);
        insuranceNode->addToken( token->getTotalValue(), token->getSkin(), nullptr);
        
        //animation
        token->runAction(Sequence::create(MoveTo::create(0.25f, insuranceNode->getPosition()), CallFuncN::create([this](Node * node)
        {
            AudioManager::getInstance().PlaySoundEffect("music/Sounds/effect");
            insuranceNode->setVisible(true);
            node->removeFromParent();
        }), NULL));
        
        return true;
    }
    
    return false;
}

int HandView::getBet()
{
    return betNode->getTotalValue();
}

int HandView::getInsurance()
{
    return insuranceNode->getTotalValue();
}

bool HandView::onTokenButtonPressed(CoinToken::CoinTokenEvent event, CoinToken * token)
{
    if (cards.size() > 0)
        return false;
    
    if (event == CoinToken::CoinTokenEvent::CANCELED)
    {
        auto removedToken = token->getTopTokenClone();
        
        token->popToken();
        token->setVisible(token->getTotalValue() > 0);
        
        auto scene = BaseScene::findBaseScene();
        if (scene)
        {
            scene->increasePlayerValues(removedToken->getTotalValue(), 0, false);
        }
        
        return true;
    }
    
    return false;
}

void HandView::addPayout(CoinToken * token, float delay)
{
    auto startPoint = payoutNode->getParent()->convertToNodeSpace(Vec2(Director::getInstance()->getWinSize().width / 2,
                                                                       Director::getInstance()->getWinSize().height + token->getContentSize().height / 2));
    
    token->setPosition(startPoint);
    payoutNode->getParent()->addChild(token);
    payoutNode->addToken(token->getTotalValue(), token->getSkin(), nullptr);
    
    //animation
    token->runAction(Sequence::create(DelayTime::create(delay), MoveTo::create(0.25f, payoutNode->getPosition()), CallFuncN::create([this](Node * node)
     {
         AudioManager::getInstance().PlaySoundEffect("music/Sounds/effect");
         payoutNode->setVisible(true);
         node->removeFromParent();
         
     }), NULL));
}

void HandView::takeBet(float delay)
{
    if (getBet() > 0)
    {
        auto token = CoinToken::create(betNode->getTotalValue(), CoinToken::CoinTokenType::NORMAL, betNode->getSkin(), nullptr);
        betNode->reset();
        betNode->setVisible(false);
        
        auto startPoint = betNode->getParent()->convertToNodeSpace(Vec2(Director::getInstance()->getWinSize().width / 2,
                                                                        Director::getInstance()->getWinSize().height + token->getContentSize().height / 2));
        
        token->setPosition(betNode->getPosition());
        betNode->getParent()->addChild(token);
        
        token->runAction(Sequence::create(DelayTime::create(delay), MoveTo::create(0.25f, startPoint), RemoveSelf::create(), NULL));
    }
}

void HandView::takeHalfBet(float delay)
{
    if (getBet() > 0)
    {
        auto token = CoinToken::create(betNode->getTotalValue() / 2, CoinToken::CoinTokenType::NORMAL, betNode->getSkin(), nullptr);
        
        betNode->reset();
        betNode->addToken(token->getTotalValue(), token->getSkin(), nullptr);
        
        auto startPoint = betNode->getParent()->convertToNodeSpace(Vec2(Director::getInstance()->getWinSize().width / 2,
                                                                        Director::getInstance()->getWinSize().height + token->getContentSize().height / 2));
        
        token->setPosition(betNode->getPosition());
        betNode->getParent()->addChild(token);
        
        token->runAction(Sequence::create(DelayTime::create(delay), MoveTo::create(0.25f, startPoint), RemoveSelf::create(), NULL));
    }
}

void HandView::takeInsurance(float delay)
{
    if (getInsurance() > 0)
    {
        auto token = CoinToken::create(insuranceNode->getTotalValue(), CoinToken::CoinTokenType::NORMAL, insuranceNode->getSkin(), nullptr);
        insuranceNode->reset();
        insuranceNode->setVisible(false);
        
        auto startPoint = insuranceNode->getParent()->convertToNodeSpace(Vec2(Director::getInstance()->getWinSize().width / 2,
                                                                              Director::getInstance()->getWinSize().height + token->getContentSize().height / 2));
        
        token->setPosition(insuranceNode->getPosition());
        insuranceNode->getParent()->addChild(token);
        
        token->runAction(Sequence::create(DelayTime::create(delay), MoveTo::create(0.25f, startPoint), RemoveSelf::create(), NULL));
    }
}

int HandView::grabAllTokens(float delay, bool animate)
{
    int result = 0;
    
    auto destPoint = insuranceNode->getParent()->convertToNodeSpace(Vec2(convertToWorldSpace(Vec2::ZERO).x, -100));
    
    Vector<CoinToken*> tokens;
    
    if (getBet() > 0)
    {
        result += getBet();
        
        tokens.pushBack(betNode);
    }
    
    if (getInsurance() > 0)
    {
        result += getInsurance();
        
        tokens.pushBack(insuranceNode);
    }
    
    if (payoutNode->getTotalValue() > 0)
    {
        result += payoutNode->getTotalValue();
        
        tokens.pushBack(payoutNode);
    }
    
    // animate
    if (animate)
    {
        for (auto &node: tokens)
        {
            auto token = CoinToken::create(node->getTotalValue(), CoinToken::CoinTokenType::NORMAL, node->getSkin(), nullptr);
            
            node->runAction(Sequence::create(DelayTime::create(delay), CallFunc::create([node]
            {
                AudioManager::getInstance().PlaySoundEffect("music/Sounds/effect");
                node->reset();
                node->setVisible(false);
            }), NULL));
            
            token->setPosition(node->getPosition());
            node->getParent()->addChild(token);
            
            token->runAction(Sequence::create(DelayTime::create(delay), MoveTo::create(0.25f, destPoint), RemoveSelf::create(), NULL));
        }
    }
    else
    {
        for (auto &node: tokens)
        {
            AudioManager::getInstance().PlaySoundEffect("music/Sounds/effect");
            node->reset();
            node->setVisible(false);
        }
    }
    
    return result;
}

void HandView::removeCards(float delay, bool animated /* = true*/)
{
    cards.clear();
    
    if (animated)
    {
        auto destPoint = cardsRoot->convertToNodeSpace(Vec2(Director::getInstance()->getVisibleOrigin().x - 100, Director::getInstance()->getWinSize().height + 100));
        
        for (auto &card: cardSprites)
        {
            card->runAction(Sequence::create(DelayTime::create(delay), MoveTo::create(0.25f, destPoint), RemoveSelf::create(), NULL));
        }
    }
    
    cardSprites.clear();
    
    showPoints(false);
}

HandView::HandComparisonResult HandView::compareTo(HandView * dealer)
{
    auto statusOwn    = GetStatus();
    auto statusDealer = dealer->GetStatus();
    
    // parity cases
    if (statusOwn == statusDealer && statusOwn == BUST)
        return EQUALS;
    
    if (statusOwn == statusDealer && statusOwn == BLACKJACK)
    {
        if (getInsurance() > 0)
            return FAILED;
        
        return EQUALS;
    }
    
    if (statusOwn == statusDealer && statusOwn == VALUE21)
        return EQUALS;
    
    // bust cases
    if (statusOwn == BUST)
        return FAILED;
    
    if (statusDealer == BUST)
        return WON;
    
    // win cases
    if (statusDealer == BLACKJACK && statusOwn != BLACKJACK)
        return FAILED;
    
    // variants
    auto ownTotal = getTotal(true);
    int  ownHigh  = (*std::max_element(ownTotal.begin(), ownTotal.end()));
    
    auto dealerTotal = dealer->getTotal(true);
    int  dealerHigh  = (*std::max_element(dealerTotal.begin(), dealerTotal.end()));
    
    if (dealerHigh == ownHigh)
        return EQUALS;
    
    if (dealerHigh > ownHigh)
        return FAILED;
    
    return WON;
}

cocos2d::Size & HandView::getContentSize()
{
    return contentSize;
}
