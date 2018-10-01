#include "AppDelegate.h"
#include "LoadingScene.h"

#include "PlayerProfile.h"
#include "AudioManager.h"
#include "Platform.h"
#include "ServerConnection.h"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(1366, 768);

static cocos2d::Size smallResolutionSize  = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize  = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate() : backgroundTime(0)
{
}

AppDelegate::~AppDelegate() 
{
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("DovemobiCasino", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("DovemobiCasino");
#endif
        director->setOpenGLView(glview);
    }
    
    //setup resources, sets must be defined in ascending order
    std::vector<GFXSet> resources;
    resources.push_back(GFXSet("Normal",         "HD" , Size(1366, 768)));
    //resources.push_back(GFXSet("HighResolution", "HDR", Size(2732, 1536)));
    GFXSet selectedResources = resources[0];

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    auto frameSize = glview->getFrameSize();
    const float ratio16_9 = 16.0f / 9.0f;
    float currentRatio = frameSize.width / frameSize.height;
    
    if (currentRatio > ratio16_9) //ultra wide device detected
    {
        glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::SHOW_ALL);
    }
    else
    {
        glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    }
    
    //select best fit resources
    for (auto &item: resources)
    {
        if (frameSize.width <= item.resolution.width)
        {
            selectedResources = item;
            
            break;
        }
    }
    std::vector<std::string> searchpaths;
    searchpaths.push_back(selectedResources.folder);
    director->setContentScaleFactor(MIN(selectedResources.resolution.height / designResolutionSize.height, selectedResources.resolution.width / designResolutionSize.width));
    FileUtils::getInstance()->setSearchPaths(searchpaths);

    register_all_packages();
    
    PlayerProfile::getInstance().Load("profile.dat");
    
    // settings
    AudioManager::getInstance().setSoundsVolume(PlayerProfile::getInstance().getUIntValue("SettingsSounds", 1) == 1 ? 1 : 0);
    AudioManager::getInstance().setMusicVolume( PlayerProfile::getInstance().getUIntValue("SettingsMusic" , 1) == 1 ? 1 : 0);

    // create a scene. it's an autorelease object
    auto scene = LoadingScene::createScene();

    // add count app
    PlayerProfile::getInstance().addOpenCountApp(1);
    CCLOG("hackyhack: OpenCountApp = %ld", PlayerProfile::getInstance().getOpenCountApp());
    
    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    PlayerProfile::getInstance().Save();
    
    AudioManager::getInstance().PauseBackgroundMusic();
    
    backgroundTime = ::Platform::unixTime();
    
    SaveOnServer();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();

    AudioManager::getInstance().ResumeBackgroundMusic();
    
    int delta = ::Platform::unixTime() - backgroundTime;
    
    if (abs(delta) > 5 * 60) // 5 minutes
    {
        Director::getInstance()->popToRootScene();
        
        auto scene = LoadingScene::createScene();
        Director::getInstance()->replaceScene(scene);
    }
}

void AppDelegate::SaveOnServer()
{
    if (PlayerProfile::getInstance().getPlayerID() != 0)
    {
        auto data = PlayerProfile::getInstance().getDataFile();
        
        if (data)
        {
            auto message = new UserDataMessage(PlayerProfile::getInstance().getPlayerID(), PlayerProfile::getInstance().getUserName(), PlayerProfile::getInstance().getLevel(), PlayerProfile::getInstance().getExperience(), PlayerProfile::getInstance().getCoins(), ::Platform::unixTime(), PlayerProfile::getInstance().getFBUserID(), data);
            ServerConnection::getInstance().sendMessage(message, this, std::bind(&AppDelegate::onDataSaved, this, std::placeholders::_1));
            delete message;
        }
    }
}

bool AppDelegate::onDataSaved(ServerMessage * message)
{
    auto result = dynamic_cast<GenericRequestResult*>(message);
    
    if (result)
    {
        log("Data saved on Server!");
    }
    
    return true;
}
