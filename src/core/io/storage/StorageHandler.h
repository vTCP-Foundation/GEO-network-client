#ifndef GEO_NETWORK_CLIENT_STORAGEHANDLER_H
#define GEO_NETWORK_CLIENT_STORAGEHANDLER_H

#include "../../logger/Logger.h"
#include "../../common/exceptions/IOError.h"
#include "../../../libs/sqlite3/sqlite3.h"
#include "IOTransaction.h"

#include <boost/filesystem.hpp>
#include <vector>

namespace fs = boost::filesystem;

class StorageHandler {

public:
    StorageHandler(
        const string &directory,
        const string &dataBaseName,
        Logger &logger);

    ~StorageHandler();

    IOTransaction::Shared beginTransaction();

    void migrateHistory();

private:
    static void checkDirectory(
        const string &directory);

    static sqlite3* connection(
        const string &dataBaseName,
        const string &directory);

    LoggerStream info() const;

    LoggerStream warning() const;

    LoggerStream error() const;

    const string logHeader() const;

private:
    const string kTrustLineTableName = "trust_lines";
    const string kTransactionTableName = "transactions";
    const string kHistoryMainTableName = "history";
    const string kHistoryAdditionalTableName = "history_additional";

    const string kOwnKeysTableName = "own_keys";
    const string kContractorKeysTableName = "contractor_keys";
    const string kOutgoingReceiptTableName = "outgoing_receipt";
    const string kIncomingReceiptTableName = "incoming_receipt";
    const string kAuditTableName = "audit";
    const string kPaymentKeysTableName = "payment_keys";
    const string kPaymentParticipantsVotesTableName = "payment_participants_votes";
    const string kPaymentTransactionsTableName = "payment_transactions";

    const string kContractorsTableName = "contractors";
    const string kContractorAddressesTableName = "contractors_addresses";

    const string kFeaturesTableName = "features";

private:
    static sqlite3 *mDBConnection;

private:
    Logger &mLog;
    TrustLineHandler mTrustLineHandler;
    TransactionsHandler mTransactionHandler;
    HistoryStorage mHistoryStorage;
    OwnKeysHandler mOwnKeysHandler;
    ContractorKeysHandler mContractorKeysHandler;
    AuditHandler mAuditHandler;
    IncomingPaymentReceiptHandler mIncomingPaymentReceiptHandler;
    OutgoingPaymentReceiptHandler mOutgoingPaymentReceiptHandler;
    PaymentKeysHandler mPaymentKeysHandler;
    PaymentParticipantsVotesHandler mPaymentParticipantsVotesHandler;
    PaymentTransactionsHandler mPaymentTransactionsHandler;
    ContractorsHandler mContractorsHandler;
    AddressHandler mAddressHandler;
    FeaturesHandler mFeaturesHandler;
    string mDirectory;
    string mDataBaseName;
};


#endif //GEO_NETWORK_CLIENT_STORAGEHANDLER_H
