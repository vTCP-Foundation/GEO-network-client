#ifndef VTCPD_MAXFLOWCACHE_H
#define VTCPD_MAXFLOWCACHE_H

#include "../../common/Types.h"
#include "../../common/time/TimeUtils.h"

class MaxFlowCache
{
public:
    typedef shared_ptr<MaxFlowCache> Shared;

public:
    MaxFlowCache(
        const TrustLineAmount &amount,
        bool isFinal = false);

    TrustLineAmount& currentFlow();

    bool isFlowFinal();

    DateTime lastModified();

    void updateCurrentFlow(
        const TrustLineAmount &amount,
        bool isFinal = false);

private:
    TrustLineAmount mCurrentFlow;
    bool mFinalFlow;
    DateTime mTimeLastModified;
};


#endif //VTCPD_MAXFLOWCACHE_H
