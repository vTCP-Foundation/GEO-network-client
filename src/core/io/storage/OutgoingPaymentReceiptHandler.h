#ifndef GEO_NETWORK_CLIENT_OUTGOINGPAYMENTRECEIPTHANDLER_H
#define GEO_NETWORK_CLIENT_OUTGOINGPAYMENTRECEIPTHANDLER_H

#include "../../logger/Logger.h"
#include "../../common/exceptions/IOError.h"
#include "../../transactions/transactions/base/TransactionUUID.h"
#include "../../common/multiprecision/MultiprecisionUtils.h"
#include "../../crypto/lamportscheme.h"
#include "record/audit/ReceiptRecord.h"

#include "../../../libs/sqlite3/sqlite3.h"

using namespace crypto::lamport;

class OutgoingPaymentReceiptHandler
{
public:
    OutgoingPaymentReceiptHandler(
        sqlite3 *dbConnection,
        const string &tableName,
        Logger &logger);

    void saveRecord(
        const TrustLineID trustLineID,
        const AuditNumber auditNumber,
        const TransactionUUID &transactionUUID,
        const KeyHash::Shared ownPublicKeyHash,
        const TrustLineAmount &amount);

    vector<pair<TransactionUUID, TrustLineAmount>> auditAmounts(
        const TrustLineID trustLineID,
        const AuditNumber auditNumber);

    vector<ReceiptRecord::Shared> receiptsByAuditNumber(
        const TrustLineID trustLineID,
        const AuditNumber auditNumber);

    vector<ReceiptRecord::Shared> receiptsLessEqualThanAuditNumber(
        const TrustLineID trustLineID,
        const AuditNumber auditNumber);

    uint32_t countReceiptsByNumber(
        const TrustLineID trustLineID,
        const AuditNumber auditNumber);

    void deleteRecords(
        const TransactionUUID &transactionUUID);

    void deleteRecords(
        const TrustLineID trustLineID);

    bool isContainsKeyHash(
        KeyHash::Shared keyHash) const;

    bool isContainsTransaction(
        const TransactionUUID &transactionUUID) const;

private:
    LoggerStream info() const;

    LoggerStream warning() const;

    const string logHeader() const;

private:
    sqlite3 *mDataBase = nullptr;
    string mTableName;
    Logger &mLog;
};


#endif //GEO_NETWORK_CLIENT_OUTGOINGPAYMENTRECEIPTHANDLER_H
