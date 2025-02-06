#ifndef VTCPD_CONFIRMCHANNELTRANSACTION_H
#define VTCPD_CONFIRMCHANNELTRANSACTION_H

#include "../base/BaseTransaction.h"
#include "../../../network/messages/trust_line_channels/InitChannelMessage.h"
#include "../../../network/messages/trust_line_channels/ConfirmChannelMessage.h"

#include "../../../contractors/ContractorsManager.h"

class ConfirmChannelTransaction : public BaseTransaction
{

public:
    typedef shared_ptr<ConfirmChannelTransaction> Shared;

public:
    ConfirmChannelTransaction(
        InitChannelMessage::Shared message,
        ContractorsManager *contractorsManager,
        StorageHandler *storageHandler,
        Logger &logger);

    TransactionResult::SharedConst run() override;

protected:
    const string logHeader() const override;

private:
    InitChannelMessage::Shared mMessage;
    ContractorsManager *mContractorsManager;
    StorageHandler *mStorageHandler;
};


#endif //VTCPD_CONFIRMCHANNELTRANSACTION_H
