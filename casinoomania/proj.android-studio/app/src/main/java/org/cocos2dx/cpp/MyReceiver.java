// Stanislav
package org.cocos2dx.cpp;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

import com.utils.NotificationUtils;

public class MyReceiver extends BroadcastReceiver
{

	@Override
	public void onReceive(Context context, Intent intent)
	{
		Log.i("App", "called receiver method");
		try{
			NotificationUtils.generateNotification(context, intent);
		}catch(Exception e){
			e.printStackTrace();
		}
	}

}
