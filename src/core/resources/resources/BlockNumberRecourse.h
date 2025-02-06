#ifndef VTCPD_BLOCKNUMBERRECOURSE_H
#define VTCPD_BLOCKNUMBERRECOURSE_H

#include "BaseResource.h"

class BlockNumberRecourse : public BaseResource
{

public:
    typedef shared_ptr<BlockNumberRecourse> Shared;

public:
    BlockNumberRecourse(
        const TransactionUUID& transactionUUID,
        BlockNumber actualObservingBlockNumber);

    BlockNumber actualObservingBlockNumber() const;

private:
    BlockNumber mActualObservingBlockNumber;
};


#endif //VTCPD_BLOCKNUMBERRECOURSE_H
