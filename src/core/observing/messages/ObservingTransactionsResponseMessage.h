#ifndef VTCPD_OBSERVINGTRANSACTIONSRESPONSEMESSAGE_H
#define VTCPD_OBSERVINGTRANSACTIONSRESPONSEMESSAGE_H

#include "base/ObservingResponseMessage.h"
#include <vector>

class ObservingTransactionsResponseMessage : public ObservingResponseMessage
{

public:
    typedef shared_ptr<ObservingTransactionsResponseMessage> Shared;

public:
    ObservingTransactionsResponseMessage(
        BytesShared buffer);

    BlockNumber actualBlockNumber() const;

    vector<ObservingTransaction::ObservingResponseType> transactionsResponses() const;

private:
    ObservingTransaction::SerializedObservingResponseType mObservingResponse;
    BlockNumber mActualBlockNumber;
    vector<ObservingTransaction::ObservingResponseType> mTransactionsAndResponses;
};


#endif //VTCPD_OBSERVINGTRANSACTIONSRESPONSEMESSAGE_H
