//
//  LocalNotificationsWrapper.m
//
//  Created by Stanislav Zheronkin on 11/02/2015.
//  Copyright 2015 CodeExotics
//


#import "LocalNotificationsWrapper.h"
#import "mConstants.h"

@implementation LocalNotificationsWrapper

+(void)scheduleNotificationRepeated:(NSDate *)dateFirstNotification repeatInterval:(NSCalendarUnit)interval
{
    //first need to cancel all notofications
    [self clearAllNotifications];
    
    UILocalNotification * _localNotification =  [[[UILocalNotification alloc] init] autorelease];
    
    int percentage = arc4random() % 100;
    NSString *alertBody = [NSString stringWithCString:NOTIFICATION_ALERT1.c_str() encoding:[NSString defaultCStringEncoding]];
    if (percentage < 20) {
        alertBody = [NSString stringWithCString:NOTIFICATION_ALERT1.c_str() encoding:[NSString defaultCStringEncoding]];
    } else if (percentage < 40) {
        alertBody = [NSString stringWithCString:NOTIFICATION_ALERT2.c_str() encoding:[NSString defaultCStringEncoding]];
    } else if (percentage < 60) {
        alertBody = [NSString stringWithCString:NOTIFICATION_ALERT3.c_str() encoding:[NSString defaultCStringEncoding]];
    } else if (percentage < 80) {
        alertBody = [NSString stringWithCString:NOTIFICATION_ALERT4.c_str() encoding:[NSString defaultCStringEncoding]];
    } else if (percentage < 100) {
        alertBody = [NSString stringWithCString:NOTIFICATION_ALERT5.c_str() encoding:[NSString defaultCStringEncoding]];
    }

    
    //setting the fire date of the local notification
    _localNotification.fireDate         = dateFirstNotification;
    _localNotification.timeZone         = [NSTimeZone defaultTimeZone];
    _localNotification.alertAction      = @"Acknowledge";
    _localNotification.alertBody        = alertBody;
    _localNotification.soundName        = UILocalNotificationDefaultSoundName;
    _localNotification.repeatCalendar   = [NSCalendar currentCalendar];
    _localNotification.repeatInterval   = interval;
    
    //displaying the badge number
    _localNotification.applicationIconBadgeNumber = 0;
    
    //debug info
    NSLog(@"fire date :%@",_localNotification.fireDate);
    
    [[UIApplication sharedApplication] scheduleLocalNotification: _localNotification];
}

+(void)clearAllNotifications
{
    [[UIApplication sharedApplication] cancelAllLocalNotifications];
}

+(void)registerLocalNotifications
{
    if([[UIApplication sharedApplication] respondsToSelector: @selector(registerForRemoteNotifications)])
    {
        [[UIApplication sharedApplication] registerUserNotificationSettings: [UIUserNotificationSettings settingsForTypes: (UIUserNotificationTypeAlert | UIUserNotificationTypeBadge | UIUserNotificationTypeSound) categories: nil]];
    }
}

@end
