/****************************************************************************
Copyright (c) 2010-2016 Codeexotics Studio
****************************************************************************/
package com.codeexotics.tools;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.util.Log;

import com.codeexotics.*;
import com.codeexotics.casino.R;
import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.games.Games;
import com.google.example.games.basegameutils.BaseGameUtils;

public class GameCenter implements
        GoogleApiClient.ConnectionCallbacks,
        GoogleApiClient.OnConnectionFailedListener
{

    /** Google Play Services */
    private static GoogleApiClient mGoogleApiClient;
    private static boolean mSignInClicked = false;
    private static boolean mResolvingConnectionFailure = false;
    private static int RC_SIGN_IN = 9001;
    private static final int REQUEST_RESOLVE_ERROR = 1001;
    private static final int REQUEST_LEADERBOARD   = 100;
    private static final int REQUEST_ACHIEVEMENTS  = 101;
    private static Activity mActivity = null;

    public static void Initialize(Context context, Activity activity) {
        GameCenter instance = new GameCenter();

        mActivity = activity;

        mGoogleApiClient = new GoogleApiClient.Builder(context)
                .addConnectionCallbacks(instance)
                .addOnConnectionFailedListener(instance)
                .addApi(Games.API).addScope(Games.SCOPE_GAMES)
                .build();
    }

    static public void onStart() {
        mGoogleApiClient.connect();
    }

    static public void onStop() {
        mGoogleApiClient.disconnect();
	}

    @Override
    public void onConnectionFailed(ConnectionResult connectionResult) {
        Log.i("GPS", "Connection failed!");
        if (mResolvingConnectionFailure) {
            // already resolving
            return;
        }

        // if the sign-in button was clicked or if auto sign-in is enabled,
        // launch the sign-in flow
        if (mSignInClicked) {
            mSignInClicked = false;
            mResolvingConnectionFailure = true;

            // Attempt to resolve the connection failure using BaseGameUtils.
            if (!BaseGameUtils.resolveConnectionFailure(mActivity,
                    mGoogleApiClient, connectionResult,
                    RC_SIGN_IN, mActivity.getString(R.string.signin_other_error_message))) {
                mResolvingConnectionFailure = false;
            }
        }
    }

    @Override
    public void onConnectionSuspended(int i) {
        // Attempt to reconnect
        mGoogleApiClient.connect();
    }

    @Override
    public void onConnected(Bundle connectionHint) {
        Log.i("GPS", "Already Connected!");
        onLoginGooglePlay();
    }

    public static void logoutGooglePlay() {
        if (mGoogleApiClient != null && mGoogleApiClient.isConnected()) {
            Games.signOut(mGoogleApiClient);
            mGoogleApiClient.disconnect();
            onLogoutGooglePlay();
        }
    }

    public static void loginGooglePlay() {
        if (mGoogleApiClient != null && !mGoogleApiClient.isConnected()) {
            mSignInClicked = true;
            mGoogleApiClient.connect();
        } else {
            onLoginGooglePlay();
        }
    }

    public static void postScore(int score, String boardName) {
        if (mGoogleApiClient != null && mGoogleApiClient.isConnected()) {
            Games.Leaderboards.submitScore(mGoogleApiClient, boardName, score);
        }
    }
    
    public static String getUserName() {
        if (mGoogleApiClient != null && mGoogleApiClient.isConnected()) {
            return Games.Players.getCurrentPlayerId(mGoogleApiClient);
        }
        
        return "";
    }

    public static boolean showLeaderboard() {
        if (mGoogleApiClient != null && mGoogleApiClient.isConnected()) {
            mActivity.startActivityForResult(Games.Leaderboards.getLeaderboardIntent(mGoogleApiClient, mActivity.getString(R.string.leaderboard_id)), REQUEST_LEADERBOARD);
            return true;
        }

        return false;
    }

    public static boolean showAchievements() {
        if (mGoogleApiClient != null && mGoogleApiClient.isConnected()) {
            mActivity.startActivityForResult(Games.Achievements.getAchievementsIntent(mGoogleApiClient), REQUEST_ACHIEVEMENTS);
            return true;
        }

        return false;
    }

    public static void postAchievement(String achievement_id, int amount)
    {
        if (mGoogleApiClient != null && mGoogleApiClient.isConnected() && amount == 100) {
            Games.Achievements.unlock(mGoogleApiClient, achievement_id);
        }
    }

    public static void unlockAchievement(String achievement_id)
    {
        if (mGoogleApiClient != null && mGoogleApiClient.isConnected()) {
            Games.Achievements.unlock(mGoogleApiClient, achievement_id);
        }
    }

    private static native void onLoginGooglePlay();
    private static native void onLogoutGooglePlay();
}
