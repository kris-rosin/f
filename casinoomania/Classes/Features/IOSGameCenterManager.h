//
//  IOSGameCenterManager.h
//
//  Created by papo on 10/31/13.
//
//

#ifndef __IOSGameCenterManager_h__
#define __IOSGameCenterManager_h__

class IOSGameCenterManager
{
public:
    static IOSGameCenterManager* shared();
    void vibration();
    void loginGameCenter();
    void showLeaderBoard();
    void submitAchievement(const char* identifier);
    void sendScore(int score, const char* cagatory);    
};


#endif /* #define __IOSManager_h__ */
