//
//  Platform.h
//
//  Created by Stanislav Zheronkin on 04.10.17.
//  Copyright (c) 2017 CodeExotics. All rights reserved.
//

#pragma once

class Platform
{
private:
    static int delta;
public:
    static unsigned int unixTime();
    static void setDeltaTime(int delta);
};
