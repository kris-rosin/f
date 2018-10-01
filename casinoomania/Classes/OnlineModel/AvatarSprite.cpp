//
//  AvatarSprite.cpp
//  2pb2
//
//  Created by liu on 6/15/17.
//
//

#include "AvatarSprite.hpp"

#include "App42API.h"
#include "App42Define.h"
#include "GameHandler.hpp"

#include "curl/curl.h"

AvatarSprite * AvatarSprite::create(const std::string & avatarName, bool isRival, bool isBig)
{
    AvatarSprite *avatarSprite = new (std::nothrow) AvatarSprite();
    if (avatarSprite && avatarSprite->initWithAvatarName(avatarName, isRival, isBig))
    {
        avatarSprite->autorelease();
        return avatarSprite;
    }
    CC_SAFE_DELETE(avatarSprite);
    return nullptr;
}

bool AvatarSprite::initWithAvatarName(const std::string &avatarName, bool isRival, bool isBig)
{
    std::string avatarFile = isBig ? "img_avatar_large.png" : "img_avatar_small.png";
    m_big = isBig;
    
    if (!Sprite::initWithFile(avatarFile))
        return false;
    
    setRival(isRival);
    
    if (!avatarName.length())
        return true;
    
    if (!m_avatarName.compare(avatarName))
        return true;
    
    m_avatarName = avatarName;
    
    std::string avatarDir = FileUtils::getInstance()->getWritablePath();
    std::stringstream ssAvatar;
    ssAvatar << avatarDir << avatarName << ".png";
    std::string filePath = ssAvatar.str();
    m_avatarPath = filePath;
    
    if (FileUtils::getInstance()->isFileExist(filePath))
    {
        Sprite *childSprite = Sprite::create(filePath);
        if (childSprite)
            adjustChildSprite(childSprite);
    }
    else
        requestAvatar();
    
    return true;
}

void AvatarSprite::refresh()
{
    if (!m_avatarPath.length())
    {
        GameHandler *handler = GameHandler::getInstance();
        if (handler->getPlayerName().length())
        {
            m_avatarName = handler->getPlayerName();
            
            std::string avatarDir = FileUtils::getInstance()->getWritablePath();
            std::stringstream ssAvatar;
            ssAvatar << avatarDir << m_avatarName << ".png";
            std::string filePath = ssAvatar.str();
            m_avatarPath = filePath;
        }
    }
    
    if (FileUtils::getInstance()->isFileExist(m_avatarPath))
    {
        Sprite *childSprite = Sprite::create(m_avatarPath);
        if (childSprite)
            adjustChildSprite(childSprite);
    }
    else
    {
        if (m_child)
        {
            m_child->removeFromParentAndCleanup(true);
            m_child = NULL;
        }
        requestAvatar();
    }
}

void AvatarSprite::setRival(bool rival)
{
//    setColor(rival ? AVATAR_RIVAL_COLOR : AVATAR_PLAYER_COLOR);
    m_rival = rival;
}

bool AvatarSprite::isRival()
{
    return m_rival;
}

void AvatarSprite::loadFromURL(const char *url)
{
    retain();

    log("onHttpRequestCompleted In the request");
    HttpRequest* request = new (std::nothrow) HttpRequest();
    request->setUrl(url);
    request->setRequestType(HttpRequest::Type::GET);
    request->setResponseCallback(CC_CALLBACK_2(AvatarSprite::onLoadedFromURL, this));
    HttpClient::getInstance()->send(request);
    request->release();
}

void AvatarSprite::onLoadedFromURL(HttpClient *sender, HttpResponse *response)
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
        Texture2D * texture = new Texture2D();
        texture->initWithImage(image);
        Sprite * sprite = Sprite::createWithTexture (texture);
        if (sprite)
        {
            adjustChildSprite(sprite);
            image->saveToFile(m_avatarPath);
        }
    }
    release();
}

void AvatarSprite::requestAvatar()
{
    if (!m_avatarName.length())
        return;
    /*
    if (IS_BOT_PLAYER(m_avatarName))
        return;
    */
    if (m_isUpdating)
        return;
    
    m_isUpdating = true;
    
    retain();
    
#ifdef USE_UPLOAD_SERVICE
    UploadService *uploadService = App42API::BuildUploadService();
    uploadService->GetFileByUser(AVATAR, m_avatarName.c_str(), app42callback(AvatarSprite::onRequestAvatar, this));
#else
    AvatarService *avatarService = App42API::BuildAvatarService();
    avatarService->GetAvatarByName(m_avatarName.c_str(), m_avatarName.c_str(), app42callback(AvatarSprite::onRequestAvatar, this));
#endif
}

void AvatarSprite::onRequestAvatar(void *response)
{
    m_isUpdating = false;
#ifdef USE_UPLOAD_SERVICE
    App42UploadResponse *uploadResponse = (App42UploadResponse *)response;
    if (uploadResponse->isSuccess)
    {
        if (uploadResponse->uploadArray.size())
        {
            App42Upload upload = uploadResponse->uploadArray.at(0);
            loadFromURL(upload.url.c_str());
        }
    }
    else
    {
        log("error details: %s", uploadResponse->errorDetails.c_str());
        log("error message: %s", uploadResponse->errorMessage.c_str());
    }
#else
    App42AvatarResponse *avatarResponse = (App42AvatarResponse *)response;
    if (avatarResponse->isSuccess)
    {
        if (avatarResponse->avatars.size())
        {
            App42Avatar avatar = avatarResponse->avatars.at(0);
            loadFromURL(avatar.url.c_str());
        }
    }
    else
    {
        log("error details: %s", avatarResponse->errorDetails.c_str());
        log("error message: %s", avatarResponse->errorMessage.c_str());
    }
#endif
    release();
}

void AvatarSprite::adjustChildSprite(Sprite * child)
{
    if (m_child)
    {
        m_child->removeFromParentAndCleanup(true);
        m_child = NULL;
    }

    m_usedTexture = child->getTexture();
    
    float kRatio = m_big ? 1.0f : 1.0f;
    child->setScale(getContentSize().width / child->getContentSize().width * kRatio, getContentSize().height /  child->getContentSize().height * kRatio);
    child->setPosition(getContentSize() * 0.5f);
    
    DrawNode * stencil = DrawNode::create();
    stencil->drawSolidCircle(child->getPosition(), getContentSize().width * 0.5f * kRatio, 0.0f, 2 << 10, Color4F::GREEN);
    
    ClippingNode * clipper = ClippingNode::create();
    clipper->setStencil(stencil);
    
    clipper->addChild(child);
    
    addChild(clipper);
    m_child = clipper;
}

void AvatarSprite::deleteAvatar()
{
    FileUtils::getInstance()->removeFile(m_avatarPath);
    
    GameHandler::getInstance()->showProgress(true);
    
    retain();
    
#ifdef USE_UPLOAD_SERVICE
    UploadService *uploadService = App42API::BuildUploadService();
    uploadService->RemoveFileByUser(AVATAR, m_avatarName.c_str(), app42callback(AvatarSprite::onDeleteAvatar, this));
#else
    AvatarService *avatarService = App42API::BuildAvatarService();
    avatarService->DeleteAvatarByName(m_avatarName.c_str(), m_avatarName.c_str(), app42callback(AvatarSprite::onDeleteAvatar, this));
#endif
}

void AvatarSprite::onDeleteAvatar(void *response)
{
    GameHandler::getInstance()->showProgress(false);
    
#ifdef USE_UPLOAD_SERVICE
    App42UploadResponse *uploadResponse = (App42UploadResponse *)response;
    if (uploadResponse->isSuccess)
    {
        if (m_usedTexture)
        {
            Director::getInstance()->getTextureCache()->removeTexture(m_usedTexture);
            m_usedTexture = NULL;
        }
        m_child->removeFromParentAndCleanup(true);
        m_child = NULL;
    }
#else
    App42AvatarResponse *avatarResponse = (App42AvatarResponse *)response;
    if (avatarResponse->isSuccess)
    {
        if (m_usedTexture)
        {
            Director::getInstance()->getTextureCache()->removeTexture(m_usedTexture);
            m_usedTexture = NULL;
        }
        m_child->removeFromParentAndCleanup(true);
        m_child = NULL;
    }
#endif
    
    release();
}

void AvatarSprite::updateAvatar(const std::string image)
{
    GameHandler::getInstance()->showProgress(true);
    retain();
#ifdef USE_UPLOAD_SERVICE
    UploadService *uploadService = App42API::BuildUploadService();
    if (m_usedTexture)
    {
        m_temp = image;
        uploadService->RemoveFileByUser(AVATAR, m_avatarName.c_str(), app42callback(AvatarSprite::onDeleteAvatarForUpload, this));
    }
    else
        uploadService->UploadFileForUser(AVATAR, m_avatarName.c_str(), image.c_str(), FileType::IMAGE, AVATAR, app42callback(AvatarSprite::onUpdateAvatar, this));
#else
    AvatarService *avatarService = App42API::BuildAvatarService();
    if (m_usedTexture)
    {
        m_temp = image;
        avatarService->DeleteAvatarByName(m_avatarName.c_str(), m_avatarName.c_str(), app42callback(AvatarSprite::onDeleteAvatarForUpload, this));
        return;
    }
    avatarService->CreateAvatar(m_avatarName.c_str(), m_avatarName.c_str(), image.c_str(), AVATAR, app42callback(AvatarSprite::onUpdateAvatar, this));
#endif
}

void AvatarSprite::onUpdateAvatar(void *response)
{
    GameHandler::getInstance()->showProgress(false);
    
    onRequestAvatar(response);
}

void AvatarSprite::onDeleteAvatarForUpload(void *response)
{
    FileUtils::getInstance()->removeFile(m_avatarPath);
    GameHandler::getInstance()->showProgress(false);

#ifdef USE_UPLOAD_SERVICE
    App42UploadResponse *uploadResponse = (App42UploadResponse *)response;
    if (uploadResponse->isSuccess)
#else
    App42AvatarResponse *avatarResponse = (App42AvatarResponse *)response;
    if (avatarResponse->isSuccess)
#endif
    {
        if (m_usedTexture)
        {
            Director::getInstance()->getTextureCache()->removeTexture(m_usedTexture);
            m_usedTexture = NULL;
        }
        updateAvatar(m_temp);
        m_temp = "";
    }
}
