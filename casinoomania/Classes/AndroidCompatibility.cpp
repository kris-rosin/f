//
//  AndroidCompatibility.cpp
//

#include "AndroidCompatibility.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "stdlib.h"
#endif
//hackyhack_publish - arc4random_uniform

//#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//
//uint32_t arc4random_uniform(uint32_t upper_bound) {
//    u_int32_t r, min;
//
//    if (upper_bound < 2)
//        return 0;
//
//#if (ULONG_MAX > 0xffffffffUL)
//    min = 0x100000000UL % upper_bound;
//#else
//    // Calculate (2**32 % upper_bound) avoiding 64-bit math
//    if (upper_bound > 0x80000000)
//        min = 1 + ~upper_bound;  // 2**32 - upper_bound
//    else {
//        // (2**32 - (x * 2)) % x == 2**32 % x when x <= 2**31
//        min = ((0xffffffff - (upper_bound * 2)) + 1) % upper_bound;
//    }
//#endif
//
//    
////    This could theoretically loop forever but each retry has
////    p > 0.5f (worst case, usually far better) of selecting a
////    number inside the range we need, so it should rarely need
////    to re-roll.
//
//    for (;;) {
//        r = arc4random();
//        if (r >= min)
//            break;
//    }
//
//    return r % upper_bound;
//}
//#endif
