package com.codeexotics.tools;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

import com.facebook.AccessToken;
import com.facebook.CallbackManager;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.FacebookSdk;
import com.facebook.GraphRequest;
import com.facebook.GraphResponse;
import com.facebook.appevents.AppEventsConstants;
import com.facebook.appevents.AppEventsLogger;
import com.facebook.login.LoginManager;
import com.facebook.login.LoginResult;
import com.facebook.share.model.GameRequestContent;
import com.facebook.share.widget.GameRequestDialog;

import org.cocos2dx.cpp.AppActivity;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Currency;
import java.util.List;

public class FacebookManager
{
    private static CallbackManager callbackManager;
    private static GameRequestDialog requestDialog;
    private static AppEventsLogger logger;
    private static Context currentContext = null;
    private static AppActivity currentActivity = null;
    private static int requestCallbackID = -1;

    private static int connectionHandlerID = 0;

    public static void Initialize(AppActivity activity)
    {
        currentActivity = activity;
        currentContext = activity.getApplicationContext();

        //FacebookSdk.sdkInitialize(currentContext);

        callbackManager = CallbackManager.Factory.create();

        LoginManager.getInstance().registerCallback(callbackManager, new FacebookCallback<LoginResult>()
        {
            @Override
            public void onSuccess(LoginResult loginResult) {
                currentActivity.runOnGLThread(new Runnable() {
                    @Override
                    public void run()
                    {
                        executeBooleanCallback(connectionHandlerID, true);
                    }
                });
            }

            @Override
            public void onCancel() {
                currentActivity.runOnGLThread(new Runnable() {
                    @Override
                    public void run()
                    {
                        executeBooleanCallback(connectionHandlerID, false);
                    }
                });
            }

            @Override
            public void onError(FacebookException exception) {
                currentActivity.runOnGLThread(new Runnable() {
                    @Override
                    public void run()
                    {
                        executeBooleanCallback(connectionHandlerID, false);
                    }
                });
            }
        });

        requestDialog = new GameRequestDialog(currentActivity);
        requestDialog.registerCallback(callbackManager,
                new FacebookCallback<GameRequestDialog.Result>() {
                    public void onSuccess(GameRequestDialog.Result result) {
                        if (requestCallbackID > -1) {
                            List<String> recipients = result.getRequestRecipients();

                            String[] ids = new String[recipients.size()];
                            ids = recipients.toArray(ids);

                            final String [] finalIDs = ids;

                            currentActivity.runOnGLThread(new Runnable() {
                                @Override
                                public void run() {
                                    executeRequestRecipients(true, finalIDs, requestCallbackID);
                                }
                            });

                            requestCallbackID = -1;
                        }
                    }

                    public void onCancel() {
                        if (requestCallbackID > -1) {
                            currentActivity.runOnGLThread(new Runnable() {
                                @Override
                                public void run() {
                                    executeRequestRecipients(false, null, requestCallbackID);
                                }
                            });

                            requestCallbackID = -1;
                        }
                    }

                    public void onError(FacebookException error) {
                        if (requestCallbackID > -1) {
                            currentActivity.runOnGLThread(new Runnable() {
                                @Override
                                public void run() {
                                    executeRequestRecipients(false, null, requestCallbackID);
                                }
                            });

                            requestCallbackID = -1;
                        }
                    }
                }
        );
    }

    public static void onActivityResult(int requestCode, int resultCode, Intent data)
    {
        if (callbackManager != null)
            callbackManager.onActivityResult(requestCode, resultCode, data);
    }

    public static void Login(int callbackID)
    {
        connectionHandlerID = callbackID;

        LoginManager.getInstance().logInWithReadPermissions(currentActivity, Arrays.asList("public_profile", "email", "user_friends"));
    }

    public static void Logout()
    {
        LoginManager.getInstance().logOut();
    }

    public static boolean isLoggedIn()
    {
        AccessToken token = AccessToken.getCurrentAccessToken();

        if  (token != null) {
            boolean result = token.getPermissions().contains("user_friends");

            return result;
        }
        return false;
    }

    public static String GetUserID()
    {
        AccessToken token = AccessToken.getCurrentAccessToken();

        if  (token != null) {
            return token.getUserId();
        }
        return "";
    }

    public static void AskFriendsForLife(final String object_id, final String title, final String message)
    {
        if (currentActivity == null) return;

        currentActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                GameRequestContent content = new GameRequestContent.Builder()
                        .setMessage(message)
                        .setObjectId(object_id)
                        .setTitle(title)
                        .setFilters(GameRequestContent.Filters.APP_USERS)
                        .setActionType(GameRequestContent.ActionType.ASKFOR)
                        .build();
                requestDialog.show(content);
            }
        });
    }

    public static void AcceptRequest(final String sender, final String request_id,final String object_id, final String title, final String message)
    {
        if (currentActivity == null) return;

        currentActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                ArrayList<String> users = new ArrayList<String>();

                users.add(sender);

                GameRequestContent content = new GameRequestContent.Builder()
                        .setMessage(message)
                        .setObjectId(object_id)
                        .setTitle(title)
                        .setRecipients(users)
                        .setActionType(GameRequestContent.ActionType.SEND)
                        .build();
                requestDialog.show(content);

                DeleteRequest(request_id);
            }
        });
    }

    public static void DeleteRequest(final String request_id)
    {
        if (AccessToken.getCurrentAccessToken() != null) {
            GraphRequest request = GraphRequest.newDeleteObjectRequest(AccessToken.getCurrentAccessToken(), request_id,
                    new GraphRequest.Callback() {
                        @Override
                        public void onCompleted(GraphResponse response) {
                            Log.d("FacebookManager", response.toString());
                        }
                    }
            );
            request.executeAsync();
        }
    }

    public static boolean hasWritePermission()
    {
        AccessToken token = AccessToken.getCurrentAccessToken();

        if  (token != null) {
            boolean result = token.getPermissions().contains("publish_actions");

            return result;
        }
        return false;
    }

    public static void GetUserProfilePictureAsync(final int callbackID, final int size, final String user_id)
    {
        if (AccessToken.getCurrentAccessToken() != null) {
            String path  = user_id + "/picture?redirect=false&type=square&height=" + size +"&width=" + size;

            final GraphRequest request = GraphRequest.newGraphPathRequest(
                    AccessToken.getCurrentAccessToken(), path, new GraphRequest.Callback() {
                        @Override
                        public void onCompleted(GraphResponse response) {
                            JSONObject jsonObject = response.getJSONObject();

                            String result = "";

                            try {
                                JSONObject jsonArray = jsonObject.getJSONObject("data");

                                if (jsonArray != null)
                                {
                                    result = jsonArray.getString("url");
                                }
                            } catch (JSONException e) {
                                e.printStackTrace();
                            }

                            final String finalResult = result;

                            currentActivity.runOnGLThread(new Runnable() {
                                @Override
                                public void run() {
                                    executeStringCallback(callbackID, finalResult);
                                }
                            });
                        }
                    }
            );
            request.executeAsync();
        }
    }

    public static void GetRequests()
    {
        if (AccessToken.getCurrentAccessToken() != null) {
            final GraphRequest request = GraphRequest.newGraphPathRequest(
                    AccessToken.getCurrentAccessToken(), "me/apprequests?fields=object,id,from", new GraphRequest.Callback() {
                        @Override
                        public void onCompleted(GraphResponse response) {
                            JSONObject jsonObject = response.getJSONObject();

                            ArrayList<String> requestIDs  = new ArrayList<String>();
                            ArrayList<String> senderIDs   = new ArrayList<String>();
                            ArrayList<String> senderNames = new ArrayList<String>();
                            ArrayList<String> objectIDs   = new ArrayList<String>();

                            try {
                                JSONArray jsonArray = jsonObject.getJSONArray("data");

                                for(int i = 0; i < jsonArray.length(); i++)
                                {
                                    JSONObject dist = jsonArray.getJSONObject(i);
                                    String id = dist.getString("id");

                                    JSONObject from = dist.getJSONObject("from");
                                    String from_id = from.getString("id");
                                    String from_name = from.getString("name");

                                    JSONObject object = dist.getJSONObject("object");
                                    String object_id = object.getString("id");

                                    requestIDs.add(id);
                                    senderIDs.add(from_id);
                                    senderNames.add(from_name);
                                    objectIDs.add(object_id);
                                }
                            } catch (JSONException e) {
                                e.printStackTrace();
                            }

                            String[] a1 = new String[requestIDs.size()];
                            String[] a2 = new String[senderIDs.size()];
                            String[] a3 = new String[senderNames.size()];
                            String[] a4 = new String[objectIDs.size()];

                            a1 = requestIDs.toArray(a1);
                            a2 = senderIDs.toArray(a2);
                            a3 = senderNames.toArray(a3);
                            a4 = objectIDs.toArray(a4);

                            final String[] finalA1 = a1;
                            final String[] finalA2 = a2;
                            final String[] finalA3 = a2;
                            final String[] finalA4 = a2;

                            currentActivity.runOnGLThread(new Runnable() {
                                @Override
                                public void run() {
                                    executeGetRequestsCallback(finalA1, finalA2, finalA3, finalA4);
                                }
                            });
                        }
                    }
            );
            request.executeAsync();
        }
    }

    public static void ActivateApp()
    {
        if (currentActivity != null)
            AppEventsLogger.activateApp(currentActivity.getApplication());
    }

    public static void GetFriends(final int callbackID)
    {
        if (AccessToken.getCurrentAccessToken() != null) {
            final GraphRequest request = GraphRequest.newMyFriendsRequest(AccessToken.getCurrentAccessToken(), new GraphRequest.GraphJSONArrayCallback() {
                @Override
                public void onCompleted(JSONArray objects, GraphResponse response)
                {
                    JSONObject jsonObject = response.getJSONObject();

                    ArrayList<String> friendIDs   = new ArrayList<String>();
                    ArrayList<String> friendNames = new ArrayList<String>();

                    try {
                        JSONArray jsonArray = jsonObject.getJSONArray("data");

                        for(int i = 0; i < jsonArray.length(); i++)
                        {
                            JSONObject dist  = jsonArray.getJSONObject(i);
                            String user_id   = dist.getString("id");
                            String user_name = dist.getString("name");

                            friendIDs.add(user_id);
                            friendNames.add(user_name);
                        }
                    } catch (JSONException e) {
                        e.printStackTrace();
                    }

                    String[] a1 = new String[friendIDs.size()];
                    String[] a2 = new String[friendNames.size()];

                    a1 = friendIDs.toArray(a1);
                    a2 = friendNames.toArray(a2);

                    final String [] finalA1 = a1;
                    final String [] finalA2 = a2;

                    currentActivity.runOnGLThread(new Runnable() {
                        @Override
                        public void run() {
                            executeGetFriendsCallback(true, finalA1, finalA2, callbackID);
                        }
                    });

                }
            });

            Bundle parameters = new Bundle();
            parameters.putString("fields", "id,name");
            request.setParameters(parameters);

            request.executeAsync();
        }
    }

    public static void GetCurrentUserName(final int callbackID)
    {
        if (AccessToken.getCurrentAccessToken() != null)
        {
            GraphRequest request = GraphRequest.newMeRequest(
                    AccessToken.getCurrentAccessToken(),
                    new GraphRequest.GraphJSONObjectCallback() {
                        @Override
                        public void onCompleted(JSONObject object, GraphResponse response)
                        {
                            JSONObject jsonObject = response.getJSONObject();

                            String result1 = "";
                            String result2 = "";
                            
                            try {
                                result1 = jsonObject.getString("name");
                                result2 = jsonObject.getString("email");
                            } catch (JSONException e) {
                                e.printStackTrace();
                            }

                            final String finalResult1 = result1;
                            final String finalResult2 = result2;
                            
                            currentActivity.runOnGLThread(new Runnable() {
                                @Override
                                public void run() {
                                    executeTwoStringsCallback(callbackID, finalResult1, finalResult2);
                                }
                            });
                        }
                    });

            Bundle parameters = new Bundle();
            parameters.putString("fields", "name, email");
            request.setParameters(parameters);
            request.executeAsync();
        }
    }

    public static void LogPurchase(double price, String currency, String type, String productID)
    {
        if (logger == null)
            logger = AppEventsLogger.newLogger(currentContext);

        Bundle parameters = new Bundle();
        parameters.putInt(AppEventsConstants.EVENT_PARAM_NUM_ITEMS, 1);
        parameters.putString(AppEventsConstants.EVENT_PARAM_CONTENT_TYPE, type);
        parameters.putString(AppEventsConstants.EVENT_PARAM_CONTENT_ID, productID);

        BigDecimal dec = BigDecimal.valueOf(price);
        Currency cur = Currency.getInstance(currency);

        logger.logPurchase(dec, cur, parameters);
    }

    public static void MakeFriendsRequest(final String title, final String message, final int filter, final int callbackID)
    {
        if (currentActivity == null) return;

        requestCallbackID = callbackID;

        currentActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                GameRequestContent content = new GameRequestContent.Builder()
                        .setMessage(message)
                        .setTitle(title)
                        .setFilters( (filter == 2 ? GameRequestContent.Filters.APP_NON_USERS : GameRequestContent.Filters.APP_USERS))
                        .setActionType(GameRequestContent.ActionType.TURN)
                        .setData(String.valueOf(callbackID))
                        .build();

                requestDialog.show(content);
            }
        });
    }

    public static void GetUsersInfo(final String[] users, final int callbackID)
    {
        String graphPath = "?ids";

        for (int i = 0; i < users.length; i++)
        {
            graphPath += (i > 0 ? "," : "=") + users[i];
        }

        GraphRequest request = GraphRequest.newGraphPathRequest(AccessToken.getCurrentAccessToken(), graphPath, new GraphRequest.Callback() {
            @Override
            public void onCompleted(GraphResponse response) {
                JSONObject jsonObject = response.getJSONObject();

                if (jsonObject != null) {
                    ArrayList<String> friendIDs   = new ArrayList<String>();
                    ArrayList<String> friendNames = new ArrayList<String>();

                    try {
                        for (int i = 0; i < users.length; i++) {
                            JSONObject dist = jsonObject.getJSONObject(users[i]);

                            if (dist != null)
                            {
                                friendIDs.add(users[i]);
                                friendNames.add(dist.getString("name"));
                            }
                        }
                    } catch (JSONException e) {
                        e.printStackTrace();
                        currentActivity.runOnGLThread(new Runnable() {
                            @Override
                            public void run() {
                                executeGetFriendsDetailsCallback(false, null, null, callbackID);
                            }
                        });

                        return;
                    }

                    String[] a1 = new String[friendIDs.size()];
                    String[] a2 = new String[friendNames.size()];

                    a1 = friendIDs.toArray(a1);
                    a2 = friendNames.toArray(a2);

                    final String[] finalA1 = a1;
                    final String[] finalA2 = a2;

                    currentActivity.runOnGLThread(new Runnable() {
                        @Override
                        public void run() {
                            executeGetFriendsDetailsCallback(true, finalA1, finalA2, callbackID);
                        }
                    });
                }
                else
                {
                    currentActivity.runOnGLThread(new Runnable() {
                        @Override
                        public void run() {
                            executeGetFriendsDetailsCallback(false, null, null, callbackID);
                        }
                    });
                }
            }
        });

        request.executeAsync();
    }


    private static native void executeBooleanCallback(int handlerID, boolean result);
    private static native void executeGetRequestsCallback(String[] requestIDs, String[] senderIDs, String[] senderNames, String[] objectIDs);
    private static native void executeStringCallback(int handlerID, String result);
    private static native void executeTwoStringsCallback(int handlerID, String result1, String result2);
    private static native void executeGetFriendsCallback(boolean success, String[] usersIDs, String[] usersNames, int handlerID);
    private static native void executeRequestRecipients(boolean success, String[] usersIDs, int handlerID);
    private static native void executeGetFriendsDetailsCallback(boolean success, String[] usersIDs, String[] usersNames, int handlerID);
}
