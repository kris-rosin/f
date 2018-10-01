//
//  SignupView.cpp
//
//  Created by Stanislav Zheronkin on 01.11.17.
//
//

#include "SignupView.h"

#include <iostream>

#include "AudioManager.h"
#include "PlayerProfile.h"
#include "LoadingScene.h"

#include "MessageAlert.h"
#include "Utilities.h"

#include "Features/AppManager.h"
#include "Features/mConstants.h"
#include "ServerConnection.h"
#include "ServerMessage.h"
#include "BaseScene.h"
#include "MessageAlert.h"
#include "BaseScene.h"

USING_NS_CC;

SignupView::SignupView()
{
    
}

SignupView * SignupView::create()
{
    SignupView * pRet = new SignupView();
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

bool SignupView::init()
{
    if (!Node::init())
        return false;
    
    setCascadeOpacityEnabled(true);
    
    auto background = Sprite::create("ui/Signup/sign-back.png");
    background->setAnchorPoint(Vec2::ZERO);
    background->setCascadeOpacityEnabled(true);
    addChild(background);
    setContentSize(background->getContentSize());
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    //close button
    auto closeButton = ui::Button::create("ui/MainMenu/close_button.png", "ui/MainMenu/close_button.png", "ui/MainMenu/close_button.png");
    closeButton->setName("close");
    closeButton->setPosition(background->getContentSize() - Size(10, 10));
    closeButton->addTouchEventListener(std::bind(&SignupView::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    background->addChild(closeButton);
    
    //page view
    pageView = ui::PageView::create();
    pageView->setContentSize(Size(901, 527));
    pageView->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    pageView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    pageView->setPosition(Vec2( 36, 40));
    background->addChild(pageView);
    
    //page 1
    auto page1 = ui::Layout::create();
    page1->setCascadeOpacityEnabled(true);
    page1->setContentSize(pageView->getContentSize());
    pageView->addPage(page1);
    
    auto signupButton = ui::Button::create("ui/Signup/signup.png", "ui/Signup/signup.png", "ui/Signup/signup.png");
    signupButton->setName("signup_page");
    signupButton->setPosition(Vec2(page1->getContentSize().width / 2, 363));
    signupButton->addTouchEventListener(std::bind(&SignupView::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    page1->addChild(signupButton);
    
    auto signinButton = ui::Button::create("ui/Signup/signin.png", "ui/Signup/signin.png", "ui/Signup/signin.png");
    signinButton->setName("signin_page");
    signinButton->setPosition(Vec2(page1->getContentSize().width / 2, 160));
    signinButton->addTouchEventListener(std::bind(&SignupView::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    page1->addChild(signinButton);
    
    auto signUpLabel = Label::createWithTTF("Sign-up to be able continue play on any your device.", "fonts/Berlin.ttf", 38);
    signUpLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    signUpLabel->enableWrap(true);
    signUpLabel->setMaxLineWidth(570);
    signUpLabel->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    signUpLabel->setPosition(Vec2(page1->getContentSize().width / 2, 460));
    signUpLabel->setColor(Color3B::WHITE);
    page1->addChild(signUpLabel);
    
    auto signInLabel = Label::createWithTTF("Already registered? Sign-in to restore your progress.", "fonts/Berlin.ttf", 38);
    signInLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    signInLabel->enableWrap(true);
    signInLabel->setMaxLineWidth(525);
    signInLabel->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    signInLabel->setPosition(Vec2(page1->getContentSize().width / 2, 258));
    signInLabel->setColor(Color3B::WHITE);
    page1->addChild(signInLabel);
    
    //page 2
    auto page2 = ui::Layout::create();
    page2->setCascadeOpacityEnabled(true);
    page2->setContentSize(pageView->getContentSize());
    pageView->addPage(page2);
    
    auto signupButton2 = ui::Button::create("ui/Signup/signup.png", "ui/Signup/signup.png");
    signupButton2->setName("signup");
    signupButton2->setPosition(Vec2(page2->getContentSize().width / 2, 144));
    signupButton2->addTouchEventListener(std::bind(&SignupView::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    page2->addChild(signupButton2);
    
    auto emailLabel = Label::createWithTTF("Email:", "fonts/Berlin.ttf", 38);
    emailLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    emailLabel->enableWrap(false);
    emailLabel->setPosition(Vec2(388, 490));
    emailLabel->setColor(Color3B::WHITE);
    page2->addChild(emailLabel);
    
    auto passLabel = Label::createWithTTF("Password:", "fonts/Berlin.ttf", 38);
    passLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    passLabel->enableWrap(false);
    passLabel->setPosition(Vec2(388, 410));
    passLabel->setColor(Color3B::WHITE);
    page2->addChild(passLabel);
    
    auto passLabel2 = Label::createWithTTF("Password again:", "fonts/Berlin.ttf", 38);
    passLabel2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    passLabel2->enableWrap(false);
    passLabel2->setPosition(Vec2(388, 330));
    passLabel2->setColor(Color3B::WHITE);
    page2->addChild(passLabel2);
    
    auto nameLabel = Label::createWithTTF("Your name in game:", "fonts/Berlin.ttf", 38);
    nameLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    nameLabel->enableWrap(false);
    nameLabel->setPosition(Vec2(388, 250));
    nameLabel->setColor(Color3B::WHITE);
    page2->addChild(nameLabel);
    
    auto fieldProto = Sprite::create("ui/Signup/text_field.png");
    float margin = fieldProto->getContentSize().height / 2;
    
    auto emailFieldBack = ui::Scale9Sprite::create("ui/Signup/text_field.png");
    emailFieldBack->setPreferredSize(Size(311, fieldProto->getContentSize().height));
    emailFieldBack->setPosition(Vec2(563, 490));
    emailFieldBack->setCascadeOpacityEnabled(true);
    page2->addChild(emailFieldBack);
    
    auto passFieldBack = ui::Scale9Sprite::create("ui/Signup/text_field.png");
    passFieldBack->setPreferredSize(Size(311, fieldProto->getContentSize().height));
    passFieldBack->setPosition(Vec2(563, 410));
    passFieldBack->setCascadeOpacityEnabled(true);
    page2->addChild(passFieldBack);
    
    auto pass2FieldBack = ui::Scale9Sprite::create("ui/Signup/text_field.png");
    pass2FieldBack->setPreferredSize(Size(311, fieldProto->getContentSize().height));
    pass2FieldBack->setPosition(Vec2(563, 330));
    pass2FieldBack->setCascadeOpacityEnabled(true);
    page2->addChild(pass2FieldBack);
    
    auto nameFieldBack = ui::Scale9Sprite::create("ui/Signup/text_field.png");
    nameFieldBack->setPreferredSize(Size(311, fieldProto->getContentSize().height));
    nameFieldBack->setPosition(Vec2(563, 250));
    nameFieldBack->setCascadeOpacityEnabled(true);
    page2->addChild(nameFieldBack);
    
    emailField = createTextField(emailFieldBack->getContentSize() - Size(fieldProto->getContentSize().height, 0), "", "user@domain.xyz");
    emailField->setPosition(Vec2(margin, margin));
    emailFieldBack->addChild(emailField);
    
    passField = createTextField(passFieldBack->getContentSize() - Size(fieldProto->getContentSize().height, 0), "", "", true);
    passField->setPosition(Vec2(margin, margin));
    passFieldBack->addChild(passField);
    
    passField2 = createTextField(pass2FieldBack->getContentSize() - Size(fieldProto->getContentSize().height, 0), "", "", true);
    passField2->setPosition(Vec2(margin, margin));
    pass2FieldBack->addChild(passField2);
    
    nameField = createTextField(nameFieldBack->getContentSize() - Size(fieldProto->getContentSize().height, 0), "", "Guest");
    nameField->setPosition(Vec2(margin, margin));
    nameFieldBack->addChild(nameField);
    
    //page 3
    auto page3 = ui::Layout::create();
    page3->setCascadeOpacityEnabled(true);
    page3->setContentSize(pageView->getContentSize());
    pageView->addPage(page3);
    
    auto signinButton2 = ui::Button::create("ui/Signup/signin.png", "ui/Signup/signin.png");
    signinButton2->setName("signin");
    signinButton2->setPosition(Vec2(page3->getContentSize().width / 2, 160));
    signinButton2->addTouchEventListener(std::bind(&SignupView::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
    page3->addChild(signinButton2);
    
    auto emailInLabel = Label::createWithTTF("Email:", "fonts/Berlin.ttf", 38);
    emailInLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    emailInLabel->enableWrap(false);
    emailInLabel->setPosition(Vec2(388, 410));
    emailInLabel->setColor(Color3B::WHITE);
    page3->addChild(emailInLabel);
    
    auto passInLabel = Label::createWithTTF("Password:", "fonts/Berlin.ttf", 38);
    passInLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    passInLabel->enableWrap(false);
    passInLabel->setPosition(Vec2(388, 330));
    passInLabel->setColor(Color3B::WHITE);
    page3->addChild(passInLabel);
    
    auto forgotLabel = Label::createWithTTF("I forgot my password", "fonts/Berlin.ttf", 38);
    forgotLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    forgotLabel->enableWrap(false);
    forgotLabel->setPosition(Vec2(page3->getContentSize().width / 2, 250));
    forgotLabel->setColor(Color3B::WHITE);
    page3->addChild(forgotLabel);
    
    //convert label to hyperlink
    auto hyperlinkListener = EventListenerTouchOneByOne::create();
    hyperlinkListener->setSwallowTouches(true);
    
    hyperlinkListener->onTouchBegan = [forgotLabel](Touch * touch, Event * event) {
        if (forgotLabel->getBoundingBox().containsPoint(forgotLabel->getParent()->convertTouchToNodeSpace(touch)))
        {
            forgotLabel->setColor(Color3B::BLUE);
            
            return true;
        }
        return false;
    };
    hyperlinkListener->onTouchEnded = [this, forgotLabel](Touch * touch, Event * event) {
        this->restorePassword();
        forgotLabel->setColor(Color3B::WHITE);
    };
    hyperlinkListener->onTouchCancelled = [this, forgotLabel](Touch * touch, Event * event) {
        forgotLabel->setColor(Color3B::WHITE);
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(hyperlinkListener, forgotLabel);
    
    
    auto emailFieldBack2 = ui::Scale9Sprite::create("ui/Signup/text_field.png");
    emailFieldBack2->setPreferredSize(Size(311, fieldProto->getContentSize().height));
    emailFieldBack2->setPosition(Vec2(563, 410));
    page3->addChild(emailFieldBack2);
    
    auto passFieldBack2 = ui::Scale9Sprite::create("ui/Signup/text_field.png");
    passFieldBack2->setPreferredSize(Size(311, fieldProto->getContentSize().height));
    passFieldBack2->setPosition(Vec2(563, 330));
    page3->addChild(passFieldBack2);
    
    emailFieldIn = createTextField(emailFieldBack2->getContentSize() - Size(fieldProto->getContentSize().height, 0), "", "user@domain.xyz");
    emailFieldIn->setPosition(Vec2(margin, margin));
    emailFieldBack2->addChild(emailFieldIn);
    
    passFieldIn = createTextField(passFieldBack2->getContentSize() - Size(fieldProto->getContentSize().height, 0), "", "", true);
    passFieldIn->setPosition(Vec2(margin, margin));
    passFieldBack2->addChild(passFieldIn);
    
    //disable signup if user is already registered
    auto m2 = PlayerProfile::getInstance().getUserName().substr(0, 5);
    
    signupButton2->setEnabled(PlayerProfile::getInstance().getEmail().empty());
    
    // enable modal behaviour
    auto eventListener = EventListenerTouchOneByOne::create();
    eventListener->setSwallowTouches(true);
    
    eventListener->onTouchBegan = [](Touch * touch, Event * event) {
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);
    
    return true;
}

void SignupView::show(cocos2d::Node * parent)
{
    auto scene = dynamic_cast<BaseScene*>(parent);
    
    if (scene == nullptr)
    {
        scene = BaseScene::findBaseScene();
    }
    
    if (scene)
    {
        scene->addChild(this, 100);
        
        setPosition(scene->getContentSize() / 2);
    }
}

void SignupView::hide()
{
    this->runAction(Sequence::create(FadeOut::create(0.2f), RemoveSelf::create(), nullptr));
}

void SignupView::onButtonPressed(Ref * sender, ui::Widget::TouchEventType event)
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

void SignupView::ProcessButtonAction(cocos2d::ui::Widget * button)
{
    if (button->getName() == "close")
    {
        button->setEnabled(false);
        
        PlayerProfile::getInstance().Save();
        
        hide();
    }
    else if (button->getName() == "signup_page")
    {
        pageView->scrollToPage(1);
    }
    else if (button->getName() == "signin_page")
    {
        pageView->scrollToPage(2);
    }
    else if (button->getName() == "signin")
    {
        if (validateEmailAddress(emailFieldIn->getString()))
        {
            if (passFieldIn->getString().empty())
            {
                MessageAlert::create("Empty password!")->show(this);
            }
            else
            {
                auto message = new SigninMessage(emailFieldIn->getString(), passFieldIn->getString());
                ServerConnection::getInstance().sendMessage(message, this, std::bind(&SignupView::onSigninCompleted, this, std::placeholders::_1));
                delete message;
            }
        }
        else
        {
            MessageAlert::create("Invalid Email address!")->show(this);
        }
    }
    else if (button->getName() == "signup")
    {
        if (validateEmailAddress(emailField->getString()))
        {
            if (passField->getString().empty())
            {
                MessageAlert::create("Empty password!")->show(this);
            }
            else
            {
                if (passField->getString() != passField2->getString())
                {
                    MessageAlert::create("Passwords are not equal!")->show(this);
                }
                else
                {
                    auto message = new SignupMessage(emailField->getString(), passField->getString(), nameField->getString());
                    ServerConnection::getInstance().sendMessage(message, this, std::bind(&SignupView::onRegistrationCallback, this, std::placeholders::_1));
                    delete message;
                }
            }
        }
        else
        {
            MessageAlert::create("Invalid Email address!")->show(this);
        }
    }
}

ui::TextField * SignupView::createTextField(cocos2d::Size textArea, std::string text, std::string placeholder, bool password)
{
    auto editTitle = ui::TextField::create(placeholder, "Berlin.ttf", 22);
    editTitle->ignoreContentAdaptWithSize(false);
    editTitle->setContentSize(textArea);
    editTitle->setTextAreaSize(textArea);
    editTitle->setTextHorizontalAlignment(TextHAlignment::LEFT);
    editTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
    editTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    editTitle->setTextColor(Color4B::WHITE);
    editTitle->setMaxLength(50);
    editTitle->setMaxLengthEnabled(true);
    editTitle->setString(text);
    editTitle->setCursorEnabled(true);
    editTitle->setCursorChar('|');
    editTitle->setPasswordEnabled(password);

    return editTitle;
}

bool SignupView::validateEmailAddress(std::string email)
{
    bool localPart = true;
    
    size_t dotPosition = -1;
    size_t dogPosition = -1;
    
    std::string validLocalSymbols  = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!#$%&'*+-/=?^_`{|}~.";
    std::string validDomainSymbols = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-.";
    
    for(size_t i = 0; i < email.length();)
    {
        int cplen = 1;
        if((email[i] & 0xf8) == 0xf0) cplen = 4;
        else if((email[i] & 0xf0) == 0xe0) cplen = 3;
        else if((email[i] & 0xe0) == 0xc0) cplen = 2;
        if((i + cplen) > email.length()) cplen = 1;
        
        std::string symbol = email.substr(i, cplen);
        
        if (symbol == "@")
        {
            if (dogPosition != -1) return false; //only one symbol allowed!
            
            dogPosition = i;
            localPart = false;
        }
        else
        {
            if (symbol == ".") dotPosition = i;
            
            if ( (localPart ? validLocalSymbols : validDomainSymbols).find(symbol) == std::string::npos)
            {
                std::cout << "Invalid Symbol detected: " << email.substr(i, cplen) << std::endl;
                return false;
            }
        }
        
        i += cplen;
    }
    
    return (dogPosition < dotPosition && dogPosition != -1 && dotPosition != -1);
}

void SignupView::restorePassword()
{
    if (emailFieldIn->getString().empty() || validateEmailAddress(emailFieldIn->getString()) == false)
    {
        MessageAlert::create("Enter valid Email address into email field first!")->show(this);
        return;
    }
    
    auto message = new PasswordRecoveryMessage(emailFieldIn->getString());
    ServerConnection::getInstance().sendMessage(message, this, std::bind(&SignupView::onPasswordRestored, this, std::placeholders::_1));
    delete message;
}

bool SignupView::onSigninCompleted(ServerMessage * message)
{
    SigninResult * result = dynamic_cast<SigninResult*>(message);
    
    if (result)
    {
        switch (result->getResult())
        {
            case SigninResultType::SuccessfullyLoggedin:
            {
                auto scene = LoadingScene::createScene();
                Director::getInstance()->replaceScene(scene);
                return true;
            }
                break;
                
            case SigninResultType::WrongLoginOrPassword:
            {
                MessageAlert::create("Wrong login or password!")->show(this);
            }
                break;
                
            default:
            {
                MessageAlert::create("Unexpected error occured, try again later.")->show(this);
            }
                break;
        }
    }
    
    return true;
}

bool SignupView::onRegistrationCallback(ServerMessage * message)
{
    SigninResult * result = dynamic_cast<SigninResult*>(message);
    
    if (result)
    {
        switch (result->getResult())
        {
            case SigninResultType::SuccessfullyLoggedin:
            {
                MessageAlert::create("We sent email to given address. Please, activate your account by clicking hyperlink in a letter.")->show(this);
                
                if (nameField->getString().empty() == false)
                {
                    PlayerProfile::getInstance().setUserName(nameField->getString());
                }
                
                PlayerProfile::getInstance().setUserLogin(emailField->getString());
                PlayerProfile::getInstance().Save();
            }
                break;
                
            case SigninResultType::LoginAlreadyInUse:
            {
                MessageAlert::create("Given Email address in use already!")->show(this);
            }
                break;
             
            case SigninResultType::WrongLoginOrPassword:
            {
                MessageAlert::create("Wrong login or password!")->show(this);
            }
                break;
                
            default:
            {
                MessageAlert::create("Unexpected error occured, try again later.")->show(this);
            }
                break;
        }
    }
    
    return true;
}

bool SignupView::onPasswordRestored(ServerMessage * message)
{
    auto result = dynamic_cast<GenericRequestResult*>(message);
    
    if (result)
    {
        if (result->getResult())
        {
            MessageAlert::create("Instrustion about password reset procedure was sent to you email address.")->show(this);
        }
        else
        {
            MessageAlert::create("Wrong email address.")->show(this);
        }
        
        return true;
    }
    
    MessageAlert::create("Unexpected server error. Please, try again.")->show(this);
    
    return false;
}
