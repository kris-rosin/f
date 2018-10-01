//
//  ShareManager-iOS.mm
//
//  Created by Stanislav Zheronkin on 23.08.17.
//

#include "ShareManager-iOS.h"
#import <MessageUI/MessageUI.h>
#import "AppController.h"
#import <Social/Social.h>
#import <FBSDKShareKit/FBSDKShareKit.h>

#include "cocos2d.h"

@interface SharingDelegate : NSObject <FBSDKSharingDelegate>
{
    std::function<void(bool)> _callback;
}

-(id) initWithCallback: (std::function<void(bool)>)callback;

@end

@implementation SharingDelegate

-(id) initWithCallback: (std::function<void(bool)>)callback
{
    if((self = [super init]))
    {
        _callback = callback;
    }
    
    return self;
}

- (void)sharer:(id<FBSDKSharing>)sharer didCompleteWithResults:(NSDictionary *)results
{
    if (_callback)
    {
        _callback(true);
    }
}

- (void)sharer:(id<FBSDKSharing>)sharer didFailWithError:(NSError *)error
{
    NSLog(@"fail %@",error.description);
    if (_callback)
    {
        _callback(false);
    }
}

- (void)sharerDidCancel:(id<FBSDKSharing>)sharer
{
    if (_callback)
    {
        _callback(false);
    }
}

@end

#pragma mark ObjC++ code

void ShareManager_iOS::sharePhotoFacebook(std::string photo, std::string optionalURL)
{
    FBSDKSharePhotoContent * content = [FBSDKSharePhotoContent new];
    UIImage * photoImage = [UIImage imageWithContentsOfFile:[NSString stringWithUTF8String:photo.c_str()]];
    FBSDKSharePhoto * photoObject = [FBSDKSharePhoto photoWithImage:photoImage userGenerated:YES];
    
    content.photos = @[photoObject];
    content.contentURL = [NSURL URLWithString: [NSString stringWithUTF8String:optionalURL.c_str()]];
    
    FBSDKShareDialog * dialog = [FBSDKShareDialog new];
    dialog.mode = FBSDKShareDialogModeNative;
    dialog.shareContent = content;
    dialog.fromViewController = (UIViewController*)((AppController*)[UIApplication sharedApplication].delegate).viewController;
    
    if ([dialog canShow])
    {
        [dialog show];
        return;
    }
    
    dialog.mode = FBSDKShareDialogModeShareSheet;
    if ([dialog canShow])
    {
        [dialog show];
        return;
    }
    
    dialog.mode = FBSDKShareDialogModeFeedWeb;
    if ([dialog canShow])
    {
        [dialog show];
        return;
    }
}

void ShareManager_iOS::shareURLFacebook(std::string url, std::function<void(bool)> callback, std::string optionalQuote)
{
    FBSDKShareLinkContent * content = [FBSDKShareLinkContent new];
    
    content.contentURL = [NSURL URLWithString:[NSString stringWithUTF8String:url.c_str()]];
    content.quote = [NSString stringWithUTF8String:optionalQuote.c_str()];
    
    SharingDelegate * sharingDelegate = [[SharingDelegate alloc] initWithCallback:callback];
    
    FBSDKShareDialog * dialog = [FBSDKShareDialog new];
    dialog.mode = FBSDKShareDialogModeNative;
    dialog.shareContent = content;
    dialog.fromViewController = (UIViewController*)((AppController*)[UIApplication sharedApplication].delegate).viewController;
    dialog.delegate = sharingDelegate;
    
    if ([dialog canShow])
    {
        [dialog show];
        return;
    }
    
    dialog.mode = FBSDKShareDialogModeShareSheet;
    if ([dialog canShow])
    {
        [dialog show];
        return;
    }
    
    dialog.mode = FBSDKShareDialogModeFeedWeb;
    if ([dialog canShow])
    {
        [dialog show];
        return;
    }
}

