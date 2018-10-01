package com.app.manager;

import android.content.ActivityNotFoundException;
import android.content.Intent;
import android.net.Uri;
import android.util.Log;
import android.widget.Toast;

import com.codeexotics.casino.R;

import org.cocos2dx.cpp.AppActivity;

public class AndroidManager {

    public static void rateApp() {
        Uri uri = Uri.parse("market://details?id="
                + AppActivity.mActivity.getApplicationContext()
                .getPackageName());
        Intent goToMarket = new Intent(Intent.ACTION_VIEW, uri);
        goToMarket.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        try {
            AppActivity.mActivity.getApplicationContext().startActivity(
                    goToMarket);
        } catch (ActivityNotFoundException e) {
            Toast.makeText(AppActivity.mActivity.getApplicationContext(),
                    "Link does not work!", Toast.LENGTH_SHORT)
                    .show();
        }
    }

    public static void openUrl(String openURL) {
        Intent i = new Intent(Intent.ACTION_VIEW);
        i.setData(Uri.parse(openURL));
        AppActivity.mActivity.startActivity(i);
    }

    public static void buyProduct(String productID) {
        // code to initiate a purchase... can be placed in onClickListener etc
        Log.v("hackyhack", "Buy Product ID: " + productID);

        AppActivity.purchase(productID);
    }

    public static void onComplete(String featureAId) {
        jniOnComplete(featureAId);
    }

    public static void onCancel() {
        jniOnCancel();
    }

    public static native void jniOnComplete(String featureAId);

    public static native void jniOnCancel();

    public static native void giveVideoReward(String value);


//    public static void updateTopScoreLeaderboard(String leaderboardId, int score) {
//        AppActivity.updateTopScoreLeaderboard(score, leaderboardId);
//    }
//
//    public static void showLeaderboards() {
//        AppActivity.showLeaderboards();
//    }
//
//    public static void submitAchievement(String itemID) {
//        Log.v("hackyhack", "Submit Achievement" + itemID);
//        AppActivity.achievementUnlocked(itemID);
//    }

    public static final int SHARE_WHATSAPP = 0;
    public static final int SHARE_EMAIL = 1;
    public static final int SHARE_SMS = 2;

    public static void shareTheGame(String text, String subject, int shareType) {
        Log.i("hackyhack", "Text: " + text + " | Share type: " + shareType);
        text = text + "\n\nGoogle Play:\nhttps://play.google.com/store/apps/details?id=" + AppActivity.mActivity.getApplicationContext().getPackageName() +
                "\n\niTunes Store:\n" + AppActivity.mActivity.getString(R.string.itunes_connect_link);

        if (shareType == SHARE_WHATSAPP) {
            Intent sendIntent = new Intent();
            sendIntent.setAction(Intent.ACTION_SEND);
            sendIntent.putExtra(Intent.EXTRA_TEXT, text);
            sendIntent.setType("text/plain");
            sendIntent.setPackage("com.whatsapp");
            AppActivity.getInstance().startActivity(sendIntent);

        } else if (shareType == SHARE_EMAIL) {
            Intent intent = new Intent(Intent.ACTION_SEND);
            intent.setType("text/html");
            intent.putExtra(Intent.EXTRA_EMAIL, "casinoomania@gmail.com");
            intent.putExtra(Intent.EXTRA_SUBJECT, subject);
            intent.putExtra(Intent.EXTRA_TEXT, text);
            AppActivity.getInstance().startActivity(Intent.createChooser(intent, "Send Email"));

        } else if (shareType == SHARE_SMS) {
            String smsBody = text;
            Intent sendIntent = new Intent(Intent.ACTION_VIEW);
            sendIntent.putExtra("sms_body", smsBody);
            sendIntent.setType("vnd.android-dir/mms-sms");
            AppActivity.getInstance().startActivity(sendIntent);
        }
    }

}
