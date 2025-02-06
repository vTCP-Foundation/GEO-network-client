﻿#ifndef VTCPD_MESSAGE_H
#define VTCPD_MESSAGE_H

#include "../../common/memory/MemoryUtils.h"
#include "../../contractors/Contractor.h"
#include "../communicator/internal/common/Packet.hpp"

#include <limits>


using namespace std;


class Message
{
public:
    typedef shared_ptr<Message> Shared;
    typedef uint16_t SerializedType;

public:
    static const size_t UnencryptedHeaderSize =
        sizeof(SerializedProtocolVersion) +
        sizeof(ContractorID);

    enum ProtocolVersion
    {
        Latest = 0,
    };

    enum MessageType
    {
        /*
         * System messages types
         */
        System_Confirmation = 0,

        /*
         * Trust lines
         */
        TrustLines_Initial = 101,
        TrustLines_Confirmation = 102,
        TrustLines_PublicKeysSharingInit = 103,
        TrustLines_PublicKey = 104,
        TrustLines_HashConfirmation = 105,
        TrustLines_Audit = 106,
        TrustLines_AuditConfirmation = 107,
        TrustLines_ConflictResolver = 108,
        TrustLines_ConflictResolverConfirmation = 109,
        TrustLines_Reset = 110,

        /*
         * Payments messages
         */
        Payments_ReceiverInitPaymentRequest = 201,
        Payments_ReceiverInitPaymentResponse = 202,
        Payments_CoordinatorReservationRequest = 203,
        Payments_CoordinatorReservationResponse = 204,
        Payments_IntermediateNodeReservationRequest = 205,
        Payments_IntermediateNodeReservationResponse = 206,

        Payments_CoordinatorCycleReservationRequest = 207,
        Payments_CoordinatorCycleReservationResponse = 208,
        Payments_IntermediateNodeCycleReservationRequest = 209,
        Payments_IntermediateNodeCycleReservationResponse = 210,

        Payments_FinalAmountsConfiguration = 211,
        Payments_FinalAmountsConfigurationResponse = 212,

        Payments_FinalPathConfiguration = 213,
        Payments_FinalPathCycleConfiguration = 214,

        Payments_TTLProlongationRequest = 215,
        Payments_TTLProlongationResponse = 216,

        Payments_TransactionPublicKeyHash = 217,
        Payments_ParticipantVote = 218,
        Payments_ParticipantsPublicKeys = 219,
        Payments_ParticipantsVotes = 220,

        Payments_VotesStatusRequest = 221,

        /*
         * Cycles
         */
        Cycles_ThreeNodesBalancesRequest = 300,
        Cycles_ThreeNodesBalancesResponse = 301,
        Cycles_FourNodesNegativeBalanceRequest = 302,
        Cycles_FourNodesPositiveBalanceRequest = 303,
        Cycles_FourNodesBalancesResponse = 304,
        Cycles_FiveNodesBoundary = 305,
        Cycles_FiveNodesMiddleware = 306,
        Cycles_SixNodesBoundary = 307,
        Cycles_SixNodesMiddleware = 308,

        /*
         * Max flow
         */
        MaxFlow_InitiateCalculation = 400,
        MaxFlow_CalculationSourceFirstLevel = 401,
        MaxFlow_CalculationTargetFirstLevel = 402,
        MaxFlow_CalculationSourceSecondLevel = 403,
        MaxFlow_CalculationTargetSecondLevel = 404,
        MaxFlow_ResultMaxFlowCalculation = 405,
        MaxFlow_ResultMaxFlowCalculationFromGateway = 406,
        MaxFlow_Confirmation = 407,

        /*
         * General
         */
        General_Ping = 500,
        General_Pong = 501,
        General_NoEquivalent = 502,

        /*
         * Channel
         */
        Channel_Init = 600,
        Channel_Confirm = 601,
        Channel_UpdateAddresses = 602,

        /*
         * Gateway notification & Routing tables
         */
        GatewayNotification = 700,
        RoutingTableResponse = 701,

        /*
         * Providing
         */
        ProvidingAddressResponse = 800,

        /*
         * DEBUG
         */
        // Obvious, that we have to set this code
        Debug = 6666,
    };

public:
    virtual ~Message() = default;

    /*
     * Returns max allowed size of the message in bytes.
     */
    static size_t maxSize()
    {
        return
            numeric_limits<PacketHeader::PacketIndex>::max() * Packet::kMaxSize -
            (numeric_limits<PacketHeader::PacketIndex>::max() * PacketHeader::kSize);
    }

    /*
     * Base "Message" is abstract.
     * Some of it's derived classes are used for various transactions responses.
     *
     * Transactions scheduler requires mechanism to know
     * which response to attach to which transaction.
     * The simplest way to do this - to attach response to the transaction by it's UUID
     * (scheduler checks if transactionUUID of the response is uqual to the transaction).
     *
     * But transactionUUID may be redundant is ome cases
     * (for example, in routing table responses,
     * max flow calculation responses, and several other)
     *
     * This methods set makes it possible for the transactions scheduler to know,
     * how to decide which response should be attached to which transaction,
     * and implement custom attach logic for each one response type.
     *
     *
     * Derived classes of specific responses must override one of this methods.
     */
    virtual const bool isTransactionMessage() const
    {
        return false;
    }

    virtual const bool isAddToConfirmationRequiredMessagesHandler() const
    {
        return false;
    }

    virtual const bool isAddToConfirmationNotStronglyRequiredMessagesHandler() const
    {
        return false;
    }

    virtual const bool isCheckCachedResponse() const
    {
        return false;
    }

    virtual const SerializedEquivalent equivalent() const
    {
        return 0;
    }

    virtual const MessageType typeID() const = 0;

    virtual pair<BytesShared, size_t> serializeToBytes() const
    {
        SerializedProtocolVersion kProtocolVersion = ProtocolVersion::Latest;
        const SerializedType kMessageType = typeID();
        bool isMessageEncrypted = isEncrypted();
        size_t dataBytesOffset = 0;
        const auto kBufferSize = sizeof(SerializedProtocolVersion) + sizeof(ContractorID) + sizeof(kMessageType);
        auto buffer = tryMalloc(
                          kBufferSize);

        memcpy(
            buffer.get(),
            &kProtocolVersion,
            sizeof(SerializedProtocolVersion));
        dataBytesOffset += sizeof(SerializedProtocolVersion);

        ContractorID ownIdOnContractorSide = this->ownIdOnContractorSide();
        memcpy(
            buffer.get() + dataBytesOffset,
            &ownIdOnContractorSide,
            sizeof(ContractorID));
        dataBytesOffset += sizeof(ContractorID);

        memcpy(
            buffer.get() + dataBytesOffset,
            &kMessageType,
            sizeof(kMessageType));

        return make_pair(
                   buffer,
                   kBufferSize);
    }

    void setSenderIncomingIP(
        const string &senderIncomingIP)
    {
        mSenderIncomingIP = senderIncomingIP;
    }

    string senderIncomingIP()
    {
        return mSenderIncomingIP;
    }

    bool isEncrypted() const
    {
        return mContractorId != std::numeric_limits<ContractorID>::max();
    }

    ContractorID contractorId() const
    {
        return mContractorId;
    }
    ContractorID ownIdOnContractorSide() const
    {
        return isEncrypted() ?
               mContractor->ownIdOnContractorSide() :
               std::numeric_limits<ContractorID>::max();
    }

    void encrypt(Contractor::Shared contractor)
    {
        mContractor = contractor;
        mContractorId = contractor->getID();
    }

protected:
    virtual const size_t kOffsetToInheritedBytes() const
    {
        return sizeof(SerializedProtocolVersion) + sizeof(ContractorID) + sizeof(SerializedType);
    }

private:
    string mSenderIncomingIP;
    ContractorID mContractorId = std::numeric_limits<ContractorID>::max();
    Contractor::Shared mContractor;
};

#endif //VTCPD_MESSAGE_H
