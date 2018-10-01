//
//  DropdownList.cpp
//
//  Created by Stanislav Zheronkin on 18.11.16.
//
//

#include "DropdownList.h"

USING_NS_CC;

DropdownList::DropdownList() : _openButton(nullptr), _expandedSprite(nullptr), container(nullptr), _closeButton(nullptr), _scroll(nullptr), _label(nullptr)
{
    state = STATE_TYPE::STATE_CLOSED;
}

DropdownList * DropdownList::create(std::string normal, std::string opened, std::string disabled)
{
    DropdownList * pRet = new DropdownList();
    if (pRet && pRet->init(normal, opened, disabled))
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

bool DropdownList::init(std::string normal, std::string opened, std::string disabled)
{
    if (!Node::init())
        return false;
    
    setCascadeOpacityEnabled(true);
    
    //container
    container = ui::Layout::create();
    container->setAnchorPoint(Vec2::ZERO);
    container->setClippingEnabled(true);
    container->setClippingType(cocos2d::ui::Layout::ClippingType::SCISSOR);
    container->setCascadeOpacityEnabled(true);
    addChild(container);
    
    //normal setup
    _openButton = ui::Button::create(normal, normal, disabled);
    _openButton->setAnchorPoint(Vec2::ZERO);
    _openButton->setCascadeOpacityEnabled(true);
    container->addChild(_openButton);
    
    _openButton->addClickEventListener([this](Ref* sender)
    {
        this->expand();
    });
    
    container->setContentSize(_openButton->getContentSize());
    container->setClippingEnabled(true);
    
    setContentSize(_openButton->getContentSize());
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    //expanded setup
    _expandedSprite = Sprite::create(opened);
    _expandedSprite->setAnchorPoint(Vec2::ZERO);
    _expandedSprite->setVisible(false);
    container->addChild(_expandedSprite);
    
    _closeButton = ui::Button::create();
    _closeButton->setScale9Enabled(true);
    _closeButton->setContentSize(_openButton->getContentSize());
    _closeButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _closeButton->setPosition(Vec2(0, _expandedSprite->getContentSize().height));
    _expandedSprite->addChild(_closeButton);
    
    _closeButton->addClickEventListener([this](Ref* sender)
    {
        this->close();
    });
    
    //scroll
    _scroll = ui::ScrollView::create();
    _scroll->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
    _scroll->setContentSize(_expandedSprite->getContentSize() - Size(0, _openButton->getContentSize().height));
    _scroll->setInnerContainerSize(_scroll->getContentSize());
    _scroll->setAnchorPoint(Vec2::ZERO);
    _expandedSprite->addChild(_scroll);
    
    return true;
}

void DropdownList::expand()
{
    if (state == STATE_TYPE::STATE_CLOSED)
    {
        _openButton->setVisible(false);
        _expandedSprite->setVisible(true);
        
        state = STATE_TYPE::STATE_LOCKED;
        
        container->setContentSize(_expandedSprite->getContentSize());
        
        if (_label)
        {
            _label->retain();
            _label->removeFromParentAndCleanup(false);
            _expandedSprite->addChild(_label);
            _label->setPositionY(_expandedSprite->getContentSize().height - _openButton->getContentSize().height / 2);
            _label->release();
        }
        
        _expandedSprite->setPositionY(-_expandedSprite->getContentSize().height + _openButton->getContentSize().height);
        _expandedSprite->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(0.3f, Vec2::ZERO)),
                                                    CallFunc::create([this]() { _closeButton->setEnabled(true); state = STATE_TYPE::STATE_OPENED; }),
                                                    NULL));
    }
}

void DropdownList::close()
{
    if (state == STATE_TYPE::STATE_OPENED)
    {
        _closeButton->setEnabled(false);
        
        state = STATE_TYPE::STATE_LOCKED;
        
        Vec2 closePosition = Vec2(0, -_expandedSprite->getContentSize().height + _openButton->getContentSize().height);
        
        auto closeCallback = [this]()
        {
            _openButton->setVisible(true);
            _expandedSprite->setVisible(false);
            container->setContentSize(_openButton->getContentSize());
            state = STATE_TYPE::STATE_CLOSED;
            
            if (_label)
            {
                _label->retain();
                _label->removeFromParentAndCleanup(false);
                _openButton->addChild(_label);
                _label->setPositionY(_openButton->getContentSize().height / 2);
                _label->release();
            }
        };
        
        _expandedSprite->runAction(Sequence::create(EaseSineIn::create(MoveTo::create(0.3f, closePosition)),
                                                    CallFunc::create(closeCallback),
                                                    NULL));
    }
}

void DropdownList::toggle()
{
    if (state == STATE_OPENED)
    {
        close();
    }
    else if (state == STATE_CLOSED)
    {
        expand();
    }
}

// size is font size for TTF and scale for FNT
void DropdownList::setLabel(std::string text, std::string font, float size)
{
    if (_label)
    {
        _label->removeFromParentAndCleanup(true);
        _label = nullptr;
    }
    
    auto pos = font.find_last_of('.');
    std::string ext = ".ttf";
    
    if (pos != std::string::npos)
    {
        ext = font.substr(pos, font.length() - pos);
    }
    
    if (ext == ".fnt")
    {
        _label = Label::createWithBMFont(font, text);
        _label->setScale(size);
    }
    else
    {
        _label = Label::createWithTTF(text, font, size);
    }
    
    _openButton->addChild(_label);
    _label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    _label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _label->setDimensions(_openButton->getContentSize().width, _openButton->getContentSize().height);
    _label->setOverflow(cocos2d::Label::Overflow::SHRINK);
    _label->setPosition(Vec2(_openButton->getContentSize().width / 2, _openButton->getContentSize().height / 2));
    _label->setColor(Color3B::BLACK);
}

void DropdownList::addItemToList(cocos2d::Node * item)
{
    float height = item->getContentSize().height * 1.1f;
    float total_height = 0;
    
    //move all items up
    for (auto &child: _scroll->getInnerContainer()->getChildren())
    {
        child->setPositionY(child->getPositionY() + height);
        total_height += height;
    }
    
    _scroll->getInnerContainer()->addChild(item);
    item->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    item->setPosition(Vec2(_scroll->getInnerContainerSize().width / 2, height / 2));
    total_height += height;
    
    _scroll->setInnerContainerSize(Size(_scroll->getContentSize().width, total_height));
}

void DropdownList::setLabelText(std::string text)
{
    if (_label)
    {
        _label->setString(text);
    }
}

void DropdownList::setEnabled(bool enable)
{
    _openButton->setEnabled(enable);
}
