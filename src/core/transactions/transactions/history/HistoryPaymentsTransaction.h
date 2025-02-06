#ifndef GEO_NETWORK_CLIENT_HISTORYPAYMENTSTRANSACTION_H
#define GEO_NETWORK_CLIENT_HISTORYPAYMENTSTRANSACTION_H

#include "../base/BaseTransaction.h"
#include "../../../interface/commands_interface/commands/history/HistoryPaymentsCommand.h"
#include "../../../io/storage/StorageHandler.h"
#include "../../../io/storage/record/payment/PaymentRecord.h"

#include <vector>

class HistoryPaymentsTransaction : public BaseTransaction
{

public:
    typedef shared_ptr<HistoryPaymentsTransaction> Shared;

public:
    HistoryPaymentsTransaction(
        HistoryPaymentsCommand::Shared command,
        StorageHandler *storageHandler,
        Logger &logger);

    TransactionResult::SharedConst run() override;

protected:
    const string logHeader() const override;

private:
    TransactionResult::SharedConst resultOk(
        const vector<PaymentRecord::Shared> &records);

private:
    HistoryPaymentsCommand::Shared mCommand;
    StorageHandler *mStorageHandler;
};


#endif //GEO_NETWORK_CLIENT_HISTORYPAYMENTSTRANSACTION_H
