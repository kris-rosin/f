//
//  ShareManager.h
//
//  Created by Stanislav Zheronkin on 23.08.17.
//

#pragma once

#include <string>
#include "cocos2d.h"

class ShareManager
{
private:
    ShareManager();
    ShareManager(const ShareManager & other);
    ShareManager & operator=(const ShareManager & other);
    ~ShareManager();
    
    cocos2d::RenderTexture * mScreenshot;
    bool isScreenshotTaken;
    
public:
    static ShareManager & getInstance();
    
    void sharePhotoFacebook(cocos2d::Image * image, std::string optionalURL = "");
    void shareURLFacebook(std::string url, std::function<void(bool)> callback, std::string optionalQuote = "");
    void releaseRetainScreenshot();
    
    cocos2d::RenderTexture * takeScreenshot();
    
    cocos2d::RenderTexture * getScreenshot();
    
};

