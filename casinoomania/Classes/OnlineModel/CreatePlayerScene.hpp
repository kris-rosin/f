//
//  CreatePlayerScene.hpp
//  DovemobiCasino
//
//  Created by liu on 9/7/17.
//
//

#ifndef CreatePlayerScene_hpp
#define CreatePlayerScene_hpp

#include "cocos2d.h"

#include "ui/CocosGUI.h"
#include "AvatarSprite.hpp"

//#include "PluginFacebook/PluginFacebook.h"

#include "cocos-ext.h"

using namespace cocos2d::network;

class CreatePlayerScene : public cocos2d::Layer, ui::EditBoxDelegate/*, sdkbox::FacebookListener*/
{
public:
    static
    Scene*      createScene();
    
    virtual
    bool        init() override;
    
    CREATE_FUNC(CreatePlayerScene);
    
    void        editBoxTextChanged(ui::EditBox* editBox, const std::string& text) override;
    void        editBoxReturn(ui::EditBox* editBox) override;
    void        editBoxEditingDidBegin(ui::EditBox* editBox) override;
    
protected:
    virtual
    void        onEnter() override;
    virtual
    void        onExit() override;
    
    void        onPlay();
    void        onNormalLogin();
    void        onLoginWithFacebook();
    
    void        onLoadedPictureFromURL(HttpClient *sender, HttpResponse *response);
    
private:
    void        setupUI();
    
    void        getMeFromFB();
    
    ui::EditBox *   m_txtPlayerName;
    Label *         m_playLabel;
    
    std::vector<std::string> m_allUsers;
    
    bool        m_isExistingUser;
    
#pragma mark request & callback for App42
    void        requestAllUsers();
    void        onAllUsers(void *response);
    
    void        createUser(const std::string & username);
    void        onCreateUser(void *response);
    void        onCreateUserFromFB(void *response);
    void        onCreateAvatar(void *response);
    
    std::string m_encodedUserName;
    
private:
    /*
#pragma mark FacebookListener
    virtual void onLogin(bool isLogin, const std::string& msg) override {};
    virtual void onSharedSuccess(const std::string& message) override {};
    virtual void onSharedFailed(const std::string& message) override {};
    virtual void onSharedCancel() override {};
    virtual void onAPI(const std::string& key, const std::string& jsonData) override;
    virtual void onPermission(bool isLogin, const std::string& msg) override ;
    virtual void onFetchFriends(bool ok, const std::string& msg) override {};
    virtual void onRequestInvitableFriends( const sdkbox::FBInvitableFriendsInfo& friends ) override {};
    virtual void onInviteFriendsWithInviteIdsResult( bool result, const std::string& msg ) override {};
    virtual void onInviteFriendsResult( bool result, const std::string& msg ) override {};
    
    virtual void onGetUserInfo( const sdkbox::FBGraphUser& userInfo ) override {};
    */
    std::string m_pictureURL;
    std::string m_tempUser;
};

#endif /* CreatePlayerScene_hpp */
