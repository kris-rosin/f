//
//  ProcessLayer.hpp
//  2pb2
//
//  Created by liu on 6/9/17.
//
//

#ifndef ProcessLayer_hpp
#define ProcessLayer_hpp

#include "cocos2d.h"

class ProcessLayer : public cocos2d::LayerColor
{
protected:
    virtual
    void        onEnter() override;
    virtual
    void 		onExit() override;

public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual
    bool        init() override;
    
    void        setTitle(const std::string & title);
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(ProcessLayer);
    
private:
    void        setupTouch();
    
    cocos2d::Label *    m_lblTitle;
};

#endif /* ProcessLayer_hpp */
