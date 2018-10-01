//
//  BJPlayer.cpp
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 10.03.17.
//
//

#include "BJPlayer.h"
#include "PlayerProfile.h"
#include "BJConfig.h"
#include "BaseScene.h"

USING_NS_CC;

BJPlayer::BJPlayer() : totalWin(0), totalLoose(0)
{
    
}

BJPlayer * BJPlayer::create(std::string name)
{
    BJPlayer * pRet = new BJPlayer();
    if (pRet && pRet->init(name))
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

bool BJPlayer::init(std::string name)
{
    if (!Node::init())
        return false;
    
    this->Name = name;
    
    return true;
}


bool BJPlayer::CanPlaceBet(int bet)
{
    return bet <= PlayerProfile::getInstance().getCoins();
}

void BJPlayer::StartHand()
{
    auto newHand = HandView::create();
    
    addChild(newHand);
    
    hands.pushBack(newHand);
}

bool BJPlayer::isDealer()
{
    return false;
}

void BJPlayer::FinishRound()
{
    for (auto &hand: hands)
    {
        hand->removeFromParent();
    }
    hands.clear();
    
    totalWin = 0;
    totalLoose = 0;
}

bool BJPlayer::CanSplitHand(int handID)
{
    if (hands.size() > handID)
    {
        if (hands.at(handID)->CanSplit() && hands.size() < MAX_PLAYERS_HANDS)
        {
            return true;
        }
    }
    
    return false;
}

bool BJPlayer::CanSplitHand(HandView * hand)
{
    auto handID = hands.getIndex(hand);
    
    if (handID != UINT_MAX)
    {
        return CanSplitHand((int)handID);
    }
    
    return false;
}

HandView * BJPlayer::SplitHand(int handID, Shoe & shoe)
{
    if(CanSplitHand(handID))
    {
        auto scene = BaseScene::findBaseScene();
        if (scene)
        {
            scene->increasePlayerValues(-hands.at(handID)->getBet(), 0, false);
        }
        
        auto newHand = hands.at(handID)->Split();
        
        addChild(newHand);
        
        hands.insert(handID + 1, newHand);
        
        //adjust hands but hit card only splitted hands
        for (int i = 0; i < hands.size(); i++)
        {
            hands.at(i)->runAction(EaseSineInOut::create(MoveTo::create(0.5f, Vec2( (i - 0.5f * (hands.size() - 1)) * 300.0f, 0))));
            
            if (i == handID || i == handID + 1)
            {
                hands.at(i)->Hit(shoe.DrawCard(), (i + 1) * 1.0f, [i, this]
                {
                    hands.at(i)->showPoints(true);
                });
            }
        }
        
        return newHand;
    }
    else
    {
        printf("%s", "place bet error");
    }
    
    return nullptr;
}

HandView * BJPlayer::SplitHand(HandView * hand, Shoe & shoe)
{
    auto handID = hands.getIndex(hand);
    
    if (handID != UINT_MAX)
    {
        return SplitHand((int)handID, shoe);
    }
    
    return nullptr;
}

long BJPlayer::TotalBet(bool includingInsurance /* = false*/)
{
    long result = 0;
    
    for (auto &hand: hands)
    {
        result += hand->getBet();
        if (includingInsurance)
            result += hand->getInsurance();
    }
    
    return result;
}

cocos2d::Vector<HandView*> & BJPlayer::getHands()
{
    return hands;
}
