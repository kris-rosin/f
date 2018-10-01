//
//  ShareManager-iOS.h
//
//  Created by Stanislav Zheronkin on 23.08.17.
//

#pragma once

#include <string>
#include <functional>

class ShareManager_iOS
{
public:
    static void sharePhotoFacebook(std::string photo, std::string optionalURL);
    static void shareURLFacebook(std::string url, std::function<void(bool)> callback, std::string optionalQuote);
};

