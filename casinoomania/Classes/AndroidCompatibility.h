//
//  AndroidCompatibility.h
//

#pragma once

#include <string>


#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
/**
 *  Calculate a uniformly distributed random number less than upper_bound
 * avoiding "modulo bias".
 *
 * Uniformity is achieved by generating new random numbers until the one
 * returned is outside the range [0, 2**32 % upper_bound).  This
 * guarantees the selected random number will be inside
 * [2**32 % upper_bound, 2**32) which maps back to [0, upper_bound)
 * after reduction modulo upper_bound.
 *
 *  @param upper_bound the upper bound for the distribution
 *
 *  @return the randomly generated value
 */
//hackyhack_publish - arc4random_uniform
//uint32_t arc4random_uniform(uint32_t upper_bound);
#endif

