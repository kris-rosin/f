//
//  App42Define.h
//  DovemobiCasino
//
//  Created by liu on 9/7/17.
//
//

#ifndef App42Define_h
#define App42Define_h

#define APP42_API_KEY "45b3eff206e915158d5bfc57222423540ed7e6a277cda1121621395e08248c71"
#define APP42_SEC_KEY "26f1f96e1d0f3399ab44b6ffde1f1216edc096daa2dfa99c06614f9cdd8f0db6"

#define AVATAR  "avatar"

#define     FBUSER_PREFIX           "@fbuser#!$"
#define     FBUSER_PRE_POS(x)       (x.find(FBUSER_PREFIX))
#define     IS_CONTAIN_FBUSER_PRE(x)(FBUSER_PRE_POS(x) != std::string::npos)

#define     PLAYERNAME_MAX_LEN      20

#define     CONNECTION_RECOVER_TIME 1.0f

#endif /* App42Define_h */
