#ifndef VTCPD_TOPOLOGYCACHEMANAGER_H
#define VTCPD_TOPOLOGYCACHEMANAGER_H

#include "TopologyCache.h"
#include "../../contractors/addresses/BaseAddress.h"
#include "../../common/time/TimeUtils.h"
#include "../../logger/Logger.h"

#include <map>
#include <unordered_map>

class TopologyCacheManager
{

public:
    TopologyCacheManager(
        const SerializedEquivalent equivalent,
        Logger &logger);

    void addCache(
        BaseAddress::Shared keyAddress,
        TopologyCache::Shared cache);

    TopologyCache::Shared cacheByAddress(
        BaseAddress::Shared nodeAddress) const;

    bool addIntoFirstLevelCache(
        ContractorID contractorID);

    bool isInFirstLevelCache(
        ContractorID contractorID) const;

    void updateCaches();

    void setInitiatorCache();

    bool isInitiatorCached();

    DateTime closestTimeEvent() const;

    void resetInitiatorCache();

    void removeCache(
        BaseAddress::Shared nodeAddress);

private:
    static const byte_t kResetSenderCacheHours = 0;
    static const byte_t kResetSenderCacheMinutes = 10;
    static const byte_t kResetSenderCacheSeconds = 0;

    static Duration &kResetSenderCacheDuration()
    {
        static auto duration = Duration(
                                   kResetSenderCacheHours,
                                   kResetSenderCacheMinutes,
                                   kResetSenderCacheSeconds);
        return duration;
    }

    static const byte_t kResetInitiatorCacheHours = 0;
    static const byte_t kResetInitiatorCacheMinutes = 0;
    static const byte_t kResetInitiatorCacheSeconds = 30;

    static Duration &kResetInitiatorCacheDuration()
    {
        static auto duration = Duration(
                                   kResetInitiatorCacheHours,
                                   kResetInitiatorCacheMinutes,
                                   kResetInitiatorCacheSeconds);
        return duration;
    }

private:
    LoggerStream info() const;

    LoggerStream debug() const;

    LoggerStream warning() const;

    const string logHeader() const;

private:
    unordered_map<string, TopologyCache::Shared> mCaches;
    map<DateTime, BaseAddress::Shared> msCache;

    typedef shared_ptr<pair<ContractorID, DateTime>> FirstLvShared;
    map<ContractorID, list<FirstLvShared>::iterator> mFirstLvCache;
    list<FirstLvShared> mFirstLvCacheList;

    pair<bool, DateTime> mInitiatorCache;
    SerializedEquivalent mEquivalent;
    Logger &mLog;
};

#endif // VTCPD_TOPOLOGYCACHEMANAGER_H
