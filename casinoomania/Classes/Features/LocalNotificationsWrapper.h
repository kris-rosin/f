//
//  LocalNotificationsWrapper.h
//
//  Created by Stanislav Zheronkin on 11/02/2015.
//  Copyright 2015 CodeExotics
//

#import <UIKit/UIKit.h>

@interface LocalNotificationsWrapper : NSObject

+(void)scheduleNotificationRepeated:(NSDate *)dateFirstNotification repeatInterval:(NSCalendarUnit)interval;
+(void)clearAllNotifications;
+(void)registerLocalNotifications;

@end
