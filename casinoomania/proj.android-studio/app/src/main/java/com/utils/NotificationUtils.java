// Stanislav
package com.utils;

import android.annotation.SuppressLint;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.os.Build;
import android.support.v4.app.NotificationCompat;

import com.codeexotics.casino.R;

import org.cocos2dx.cpp.AppActivity;

@SuppressLint("InlinedApi")
public class NotificationUtils {

	public static NotificationManager mManager;

	@SuppressWarnings("static-access")
	public static void generateNotification(Context context, Intent intent) {
		
		mManager = (NotificationManager) context.getSystemService(context.NOTIFICATION_SERVICE);
		Intent intent1 = new Intent(context, AppActivity.class);
		intent1.addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP| Intent.FLAG_ACTIVITY_CLEAR_TOP);
		PendingIntent pendingNotificationIntent = PendingIntent.getActivity(context,0, intent1, PendingIntent.FLAG_UPDATE_CURRENT);
		
		Notification notification = null;
		
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN)
		{
			notification = new NotificationCompat.Builder(context)
				.setSmallIcon(R.mipmap.ic_launcher)
				.setContentTitle(context.getString(R.string.app_name))
				.setContentText(intent.getStringExtra("text"))
				.setWhen(System.currentTimeMillis())
				.setAutoCancel(true)
				.setDefaults(Notification.DEFAULT_SOUND)
				.setContentIntent(pendingNotificationIntent)
				.setPriority(Notification.PRIORITY_HIGH)
				.build();
		}
		else
		{
			notification = new NotificationCompat.Builder(context)
			.setSmallIcon(R.mipmap.ic_launcher)
			.setContentTitle(context.getString(R.string.app_name))
			.setContentText(intent.getStringExtra("text"))
			.setWhen(System.currentTimeMillis())
			.setAutoCancel(true)
			.setDefaults(Notification.DEFAULT_SOUND)
			.setContentIntent(pendingNotificationIntent)
			.setPriority(Notification.PRIORITY_HIGH)
			.build();
		}

		mManager.notify(0, notification);
	}
}
