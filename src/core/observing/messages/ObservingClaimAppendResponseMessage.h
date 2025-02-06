#ifndef VTCPD_OBSERVINGCLAIMAPPENDRESPONSEMESSAGE_H
#define VTCPD_OBSERVINGCLAIMAPPENDRESPONSEMESSAGE_H

#include "base/ObservingResponseMessage.h"

class ObservingClaimAppendResponseMessage : public ObservingResponseMessage
{

public:
    typedef shared_ptr<ObservingClaimAppendResponseMessage> Shared;

public:
    ObservingClaimAppendResponseMessage(
        BytesShared buffer);

    ObservingTransaction::ObservingResponseType observingResponse() const;
};


#endif //VTCPD_OBSERVINGCLAIMAPPENDRESPONSEMESSAGE_H
