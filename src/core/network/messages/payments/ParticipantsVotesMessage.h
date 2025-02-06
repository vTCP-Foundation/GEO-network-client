﻿#ifndef GEO_NETWORK_CLIENT_PARTICIPANTSAPPROVINGMESSAGE_H
#define GEO_NETWORK_CLIENT_PARTICIPANTSAPPROVINGMESSAGE_H


#include "../base/transaction/TransactionMessage.h"
#include "../../../crypto/lamportscheme.h"

#include <map>

using namespace crypto;

/**
 * This message is used to achieve consensus between transaction participants.
 * It contains UUIDs of all nodes that are involved into the operation and their votes.
 *
 * TODO: [mvp+] add participants signing
 */
class ParticipantsVotesMessage:
    public TransactionMessage
{

public:
    typedef shared_ptr<ParticipantsVotesMessage> Shared;

public:
    ParticipantsVotesMessage(
        const SerializedEquivalent equivalent,
        // todo : use &
        vector<BaseAddress::Shared> senderAddresses,
        const TransactionUUID &transactionUUID,
        map<PaymentNodeID, lamport::Signature::Shared> &participantsSignatures);

    ParticipantsVotesMessage(
        BytesShared buffer);

    const MessageType typeID() const override;

    const map<PaymentNodeID, lamport::Signature::Shared>& participantsSignatures() const;

    pair<BytesShared, size_t> serializeToBytes() const override;

private:
    map<PaymentNodeID, lamport::Signature::Shared> mParticipantsSignatures;
};
#endif //GEO_NETWORK_CLIENT_PARTICIPANTSAPPROVINGMESSAGE_H
