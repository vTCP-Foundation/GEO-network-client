#ifndef GEO_NETWORK_CLIENT_HISTORYWITHCONTRACTORTRANSACTION_H
#define GEO_NETWORK_CLIENT_HISTORYWITHCONTRACTORTRANSACTION_H

#include "../base/BaseTransaction.h"
#include "../../../interface/commands_interface/commands/history/HistoryWithContractorCommand.h"
#include "../../../io/storage/StorageHandler.h"
#include "../../../io/storage/record/payment/PaymentRecord.h"
#include "../../../io/storage/record/trust_line/TrustLineRecord.h"

#include <vector>

class HistoryWithContractorTransaction : public BaseTransaction
{

public:
    typedef shared_ptr<HistoryWithContractorTransaction> Shared;

public:
    HistoryWithContractorTransaction(
        HistoryWithContractorCommand::Shared command,
        StorageHandler *storageHandler,
        Logger &logger);

    TransactionResult::SharedConst run() override;

protected:
    const string logHeader() const override;

private:
    TransactionResult::SharedConst resultOk(
        const vector<Record::Shared> &records);

private:
    HistoryWithContractorCommand::Shared mCommand;
    StorageHandler *mStorageHandler;
};


#endif //GEO_NETWORK_CLIENT_HISTORYWITHCONTRACTORTRANSACTION_H
