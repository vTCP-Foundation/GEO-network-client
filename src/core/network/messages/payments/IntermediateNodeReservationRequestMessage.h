#ifndef VTCPD_INTERMEDIATENODERESERVATIONREQUESTMESSAGE_H
#define VTCPD_INTERMEDIATENODERESERVATIONREQUESTMESSAGE_H

#include "base/RequestMessageWithReservations.h"


class IntermediateNodeReservationRequestMessage:
    public RequestMessageWithReservations
{

public:
    typedef shared_ptr<IntermediateNodeReservationRequestMessage> Shared;
    typedef shared_ptr<const IntermediateNodeReservationRequestMessage> ConstShared;

public:
    using RequestMessageWithReservations::RequestMessageWithReservations;

protected:
    const MessageType typeID() const override;
};

#endif // VTCPD_INTERMEDIATENODERESERVATIONREQUESTMESSAGE_H
