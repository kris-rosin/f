//
//  PlayerProfile.h
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 31.10.16.
//
//

#pragma once

#include <string>
#include <map>

class DataBuffer;

class PlayerProfile
{
private:
    std::map< std::string, std::string > valuesMap;
    
    std::string loadedFilePath;
    const unsigned char binaryVersion = 0x02;
private:
    PlayerProfile();
    
    void generateRandomSlotsJackpot();
    unsigned int experinceForLevel(unsigned int level);

public:
    static PlayerProfile & getInstance();
    
    void setDefaults();
    
    void Load(std::string filename);
    void Save();
    void reinitWithData(DataBuffer * buffer);
    DataBuffer * getDataFile();
    
    unsigned long long getCoins() const;
    void setCoins(unsigned long long value);
    void addCoins(long delta);
    
    int  getLevel() const;
    void setLevel(int value);
    void levelUp();
    
    std::string getUserName() const;
    void setUserName(std::string name);
    
    std::string getEmail() const;
    void setEmail(std::string email);
    
    std::string getFBUserEmail() const;
    void setFBUserEmail(std::string email);
    
    std::string getUserLogin() const;
    void setUserLogin(std::string login);
    
    bool  isFacebookPageLiked() const;
    void  setFacebookPageLiked();
    
    bool isUserGivenRating() const;
    void setUserGivenRating();
    
    unsigned int getExperience() const;
    unsigned int getExperienceForCoins(int coinsSpent);
    void setExperience(unsigned int value);
    void  addExperience(int experience);
    float getLevelProgress() const;
    bool isExperienceEnoughForLevelUp(int additionalExperience);
    
    unsigned int grabSlotsJackpot();
    unsigned int getSlotsJackpot() const;
    void addSlotsJackpot(unsigned int value);
    
    void incrementWonCounter();
    
    long getLeaderboardScore() const;
    void setLeaderboardScore(long value);
    void addLeaderboardScore(long delta);
    
    long getOpenCountApp() const;
    void setOpenCountApp(long value);
    void addOpenCountApp(long delta);

    long getShowRateWhenSpinning(int game) const;
    void setShowRateWhenSpinning(int game, long value);
    
    void addPiggyBankCoins(unsigned int value);
    unsigned int breakPiggyBank();
    
    void activateVipSubscription(int days, bool superVIP);
    int  getVipSubscriptionDaysRemains(bool superVIP);
    int  getExperienceMultiplier();
    int  getDailyBonusMultiplier();
    int  getLevelUpBonus();
    bool isAnySubscriptionActive();
    
    std::string getFBUserID();
    std::string getFBUserName();
    
    void setFBUserID(std::string value);
    void setFBUserName(std::string value);
    
    unsigned int getPlayerID() const;
    void setPlayerID(unsigned int playerID);
    
    void setFBUserPictureURL(std::string path);
    std::string getFBUserPictureURL() const;
    
    void setTimestamp(unsigned int timestamp);
    unsigned int getTimestamp();
    
    //stats
    void statsAddBetAmount(std::string gameID, unsigned int value);
    void statsAddWinAmount(std::string gameID, unsigned int value);
    
    unsigned long long statsGetBetAmount(std::string gameID);
    unsigned long long statsGetWinAmount(std::string gameID);
    
    //get
    std::string getStringValue(std::string key, std::string defValue = "")  const;
    unsigned int getUIntValue(std::string key, unsigned int defValue = 0)  const;
    long getLongValue(std::string key, long defValue = 0)  const;
    unsigned long long getULongLong(std::string key, unsigned long long defValue = 0) const;
    
    //set
    void setStringValue(std::string key, std::string value);
    void setUIntValue(std::string key, unsigned int value);
    void setLongValue(std::string key, long value);
    void setULongLongValue(std::string key, unsigned long long value);
};
