package com.codeexotics.tools;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.util.Log;
import android.widget.Toast;

import com.facebook.CallbackManager;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.share.Sharer;
import com.facebook.share.model.ShareLinkContent;
import com.facebook.share.model.SharePhoto;
import com.facebook.share.model.SharePhotoContent;
import com.facebook.share.widget.ShareDialog;
import com.facebook.share.widget.ShareDialog.Mode;

import org.cocos2dx.cpp.AppActivity;


public class ShareManager
{
    public static void shareURLFacebook(String url, String quote)
    {
        ShareLinkContent content = new ShareLinkContent.Builder()
                .setContentUrl(Uri.parse(url))
                .setQuote(quote)
                .build();

        ShareDialog shareDialog = new ShareDialog(AppActivity.getInstance());
        shareDialog.registerCallback(AppActivity.getInstance().getFBCallbackManager(), new FacebookCallback<Sharer.Result>() {
            @Override
            public void onSuccess(Sharer.Result result) {
                Log.i("hackyhack", "Facebook Share Successful");
                AppActivity.getInstance().shareFbReward();
            }

            @Override
            public void onCancel() {
                Log.i("hackyhack", "Facebook Share Cancel");
            }

            @Override
            public void onError(FacebookException error) {
                Log.i("hackyhack", "Facebook Share Error");
            }
        });
        shareDialog.show(content, Mode.AUTOMATIC);
    }

    public static void sharePhotoFacebook(String imagepath, String optionalLink)
    {
        BitmapFactory.Options options = new BitmapFactory.Options();
        options.inPreferredConfig = Bitmap.Config.ARGB_8888;
        Bitmap bitmap = BitmapFactory.decodeFile(imagepath, options);

        SharePhoto photo = new SharePhoto.Builder()
                .setUserGenerated(true)
                .setBitmap(bitmap)
                .build();

        SharePhotoContent content = new SharePhotoContent.Builder()
                .addPhoto(photo)
                .build();

        ShareDialog shareDialog = new ShareDialog(AppActivity.getInstance());

        if (shareDialog.canShow(content, Mode.AUTOMATIC))
        {
            shareDialog.show(content, Mode.AUTOMATIC);
        }
        else
        {
            AppActivity.getInstance().runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Toast.makeText(AppActivity.getContext(), "To share photo you need Facebook application installed.", Toast.LENGTH_LONG).show();
                }
            });
        }
    }
}
