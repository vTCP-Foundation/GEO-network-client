#ifndef VTCPD_OBSERVINGPARTICIPANTSVOTESAPPENDRESPONSEMESSAGE_H
#define VTCPD_OBSERVINGPARTICIPANTSVOTESAPPENDRESPONSEMESSAGE_H

#include "base/ObservingResponseMessage.h"

class ObservingParticipantsVotesAppendResponseMessage : public ObservingResponseMessage
{

public:
    typedef shared_ptr<ObservingParticipantsVotesAppendResponseMessage> Shared;

public:
    ObservingParticipantsVotesAppendResponseMessage(
        BytesShared buffer);

    ObservingTransaction::ObservingResponseType observingResponse() const;
};


#endif //VTCPD_OBSERVINGPARTICIPANTSVOTESAPPENDRESPONSEMESSAGE_H
