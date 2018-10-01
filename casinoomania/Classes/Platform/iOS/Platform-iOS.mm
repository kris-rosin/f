//
//  Platform.mm
//

#include "Platform-iOS.h"

unsigned int Platform_iOS::unixTime()
{
    return [[NSDate date] timeIntervalSince1970];
}
