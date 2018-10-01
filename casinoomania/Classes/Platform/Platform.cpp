//
//  Platform.cpp
//
//  Created by Stanislav Zheronkin on 04.10.17.
//  Copyright (c) 2017 CodeExotics. All rights reserved.
//

#include "Platform.h"

#include "platform/CCPlatformConfig.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "Platform-iOS.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"

#define LOG_TAG "Platform"
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

#endif

int Platform::delta = 0;

unsigned int Platform::unixTime()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    unsigned int result = Platform_iOS::unixTime();
    
    return result + delta;
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    cocos2d::JniMethodInfo method;
    
    if(!cocos2d::JniHelper::getStaticMethodInfo(method, "com/codeexotics/tools/Platform", "unixTime", "()I"))
    {
        LOGD("Failed to find static method id of com.codeexotics.tools.Platform.unixTime()");
        
        return 0;
    }
    
    int result = method.env->CallStaticIntMethod(method.classID, method.methodID);
    
    method.env->DeleteLocalRef(method.classID);
    
    return result + delta;
    
#endif
}

void Platform::setDeltaTime(int deltatime)
{
    delta += deltatime;
}
