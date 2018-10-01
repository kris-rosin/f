// Stanislav
package com.utils;

import android.app.Activity;
import android.app.AlarmManager;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.os.Build;

import org.cocos2dx.cpp.MyReceiver;

import java.util.Calendar;

public class NotificationsManager
{
 private static int notificationIndex = 45;

 public static void clearNotification(Activity activity)
 {
     int maxIndex = LocalStorage.getInteger("MaxNotificationIndex");

     NotificationManager mManager = (NotificationManager) activity.getSystemService(Context.NOTIFICATION_SERVICE);
     mManager.cancelAll();

     for(int i = 45; i < maxIndex; i++) {
         Intent intent = new Intent(activity, MyReceiver.class);
         
         String packageName = activity.getClass().getName().substring(0, activity.getClass().getName().lastIndexOf("."));
         intent.setAction(packageName + ".LOCAL_NOTIFICATION");

         PendingIntent displayIntent = PendingIntent.getBroadcast(activity, i, intent, 0);

         AlarmManager alarmManager = (AlarmManager) activity.getSystemService(Context.ALARM_SERVICE);

         alarmManager.cancel(displayIntent);
     }

     notificationIndex = 45;

     LocalStorage.setInteger("MaxNotificationIndex", notificationIndex);
 }

 public static void scheduleNotification(Activity activity, String message, String actionText, int delay)
 {
     if(delay <= 0)
         return;

     Calendar calendar = Calendar.getInstance();

     calendar.setTimeInMillis(System.currentTimeMillis());
     calendar.add(Calendar.SECOND, delay);

     Intent myIntent = new Intent(activity, MyReceiver.class);
     myIntent.putExtra("text", message);
     
     String packageName = activity.getClass().getName().substring(0, activity.getClass().getName().lastIndexOf("."));
     
     myIntent.setAction(packageName + ".LOCAL_NOTIFICATION");
     myIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);

     PendingIntent pendingIntent = PendingIntent.getBroadcast(activity, notificationIndex++ , myIntent, PendingIntent.FLAG_UPDATE_CURRENT);

     AlarmManager alarmManager = (AlarmManager)activity.getSystemService(Context.ALARM_SERVICE);

     if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
         alarmManager.setExact(AlarmManager.RTC_WAKEUP, calendar.getTimeInMillis(), pendingIntent);
     } else {
         alarmManager.set(AlarmManager.RTC_WAKEUP, calendar.getTimeInMillis(), pendingIntent);
     }

     LocalStorage.setInteger("MaxNotificationIndex", notificationIndex);
 }
 
 public static void scheduleRepeatingNotification(Activity activity, String message, String actionText, int delay, int interval)
 {
     if(delay <= 0)
         return;
     
     long repeatingInterval = interval * 1000;
     
     Calendar calendar = Calendar.getInstance();

     calendar.setTimeInMillis(System.currentTimeMillis());
     calendar.add(Calendar.SECOND, delay);

     Intent myIntent = new Intent(activity, MyReceiver.class);
     myIntent.putExtra("text", message);
     
     String packageName = activity.getClass().getName().substring(0, activity.getClass().getName().lastIndexOf("."));
     
  
     myIntent.setAction(packageName + ".LOCAL_NOTIFICATION");
     myIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);

     PendingIntent pendingIntent = PendingIntent.getBroadcast(activity, notificationIndex++ , myIntent, PendingIntent.FLAG_UPDATE_CURRENT);

     AlarmManager alarmManager = (AlarmManager)activity.getSystemService(Context.ALARM_SERVICE);

     alarmManager.setRepeating(AlarmManager.RTC_WAKEUP, calendar.getTimeInMillis(), repeatingInterval, pendingIntent);

     LocalStorage.setInteger("MaxNotificationIndex", notificationIndex);
 }
}

