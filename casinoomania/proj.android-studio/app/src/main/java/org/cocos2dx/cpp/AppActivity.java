/****************************************************************************
 Copyright (c) 2015 Chukong Technologies Inc.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
package org.cocos2dx.cpp;

import android.annotation.TargetApi;
import android.content.Intent;
import android.graphics.Point;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.Display;
import android.widget.Toast;

import com.anjlab.android.iab.v3.BillingProcessor;
import com.anjlab.android.iab.v3.TransactionDetails;
import com.app.manager.AndroidManager;
import com.chartboost.sdk.CBLocation;
import com.chartboost.sdk.Chartboost;
import com.chartboost.sdk.ChartboostDelegate;
import com.chartboost.sdk.Model.CBError.CBImpressionError;
import com.codeexotics.casino.R;
import com.codeexotics.tools.FacebookManager;
import com.codeexotics.tools.GameCenter;
import com.crashlytics.android.Crashlytics;
import com.crashlytics.android.ndk.CrashlyticsNdk;
import com.facebook.CallbackManager;
import com.facebook.FacebookSdk;
import com.facebook.appevents.AppEventsLogger;
import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.InterstitialAd;
import com.google.android.gms.ads.MobileAds;
import com.google.android.gms.ads.reward.RewardItem;
import com.google.android.gms.ads.reward.RewardedVideoAd;
import com.google.android.gms.ads.reward.RewardedVideoAdListener;
import com.google.android.gms.games.Games;


import io.fabric.sdk.android.Fabric;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import com.utils.NotificationsManager;

import static org.cocos2dx.lib.Cocos2dxActivity.getContext;


public class AppActivity extends Cocos2dxActivity implements BillingProcessor.IBillingHandler {


    private static BillingProcessor bp = null;

    private InterstitialAd mInterstitialAd;
    private RewardedVideoAd mRewardedAd;

    public static AppActivity mActivity = null;
    public static AppActivity me = null;

    private CallbackManager callbackManager;

    // Helper get display screen to avoid deprecated function use
    private Point getDisplaySize(Display d) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
            return getDisplaySizeGE11(d);
        }
        return getDisplaySizeLT11(d);
    }

    @TargetApi(Build.VERSION_CODES.HONEYCOMB_MR2)
    private Point getDisplaySizeGE11(Display d) {
        Point p = new Point(0, 0);
        d.getSize(p);
        return p;
    }

    private Point getDisplaySizeLT11(Display d) {
        try {
            Method getWidth = Display.class.getMethod("getWidth",
                    new Class[]{});
            Method getHeight = Display.class.getMethod("getHeight",
                    new Class[]{});
            return new Point(
                    ((Integer) getWidth.invoke(d, (Object[]) null)).intValue(),
                    ((Integer) getHeight.invoke(d, (Object[]) null)).intValue());
        } catch (NoSuchMethodException e2) // None of these exceptions should
        // ever occur.
        {
            return new Point(-1, -1);
        } catch (IllegalArgumentException e2) {
            return new Point(-2, -2);
        } catch (IllegalAccessException e2) {
            return new Point(-3, -3);
        } catch (InvocationTargetException e2) {
            return new Point(-4, -4);
        }
    }

    public Cocos2dxGLSurfaceView onCreateView() {
        Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
        // JungleRunner should create stencil buffer
        glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);

        return glSurfaceView;
    }

    @Override
    protected void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Fabric.with(this, new Crashlytics(), new CrashlyticsNdk());

        me = this;
        mActivity = this;

//        GameCenter.Initialize(getContext(), this);

        //CHARTBOOST_INTERSTITIAL
        Chartboost.startWithAppId(this, getString(R.string.chartboost_id), getString(R.string.chartboost_sig));
        Chartboost.setDelegate(chartboostDelegate);
        Chartboost.onCreate(this);

        //ADMOB_INTERSTITIAL
        mInterstitialAd = new InterstitialAd(this);
        mInterstitialAd.setAdUnitId(getString(R.string.admob_interstitial));
        requestNewInterstitial();
        mInterstitialAd.setAdListener(new AdListener() {
            @Override
            public void onAdClosed() {
                requestNewInterstitial();
            }
        });

        //ADMOB_REWARDED_VIDEO
        mRewardedAd = MobileAds.getRewardedVideoAdInstance(this);
        mRewardedAd.setRewardedVideoAdListener(admobRewardedVideoDelegate);
        loadRewardedVideoAd();

        //LOCAL NOTIFICATION REMINDER
        NotificationsManager.clearNotification(this);
        int ONE_DAY = 60 * 60 * 8;
        int random = Random.random.nextInt(100);
        // hackyhack_publish
        String text = getString(R.string.local_push_noti_text1);
        if (random < 20) {
            text = getString(R.string.local_push_noti_text1);
        } else if (random < 40) {
            text = getString(R.string.local_push_noti_text2);
        } else if (random < 60) {
            text = getString(R.string.local_push_noti_text3);
        } else if (random < 80) {
            text = getString(R.string.local_push_noti_text4);
        } else if (random < 100) {
            text = getString(R.string.local_push_noti_text5);
        }
        NotificationsManager.scheduleRepeatingNotification(this, text,
                getString(R.string.app_id), ONE_DAY, ONE_DAY); // 60 sec delay, 60sec

        bp = new BillingProcessor(this, getString(R.string.base64), this);

        //Facebook
        FacebookManager.Initialize(this);
        AppEventsLogger.activateApp(this);

        callbackManager = CallbackManager.Factory.create();
    }

    public CallbackManager getFBCallbackManager() {
        return callbackManager;
    }


    @Override
    public void onStart() {
        super.onStart();

//        GameCenter.onStart();

        Chartboost.onStart(this);
        Chartboost.cacheInterstitial(CBLocation.LOCATION_DEFAULT);
        Chartboost.cacheRewardedVideo(CBLocation.LOCATION_DEFAULT);

    }

    @Override
    public void onStop() {
        super.onStop();

//        GameCenter.onStop();
        Chartboost.onStop(this);
    }

    @Override
    protected void onPause() {
        super.onPause();
        Chartboost.onPause(this);
    }

    @Override
    public void onDestroy() {
        Chartboost.onDestroy(this);
        super.onDestroy();
    }

    @Override
    protected void onResume() {
        super.onResume();
        Chartboost.onResume(this);
    }


    static public AppActivity getInstance() {
        return mActivity;
    }


//***********************************************************************************************************************
//   ADS JNI START_HERE
//***********************************************************************************************************************

    public static void showInterstitialChartboost() {
        Log.i("hackyhack", "interstitial admob");
        me.runOnUiThread(new Runnable() {
            public void run() {
                //Using Chartboost
                int rand = Random.random.nextInt(100);

                if (rand < 50) {
                    if (me.mInterstitialAd.isLoaded()) {
                        me.mInterstitialAd.show();
                    }
                } else {
                    Chartboost.showInterstitial(CBLocation.LOCATION_DEFAULT);
                    Chartboost.cacheInterstitial(CBLocation.LOCATION_DEFAULT);
                }
            }
        });
    }

    private void requestNewInterstitial() {
        AdRequest adRequest = new AdRequest.Builder().addTestDevice(
                "YOUR_DEVICE_HASH").build();

        mInterstitialAd.loadAd(adRequest);
    }

    public static void showInterstitialAdmob() {
        Log.i("hackyhack", "interstitial admob, if not loaded, show Chartboost!");
        me.runOnUiThread(new Runnable() {
            public void run() {
                if (me.mInterstitialAd.isLoaded()) {
                    me.mInterstitialAd.show();
                } else {
                    me.requestNewInterstitial();
                    Chartboost.showInterstitial(CBLocation.LOCATION_DEFAULT);
                    Chartboost.cacheInterstitial(CBLocation.LOCATION_DEFAULT);
                }
            }
        });
    }

    private void loadRewardedVideoAd() {
        mRewardedAd.loadAd(getString(R.string.admob_rewarded_video), new AdRequest.Builder().build());
    }

    public static void playRewardedVideo() {
        Log.i("hackyhack", "Play Rewarded Video");
        me.runOnUiThread(new Runnable() {
            public void run() {
                if (me.mRewardedAd.isLoaded()) {
                    Log.i("hackyhack", "Rewarded Video: Admob");
                    me.mRewardedAd.show();
                } else {
                    Chartboost.showRewardedVideo(CBLocation.LOCATION_DEFAULT);
                    Chartboost.cacheRewardedVideo(CBLocation.LOCATION_DEFAULT);
                    me.loadRewardedVideoAd();
                }
            }
        });
    }

    /**
     * Admob Delegate
     */
    private RewardedVideoAdListener admobRewardedVideoDelegate = new RewardedVideoAdListener() {
        @Override
        public void onRewardedVideoAdLoaded() {

        }

        @Override
        public void onRewardedVideoAdOpened() {

        }

        @Override
        public void onRewardedVideoStarted() {
            Log.i("hackyhack", "onRewardedVideoStarted(), loadRewardedVideoAd() function called");
            loadRewardedVideoAd();
        }

        @Override
        public void onRewardedVideoAdClosed() {
            Log.i("hackyhack", "onRewardedVideoAdClosed()");
        }

        @Override
        public void onRewarded(RewardItem reward) {
            Log.i("hackyhack", "Admob Rewarded Video Amount:" + reward.getAmount());

            AndroidManager.giveVideoReward(reward.getAmount() + "");
        }

        @Override
        public void onRewardedVideoAdLeftApplication() {
            Log.i("hackyhack", "onRewardedVideoAdLeftApplication()");
        }

        @Override
        public void onRewardedVideoAdFailedToLoad(int i) {
            Log.i("hackyhack", "onRewardedVideoAdFailedToLoad");
        }
    };

    /**
     * Chartboost Delegate
     */
    private ChartboostDelegate chartboostDelegate = new ChartboostDelegate() {
        // Override the Chartboost delegate callbacks you wish to track and
        // control
        @Override
        public void didCacheInterstitial(String location) {
            Log.i("hackyhack", "DID CACHE INTERSTITIAL '"
                    + (location != null ? location : "null"));
        }

        @Override
        public void didFailToLoadInterstitial(String location,
                                              CBImpressionError error) {
            Log.i("hackyhack", String.format(
                    "DID FAIL TO LOAD INTERSTITIAL: '%s', Error:  %s",
                    (location != null ? location : "null"), error.name()));
            // Log.i("hackyhack",
            // String.format("DID FAIL TO LOAD REWARDED VIDEO '%s' because %s",
            // location, error.name()));
            // Toast.makeText(getApplicationContext(), String.format(
            // "Fail to load chartboost interstitial: '%s', Error:  %s",
            // (location != null ? location : "null"), error.name()),
            // Toast.LENGTH_SHORT).show();
        }

        @Override
        public void didCloseInterstitial(String location) {
            Log.i("hackyhack", "DID CLOSE INTERSTITIAL: "
                    + (location != null ? location : "null"));
        }

        @Override
        public boolean shouldDisplayRewardedVideo(String location) {
            Log.i("hackyhack", String.format(
                    "SHOULD DISPLAY REWARDED VIDEO: '%s'",
                    (location != null ? location : "null")));
            return true;
        }

        @Override
        public void didCacheRewardedVideo(String location) {
            Log.i("hackyhack", String.format("DID CACHE REWARDED VIDEO: '%s'",
                    (location != null ? location : "null")));
        }

        @Override
        public void didFailToLoadRewardedVideo(String location,
                                               CBImpressionError error) {
            Log.i("hackyhack", String.format(
                    "DID FAIL TO LOAD REWARDED VIDEO: '%s', Error:  %s",
                    (location != null ? location : "null"), error.name()));
            // Log.i("hackyhack",
            // String.format("DID FAIL TO LOAD REWARDED VIDEO '%s' because %s",
            // location, error.name()));
            // Toast.makeText(getApplicationContext(), String.format(
            // "Fail to load chartboost video: '%s', Error:  %s", (location !=
            // null ? location : "null"), error.name()),
            // Toast.LENGTH_SHORT).show();
        }

        @Override
        public void didCompleteRewardedVideo(String location, final int reward) {
            Log.i("hackyhack", String.format(
                    "DID COMPLETE REWARDED VIDEO '%s' FOR REWARD %d",
                    (location != null ? location : "null"), reward));

            AndroidManager.giveVideoReward(reward + "");
        }
    };

    private native void nativeSetPhoneID(String id);
    public static native void shareFbReward();

    public static void openUrl(String URL) {
        Intent myIntent = new Intent(Intent.ACTION_VIEW,
                Uri.parse("market://details?id=" + me.getPackageName()));
        me.startActivity(myIntent);
    }


//***********************************************************************************************************************
//   ADS JNI END_HERE
//***********************************************************************************************************************

//***********************************************************************************************************************
//   IN APP PURCHASE START_HERE
//***********************************************************************************************************************

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        // TODO Auto-generated method stub
        if (!bp.handleActivityResult(requestCode, resultCode, data)) {
            Log.e("hackyhack " + getString(R.string.app_name),
                    "activityResult - called");
            super.onActivityResult(requestCode, resultCode, data);
            if( callbackManager != null) callbackManager.onActivityResult(requestCode, resultCode, data);
        }

        FacebookManager.onActivityResult(requestCode, resultCode, data);
    }

    public static void purchase(String featureAId) {
        bp.purchase(mActivity, featureAId);
    }

    @Override
    public void onProductPurchased(String productId, TransactionDetails details) {
        // TODO Auto-generated method stub
        Log.e("hackyhack " + getString(R.string.app_name),
                "onProductPurchased - productId = " + productId);
        // TODO Auto-generated method stub
        bp.consumePurchase(productId);
        AndroidManager.onComplete(productId);
    }

    @Override
    public void onPurchaseHistoryRestored() {
        // TODO Auto-generated method stub

    }

    @Override
    public void onBillingError(int errorCode, Throwable error) {
        final int code = errorCode;
        mActivity.runOnUiThread(new Runnable() {

            @Override
            public void run() {
                Toast.makeText(mActivity, "Purchased is Cancelled! Please try again!", Toast.LENGTH_SHORT).show();
                Log.i("hackyhack", "Purchase Failed! - onBillingError: " + Integer.toString(code));
            }
        });
        AndroidManager.onCancel();
    }

    @Override
    public void onBillingInitialized() {
        // TODO Auto-generated method stub

    }

//***********************************************************************************************************************
//   IN APP PURCHASE END_HERE
//***********************************************************************************************************************

//***********************************************************************************************************************
//   GAMES SERVICES START_HERE
//***********************************************************************************************************************

//    public static void gameServicesSignIn() {
//        ((AppActivity) mActivity).runOnUiThread(new Runnable() {
//            public void run() {
//                ((AppActivity) mActivity).beginUserInitiatedSignIn();
//            }
//        });
//
//    }
//
//    public static void updateTopScoreLeaderboard(int score, String leaderboardId) {
//        Log.i("hackyhack", "score: " + score + "| leaderboard: "
//                + leaderboardId);
//        if (((AppActivity) mActivity).getApiClient().isConnected()) {
//            Log.i("hackyhack", "score is submitted");
//            Games.Leaderboards.submitScore(
//                    ((AppActivity) mActivity).getApiClient(), leaderboardId,
//                    score);
//        }
//    }
//
//    public static void achievementUnlocked(String acID) {
//        if (((AppActivity) mActivity).getApiClient().isConnected()) {
//            Log.i("hackyhack", "Achievement Unlocked: " + acID);
//            Games.Achievements.unlock(((AppActivity) mActivity).getApiClient(),
//                    acID);
//        }
//    }
//
//    public static void updateAchievement(String id, int percentage) {
//        Games.Achievements.increment(((AppActivity) mActivity).getApiClient(),
//                id, percentage);
//    }
//
//    public static void showLeaderboards() {
//
//        if (((AppActivity) mActivity).getApiClient().isConnected() == false) {
//            gameServicesSignIn();
//            return;
//        }
//        ((AppActivity) mActivity).runOnUiThread(new Runnable() {
//            String leaderboardId = ((AppActivity) mActivity)
//                    .getString(R.string.leaderboard_id);
//
//            public void run() {
//                ((AppActivity) mActivity).startActivityForResult(
//                        Games.Leaderboards.getLeaderboardIntent(
//                                ((AppActivity) mActivity).getApiClient(),
//                                leaderboardId), 5001);
//                ((AppActivity) mActivity).startActivityForResult(
//                        Games.Achievements
//                                .getAchievementsIntent(((AppActivity) mActivity)
//                                        .getApiClient()), 5001);
//
//            }
//        });
//    }
//
//    public static void showAchievements() {
//        ((AppActivity) mActivity).runOnUiThread(new Runnable() {
//            public void run() {
//                ((AppActivity) mActivity).startActivityForResult(
//                        Games.Achievements
//                                .getAchievementsIntent(((AppActivity) mActivity)
//                                        .getApiClient()), 5001);
//            }
//        });
//    }
//
//    @Override
//    public void onSignInFailed() {
//        // TODO Auto-generated method stub
//
//    }
//
//    @Override
//    public void onSignInSucceeded() {
//        // TODO Auto-generated method stub
//
//    }

//***********************************************************************************************************************
//   GAMES SERVICES END_HERE
//***********************************************************************************************************************

}
