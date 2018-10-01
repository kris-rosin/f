//Stanislav
package com.utils;

import android.content.Context;
import android.content.SharedPreferences;

import org.cocos2dx.cpp.AppActivity;

public class LocalStorage
{
    private static SharedPreferences getPreferences()
    {
        return AppActivity.mActivity.getPreferences(Context.MODE_PRIVATE);
    }

    public static void setString(String key, String value)
    {
        SharedPreferences.Editor editor = getPreferences().edit();

        editor.putString(key, value);

        editor.commit();
    }

    public static String getString(String key)
    {
        return getPreferences().getString(key, "");
    }

    public static void setInteger(String key, int value)
    {
        SharedPreferences.Editor editor = getPreferences().edit();

        editor.putInt(key, value);

        editor.commit();
    }

    public static int getInteger(String key)
    {
        return getPreferences().getInt(key, 0);
    }

    public static void setFloat(String key, float value)
    {
        SharedPreferences.Editor editor = getPreferences().edit();

        editor.putFloat(key, value);

        editor.commit();
    }

    public static float getFloat(String key)
    {
        return getPreferences().getFloat(key, 0.0f);
    }

    public static void setBoolean(String key, boolean value)
    {
        SharedPreferences.Editor editor = getPreferences().edit();

        editor.putBoolean(key, value);

        editor.commit();
    }

    public static boolean getBoolean(String key)
    {
        return getPreferences().getBoolean(key, false);
    }
}
