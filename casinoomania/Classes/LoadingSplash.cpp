#include "LoadingSplash.h"

#include "MainMenuScene.h"
#include "SlotsSelectThemeScene.h"
#include "SlotsGameScene.h"
#include "RouletteSelectThemeScene.h"
#include "RouletteGameScene.h"
#include "BJSelectThemeScene.h"
#include "BJGameScene.h"
#include "CasinowarThemeScene.h"
#include "CasinowarGameScene.h"
#include "BaccaratSelectThemeScene.h"
#include "BaccaratGameScene.h"
#include "Features/mConstants.h"

#ifdef CAPSA_IN_DEVELOPING
 #include "CapsaThemeScene.hpp"
 #include "CapsaLobbyScene.hpp"
 #include "GameHandler.hpp"
 #include "CreatePlayerScene.hpp"
#endif

USING_NS_CC;

Scene * LoadingSplash::createScene(TargetSceneType target, std::string theme)
{
    auto scene = Scene::create();
    
    auto layer = LoadingSplash::create();
    layer->target = target;
    layer->theme  = theme;
    scene->addChild(layer);

    return scene;
}

bool LoadingSplash::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto winSize = Director::getInstance()->getWinSize();

    auto sprite = Sprite::create("ui/MainMenu/main_background.png");
    sprite->setPosition(winSize / 2);
    this->addChild(sprite, 0);
    
    auto label = Label::createWithTTF(GAME_TITLE, "fonts/Berlin.ttf", 64);
    label->setPosition(Vec2(winSize / 2) );
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    label->setColor(Color3B(255,255,255));
    addChild(label, 1);
    
    //animation
    Vector<SpriteFrame*> frames;
    
    for (int i = 1; i < 100; i++)
    {
        auto texture = Director::getInstance()->getTextureCache()->addImage(StringUtils::format("ui/Loading/loading%02d.png", i));
        
        if (texture == nullptr)
            break;
        
        auto frame = SpriteFrame::createWithTexture(texture, Rect(Vec2::ZERO, texture->getContentSize()));
        
        frames.pushBack(frame);
    }
    
    auto loading = Sprite::createWithSpriteFrame(frames.front());
    loading->setPosition(Vec2(winSize / 2) + Vec2(0, label->getContentSize().height / 2));
    label->setPosition(loading->getPosition() - Vec2(0, loading->getContentSize().height / 2));
    addChild(loading);
    
    auto anim = Animation::createWithSpriteFrames(frames);
    anim->setLoops(-1);
    anim->setDelayPerUnit(0.1f);
    
    loading->runAction(Animate::create(anim));
    
    delaySeconds = 0.5f;
    
    scheduleUpdate();
    
    return true;
}

void LoadingSplash::update(float deltaTime)
{
    delaySeconds -= deltaTime;
    
    if (delaySeconds <  0)
    {
        unscheduleUpdate();
        
        switch (target)
        {
            case MAIN_MENU:
            {
                auto scene = MainMenuScene::createScene();
                Director::getInstance()->replaceScene(scene);
                break;
            }
            case SLOTS_THEME:
            {
                auto scene = SlotsSelectThemeScene::createScene();
                Director::getInstance()->replaceScene(scene);
                break;
            }
            case ROULETTE_THEME:
            {
                auto scene = RouletteSelectThemeScene::createScene();
                Director::getInstance()->replaceScene(scene);
                break;
            }
            case BLACKJACK_THEME:
            {
                auto scene = BJSelectThemeScene::createScene();
                Director::getInstance()->replaceScene(scene);
                break;
            }
            case CASINOWAR_THEME:
            {
                auto scene = CasinowarThemeScene::createScene();
                Director::getInstance()->replaceScene(scene);
                break;
            }
            case BACCARAT_THEME:
            {
                auto scene = BaccaratSelectThemeScene::createScene();
                Director::getInstance()->replaceScene(scene);
                break;
            }
            // games
            case SLOTS_GAME:
            {
                auto scene = SlotsGameScene::createScene(theme, 30);
                Director::getInstance()->replaceScene(scene);
                break;
            }
            case ROULETTE_GAME:
            {
                auto scene = RouletteGameScene::createScene(theme);
                Director::getInstance()->replaceScene(scene);
                break;
            }
            case BLACKJACK_GAME:
            {
                auto scene = BJGameScene::createScene(theme);
                Director::getInstance()->replaceScene(scene);
                break;
            }
            case BACCARAT_GAME:
            {
                auto scene = BaccaratGameScene::createScene(theme);
                Director::getInstance()->replaceScene(scene);
                break;
            }
            case CASINOWAR_GAME:
            {
                auto scene = CasinowarGameScene::createScene(theme);
                Director::getInstance()->replaceScene(scene);
                break;
            }
#ifdef CAPSA_IN_DEVELOPING
            case CAPSA_THEME:
            {
                auto scene = CapsaThemeScene::createScene();
                Director::getInstance()->replaceScene(scene);
                break;
            }
                break;
            case CAPSA_GAME:
            {
                CapsaLobbyScene::setTheme(theme);
                auto scene = CapsaLobbyScene::createScene();
                Director::getInstance()->replaceScene(scene);
                break;
            }
                break;
#endif
            default:
                break;
        }
        
    }
}

