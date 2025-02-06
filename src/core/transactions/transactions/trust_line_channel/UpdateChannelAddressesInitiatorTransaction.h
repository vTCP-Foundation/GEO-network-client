#ifndef VTCPD_UPDATECHANNELADDRESSESINITIATORTRANSACTION_H
#define VTCPD_UPDATECHANNELADDRESSESINITIATORTRANSACTION_H

#include "../base/BaseTransaction.h"
#include "../../../contractors/ContractorsManager.h"
#include "../../../network/messages/trust_line_channels/UpdateChannelAddressesMessage.h"

class UpdateChannelAddressesInitiatorTransaction : public BaseTransaction
{

public:
    typedef shared_ptr<UpdateChannelAddressesInitiatorTransaction> Shared;

public:
    UpdateChannelAddressesInitiatorTransaction(
        ContractorsManager *contractorsManager,
        Logger &logger);

    TransactionResult::SharedConst run() override;

protected:
    const string logHeader() const override;

private:
    ContractorsManager *mContractorsManager;
};


#endif //VTCPD_UPDATECHANNELADDRESSESINITIATORTRANSACTION_H
