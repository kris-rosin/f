package com.codeexotics.tools;

public class Platform
{
    public static int unixTime()
    {
        return (int)(System.currentTimeMillis() / 1000L);
    }
}
