#ifndef VTCPD_OBSERVINGRESPONSEMESSAGE_H
#define VTCPD_OBSERVINGRESPONSEMESSAGE_H

#include "../../ObservingTransaction.h"

class ObservingResponseMessage
{

public:
    ObservingResponseMessage(
        BytesShared buffer);

    const size_t kOffsetToInheritedBytes() const;

private:
    const ObservingTransaction::SerializedObservingResponseType kObserverCommunicatorMinimalError = 200;

protected:
    ObservingTransaction::SerializedObservingResponseType mObservingResponse;
};


#endif //VTCPD_OBSERVINGRESPONSEMESSAGE_H
