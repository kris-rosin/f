//
//  CongratulationsView.cpp
//
//  Created by Stanislav Zheronkin on 03.12.16.
//
//

#include "CongratulationsView.h"
#include "AudioManager.h"
#include "PlayerProfile.h"
#include "SlotsConfig.h"
#include "Features/mConstants.h"
#include "SharePhotoPopup.h"

#include "AndroidCompatibility.h"

USING_NS_CC;

CongratulationsView::CongratulationsView() : arrow(nullptr)
{
    
}

CongratulationsView * CongratulationsView::create(CongratulationType type, std::function< void() > callback)
{
    CongratulationsView * pRet = new CongratulationsView();
    if (pRet && pRet->init(type, callback))
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

bool CongratulationsView::init(CongratulationType type, std::function< void() > callback)
{
    if (!Node::init())
        return false;
    
    this->callback = callback;
    this->type = type;
    
    setCascadeOpacityEnabled(true);
    
    light = Sprite::create("effects/bigwin_light.png");
    light->setPosition(light->getContentSize() / 2);
    light->setScale(0.1f);
    addChild(light);
    
    if (type == LEVELUP)
    {
        arrow = Sprite::create("effects/levelup_arrow.png");
        arrow->setPosition(Vec2(light->getContentSize().width / 2, -arrow->getContentSize().height / 2) );
        arrow->setOpacity(0);
        addChild(arrow, 1);
    }
    
    std::vector< std::string > titles = { "bigwin", "bonus", "levelup" };
    
    title = Sprite::create(StringUtils::format("effects/%s_character.png", titles[type].c_str() ).c_str() );
    title->setPosition(light->getContentSize() / 2);
    title->setScale(0.1f);
    addChild(title, 2);
    
    switch (type)
    {
        case BIGWIN:
        {
            elements.resize(6);
            for (int i = 0; i < elements.size(); i++)
            {
                elements[i] = Sprite::create(StringUtils::format("effects/bigwin_star%d.png", i % 3).c_str());
                elements[i]->setScale(0.1f);
                elements[i]->setVisible(false);
                addChild(elements[i], 3);
            }
            
            Vec2 center = light->getContentSize() / 2;
            
            elements[0]->setPosition(center + Vec2(-431,  47)); //small (left)
            elements[1]->setPosition(center + Vec2( 392, -45)); //medium (rigth)
            elements[2]->setPosition(center + Vec2(-388, -45)); //big (left)
            elements[3]->setPosition(center + Vec2( 436,  47)); //small (right)
            elements[4]->setPosition(center + Vec2(-348, 111)); //medium (left)
            elements[5]->setPosition(center + Vec2( 350, 111)); //big (right)
        }
            break;
        
        case BONUS:
        {
            elements.resize(4);
            elements[0] = Sprite::create("effects/bonus_coins2.png");
            elements[1] = Sprite::create("effects/bonus_coins2.png");
            elements[2] = Sprite::create("effects/bonus_coins1.png");
            elements[3] = Sprite::create("effects/bonus_coins0.png");
            
            for (int i = 0; i < elements.size(); i++)
            {
                elements[i]->setScale(0.1f);
                elements[i]->setVisible(false);
                addChild(elements[i], 3);
            }
            
            Vec2 center = light->getContentSize() / 2;
            
            elements[0]->setPosition(center + Vec2( 285, -60));
            elements[1]->setPosition(center + Vec2(-319,   0));
            elements[2]->setPosition(center + Vec2( 263,  90));
            elements[3]->setPosition(center + Vec2( 358,  58));
        }
            break;
            
        case LEVELUP:
        {
            elements.resize(2);
            auto levelNumber = Label::createWithTTF(StringUtils::format("%d", PlayerProfile::getInstance().getLevel()), "fonts/Berlin.ttf", 100);
            levelNumber->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            levelNumber->setPosition(Vec2(title->getPositionX(), title->getPositionY() - title->getContentSize().height / 2 + 20.0f));
            levelNumber->setOpacity(0);
            levelNumber->enableShadow();
            addChild(levelNumber, 2);
            
            elements[0] = levelNumber;
            
            auto levelTip = Label::createWithTTF(tipForLevelUp(PlayerProfile::getInstance().getLevel()), "fonts/Berlin.ttf", 36);
            levelTip->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            levelTip->setPosition(Vec2(title->getPositionX(), levelNumber->getPositionY() - levelNumber->getContentSize().height - 10.0f));
            levelTip->setOpacity(0);
            levelTip->setTextColor(Color4B(245, 212, 90, 255));
            levelTip->enableShadow();
            addChild(levelTip, 2);
            
            elements[1] = levelTip;
        }
            break;
        default:
            break;
    }
    
    setContentSize(light->getContentSize());
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    return true;
}

void CongratulationsView::show(cocos2d::Node * parent)
{
    std::vector< std::string > sounds = { "BigWin", "Bonus", "LevelUp" };
    AudioManager::getInstance().PlaySoundEffect(StringUtils::format("music/SlotsSound/%s", sounds[type].c_str() ) );
    parent->addChild(this, 100);
    
    setPosition(parent->getContentSize() / 2);
    
    animate();
}

void CongratulationsView::hide()
{
    if (type == BIGWIN || type == LEVELUP)
    {
        auto screenshot = takeScreenshot();
        
        // Debug
        //screenshot->newImage()->saveToFile(FileUtils::getInstance()->getWritablePath() + "photo.png");
        if (type == BIGWIN) {
            /*int random = arc4random_uniform(100);
            if (random < 10) {*/
                SharePhotoPopup::create(screenshot)->show(this);
            //}
        } else {
            SharePhotoPopup::create(screenshot)->show(this);
        }
    }
    
    this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([this](){ if (callback) callback(); }), RemoveSelf::create(), NULL));
    
    light->runAction(ScaleTo::create(0.5f, 0.1f));
    title->runAction(ScaleTo::create(0.5f, 0.1f));
    
    if (arrow)
        arrow->runAction(ScaleTo::create(0.5f, 0.1f));
    
    for (int i = 0; i < elements.size(); i++)
    {
        elements[i]->runAction(ScaleTo::create(0.5f, 0.1f));
    }
}

cocos2d::RenderTexture * CongratulationsView::takeScreenshot()
{
    auto  wndSize = Director::getInstance()->getVisibleSize();
    
    RenderTexture * screenshot = RenderTexture::create((int)wndSize.width, (int)wndSize.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
    //screenshot->setContentSize(wndSize);
    screenshot->beginWithClear(0, 0, 0, 0, 1.0f);
    
    Director::getInstance()->getRunningScene()->setPosition(- Director::getInstance()->getVisibleOrigin());
    Director::getInstance()->getRunningScene()->visit();
    
    screenshot->end();
    
    Director::getInstance()->getRenderer()->render();
    
    Director::getInstance()->getRunningScene()->setPosition(Vec2::ZERO);
    
    return screenshot;
}

void CongratulationsView::animate()
{
    light->runAction(Sequence::create(ScaleTo::create(0.5f, 1), NULL));
    title->runAction(Sequence::create(DelayTime::create(0.25f), ScaleTo::create(0.5f, 1), NULL));
    if (arrow)
        arrow->runAction(Sequence::create(DelayTime::create(0.25f), Spawn::createWithTwoActions(FadeIn::create(0.25f),
                                                                                                MoveTo::create(0.5f, light->getPosition())), NULL));
    
    switch (type)
    {
        case BONUS:
        case BIGWIN:
        {
            for (int i = 0; i < elements.size(); i++)
            {
                elements[i]->runAction(Sequence::create(DelayTime::create(0.5f + 0.2f * i), Show::create(), ScaleTo::create(0.5f, 1.0f), i == (elements.size() - 1) ? CallFunc::create(std::bind(&CongratulationsView::hide, this)) : NULL, NULL));
            }
        }
            break;
            
        case LEVELUP:
        {
            for (int i = 0; i < elements.size(); i++)
            {
                elements[i]->runAction(Sequence::create(DelayTime::create(0.5f + 0.2f * i), Show::create(), FadeIn::create(0.5f), DelayTime::create(2.0f), i == (elements.size() - 1) ? CallFunc::create(std::bind(&CongratulationsView::hide, this)) : NULL, NULL));
            }
        }
            break;
    }
}

//hackyhack_bonus_level
std::string CongratulationsView::tipForLevelUp(int level)
{
    int coins = level * PlayerProfile::getInstance().getLevelUpBonus();
    
    std::string result = StringUtils::format("Got %d Coins", coins);

    std::string unlocked = SlotsConfig::getThemeUnlockedOnLevel(level);
    
    if (unlocked.empty() == false)
    {
        result.append(", Unlocked ").append(unlocked);
    }
    
    return result;
}
