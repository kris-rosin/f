//
//  PlayerProfile.cpp
//  DovemobiCasino
//
//  Created by Stanislav Zheronkin on 31.10.16.
//
//

#include "PlayerProfile.h"
#include "Base64_RC4.h"
#include "Features/mConstants.h"
#include "DataBuffer.h"
#include "Platform.h"
#include "AndroidCompatibility.h"
#include "Features/AppManager.h"
#include "Features/mConstants.h"

#include "cocos2d.h"

PlayerProfile::PlayerProfile()
{
    
}

PlayerProfile & PlayerProfile::getInstance()
{
    static PlayerProfile _instance;
    
    return _instance;
}

void PlayerProfile::reinitWithData(DataBuffer * buffer)
{
    cocos2d::Data data;
    data.copy(buffer->getData(), buffer->getSize());
    cocos2d::FileUtils::getInstance()->writeDataToFile(data, loadedFilePath);
    
    auto pos = loadedFilePath.find_last_of('/');
    auto filename = loadedFilePath.substr(pos + 1);
    
    Load(filename);
}

DataBuffer * PlayerProfile::getDataFile()
{
    if (cocos2d::FileUtils::getInstance()->isFileExist(loadedFilePath))
    {
        auto data = cocos2d::FileUtils::getInstance()->getDataFromFile(loadedFilePath);
        
        DataBuffer * buffer = new DataBuffer(data.getBytes(), data.getSize());
        
        return buffer;
    }
    
    return nullptr;
}

void PlayerProfile::setDefaults()
{
    setUIntValue("UserLevel", INITIAL_LEVEL);
    setLongValue("UserCoins", INITIAL_COINS);
    setUIntValue("UserExperience", 0);
    setUIntValue("TargetExperience", experinceForLevel(2));
    setLongValue("MaxWin", 0);
    setUIntValue("WonCounter", 0);
    setUIntValue("PreviousExperience", 0);
    
    generateRandomSlotsJackpot();
}

void PlayerProfile::Load(std::string filename)
{
    std::string path = cocos2d::FileUtils::getInstance()->getWritablePath().append(filename);
    
    loadedFilePath = path;
    
    if (cocos2d::FileUtils::getInstance()->isFileExist(path))
    {
        auto data = cocos2d::FileUtils::getInstance()->getDataFromFile(path);
        
        DataBuffer * buffer = new DataBuffer(data.getBytes(), data.getSize());
        
        RC4 encoder(ENCODE_KEY);
        
        encoder.Decode( (char*)buffer->getData() + 1, buffer->getSize() - 1); // exclude first byte from decryption process
        
        unsigned char version = buffer->readByte();
        
        if (version > 1)
        {
            //values map
            unsigned int counter = buffer->readUnsignedInt();
            valuesMap.clear();
            
            for (unsigned int i = 0; i < counter; i++)
            {
                std::string key = buffer->readString();
                valuesMap[key]  = buffer->readString();
            }
        }
        else
        {
            setDefaults();
        }
        
        delete buffer;
        buffer = nullptr;
    }
    else
    {
        setDefaults();
    }
}

void PlayerProfile::Save()
{
    if (loadedFilePath.empty())
        return;
    
    DataBuffer * buffer = new DataBuffer();
    buffer->writeByte(binaryVersion);
    
    //timestamp
    setTimestamp(Platform::unixTime());
    
    //values map
    buffer->writeInt((unsigned int)valuesMap.size());
    
    for (auto &itr: valuesMap)
    {
        buffer->writeString(itr.first);
        buffer->writeString(itr.second);
    }
    
    RC4 encoder(ENCODE_KEY);
    encoder.Encode((char*)buffer->getData() + 1, buffer->getSize() - 1); //exclude first byte from encryption
    
    cocos2d::Data data;
    data.copy(buffer->getData(), buffer->getSize());
    delete buffer;
    buffer = nullptr;
    
    cocos2d::FileUtils::getInstance()->writeDataToFile(data, loadedFilePath);
}

void PlayerProfile::incrementWonCounter()
{
    setUIntValue("WonCounter", getUIntValue("WonCounter", 0) + 1);
}

void PlayerProfile::setTimestamp(unsigned int timestamp)
{
    setUIntValue("LASTUPDATE_TIMESTAMP", timestamp);
}

unsigned int PlayerProfile::getTimestamp()
{
    return getUIntValue("LASTUPDATE_TIMESTAMP", 0);
}

//******************************************************************************
// PlayerProfile --> Coins
//******************************************************************************
unsigned long long PlayerProfile::getCoins() const
{
    return getULongLong("UserCoins", 0);
}

void PlayerProfile::setCoins(unsigned long long value)
{
    setULongLongValue("UserCoins", value);
}

void PlayerProfile::addCoins(long delta)
{
    auto coins = getULongLong("UserCoins", 0);
    
    if (delta > 0 || (delta < 0 && abs(delta) < coins))
    {
        setULongLongValue("UserCoins", coins + delta);
        addLeaderboardScore(delta);
    }
}

//******************************************************************************
// PlayerProfile --> Leaderboard
//******************************************************************************
long PlayerProfile::getLeaderboardScore() const
{
    return getLongValue("LeaderboardScore", 0);
}

void PlayerProfile::setLeaderboardScore(long value)
{
    setLongValue("LeaderboardScore", value);
}

void PlayerProfile::addLeaderboardScore(long delta)
{
    if (delta < 0) {
        return;
    }
    
    setLongValue("LeaderboardScore", getLongValue("LeaderboardScore", 0) + delta/10);
    int leaderboardScore = (int) getLeaderboardScore();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    AppManager::shared()->sendScore(leaderboardScore, IOS_LEADERBOARD_ID);
    
    if (leaderboardScore >= ACHIEVEMENT_TROPHY_100000_VALUE)
    {
        AppManager::shared()->submitAchievement(IOS_ACHIEVEMENT_TROPHY_100000);
    }
    else if (leaderboardScore >= ACHIEVEMENT_TROPHY_50000_VALUE)
    {
        AppManager::shared()->submitAchievement(IOS_ACHIEVEMENT_TROPHY_50000);
    }
    else if (leaderboardScore >= ACHIEVEMENT_TROPHY_10000_VALUE)
    {
        AppManager::shared()->submitAchievement(IOS_ACHIEVEMENT_TROPHY_10000);
    }
    else if (leaderboardScore >= ACHIEVEMENT_TROPHY_5000_VALUE)
    {
        AppManager::shared()->submitAchievement(IOS_ACHIEVEMENT_TROPHY_5000);
    }
    else if (leaderboardScore >= ACHIEVEMENT_TROPHY_1000_VALUE)
    {
        AppManager::shared()->submitAchievement(IOS_ACHIEVEMENT_TROPHY_1000);
    }
    else if (leaderboardScore >= ACHIEVEMENT_TROPHY_500_VALUE)
    {
        AppManager::shared()->submitAchievement(IOS_ACHIEVEMENT_TROPHY_500);
    }
    else if (leaderboardScore >= ACHIEVEMENT_TROPHY_100_VALUE)
    {
        AppManager::shared()->submitAchievement(IOS_ACHIEVEMENT_TROPHY_100);
    }
    
#else
    
    AppManager::shared()->sendScore(leaderboardScore, AND_LEADERBOARD_ID);
    
    if (leaderboardScore >= ACHIEVEMENT_TROPHY_100000_VALUE)
    {
        AppManager::shared()->submitAchievement(AND_ACHIEVEMENT_TROPHY_100000);
    }
    else if (leaderboardScore >= ACHIEVEMENT_TROPHY_50000_VALUE)
    {
        AppManager::shared()->submitAchievement(AND_ACHIEVEMENT_TROPHY_50000);
    }
    else if (leaderboardScore >= ACHIEVEMENT_TROPHY_10000_VALUE)
    {
        AppManager::shared()->submitAchievement(AND_ACHIEVEMENT_TROPHY_10000);
    }
    else if (leaderboardScore >= ACHIEVEMENT_TROPHY_5000_VALUE)
    {
        AppManager::shared()->submitAchievement(AND_ACHIEVEMENT_TROPHY_5000);
    }
    else if (leaderboardScore >= ACHIEVEMENT_TROPHY_1000_VALUE)
    {
        AppManager::shared()->submitAchievement(AND_ACHIEVEMENT_TROPHY_1000);
    }
    else if (leaderboardScore >= ACHIEVEMENT_TROPHY_500_VALUE)
    {
        AppManager::shared()->submitAchievement(AND_ACHIEVEMENT_TROPHY_500);
    }
    else if (leaderboardScore >= ACHIEVEMENT_TROPHY_100_VALUE)
    {
        AppManager::shared()->submitAchievement(AND_ACHIEVEMENT_TROPHY_100);
    }
    
#endif
    
}

//******************************************************************************
// PlayerProfile --> Count App
//******************************************************************************
long PlayerProfile::getOpenCountApp() const
{
    return getLongValue("OpenCountApp", 0);
}

void PlayerProfile::setOpenCountApp(long value)
{
    setLongValue("OpenCountApp", value);
}

void PlayerProfile::addOpenCountApp(long delta)
{
    setLongValue("OpenCountApp", getLongValue("OpenCountApp", 0) + delta);
}

//******************************************************************************
// PlayerProfile --> ShowRateWhenSpinning
//******************************************************************************
long PlayerProfile::getShowRateWhenSpinning(int game) const
{
    if (game == 0) {
        return getLongValue("SpinSlots", 0);
    } else if (game == 1) {
        return getLongValue("SpinRoulette", 0);
    } else if (game == 2) {
        return getLongValue("SpinBlackjack", 0);
    } else if (game == 3) {
        return getLongValue("SpinBaccarat", 0);
    } else if (game == 4) {
        return getLongValue("SpinCasinoWar", 0);
    }
    return 0;
}

void PlayerProfile::setShowRateWhenSpinning(int game, long value)
{
    if (game == 0) {
        setLongValue("SpinSlots", value);
    } else if (game == 1) {
        setLongValue("SpinRoulette", value);
    } else if (game == 2) {
        setLongValue("SpinBlackjack", value);
    } else if (game == 3) {
        setLongValue("SpinBaccarat", value);
    } else if (game == 4) {
        setLongValue("SpinCasinoWar", value);
    }
}


//******************************************************************************
// PlayerProfile --> Level
//******************************************************************************

int PlayerProfile::getLevel() const
{
    return getUIntValue("UserLevel", 1);
}

void PlayerProfile::setLevel(int value)
{
    setUIntValue("UserLevel", value);
}

void PlayerProfile::levelUp()
{
    setLevel(getLevel() + 1);
}

//******************************************************************************
// PlayerProfile --> Like Facebook Page
//******************************************************************************

bool PlayerProfile::isFacebookPageLiked() const
{
    int fbPageLiked = getUIntValue("FBPageLiked", 0);
    if (fbPageLiked == 0) {
        return false;
    } else {
        return true;
    }
    return false;
}

void PlayerProfile::setFacebookPageLiked()
{
    setUIntValue("FBPageLiked", 1);
}


//******************************************************************************
// PlayerProfile --> Set User Given Rating
//******************************************************************************

bool PlayerProfile::isUserGivenRating() const
{
    int userGivenRating = getUIntValue("userGivenRating", 0);
    if (userGivenRating == 0) {
        return false;
    } else {
        return true;
    }
    return false;
}

void PlayerProfile::setUserGivenRating()
{
    setUIntValue("userGivenRating", 1);
}


//******************************************************************************
// PlayerProfile --> Add Slots Jackpot
//******************************************************************************

void PlayerProfile::addSlotsJackpot(unsigned int value)
{
    unsigned int newvalue = getUIntValue("SlotsJackpot", 0) + value;
    
    if (newvalue > 10000)
    {
        generateRandomSlotsJackpot();
        return;
    }
    
    setUIntValue("SlotsJackpot", newvalue);
}

unsigned int PlayerProfile::getSlotsJackpot() const
{
    return getUIntValue("SlotsJackpot", 0);
}

unsigned int PlayerProfile::grabSlotsJackpot()
{
    unsigned int value = getUIntValue("SlotsJackpot", 0);
    
    generateRandomSlotsJackpot();
    
    return value;
}

void PlayerProfile::generateRandomSlotsJackpot()
{
    //hackyhack_publish - arc4random_uniform
//    unsigned int num = 800 + arc4random_uniform(500);
    unsigned int num = 800 + (rand() % 500);
    
    setUIntValue("SlotsJackpot", num);
}

unsigned int PlayerProfile::getExperienceForCoins(int coinsSpent)
{
    return 3 * (int)sqrt(1 + abs(coinsSpent) / 20) + 3;
}

bool PlayerProfile::isExperienceEnoughForLevelUp(int additionalExperience)
{
    unsigned int new_exp = getExperience() + getExperienceMultiplier() * additionalExperience;
    unsigned int target  = experinceForLevel(getLevel() + 1);
    
    if (new_exp >= target)
    {
        return true;
    }
    
    return false;
}

void PlayerProfile::addExperience(int experience)
{
    unsigned int new_exp = getExperience() + getExperienceMultiplier() * experience;
    unsigned int target  = getUIntValue("TargetExperience", 0);
    
    if (new_exp >= experinceForLevel(getLevel() + 1))
    {
        levelUp();
        
        setUIntValue("TargetExperience", experinceForLevel(getLevel() + 1));
        setUIntValue("PreviousExperience", target);
    }
    
    setExperience(new_exp);
}

unsigned int PlayerProfile::getExperience() const
{
    return getUIntValue("UserExperience", 0);
}

void PlayerProfile::setExperience(unsigned int value)
{
    setUIntValue("UserExperience", value);
}

unsigned int PlayerProfile::experinceForLevel(unsigned int level)
{
    if (level == 0) level = 1;
    
    unsigned int result = ( (level * 50 - 25) * (level * 50 - 25) - 625) / 40;
    return result;
}

float PlayerProfile::getLevelProgress() const
{
    unsigned int target  = getUIntValue("TargetExperience", 0);
    unsigned int exp     = getUIntValue("UserExperience", 0);
    unsigned int prevExp = getUIntValue("PreviousExperience", 0);
    
    if (exp >= target)
        return 1.0f;
    
    int delta = target - exp;
    int maxProgress = target - prevExp;
    
    return 1.0f - std::min(1.0f, (float)delta / maxProgress);
}

std::string PlayerProfile::getStringValue(std::string key, std::string defValue) const
{
    auto itr = valuesMap.find(key);
    
    if (itr != valuesMap.end())
    {
        return itr->second;
    }
    
    return defValue;
}

unsigned int PlayerProfile::getUIntValue(std::string key, unsigned int defValue) const
{
    auto value = getStringValue(key);
    
    if (value.empty())
    {
        return  defValue;
    }
    
    return atoi(value.c_str());
}

long PlayerProfile::getLongValue(std::string key, long defValue) const
{
    auto value = getStringValue(key);
    
    if (value.empty())
    {
        return  defValue;
    }
    
    return atol(value.c_str());
}

void PlayerProfile::setStringValue(std::string key, std::string value)
{
    valuesMap[key] = value;
}

void PlayerProfile::setUIntValue(std::string key, unsigned int value)
{
    char buff[32];
    
    snprintf(buff, 32, "%d", value);
    
    valuesMap[key] = buff;
}

void PlayerProfile::setLongValue(std::string key, long value)
{
    char buff[64];
    
    snprintf(buff, 64, "%ld", value);
    
    valuesMap[key] = buff;
}

unsigned long long PlayerProfile::getULongLong(std::string key, unsigned long long defValue) const
{
    auto value = getStringValue(key);
    
    if (value.empty())
    {
        return  defValue;
    }
    
    return std::strtoull(value.c_str(), nullptr, 10);
}

void PlayerProfile::setULongLongValue(std::string key, unsigned long long value)
{
    char buff[22];
    
    snprintf(buff, 22, "%llu", value);
    
    valuesMap[key] = buff;
}

void PlayerProfile::addPiggyBankCoins(unsigned int value)
{
    setUIntValue("PIGGY_BANK_COINS", getUIntValue("PIGGY_BANK_COINS", 0) + value);
}

unsigned int PlayerProfile::breakPiggyBank()
{
    auto coins = getUIntValue("PIGGY_BANK_COINS", 0);
    setUIntValue("PIGGY_BANK_COINS", 0);
    
    return coins;
}

// SUBSCRIPTIONS

void PlayerProfile::activateVipSubscription(int days, bool superVIP)
{
    unsigned int timestamp = 0;
    
    if (superVIP)
    {
        timestamp = getUIntValue("VVIP_SUBSCRIPTION_EXP_TIMESTAMP", 0);
    }
    else
    {
        timestamp = getUIntValue("VIP_SUBSCRIPTION_EXP_TIMESTAMP", 0);
    }
    
    // first activation, or previous subscription expired
    if (timestamp == 0 || timestamp < Platform::unixTime())
    {
        timestamp = Platform::unixTime();
    }
    
    //append days
    timestamp += days * 24 * 3600;
    
    //save
    setUIntValue(superVIP ? "VVIP_SUBSCRIPTION_EXP_TIMESTAMP" : "VIP_SUBSCRIPTION_EXP_TIMESTAMP", timestamp);
    Save();
}

int  PlayerProfile::getVipSubscriptionDaysRemains(bool superVIP)
{
    unsigned int timestamp = 0;
    
    if (superVIP)
    {
        timestamp = getUIntValue("VVIP_SUBSCRIPTION_EXP_TIMESTAMP", 0);
    }
    else
    {
        timestamp = getUIntValue("VIP_SUBSCRIPTION_EXP_TIMESTAMP", 0);
    }
    
    if (timestamp < Platform::unixTime())
    {
        return 0;
    }
    
    return 1 + (timestamp - Platform::unixTime()) / 3600 / 24;
}

bool PlayerProfile::isAnySubscriptionActive()
{
    return getVipSubscriptionDaysRemains(false) > 0 || getVipSubscriptionDaysRemains(true) > 0;
}

int  PlayerProfile::getExperienceMultiplier()
{
    if (getVipSubscriptionDaysRemains(true) > 0)
    {
        return std::get<0>(vvipBonuses);
    }
    
    if (getVipSubscriptionDaysRemains(false) > 0)
    {
        return std::get<0>(vipBonuses);
    }
    
    return 1;
}

int  PlayerProfile::getDailyBonusMultiplier()
{
    if (getVipSubscriptionDaysRemains(true) > 0)
    {
        return std::get<2>(vvipBonuses);
    }
    
    if (getVipSubscriptionDaysRemains(false) > 0)
    {
        return std::get<2>(vipBonuses);
    }
    
    return 1;
}

int  PlayerProfile::getLevelUpBonus()
{
    if (getVipSubscriptionDaysRemains(true) > 0)
    {
        return std::get<1>(vvipBonuses);
    }
    
    if (getVipSubscriptionDaysRemains(false) > 0)
    {
        return std::get<1>(vipBonuses);
    }
    
    return BONUS_LEVEL_UP;
}

std::string PlayerProfile::getFBUserID()
{
    return getStringValue("FBUserID");
}

std::string PlayerProfile::getFBUserName()
{
    return getStringValue("FBUserName");
}

void PlayerProfile::setFBUserID(std::string value)
{
    setStringValue("FBUserID", value);
}

void PlayerProfile::setFBUserName(std::string value)
{
    setStringValue("FBUserName", value);
}

unsigned int PlayerProfile::getPlayerID() const
{
    return getUIntValue("PlayerID", 0);
}

void PlayerProfile::setPlayerID(unsigned int playerID)
{
    setUIntValue("PlayerID", playerID);
}

void PlayerProfile::statsAddBetAmount(std::string gameID, unsigned int value)
{
    std::string key = "STATS_TOTAL_BET_";
    key.append(gameID);
    
    auto old_value = getULongLong(key);
    
    old_value += value;
    
    setULongLongValue(key, old_value);
}

void PlayerProfile::statsAddWinAmount(std::string gameID, unsigned int value)
{
    std::string key = "STATS_TOTAL_WIN_";
    key.append(gameID);
    
    auto old_value = getULongLong(key);
    
    old_value += value;
    
    setULongLongValue(key, old_value);
}

unsigned long long PlayerProfile::statsGetBetAmount(std::string gameID)
{
    std::string key = "STATS_TOTAL_BET_";
    key.append(gameID);
    
    auto value = getULongLong(key);
    
    return value;
}

unsigned long long PlayerProfile::statsGetWinAmount(std::string gameID)
{
    std::string key = "STATS_TOTAL_WIN_";
    key.append(gameID);
    
    auto value = getULongLong(key);
    
    return value;
}


void PlayerProfile::setFBUserPictureURL(std::string path)
{
    setStringValue("FBUserPictureURL", path);
}

std::string PlayerProfile::getFBUserPictureURL() const
{
    return getStringValue("FBUserPictureURL");
}

std::string PlayerProfile::getUserName() const
{
    return getStringValue("UserName");
}

void PlayerProfile::setUserName(std::string name)
{
    setStringValue("UserName", name);
}

std::string PlayerProfile::getUserLogin() const
{
    return getStringValue("UserLogin");
}

void PlayerProfile::setUserLogin(std::string login)
{
    setStringValue("UserLogin", login);
}

std::string PlayerProfile::getEmail() const
{
    return getStringValue("UserEmail");;
}

void PlayerProfile::setEmail(std::string email)
{
    setStringValue("UserEmail", email);
}

std::string PlayerProfile::getFBUserEmail() const
{
    return getStringValue("FBUserEmail");;
}

void PlayerProfile::setFBUserEmail(std::string email)
{
    setStringValue("FBUserEmail", email);
}
