//
//  FacebookManager-iOS.mm
//
//  Created by Stanislav Zheronkin on 06.09.16.
//  Copyright (c) 2016 CodeExotics. All rights reserved.
//

#import "FacebookManager-iOS.h"

//#import "SendRequestDelegate.h"
#import <FBSDKCoreKit/FBSDKCoreKit.h>
#import <FBSDKLoginKit/FBSDKLoginKit.h>
#import <FBSDKShareKit/FBSDKShareKit.h>
#import <Bolts/Bolts.h>
#include "AppController.h"

@interface SendRequestDelegate : NSObject <FBSDKGameRequestDialogDelegate>
{
    std::function<void(bool, std::vector<std::string>)> _callback;
}

-(id) initWithCallback: (std::function<void(bool, std::vector<std::string>)>)callback;

@end

@implementation SendRequestDelegate

-(id) initWithCallback: (std::function<void(bool, std::vector<std::string>)>)callback
{
    if((self = [super init]))
    {
        _callback = callback;
    }
    
    return self;
}

- (void)gameRequestDialog:(FBSDKGameRequestDialog *)gameRequestDialog didCompleteWithResults:(NSDictionary *)results {
    NSLog(@"%@", results);
    
    std::vector<std::string> result;
    
    if (results != nil)
    {
        NSArray * ids = [results objectForKey:@"to"];
        
        if (ids != nil)
        {
            for (NSString * line in ids)
            {
                std::string user_id =  [line UTF8String];
                
                result.push_back(user_id);
            }
        }
    }
    
    if (_callback)
        _callback(true, result);
}

- (void)gameRequestDialog:(FBSDKGameRequestDialog *)gameRequestDialog didFailWithError:(NSError *)error {
    NSLog(@"%@", error);
    
    std::vector<std::string> result;
    
    if (_callback)
        _callback(false, result);
}

- (void)gameRequestDialogDidCancel:(FBSDKGameRequestDialog *)gameRequestDialog {
    NSLog(@"a game request is canceled");
    
    std::vector<std::string> result;
    
    if (_callback)
        _callback(false, result);
}

@end

#pragma mark ObjC++ code

static SendRequestDelegate * gameRequestDelegate = nil;

void FacebookManager_iOS::Login(std::function<void(bool)> completionHandler)
{
    FBSDKLoginManager * login = [[FBSDKLoginManager alloc] init];
    [login logInWithReadPermissions: @[@"public_profile", @"email", @"user_friends"]
                 fromViewController: (UIViewController*)((AppController*) [UIApplication sharedApplication].delegate).viewController
                            handler:^(FBSDKLoginManagerLoginResult *result, NSError *error)
    {
         if (error) {
             NSLog(@"Process error");
             if (completionHandler)
                 completionHandler(false);
         } else if (result.isCancelled) {
             NSLog(@"Cancelled");
             if (completionHandler)
                 completionHandler(false);
         } else {
             NSLog(@"Logged in");
             if (completionHandler)
                 completionHandler(true);
         }
    }];
}

void FacebookManager_iOS::Logout(std::function<void(bool)> completionHandler)
{
    FBSDKLoginManager * login = [[FBSDKLoginManager alloc] init];
    [login logOut];
    
    if (completionHandler)
        completionHandler(true);

}

bool FacebookManager_iOS::isLoggedIn()
{
    if ([[FBSDKAccessToken currentAccessToken] hasGranted:@"user_friends"])
    {
        return true;
    }
    
    return false;
}

//Combo method
void FacebookManager_iOS::LoginAndGetName(std::function<void(bool, const std::string &, const std::string &)> callback)
{
    if (isLoggedIn())
    {
        GetCurrentUserName([callback](std::string name, std::string email)
                           {
                               if (callback)
                                   callback(!name.empty(),GetUserID(), name);
                           });
    }
    else
    {
        Login([callback](bool result)
        {
            if (result)
            {
                GetCurrentUserName([callback](std::string name, std::string email)
                {
                    if (callback)
                        callback(!name.empty(),GetUserID(), name);
                });
            }
            else
            {
                if (callback)
                    callback(false, "", "");
            }
        });
    }
}

std::string FacebookManager_iOS::GetUserID()
{
    if (isLoggedIn())
    {
        return [[[FBSDKAccessToken currentAccessToken] userID] UTF8String];
    }
    else
    {
        return "";
    }
}

void FacebookManager_iOS::GetCurrentUserName(std::function<void(std::string, std::string)> completionHandler)
{
    if ([FBSDKAccessToken currentAccessToken]) {
        [[[FBSDKGraphRequest alloc] initWithGraphPath:@"me?fields=name,email" parameters:nil]
         startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
             std::string name;
             std::string email;
             
             if (!error)
             {
                 NSString *_name = [result objectForKey:@"name"];
                 NSString *_mail = [result objectForKey:@"email"];
                 name  = [_name UTF8String];
                 email = [_mail UTF8String];
             }
             
             if (completionHandler) completionHandler(name, email);
         }];
    }
}

void FacebookManager_iOS::GetFriends(std::vector<FacebookManager::FriendDetails> & friendslist, std::function<void(bool)> completionHandler /*=nullptr*/, std::string page /*= ""*/)
{
    if ([FBSDKAccessToken currentAccessToken])
    {
        [[[FBSDKGraphRequest alloc] initWithGraphPath:(page.empty() ? @"me/friends?fields=id,name" : [NSString stringWithUTF8String:page.c_str()] ) parameters:nil]
         startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
             if (!error)
             {
                 NSLog(@"fetched result:%@", result);
                 
                 NSArray * array = [result valueForKey:@"data"];
                 
                 for(int i = 0; i < array.count; i++)
                 {
                     NSDictionary * friendObject = [array objectAtIndex:i];
                     
                     NSString * fid   = [friendObject objectForKey:@"id"];
                     NSString * fname = [friendObject objectForKey:@"name"];
                     
                     FacebookManager::FriendDetails item;
                     item.user_id = [fid UTF8String];
                     item.name    = [fname UTF8String];
                     
                     friendslist.push_back(item);
                 }
                 
                 NSDictionary * paging = [result valueForKey:@"paging"];
                 if (paging != nil)
                 {
                     NSString * next = [paging objectForKey:@"next"];
                     if (next)
                     {
                         NSLog(@"next page available:%@", next);
                         GetFriends(friendslist, completionHandler, [next UTF8String]);
                         return;
                     }
                     else
                     {
                         if (completionHandler)
                             completionHandler(true);
                         return;
                     }
                 }
                 else
                 {
                     if (completionHandler)
                         completionHandler(true);
                     return;
                 }
             }
             else
             {
                 if (completionHandler)
                     completionHandler(false);
                 return;
             }
         }];
        
        return;
    }
    
    if (completionHandler)
        completionHandler(false);
}

void FacebookManager_iOS::AskForFriendsRequest(std::string object_id, std::string title, std::string message, FacebookManager::FriendsFilter filter)
{
    FBSDKGameRequestContent * gameRequestContent = [[FBSDKGameRequestContent alloc] init];
    
    gameRequestContent.message = [NSString stringWithUTF8String:title.c_str()];
    gameRequestContent.title   = [NSString stringWithUTF8String:message.c_str()];
    gameRequestContent.filters = (filter == FacebookManager::FriendsFilter::ALL_FRIENDS ? FBSDKGameRequestFilterNone : (filter == FacebookManager::FriendsFilter::APP_USERS ? FBSDKGameRequestFilterAppUsers : FBSDKGameRequestFilterAppNonUsers));
    gameRequestContent.objectID = [NSString stringWithUTF8String:object_id.c_str()];
    gameRequestContent.actionType = FBSDKGameRequestActionTypeAskFor;
    
    if(!gameRequestDelegate){
        gameRequestDelegate = [[SendRequestDelegate alloc] init];
    }
    
    [FBSDKGameRequestDialog showWithContent:gameRequestContent delegate:gameRequestDelegate];
}

void FacebookManager_iOS::MakeFriendsRequest(std::string title, std::string message, FacebookManager::FriendsFilter filter, std::function<void(bool, std::vector<std::string>)> callback)
{
    FBSDKGameRequestContent * gameRequestContent = [[FBSDKGameRequestContent alloc] init];
    
    gameRequestContent.message = [NSString stringWithUTF8String:message.c_str()];
    gameRequestContent.title   = [NSString stringWithUTF8String:title.c_str()];
    gameRequestContent.filters = (filter == FacebookManager::FriendsFilter::ALL_FRIENDS ? FBSDKGameRequestFilterNone : (filter == FacebookManager::FriendsFilter::APP_USERS ? FBSDKGameRequestFilterAppUsers : FBSDKGameRequestFilterAppNonUsers));
    gameRequestContent.actionType = FBSDKGameRequestActionTypeNone;
    
    SendRequestDelegate * localDelegate = [[SendRequestDelegate alloc] initWithCallback:callback];
    [localDelegate retain];
    
    [FBSDKGameRequestDialog showWithContent:gameRequestContent delegate:localDelegate];
}

void FacebookManager_iOS::GetRequests(std::function<void(std::vector<FacebookManager::GameRequestRecord>)> completionHandler/* = nullptr*/)
{
    if ([FBSDKAccessToken currentAccessToken]) {
        [[[FBSDKGraphRequest alloc] initWithGraphPath:@"me/apprequests?fields=object,id,from" parameters:nil]
         startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
             std::vector<FacebookManager::GameRequestRecord> requests;
             
             if (!error) {
                 NSLog(@"fetched user:%@", result);
                 
                 requests.reserve(5);
                 
                 NSArray * array = [result valueForKey:@"data"];
                 
                 for(int i = 0; i < array.count; i++)
                 {
                     NSDictionary * dist = [array objectAtIndex:i];
                     NSString * rid = [dist objectForKey:@"id"];
                     NSDictionary * from = [dist objectForKey:@"from"];
                     NSString * from_id = [from objectForKey:@"id"];
                     NSString * from_name = [from objectForKey:@"name"];
                     NSDictionary * object = [dist objectForKey:@"object"];
                     NSString * object_id = [object objectForKey:@"id"];
                     
                     FacebookManager::GameRequestRecord record;
                     record.request_id = [rid UTF8String];
                     record.sender     = [from_id UTF8String];
                     record.name       = [from_name UTF8String];
                     record.object     = [object_id UTF8String];
                     
                     requests.push_back(record);
                 }
             }
             
             if (completionHandler) completionHandler(requests);
         }];
    }
}

bool FacebookManager_iOS::hasWritePermission()
{
    return [[FBSDKAccessToken currentAccessToken] hasGranted:@"publish_actions"];
}

void FacebookManager_iOS::RequestWritePermission(std::function<void(bool)> completionHandler)
{
    FBSDKLoginManager * loginManager = [[FBSDKLoginManager alloc] init];
    
    [loginManager logInWithPublishPermissions:@[@"publish_actions"]
                           fromViewController:(UIViewController*)((AppController*) [UIApplication sharedApplication].delegate).viewController
                                      handler:^(FBSDKLoginManagerLoginResult *result, NSError *error)
    {
        if (hasWritePermission())
        {
            if (completionHandler) completionHandler(true);
        }
        else
        {
            if (completionHandler) completionHandler(false);
        }
    }];
}

void FacebookManager_iOS::AcceptAskForRequest(FacebookManager::GameRequestRecord request, std::string response_object_id, std::string title, std::string message)
{
    FBSDKGameRequestContent * gameRequestContent = [[FBSDKGameRequestContent alloc] init];
    
    gameRequestContent.message = [NSString stringWithUTF8String:message.c_str()];
    gameRequestContent.title   = [NSString stringWithUTF8String:title.c_str()];
    gameRequestContent.recipients = [NSArray arrayWithObject:[NSString stringWithUTF8String:request.sender.c_str()]];
    gameRequestContent.objectID = [NSString stringWithUTF8String:response_object_id.c_str()];
    gameRequestContent.actionType = FBSDKGameRequestActionTypeSend;
    
    if(!gameRequestDelegate){
        gameRequestDelegate = [[SendRequestDelegate alloc] init];
    }
    
    [FBSDKGameRequestDialog showWithContent:gameRequestContent delegate:gameRequestDelegate];
    
    DeleteRequest(request);
}

void FacebookManager_iOS::DeleteRequest(FacebookManager::GameRequestRecord request)
{
    if ([FBSDKAccessToken currentAccessToken]) {
        [[[FBSDKGraphRequest alloc] initWithGraphPath:[NSString stringWithUTF8String:request.request_id.c_str()] parameters:nil HTTPMethod:@"DELETE"]
         startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
             if (!error) {
                 NSLog(@"fetched responce:%@", result);
             }
         }];
    }
}

void FacebookManager_iOS::GetUserProfilePictureAsync(std::string userID, int size, std::function<void(std::string)> callback)
{
    if (userID.empty())
    {
        if (callback) callback("");
        return;
    }
    
    NSString * graphPath = [NSString stringWithFormat:@"%@/picture?redirect=false&type=square&height=%d&width=%d", [NSString stringWithUTF8String:userID.c_str()], size, size];
    
    if ([FBSDKAccessToken currentAccessToken]) {
        [[[FBSDKGraphRequest alloc] initWithGraphPath:graphPath parameters:nil]
         startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
             std::string path = "";
             
             if (!error) {
                 NSLog(@"fetched responce:%@", result);
                 NSDictionary * array = [result valueForKey:@"data"];
                 if (array != nil)
                 {
                     NSString * image_url = [array objectForKey:@"url"];
                     path = [image_url UTF8String];
                 }
             }
             
             if (callback)
                 callback(path);
         }];
    }
}

void FacebookManager_iOS::ActivateApp()
{
    [FBSDKAppEvents activateApp];
}

void FacebookManager_iOS::Initialize(void * application, void * launchOptions)
{
    [[FBSDKApplicationDelegate sharedInstance] application:(UIApplication*)application
                             didFinishLaunchingWithOptions:(NSDictionary*)launchOptions];
}

bool FacebookManager_iOS::handleOpenURL(void * application, void * url, void * options)
{
    bool handled = [[FBSDKApplicationDelegate sharedInstance] application:(UIApplication*)application
                                                                  openURL:(NSURL*)url
                                                        sourceApplication:((NSDictionary<UIApplicationOpenURLOptionsKey,id>*)options)[UIApplicationOpenURLOptionsSourceApplicationKey]
                                                               annotation:((NSDictionary<UIApplicationOpenURLOptionsKey,id>*)options)[UIApplicationOpenURLOptionsAnnotationKey]
                    ];
    // Add any custom logic here.
    return handled;
}

void FacebookManager_iOS::InviteFriends(std::string AppLink)
{
    if ([FBSDKAccessToken currentAccessToken])
    {
        FBSDKAppInviteContent * content = [[FBSDKAppInviteContent alloc] init];
        content.appLinkURL = [NSURL URLWithString:[NSString stringWithUTF8String:AppLink.c_str()]];
        //optionally set previewImageURL
        //content.appInvitePreviewImageURL = [NSURL URLWithString:@"http://www.domain.com/image.jpg"];
        
        // Present the dialog. Assumes self is a view controller
        // which implements the protocol `FBSDKAppInviteDialogDelegate`.
        [FBSDKAppInviteDialog showFromViewController:(UIViewController*)((AppController*) [UIApplication sharedApplication].delegate).viewController
                                         withContent:content
                                            delegate:nil];
    }
}

void FacebookManager_iOS::GetUsersInfo(std::vector<std::string> users, std::function<void(bool, std::vector<FacebookManager::FriendDetails>)> callback)
{
    if ([FBSDKAccessToken currentAccessToken])
    {
        std::string request("?ids");
        
        for (int i = 0; i < users.size(); i++)
        {
            request.append(i > 0 ? "," : "=").append(users[i]);
        }
        
        request.append("&fields=id,name");
        
        [[[FBSDKGraphRequest alloc] initWithGraphPath:[NSString stringWithUTF8String:request.c_str()] parameters:nil]
         startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error)
        {
            std::vector<FacebookManager::FriendDetails> friendslist;
            
             if (!error)
             {
                 NSLog(@"fetched result:%@", result);
                 
                 for (int i = 0; i < users.size(); i++)
                 {
                     NSDictionary * friendInfo = [result objectForKey:[NSString stringWithUTF8String:users[i].c_str()]];
                     
                     NSString * fid   = [friendInfo objectForKey:@"id"];
                     NSString * fname = [friendInfo objectForKey:@"name"];
                     
                     FacebookManager::FriendDetails item;
                     item.user_id = [fid UTF8String];
                     item.name    = [fname UTF8String];
                     
                     friendslist.push_back(item);
                 }
                 
                 if (callback)
                     callback(true, friendslist);
                 
                 return;
            }
            else
            {
                std::vector<FacebookManager::FriendDetails> empty;
                
                if (callback)
                    callback(false, empty);
            }
            
         }];
        
        return;
    }
    
    std::vector<FacebookManager::FriendDetails> empty;
    
    if (callback)
        callback(false, empty);
}

void FacebookManager_iOS::LogPurchase(double price, void * currency, void * product, void * productID)
{
    [FBSDKAppEvents logPurchase:price currency:(NSString*)currency parameters:@{ FBSDKAppEventParameterNameNumItems : @1, FBSDKAppEventParameterNameContentType : (NSString*)product, FBSDKAppEventParameterNameContentID : (NSString*)productID }];
}
