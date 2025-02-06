#ifndef VTCPD_OBSERVINGBLOCKNUMBERRESPONSE_H
#define VTCPD_OBSERVINGBLOCKNUMBERRESPONSE_H

#include "base/ObservingResponseMessage.h"

class ObservingBlockNumberResponse : public ObservingResponseMessage
{

public:
    typedef shared_ptr<ObservingBlockNumberResponse> Shared;

public:
    ObservingBlockNumberResponse(
        BytesShared buffer);

    BlockNumber actualBlockNumber() const;

private:
    ObservingTransaction::SerializedObservingResponseType mObservingResponse;
    BlockNumber mActualBlockNumber;
};


#endif //VTCPD_OBSERVINGBLOCKNUMBERRESPONSE_H
