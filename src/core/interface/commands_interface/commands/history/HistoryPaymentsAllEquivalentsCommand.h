#ifndef VTCPD_HISTORYPAYMENTSALLEQUIVALENTSCOMMAND_H
#define VTCPD_HISTORYPAYMENTSALLEQUIVALENTSCOMMAND_H

#include "../BaseUserCommand.h"
#include "../../../../common/multiprecision/MultiprecisionUtils.h"

class HistoryPaymentsAllEquivalentsCommand : public BaseUserCommand
{

public:
    typedef shared_ptr<HistoryPaymentsAllEquivalentsCommand> Shared;

public:
    HistoryPaymentsAllEquivalentsCommand(
        const CommandUUID &uuid,
        const string &commandBuffer);

    static const string &identifier();

    CommandResult::SharedConst resultOk(
        string &historyPaymentsStr) const;

    const size_t historyFrom() const;

    const size_t historyCount() const;

    const DateTime timeFrom() const;

    const DateTime timeTo() const;

    const bool isTimeFromPresent() const;

    const bool isTimeToPresent() const;

    const TrustLineAmount& lowBoundaryAmount() const;

    const TrustLineAmount& highBoundaryAmount() const;

    const bool isLowBoundaryAmountPresent() const;

    const bool isHighBoundaryAmountPresent() const;

    const CommandUUID &paymentRecordCommandUUID() const;

    const bool isPaymentRecordCommandUUIDPresent() const;

private:
    string kNullParameter = "null";

private:
    size_t mHistoryFrom;
    size_t mHistoryCount;
    DateTime mTimeFrom;
    DateTime mTimeTo;
    bool mIsTimeFromPresent;
    bool mIsTimeToPresent;
    TrustLineAmount mLowBoundaryAmount;
    TrustLineAmount mHighBoundaryAmount;
    bool mIsLowBoundaryAmountPresent;
    bool mIsHighBoundaryAmountPresent;
    CommandUUID mPaymentRecordCommandUUID;
    bool mIsPaymentRecordCommandUUIDPresent;
};


#endif //VTCPD_HISTORYPAYMENTSALLEQUIVALENTSCOMMAND_H
