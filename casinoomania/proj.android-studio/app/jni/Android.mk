LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../../Classes/Features/AdmobChartboost.cpp \
                   ../../../Classes/Features/AppManager.cpp \
                   ../../../Classes/Features/PaymentManager.cpp \
                   ../../../Classes/Features/mConstants.cpp \
                   ../../../Classes/FacebookManager/FacebookManager.cpp \
                   ../../../Classes/ServerAPI/ServerMessage.cpp \
                   ../../../Classes/ServerAPI/ServerConnection.cpp \
                   ../../../Classes/ServerAPI/MessageContainer.cpp \
                   ../../../Classes/ServerAPI/crc32.cpp \
                   ../../../Classes/Platform/Platform.cpp \
                   ../../../Classes/AndroidCompatibility.cpp \
                   ../../../Classes/BaseScene.cpp \
                   ../../../Classes/AppDelegate.cpp \
                   ../../../Classes/AudioManager.cpp \
                   ../../../Classes/BaccaratConfig.cpp \
                   ../../../Classes/BaccaratDeck.cpp \
                   ../../../Classes/BaccaratGameScene.cpp \
                   ../../../Classes/BaccaratHand.cpp \
                   ../../../Classes/BaccaratSelectThemeScene.cpp \
                   ../../../Classes/BJConfig.cpp \
                   ../../../Classes/BJDealer.cpp \
                   ../../../Classes/BJGameScene.cpp \
                   ../../../Classes/BJPlayer.cpp \
                   ../../../Classes/BJSelectThemeScene.cpp \
                   ../../../Classes/BottomBar.cpp \
                   ../../../Classes/CardObject.cpp \
                   ../../../Classes/CasinowarConfig.cpp \
                   ../../../Classes/CasinowarDeck.cpp \
                   ../../../Classes/CasinowarGameScene.cpp \
                   ../../../Classes/CasinowarHand.cpp \
                   ../../../Classes/CasinowarThemeScene.cpp \
                   ../../../Classes/ChestsView.cpp \
                   ../../../Classes/CoinsPanel.cpp \
                   ../../../Classes/CoinsRainView.cpp \
                   ../../../Classes/CoinToken.cpp \
                   ../../../Classes/CongratulationsView.cpp \
                   ../../../Classes/DataBuffer.cpp \
                   ../../../Classes/DealerHand.cpp \
                   ../../../Classes/Deck.cpp \
                   ../../../Classes/DropDownList.cpp \
                   ../../../Classes/ExperiencePanel.cpp \
                   ../../../Classes/FreeSpinView.cpp \
                   ../../../Classes/FriendsItem.cpp \
                   ../../../Classes/FriendsView.cpp \
                   ../../../Classes/Hand.cpp \
                   ../../../Classes/HandView.cpp \
                   ../../../Classes/JackpotPanel.cpp \
                   ../../../Classes/LoadingScene.cpp \
                   ../../../Classes/LoadingSplash.cpp \
                   ../../../Classes/MainMenuScene.cpp \
                   ../../../Classes/MessageAlert.cpp \
                   ../../../Classes/RatemeAlert.cpp \
                   ../../../Classes/PayoutsView.cpp \
                   ../../../Classes/PopupMenu.cpp \
                   ../../../Classes/PlayerProfile.cpp \
                   ../../../Classes/RouletteConfig.cpp \
                   ../../../Classes/RouletteGameScene.cpp \
                   ../../../Classes/RouletteSelectThemeScene.cpp \
                   ../../../Classes/ScrollBar.cpp \
                   ../../../Classes/SettingsView.cpp \
                   ../../../Classes/SignupView.cpp \
                   ../../../Classes/Shoe.cpp \
                   ../../../Classes/ShopView.cpp \
                   ../../../Classes/SharePhotoPopup.cpp \
                   ../../../Classes/ShareManager/ShareManager.cpp \
                   ../../../Classes/SlotsConfig.cpp \
                   ../../../Classes/SlotsGameScene.cpp \
                   ../../../Classes/SlotsSelectThemeScene.cpp \
                   ../../../Classes/SwitchControl.cpp \
                   ../../../Classes/TopBar.cpp \
                   ../../../Classes/Utilities.cpp \
                   ../../../Classes/WheelView.cpp \
                   ../../../Classes/FacebookAddFriendsPopup.cpp \
									 ../../../Classes/Capsa/Big2PokerModel.cpp \
									 ../../../Classes/Capsa/CapsaCreateRoomLayer.cpp \
									 ../../../Classes/Capsa/CapsaGameScene.cpp \
									 ../../../Classes/Capsa/CapsaLobbyScene.cpp \
									 ../../../Classes/Capsa/CapsaPlayer.cpp \
									 ../../../Classes/Capsa/CapsaPlayerNode.cpp \
									 ../../../Classes/Capsa/CapsaRoom.cpp \
									 ../../../Classes/Capsa/CapsaThemeScene.cpp \
									 ../../../Classes/Capsa/CapsaTutorialScene.cpp \
									 ../../../Classes/Capsa/CapsaRuleLayer.cpp \
									 ../../../Classes/Capsa/CapsaInviteList.cpp \
									 ../../../Classes/Capsa/CapsaInviteUser.cpp \
									 ../../../Classes/Capsa/CapsaInviteUserManager.cpp \
									 ../../../Classes/Capsa/CardSprite.cpp \
									 ../../../Classes/Capsa/RoomSprite.cpp \
									 ../../../Classes/OnlineModel/AvatarSprite.cpp \
									 ../../../Classes/OnlineModel/CreatePlayerScene.cpp \
									 ../../../Classes/OnlineModel/GameHandler.cpp \
									 ../../../Classes/OnlineModel/Label64.cpp \
									 ../../../Classes/OnlineModel/ProcessLayer.cpp \
									 ../../../Classes/AppWarpX/SHA1.cpp \
									 ../../../Classes/AppWarpX/appwarp.cpp \
									 ../../../Classes/AppWarpX/appwarp_extended.cpp \
									 ../../../Classes/AppWarpX/requests.cpp \
									 ../../../Classes/AppWarpX/socket.cpp \
									 ../../../Classes/AppWarpX/udpsocket.cpp \
									 ../../../Classes/AppWarpX/utilities.cpp \
									 ../../../Classes/App42PushHelper/App42PushHelper.cpp \
									 ../../../Classes/App42/App42HTTP/App42HttpClient.cpp \
									 ../../../Classes/App42/AvatarService/App42AvatarResponse.cpp \
									 ../../../Classes/App42/AvatarService/AvatarService.cpp \
									 ../../../Classes/App42/BuddyService/App42BuddyResponse.cpp \
									 ../../../Classes/App42/BuddyService/BuddyService.cpp \
									 ../../../Classes/App42/cJSON/cJSON.cpp \
									 ../../../Classes/App42/Common/ACL.cpp \
									 ../../../Classes/App42/Common/App42API.cpp \
									 ../../../Classes/App42/Common/App42Exception.cpp \
									 ../../../Classes/App42/Common/App42GeoPoint.cpp \
									 ../../../Classes/App42/Common/App42Object.cpp \
									 ../../../Classes/App42/Common/App42Response.cpp \
									 ../../../Classes/App42/Common/App42Service.cpp \
									 ../../../Classes/App42/Common/JSONDocument.cpp \
									 ../../../Classes/App42/Common/MetaResponse.cpp \
									 ../../../Classes/App42/Crypto/app42base64.cpp \
									 ../../../Classes/App42/Crypto/HMAC_SHA1.cpp \
									 ../../../Classes/App42/Crypto/urlencode.cpp \
									 ../../../Classes/App42/CustomCodeService/App42CustomCodeResponse.cpp \
									 ../../../Classes/App42/CustomCodeService/CustomCodeService.cpp \
									 ../../../Classes/App42/EmailService/App42EmailResponse.cpp \
									 ../../../Classes/App42/EmailService/EmailService.cpp \
									 ../../../Classes/App42/GameService/App42GameResponse.cpp \
									 ../../../Classes/App42/GameService/App42RewardResponse.cpp \
									 ../../../Classes/App42/GameService/App42Score.cpp \
									 ../../../Classes/App42/GameService/GameService.cpp \
									 ../../../Classes/App42/GameService/RewardService.cpp \
									 ../../../Classes/App42/GameService/ScoreBoardService.cpp \
									 ../../../Classes/App42/GameService/ScoreService.cpp \
									 ../../../Classes/App42/GeoService/App42GeoResponse.cpp \
									 ../../../Classes/App42/GeoService/GeoService.cpp \
									 ../../../Classes/App42/PushNotificationService/App42PushNotificationResponse.cpp \
									 ../../../Classes/App42/PushNotificationService/PushNotificationService.cpp \
									 ../../../Classes/App42/SessionService/App42SessionResponse.cpp \
									 ../../../Classes/App42/SessionService/SessionService.cpp \
									 ../../../Classes/App42/SocialService/App42FacebookProfile.cpp \
									 ../../../Classes/App42/SocialService/App42Friend.cpp \
									 ../../../Classes/App42/SocialService/App42PublicProfile.cpp \
									 ../../../Classes/App42/SocialService/App42Social.cpp \
									 ../../../Classes/App42/SocialService/App42SocialResponse.cpp \
									 ../../../Classes/App42/SocialService/SocialService.cpp \
									 ../../../Classes/App42/StorageService/App42StorageResponse.cpp \
									 ../../../Classes/App42/StorageService/Query.cpp \
									 ../../../Classes/App42/StorageService/QueryBuilder.cpp \
									 ../../../Classes/App42/StorageService/StorageService.cpp \
									 ../../../Classes/App42/TimerService/App42TimerResponse.cpp \
									 ../../../Classes/App42/TimerService/TimerService.cpp \
									 ../../../Classes/App42/UploadService/App42UploadResponse.cpp \
									 ../../../Classes/App42/UploadService/UploadService.cpp \
									 ../../../Classes/App42/UserService/App42UserResponse.cpp \
									 ../../../Classes/App42/UserService/UserService.cpp 

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes \
                 $(LOCAL_PATH)/../../../Classes/Features \
                 $(LOCAL_PATH)/../../../Classes/ShareManager \
                 $(LOCAL_PATH)/../../../Classes/ServerAPI \
                 $(LOCAL_PATH)/../../../Classes/Platform \
                 $(LOCAL_PATH)/../../../Classes/FacebookManager \
				 $(LOCAL_PATH)/../../../Classes/Capsa \
				 $(LOCAL_PATH)/../../../Classes/OnlineModel \
				 $(LOCAL_PATH)/../../../Classes/AppWarpX \
                 $(LOCAL_PATH)/../../../Classes/App42/App42HTTP \
                 $(LOCAL_PATH)/../../../Classes/App42/AvatarService \
                 $(LOCAL_PATH)/../../../Classes/App42/BuddyService \
                 $(LOCAL_PATH)/../../../Classes/App42/cJSON \
                 $(LOCAL_PATH)/../../../Classes/App42/Common \
                 $(LOCAL_PATH)/../../../Classes/App42/Crypto \
                 $(LOCAL_PATH)/../../../Classes/App42/CustomCodeService \
                 $(LOCAL_PATH)/../../../Classes/App42/EmailService \
                 $(LOCAL_PATH)/../../../Classes/App42/GameService \
                 $(LOCAL_PATH)/../../../Classes/App42/GeoService \
                 $(LOCAL_PATH)/../../../Classes/App42/PushNotificationService \
                 $(LOCAL_PATH)/../../../Classes/App42/SessionService \
                 $(LOCAL_PATH)/../../../Classes/App42/SocialService \
                 $(LOCAL_PATH)/../../../Classes/App42/StorageService \
                 $(LOCAL_PATH)/../../../Classes/App42/TimerService \
                 $(LOCAL_PATH)/../../../Classes/App42/UploadService \
                 $(LOCAL_PATH)/../../../Classes/App42/UserService \
                 $(LOCAL_PATH)/../../../Classes/App42/Util \
			     $(LOCAL_PATH)/../../../Classes/App42PushHelper \
                 $(LOCAL_PATH)/../../../cocos2d/external/curl/include/android \
                 $(LOCAL_PATH)/hellocpp

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_curl_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,audio/android)
$(call import-module,curl/prebuilt/android)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
