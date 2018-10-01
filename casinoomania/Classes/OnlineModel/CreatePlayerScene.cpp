//
//  CreatePlayerScene.cpp
//  DovemobiCasino
//
//  Created by liu on 9/7/17.
//
//

#include "CreatePlayerScene.hpp"

#include "App42API.h"

#include "GameHandler.hpp"
#include "App42Define.h"
#include "CapsaDefine.h"
#include "Label64.hpp"
#include "CapsaThemeScene.hpp"


//#include "SafeguardAccountScene.hpp"
//#include "GameScene.hpp"

Scene * CreatePlayerScene::createScene()
{
    auto scene = Scene::create();
    auto layer = CreatePlayerScene::create();
    scene->addChild(layer);
    return scene;
}

bool CreatePlayerScene::init()
{
    if (!Layer::init())
        return false;
    
    requestAllUsers();
    setupUI();
    
    return true;
}

void CreatePlayerScene::onEnter()
{
    Layer::onEnter();
    //    m_txtPlayerName->touchDownAction(NULL, ui::Widget::TouchEventType::ENDED);
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//    setKeypadEnabled(true);
//    auto pKeybackListener = EventListenerKeyboard::create();
//    pKeybackListener->onKeyReleased = CC_CALLBACK_2(CreatePlayerScene::onKey, this);
//    getEventDispatcher()->addEventListenerWithSceneGraphPriority(pKeybackListener, this);
#endif
}

void CreatePlayerScene::onExit()
{
    Layer::onExit();
}

void CreatePlayerScene::setupUI()
{
//    DrawNode *bgNode = DrawNode::create();
//    addChild(bgNode, -1);
//    bgNode->drawSolidRect(Vec2::ZERO, WIN_SIZE, BACKGROUND_COLOR);
    
    float fontSize = WIN_SIZE.height * 0.04f;
    char szDescription[256];
    getLocalString("Create a player name\n by typing in this box", szDescription);
    Label *lblDescription = createLabelWithMultiString(szDescription, fontSize);
    addChild(lblDescription);
    lblDescription->setPosition(Vec2(WIN_SIZE.width * 0.5f, WIN_SIZE.height * 0.9f));
//    lblDescription->setColor(BACKGROUND_TEXT_COLOR);
    lblDescription->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    
    m_txtPlayerName = ui::EditBox::create(Size(WIN_SIZE.width * 0.8f, fontSize * 1.5f), ui::Scale9Sprite::create("ui/Dummy/btn_lightblue.png"));
    addChild(m_txtPlayerName);
    m_txtPlayerName->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    m_txtPlayerName->setPosition(Vec2(WIN_SIZE.width * 0.5f, WIN_SIZE.height *0.8f));
//    m_txtPlayerName->setTextHorizontalAlignment(TextHAlignment::CENTER);
    m_txtPlayerName->setMaxLength(PLAYERNAME_MAX_LEN);
    m_txtPlayerName->setFontSize(fontSize);
    m_txtPlayerName->setFontColor(Color3B::BLACK);
    m_txtPlayerName->setDelegate(this);
    
    //    colorNode->drawSolidRect(m_txtPlayerName->getBoundingBox().origin, m_txtPlayerName->getBoundingBox().origin + m_txtPlayerName->getBoundingBox().size, Color4F(0.3f, 0.3f, 0.3f, 1.0f));
    //
    char szHint[256];
    getLocalString("Tap on play when it turns\n to green to choose your name", szHint);
    Label *lblTapHint = createLabelWithMultiString(szHint, fontSize * 0.8f);
    addChild(lblTapHint);
    lblTapHint->setPosition(Vec2(WIN_SIZE.width * 0.5f, WIN_SIZE.height * 0.68f));
//    lblTapHint->setColor(BACKGROUND_TEXT_COLOR);
    lblTapHint->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    
    ui::Button * btnPlay = ui::Button::create("ui/Dummy/btn_blue.png");
    addChild(btnPlay);
    btnPlay->setPosition(Vec2(WIN_SIZE.width * 0.5f, WIN_SIZE.height * 0.55f));
    char szPlay[256];
    getLocalString("Play game", szPlay);
    Label *lblPlay = createLabelWithMultiString(szPlay, fontSize);
//    lblPlay->setColor(BACKGROUND_LIGHT_TEXT_COLOR);
    btnPlay->addChild(lblPlay);
    lblPlay->setPosition(Vec2(btnPlay->getContentSize().width * 0.5f, btnPlay->getContentSize().height * 0.5f));
    btnPlay->addClickEventListener(CC_CALLBACK_0(CreatePlayerScene::onPlay, this));
    m_playLabel = lblPlay;
    
    ui::Button * btnLogin = ui::Button::create("ui/Dummy/btn_blue.png");
    addChild(btnLogin);
    btnLogin->setPosition(Vec2(WIN_SIZE.width * 0.5f, WIN_SIZE.height * 0.35f));
    char szLogin[256];
    getLocalString("I have an account", szLogin);
    Label *lblLogin = createLabelWithMultiString(szLogin, fontSize);
//    lblLogin->setColor(BACKGROUND_LIGHT_TEXT_COLOR);
    btnLogin->addChild(lblLogin);
    lblLogin->setPosition(Vec2(btnLogin->getContentSize().width * 0.5f, btnLogin->getContentSize().height * 0.5f));
    btnLogin->addClickEventListener(CC_CALLBACK_0(CreatePlayerScene::onNormalLogin, this));
    
    ui::Button * btnLoginFacebook = ui::Button::create("ui/Dummy/btn_blue.png");
    addChild(btnLoginFacebook);
    btnLoginFacebook->setPosition(Vec2(WIN_SIZE.width * 0.5f, WIN_SIZE.height * 0.15f));
    char szFacebook[256];
    getLocalString("Login with Facebook", szFacebook);
    Label *lblLoginFacebook = createLabelWithMultiString(szFacebook, fontSize * 0.9);
//    lblLoginFacebook->setColor(BACKGROUND_LIGHT_TEXT_COLOR);
    btnLoginFacebook->addChild(lblLoginFacebook);
    lblLoginFacebook->setPosition(Vec2(btnLoginFacebook->getContentSize().width * 0.5f, btnLoginFacebook->getContentSize().height * 0.5f));
    btnLoginFacebook->addClickEventListener(CC_CALLBACK_0(CreatePlayerScene::onLoginWithFacebook, this));
}

void CreatePlayerScene::onPlay()
{
    if (m_isExistingUser)
    {
        char szString[256];
        getLocalString("Not Available", szString);
        m_playLabel->setString(szString);
        return;
    }
    
    std::string playerName = m_txtPlayerName->getText();
    trim(playerName);
    
    GameHandler *document = GameHandler::getInstance();
    document->showProgress(true);
    
    createUser(playerName);
}

void CreatePlayerScene::onNormalLogin()
{
//    RootScene::getInstance()->push(SafeguardAccountScene::create());
}

void CreatePlayerScene::onLoginWithFacebook()
{
//    sdkbox::PluginFacebook::setListener(this);
//    sdkbox::PluginFacebook::requestReadPermissions({sdkbox::FB_PERM_READ_PUBLIC_PROFILE, sdkbox::FB_PERM_READ_EMAIL});
}

/*
void CreatePlayerScene::getMeFromFB()
{
    sdkbox::FBAPIParam params;
    params["fields"] = "id,name,email,picture";
    sdkbox::PluginFacebook::api("me", "GET", params, "me");
}

void CreatePlayerScene::onPermission(bool isLogin, const std::string& msg)
{
    if (isLogin)
        getMeFromFB();
}

void CreatePlayerScene::onAPI(const std::string& key, const std::string& jsonData)
{
    cJSON *jsonRoot = cJSON_Parse(jsonData.c_str());
    cJSON *jsonID = cJSON_GetObjectItem(jsonRoot, "id");
    if (!jsonID)
        return;
    
    cJSON *jsonName = cJSON_GetObjectItem(jsonRoot, "name");
    cJSON *jsonEmail = cJSON_GetObjectItem(jsonRoot, "email");
    cJSON *jsonPicture = cJSON_GetObjectItem(jsonRoot, "picture");
    cJSON *jsonDat = NULL;
    cJSON *jsonURL = NULL;
    if (jsonPicture)
    {
        jsonDat = cJSON_GetObjectItem(jsonPicture, "data");
        if (jsonDat)
            jsonURL = cJSON_GetObjectItem(jsonDat, "url");
        if (jsonURL)
            m_pictureURL = jsonURL->valuestring;
    }
    
    char szPlayer[256];
    sprintf(szPlayer, "%s%s%s", jsonName->valuestring, FBUSER_PREFIX, jsonID->valuestring);
    
    unsigned char *szUserName = (unsigned char *)szPlayer;
    size_t len = strlen((const char *)szUserName);
    std::string playerName = base64_encode(szUserName, (unsigned int)len);
    m_encodedUserName = playerName;
    
    GameHandler *document = GameHandler::getInstance();
    document->showProgress(true);
    
    retain();
    
    UserService *userService = App42API::BuildUserService();
    userService->CreateUser(playerName.c_str(), "123456", jsonEmail->valuestring, app42callback(CreatePlayerScene::onCreateUserFromFB, this));
    
    cJSON_Delete(jsonRoot);
}
*/
void CreatePlayerScene::editBoxEditingDidBegin(cocos2d::ui::EditBox *editBox)
{
    char szString[256];
    getLocalString("Play game", szString);
    m_playLabel->setString(szString);
}

void CreatePlayerScene::editBoxReturn(ui::EditBox *editBox)
{
    
}

void CreatePlayerScene::editBoxTextChanged(ui::EditBox *editBox, const std::string &text)
{
    m_isExistingUser = false;
    
    std::string playerName = text;
    trim(playerName);
    
    if (!playerName.length())
    {
        m_playLabel->setColor(Color3B::WHITE);
        return;
    }
    
    for (std::vector<std::string>::iterator it = m_allUsers.begin(); it != m_allUsers.end(); ++it)
    {
        if (!it->compare(playerName))
        {
            m_isExistingUser = true;
            break;
        }
    }
    
    if (IS_CONTAIN_FBUSER_PRE(playerName))
        m_isExistingUser = true;
    
    m_playLabel->setColor(m_isExistingUser ? Color3B::WHITE : Color3B::GREEN);
}

void CreatePlayerScene::requestAllUsers()
{
    m_allUsers.clear();
    
    retain();
    
    UserService *userService = App42API::BuildUserService();
    userService->GetAllUsers(app42callback(CreatePlayerScene::onAllUsers, this));
}

void CreatePlayerScene::onAllUsers(void *response)
{
    App42UserResponse *userResponse = (App42UserResponse *)response;
    if (userResponse->isSuccess)
    {
        for (std::vector<App42User>::iterator it = userResponse->users.begin(); it != userResponse->users.end(); ++it)
        {
            std::string decodeUserName = base64_decode(it->userName);
            m_allUsers.push_back(decodeUserName);
        }
    }
    else
    {
        log("error details: %s", userResponse->errorDetails.c_str());
        log("error message: %s", userResponse->errorMessage.c_str());
    }
    
    release();
}

void CreatePlayerScene::createUser(const std::string &username)
{
    retain();
    
    unsigned char *szUserName = (unsigned char *)username.c_str();
    unsigned int len = strlen((const char *)szUserName);
    m_encodedUserName = base64_encode(szUserName, len);
    UserService *userService = App42API::BuildUserService();
    std::string tempPassword = "123456";
    std::string tempEmail = m_encodedUserName;
    userService->CreateUser(m_encodedUserName.c_str(), tempPassword.c_str(), tempEmail.c_str(), app42callback(CreatePlayerScene::onCreateUser, this));
}

void CreatePlayerScene::onCreateUser(void * response)
{
    GameHandler *document = GameHandler::getInstance();
    document->showProgress(false);
    
    App42UserResponse *userResponse = (App42UserResponse *)response;
    if (userResponse->isSuccess)
    {
        App42User user = userResponse->users.at(0);
        
        document->setPlayerName(user.userName);
        document->savePlayerName();
        
        Director::getInstance()->replaceScene(CapsaThemeScene::createScene());
    }
    else
    {
        log("error details: %s", userResponse->errorDetails.c_str());
        log("error message: %s", userResponse->errorMessage.c_str());
        
        std::string details = userResponse->errorDetails;
        if (details.find(m_encodedUserName) != string::npos)
        {
            replace(details, m_encodedUserName, base64_decode(m_encodedUserName));
        }
        
        MessageBox(details.c_str(), userResponse->errorMessage.c_str());
    }
    
    release();
}

void CreatePlayerScene::onCreateUserFromFB(void * response)
{
    /*
    GameHandler *handler = GameHandler::getInstance();
    handler->showProgress(false);
    
    App42UserResponse *userResponse = (App42UserResponse *)response;
    if (userResponse->isSuccess)
    {
        App42User user = userResponse->users.at(0);
        
        handler->setPlayerName(user.userName);
        handler->savePlayerName();
        handler->setPlayerEmail(user.email);
        
        if (m_pictureURL.length())
        {
            handler->showProgress(true);
            
            retain();
            
#ifdef USE_UPLOAD_SERVICE
            m_tempUser = user.userName;
            HttpRequest* request = new (std::nothrow) HttpRequest();
            request->setUrl(m_pictureURL.c_str());
            request->setRequestType(HttpRequest::Type::GET);
            request->setResponseCallback(CC_CALLBACK_2(CreatePlayerScene::onLoadedPictureFromURL, this));
            HttpClient::getInstance()->send(request);
#else
            AvatarService *avatarService = App42API::BuildAvatarService();
            avatarService->CreateAvatarFromWebURL(user.userName.c_str(), user.userName.c_str(), m_pictureURL.c_str(), "avatar", app42callback(CreatePlayerScene::onCreateAvatar, this));
#endif
        }
        else
            RootScene::getInstance()->popToRoot();
    }
    else
    {
        std::string details = userResponse->errorDetails;
        bool alreadyExist = details.find("already exist") != std::string::npos;
        if (userResponse->appErrorCode == 2005)
            alreadyExist = false;
        if (!alreadyExist)
            MessageBox(details.c_str(), userResponse->errorMessage.c_str());
        else
        {
            document->showProgress(true);
            retain();
            
            UserService *userService = App42API::BuildUserService();
            userService->GetUser(m_encodedUserName.c_str(), app42callback(CreatePlayerScene::onCreateUserFromFB, this));
            
        }
    }
    
    release();
     */
}

void CreatePlayerScene::onCreateAvatar(void *response)
{
#ifdef USE_UPLOAD_SERVICE
    App42UploadResponse *avatarResponse = (App42UploadResponse *)response;
#else
    App42AvatarResponse *avatarResponse = (App42AvatarResponse *)response;
#endif
    if (avatarResponse->isSuccess)
    {
        
    }
    else
    {
        log("error details: %s", avatarResponse->errorDetails.c_str());
        log("error message: %s", avatarResponse->errorMessage.c_str());
    }
    
    GameHandler::getInstance()->showProgress(false);
    Director::getInstance()->popScene();
    
    release();
}

void CreatePlayerScene::onLoadedPictureFromURL(HttpClient *sender, HttpResponse *response)
{
    if (!response->isSucceed())
    {
        log("onHttpRequestCompleted - Error buffer: %s", response->getErrorBuffer());
        release();
        return;
    }
    log("onHttpRequestCompleted - Response code: %s", response->getResponseDataString());
    
    std::vector<char> *buffer = response->getResponseData();
    Image * image = new  Image ();
    image-> initWithImageData ( reinterpret_cast<const unsigned char*>(&(buffer->front())), buffer->size());
    if (image->getData())
    {
        retain();
        std::string filePath = FileUtils::getInstance()->getWritablePath().append("avatartemp.png");
        image->saveToFile(filePath);
        UploadService *uploadService = App42API::BuildUploadService();
        uploadService->UploadFileForUser(AVATAR, m_tempUser.c_str(), filePath.c_str(), FileType::IMAGE, AVATAR, app42callback(CreatePlayerScene::onCreateAvatar, this));
    }
    
    release();
}
