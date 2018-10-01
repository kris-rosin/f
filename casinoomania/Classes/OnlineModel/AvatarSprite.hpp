//
//  AvatarSprite.hpp
//  2pb2
//
//  Created by liu on 6/15/17.
//
//

#ifndef AvatarSprite_hpp
#define AvatarSprite_hpp

#include "cocos2d.h"

USING_NS_CC;

#include "network/HttpClient.h"
#include "cocos-ext.h"

using namespace cocos2d::network;

// Note
// Avatar Service has some bugs. so i move upload service
// if you comment macro below, it will work as avatarservice, otherwise it works as upload service
#define USE_UPLOAD_SERVICE

class AvatarSprite : public Sprite
{
public:
    static AvatarSprite *   create(const std::string & avatarName, bool isRival = false, bool isBig = false);
    
    bool                    initWithAvatarName(const std::string & avatarName, bool isRival = false, bool isBig = false);
    
    
    void                    deleteAvatar();
    void                    updateAvatar(const std::string image);
    
    void                    setRival(bool rival);
    bool                    isRival();
    
    void                    refresh();
    void                    requestAvatar();
    
private:
    void    adjustChildSprite(Sprite * child);
    
    void                    loadFromURL(const char * url);
    void                    onLoadedFromURL(HttpClient *sender, HttpResponse *response);
    
#pragma mark App42  callback
    void    onRequestAvatar(void * response);
    
    void    onUpdateAvatar(void * response);
    void    onDeleteAvatar(void * response);
    void    onDeleteAvatarForUpload(void * response);
    
    std::string m_avatarName;
    std::string m_avatarPath;
    
    std::string m_temp;
    
    Node *  m_child;
    Texture2D * m_usedTexture;
    
    bool                    m_isUpdating;
    bool                    m_rival;
    
    bool                    m_big;
};

#endif /* AvatarSprite_hpp */
