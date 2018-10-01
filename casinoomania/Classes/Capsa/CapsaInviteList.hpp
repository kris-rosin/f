//
//  CapsaInviteList.hpp
//  DovemobiCasino-mobile
//
//  Created by Frank on 11/8/17.
//

#ifndef CapsaInviteList_hpp
#define CapsaInviteList_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class CapsaInviteList : public cocos2d::Layer
{
private:
    CapsaInviteList();
    virtual ~CapsaInviteList();
    
public:
    static cocos2d::Scene * createScene();
    
    virtual bool init();
    
    void onClickLobby(Ref* sender);
    void onClickFriends(Ref* sender);
    void onClickClose(Ref* sender);
    
    void drawUsers(bool isFlag);
    void redrawUsers();
    
    CREATE_FUNC(CapsaInviteList);
    
public:
    ui::ScrollView* m_pScrollView;
    
    MenuItemImage*  m_pBtnLobby;
    MenuItemImage*  m_pBtnFriends;
    MenuItemImage*  m_pBtnClose;
    
    float           m_rScrollVH;
    
    bool            m_fSearchState;

};

#endif /* CapsaInviteList_hpp */
