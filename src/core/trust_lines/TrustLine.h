﻿#ifndef VTCPD_TRUSTLINE_H
#define VTCPD_TRUSTLINE_H

#include "../common/Types.h"
#include "../common/memory/MemoryUtils.h"
#include "../common/multiprecision/MultiprecisionUtils.h"

#include "../common/exceptions/RuntimeError.h"
#include "../common/exceptions/MemoryError.h"
#include "../common/exceptions/ValueError.h"
#include "../common/exceptions/OverflowError.h"

#include <vector>


using namespace std;

class TrustLine
{
public:
    typedef shared_ptr<TrustLine> Shared;
    typedef shared_ptr<const TrustLine> ConstShared;
    typedef uint16_t SerializedTrustLineState;

public:
    enum TrustLineState
    {
        Init = 1,
        Active = 2,
        AuditPending = 3,
        Archived = 4,
        Conflict = 5,
        ConflictResolving = 6,
        ResetPending = 7,
        Reset = 8,
    };

public:
    TrustLine(
        const TrustLineID trustLineID,
        const ContractorID contractorID,
        bool isContractorGateway,
        TrustLineState state);

    TrustLine(
        const ContractorID contractorID,
        const TrustLineID trustLineID);

    void setIncomingTrustAmount(
        const TrustLineAmount &amount);

    void setOutgoingTrustAmount(
        const TrustLineAmount &amount);

    void setBalance(
        const TrustLineBalance &balance);

    void pay(
        const TrustLineAmount &amount);

    void acceptPayment(
        const TrustLineAmount &amount);

    const ContractorID contractorID() const;

    const TrustLineAmount& incomingTrustAmount() const;

    const TrustLineAmount& outgoingTrustAmount() const;

    const TrustLineBalance& balance() const;

    const TrustLineID trustLineID() const;

    const TrustLineState state() const;

    const AuditNumber currentAuditNumber() const;

    void setState(
        TrustLineState newState);

    void setAuditNumber(
        AuditNumber newAuditNumber);

    ConstSharedTrustLineAmount availableOutgoingAmount() const;

    ConstSharedTrustLineAmount availableIncomingAmount() const;

    ConstSharedTrustLineAmount usedAmountByContractor() const;

    ConstSharedTrustLineAmount usedAmountBySelf() const;

    void setTotalOutgoingReceiptsAmount(
        const TrustLineAmount &amount);

    void setTotalIncomingReceiptsAmount(
        const TrustLineAmount &amount);

    bool isTrustLineOverflowed() const;

    void resetTotalReceiptsAmounts();

    bool isContractorGateway() const;

    void setContractorAsGateway(
        bool contractorAsGateway);

    void setIsOwnKeysPresent(
        bool isOwnKeysPresent);

    bool isOwnKeysPresent() const;

    bool isContractorKeysPresent() const;

    void setIsContractorKeysPresent(
        bool isContractorKeysPresent);

    static const TrustLineBalance& kZeroBalance();

    static const TrustLineAmount& kZeroAmount();

    friend bool operator== (
        const TrustLine::Shared contractor1,
        const TrustLine::Shared contractor2);

    friend bool operator== (
        const TrustLine &contractor1,
        const TrustLine &contractor2);

public:
    static const AuditNumber kInitialAuditNumber = 0;

private:
    ContractorID mContractorID;
    TrustLineID mID;
    TrustLineAmount mIncomingTrustAmount;
    TrustLineAmount mOutgoingTrustAmount;
    TrustLineBalance mBalance;
    TrustLineAmount mTotalOutgoingReceiptsAmount;
    TrustLineAmount mTotalIncomingReceiptsAmount;
    bool mIsContractorGateway;
    TrustLineState mState;
    AuditNumber mCurrentAudit;
    bool mIsOwnKeysPresent;
    bool mIsContractorKeysPresent;
};

#endif //VTCPD_TRUSTLINE_H
