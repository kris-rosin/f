//
//  SharePhotoPopup.cpp
//
//  Created by Stanislav Zheronkin on 22.08.17.
//
//

#include "SharePhotoPopup.h"
#include "AudioManager.h"
#include "ShareManager.h"
#include "PlayerProfile.h"
#include "BaseScene.h"

USING_NS_CC;

SharePhotoPopup::SharePhotoPopup()
{
    
}

SharePhotoPopup * SharePhotoPopup::create(cocos2d::RenderTexture * screenshot)
{
    SharePhotoPopup * pRet = new SharePhotoPopup();
    if (pRet && pRet->init(screenshot))
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

bool SharePhotoPopup::init(cocos2d::RenderTexture * screenshot)
{
    if (!Node::init())
        return false;
    
    _photo = screenshot;
    
    setCascadeOpacityEnabled(true);
    
    auto background = Sprite::create("ui/SharePopup/share_win.png");
    background->setAnchorPoint(Vec2::ZERO);
    background->setCascadeOpacityEnabled(true);
    addChild(background);
    setContentSize(background->getContentSize());
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);

    //close button
//    auto button = ui::Button::create("ui/SharePopup/btn_share_no.png", "ui/SharePopup/btn_share_no.png");
//    button->setName("close");
//    button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    button->setPosition(Vec2(350, 50));
//    button->addTouchEventListener(std::bind(&SharePhotoPopup::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
//    background->addChild(button);
    
    //close button
    auto button = ui::Button::create("ui/MainMenu/close_button.png", "ui/MainMenu/close_button.png");
    button->setName("close");
    button->setAnchorPoint(Vec2(1.0f, 1.0f));
    button->setPosition(Vec2(getContentSize().width + 25, getContentSize().height + 25));
    button->addTouchEventListener(std::bind(&SharePhotoPopup::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    background->addChild(button);

    
    //share button
    auto share = ui::Button::create("ui/SharePopup/btn_share_yes.png", "ui/SharePopup/btn_share_yes.png");
    share->setName("share");
    share->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    share->setPosition(Vec2(getContentSize().width / 2, 70));
    share->addTouchEventListener(std::bind(&SharePhotoPopup::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    background->addChild(share);
    
    //preview
    const float maxw = 710;
    const float maxh = 500;
    
    float wratio = maxw / screenshot->getSprite()->getContentSize().width;
    float hratio = maxh / screenshot->getSprite()->getContentSize().height;
    
    float targetScale = std::min(wratio, hratio);
    
    screenshot->setScale(targetScale);
    screenshot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    screenshot->setPosition(Vec2(background->getContentSize().width / 2, 335));
    background->addChild(screenshot);
    
    // enable modal behaviour
    auto eventListener = EventListenerTouchOneByOne::create();
    eventListener->setSwallowTouches(true);
    
    eventListener->onTouchBegan = [](Touch * touch, Event * event)
    {
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);
    
    return true;
}

void SharePhotoPopup::show(cocos2d::Node * parent)
{
    auto scene = findScene(parent);
    
    if (scene)
    {
        scene->addChild(this, 100);
    
        setPosition(scene->getContentSize() / 2);
    }
}

void SharePhotoPopup::hide()
{
    this->runAction(Sequence::create(FadeOut::create(0.2f), RemoveSelf::create(), nullptr));
}

void SharePhotoPopup::onButtonPressed(Ref * sender, ui::Widget::TouchEventType event)
{
    auto button = dynamic_cast<ui::Button*>(sender);
    
    if (event == ui::Widget::TouchEventType::BEGAN)
    {
        AudioManager::getInstance().PlaySoundEffect("music/SlotsSound/TapBt");
        button->setColor(Color3B(178, 178, 178));
    }
    else if (event == ui::Widget::TouchEventType::ENDED)
    {
        button->setColor(Color3B::WHITE);
        
        ProcessButtonAction(button);
    }
    else if (event == ui::Widget::TouchEventType::CANCELED)
    {
        button->setColor(Color3B::WHITE);
    }
}

void SharePhotoPopup::ProcessButtonAction(cocos2d::ui::Widget * button)
{
    if (button->getName() == "close")
    {
        hide();
    }
    else if (button->getName() == "share")
    {
        auto image = _photo->newImage();
        
        ShareManager::getInstance().sharePhotoFacebook(image);
        
        auto scene = BaseScene::findBaseScene();
        if (scene)
        {
            scene->increasePlayerValues(500, 0, false);
        }
        
        hide();
    }
}

cocos2d::Scene * SharePhotoPopup::findScene(cocos2d::Node * node)
{
    if (node == nullptr)
        return nullptr;
    
    auto scene = dynamic_cast<Scene*>(node);
    if (scene)
    {
        return scene;
    }
    
    return findScene(node->getParent());
}
