//
//  CapsaCreateRoomLayer.hpp
//  DovemobiCasino
//
//  Created by Admin on 10/19/17.
//
//

#ifndef CapsaCreateRoomLayer_hpp
#define CapsaCreateRoomLayer_hpp

#include "cocos2d.h"
USING_NS_CC;

#include "ui/CocosGUI.h"

class CapsaCreateRoomLayer : public Layer
{
public:
    virtual
    bool        init();
    virtual
    void        onEnter() override;

    
    CREATE_FUNC(CapsaCreateRoomLayer);
    
    void        setCallback(const ui::Widget::ccWidgetClickCallback & callbackCreate,
                            const ui::Widget::ccWidgetClickCallback & callbackRule,
                            const ui::Widget::ccWidgetClickCallback & callbackHelp);

    bool        isFastSpeed();
    int         getMaxPlayers();
    bool        isPrivateRoom();
    int         getDeposite();
    
private:
    ui::Button *    m_btnCreate;
    ui::Button *    m_btnRule;
    ui::Button *    m_btnHelp;
    
    MenuItemToggle *    m_toggleSpeed;
    MenuItemToggle *    m_toggleMaxPlayers;
    MenuItemToggle *    m_togglePrivateRoom;
    
    bool                m_isFastSpeed;
    int                 m_maxPlayers;
    bool                m_isPrivateRoom;
    int                 m_deposite;
    
private:
    void        onCloseDialog();
    
    void        onToggleSpeed();
    void        onToggleMaxPlayers();
    void        onTogglePrivateRoom();
    
    void        setupTouch();

};

#endif /* CapsaCreateRoomLayer_hpp */
