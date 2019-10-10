#include "HistoryStorage.h"

HistoryStorage::HistoryStorage(
    sqlite3 *dbConnection,
    const string &mainTableName,
    const string &additionalTableName,
    Logger &logger) :

    mDataBase(dbConnection),
    mMainTableName(mainTableName),
    mAdditionalTableName(additionalTableName),
    mLog(logger)
{
    sqlite3_stmt *stmt;
    // creating main table
    string query = "CREATE TABLE IF NOT EXISTS " + mMainTableName +
                   "(operation_uuid BLOB NOT NULL, "
                       "operation_timestamp INTEGER NOT NULL, "
                       "record_type INTEGER NOT NULL, "
                       "record_body BLOB NOT NULL, "
                       "record_body_bytes_count INT NOT NULL, "
                       "equivalent INTEGER NOT NULL, "
                       "command_uuid BLOB);";
    int rc = sqlite3_prepare_v2( mDataBase, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::creating main table: "
                          "Bad query; sqlite error: " + to_string(rc));
    }
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
    } else {
        throw IOError("HistoryStorage::creating main table: "
                          "Run query; sqlite error: " + to_string(rc));
    }
    query = "CREATE INDEX IF NOT EXISTS " + mMainTableName
            + "_operation_uuid_idx on " + mMainTableName + "(operation_uuid);";
    rc = sqlite3_prepare_v2(mDataBase, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::main creating index for operation_uuid: "
                          "Bad query; sqlite error: " + to_string(rc));
    }
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
    } else {
        throw IOError("HistoryStorage::main creating index for operation_uuid: "
                          "Run query; sqlite error: " + to_string(rc));
    }
    query = "CREATE INDEX IF NOT EXISTS " + mMainTableName
            + "_operation_timestamp_idx on " + mMainTableName + "(operation_timestamp);";
    rc = sqlite3_prepare_v2(mDataBase, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::main creating index for operation_timestamp: "
                          "Bad query; sqlite error: " + to_string(rc));
    }
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
    } else {
        throw IOError("HistoryStorage::main creating index for operation_timestamp: "
                          "Run query; sqlite error: " + to_string(rc));
    }
    query = "CREATE INDEX IF NOT EXISTS " + mMainTableName
            + "_record_type_idx on " + mMainTableName + "(record_type);";
    rc = sqlite3_prepare_v2(mDataBase, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::main creating index for record_type: "
                          "Bad query; sqlite error: " + to_string(rc));
    }
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
    } else {
        throw IOError("HistoryStorage::main creating index for record_type: "
                          "Run query; sqlite error: " + to_string(rc));
    }

    query = "CREATE INDEX IF NOT EXISTS " + mMainTableName
            + "_command_uuid_idx on " + mMainTableName + "(command_uuid);";
    rc = sqlite3_prepare_v2(mDataBase, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::main creating index for command_uuid: "
                          "Bad query; sqlite error: " + to_string(rc));
    }
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
    } else {
        throw IOError("HistoryStorage::main creating index for command_uuid: "
                          "Run query; sqlite error: " + to_string(rc));
    }

    query = "CREATE INDEX IF NOT EXISTS " + mMainTableName
            + "_equivalent_idx on " + mMainTableName + "(equivalent);";
    rc = sqlite3_prepare_v2(mDataBase, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage:: main creating index for Equivalent: "
                          "Bad query; sqlite error: " + to_string(rc));
    }
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
    } else {
        throw IOError("HistoryStorage::main creating index for Equivalent: "
                          "Run query; sqlite error: " + to_string(rc));
    }

    // creating payments additional table
    query = "CREATE TABLE IF NOT EXISTS " + mAdditionalTableName +
                   "(operation_uuid BLOB NOT NULL, "
                       "operation_timestamp INTEGER NOT NULL, "
                       "record_type INTEGER NOT NULL, "
                       "record_body BLOB NOT NULL, "
                       "record_body_bytes_count INT NOT NULL, "
                       "equivalent INTEGER NOT NULL);";
    rc = sqlite3_prepare_v2( mDataBase, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::creating additional table: "
                          "Bad query; sqlite error: " + to_string(rc));
    }
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
    } else {
        throw IOError("HistoryStorage::creating additional table: "
                          "Run query; sqlite error: " + to_string(rc));
    }
    query = "CREATE INDEX IF NOT EXISTS " + mAdditionalTableName
            + "_operation_uuid_idx on " + mAdditionalTableName + "(operation_uuid);";
    rc = sqlite3_prepare_v2(mDataBase, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::additional creating index for operation_uuid: "
                          "Bad query; sqlite error: " + to_string(rc));
    }
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
    } else {
        throw IOError("HistoryStorage::additional creating index for operation_uuid: "
                          "Run query; sqlite error: " + to_string(rc));
    }
    query = "CREATE INDEX IF NOT EXISTS " + mAdditionalTableName
            + "_operation_timestamp_idx on " + mAdditionalTableName + "(operation_timestamp);";
    rc = sqlite3_prepare_v2(mDataBase, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::additional creating index for operation_timestamp: "
                          "Bad query; sqlite error: " + to_string(rc));
    }
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
    } else {
        throw IOError("HistoryStorage::additional creating index for operation_timestamp: "
                          "Run query; sqlite error: " + to_string(rc));
    }
    query = "CREATE INDEX IF NOT EXISTS " + mAdditionalTableName
            + "_record_type_idx on " + mAdditionalTableName + "(record_type);";
    rc = sqlite3_prepare_v2(mDataBase, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::additional creating index for record_type: "
                          "Bad query; sqlite error: " + to_string(rc));
    }
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
    } else {
        throw IOError("HistoryStorage::additional creating index for record_type: "
                          "Run query; sqlite error: " + to_string(rc));
    }
    query = "CREATE INDEX IF NOT EXISTS " + mAdditionalTableName
            + "_equivalent_idx on " + mAdditionalTableName + "(equivalent);";
    rc = sqlite3_prepare_v2(mDataBase, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::additional creating index for Equivalent: "
                          "Bad query; sqlite error: " + to_string(rc));
    }
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
    } else {
        throw IOError("HistoryStorage::additional creating index for Equivalent: "
                          "Run query; sqlite error: " + to_string(rc));
    }
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
}

void HistoryStorage::saveTrustLineRecord(
    TrustLineRecord::Shared record,
    const SerializedEquivalent equivalent)
{
    string query = "INSERT INTO " + mMainTableName
                   + "(operation_uuid, operation_timestamp, equivalent, "
                           "record_type, record_body, record_body_bytes_count) "
                           "VALUES(?, ?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(mDataBase, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::insert trustline: "
                          "Bad query; sqlite error: " + to_string(rc));
    }
    rc = sqlite3_bind_blob(stmt, 1, record->operationUUID().data, TransactionUUID::kBytesSize, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::insert trustline: "
                          "Bad binding of OperationUUID; sqlite error: " + to_string(rc));
    }
    GEOEpochTimestamp timestamp = microsecondsSinceGEOEpoch(record->timestamp());
    rc = sqlite3_bind_int64(stmt, 2, timestamp);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::insert trustline: "
                          "Bad binding of Timestamp; sqlite error: " + to_string(rc));
    }
    rc = sqlite3_bind_int(stmt, 3, equivalent);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::insert trustline: "
                          "Bad binding of Equivalent; sqlite error: " + to_string(rc));
    }
    rc = sqlite3_bind_int(stmt, 4, record->recordType());
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::insert trustline: "
                          "Bad binding of RecordType; sqlite error: " + to_string(rc));
    }
    auto serializedTrustLineRecordAndSize = record->serializedHistoryRecordBody();
    rc = sqlite3_bind_blob(stmt, 5, serializedTrustLineRecordAndSize.first.get(),
                           (int) serializedTrustLineRecordAndSize.second,
                           SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::insert trustline: "
                          "Bad binding of RecordBody; sqlite error: " + to_string(rc));
    }
    rc = sqlite3_bind_int(stmt, 6, (int) serializedTrustLineRecordAndSize.second);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::insert trustline: "
                          "Bad binding of RecordBody bytes count; sqlite error: " + to_string(rc));
    }

    rc = sqlite3_step(stmt);
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    if (rc == SQLITE_DONE) {
#ifdef STORAGE_HANDLER_DEBUG_LOG
        info() << "prepare inserting of trustline is completed successfully";
#endif
    } else {
        throw IOError("HistoryStorage::insert trustline: "
                          "Run query; sqlite error: " + to_string(rc));
    }
}

void HistoryStorage::savePaymentRecord(
    PaymentRecord::Shared record,
    const SerializedEquivalent equivalent)
{
    switch (record->paymentOperationType()) {
        case PaymentRecord::OutgoingPaymentType:
            savePaymentMainOutgoingRecord(
                record,
                equivalent);
            break;
        case PaymentRecord::IncomingPaymentType:
            savePaymentMainIncomingRecord(
                record,
                equivalent);
            break;
        default:
            throw ValueError("HistoryStorage::savePaymentRecord: "
                                 "invalid payment operation type");
    }
}

void HistoryStorage::savePaymentMainOutgoingRecord(
    PaymentRecord::Shared record,
    const SerializedEquivalent equivalent)
{
    string query = "INSERT INTO " + mMainTableName
                   + "(operation_uuid, operation_timestamp, equivalent, record_type, "
                     "record_body, record_body_bytes_count, command_uuid) "
                     "VALUES(?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(mDataBase, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::insert main payment: "
                          "Bad query; sqlite error: " + to_string(rc));
    }

    rc = sqlite3_bind_blob(stmt, 1, record->operationUUID().data, TransactionUUID::kBytesSize, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::insert main payment: "
                          "Bad binding of OperationUUID; sqlite error: " + to_string(rc));
    }

    GEOEpochTimestamp timestamp = microsecondsSinceGEOEpoch(record->timestamp());
    rc = sqlite3_bind_int64(stmt, 2, timestamp);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::insert main payment: "
                          "Bad binding of Timestamp; sqlite error: " + to_string(rc));
    }

    rc = sqlite3_bind_int(stmt, 3, equivalent);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::insert main payment: "
                          "Bad binding of Equivalent; sqlite error: " + to_string(rc));
    }

    rc = sqlite3_bind_int(stmt, 4, record->recordType());
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::insert main payment: "
                          "Bad binding of RecordType; sqlite error: " + to_string(rc));
    }

    auto serializedPaymentRecordAndSize = record->serializedHistoryRecordBody();
    rc = sqlite3_bind_blob(stmt, 5, serializedPaymentRecordAndSize.first.get(),
                           (int) serializedPaymentRecordAndSize.second, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::insert main payment: "
                          "Bad binding of RecordBody; sqlite error: " + to_string(rc));
    }

    rc = sqlite3_bind_int(stmt, 6, (int) serializedPaymentRecordAndSize.second);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::insert main payment: "
                          "Bad binding of RecordBody bytes count; sqlite error: " + to_string(rc));
    }

    rc = sqlite3_bind_blob(stmt, 7, record->commandUUID().data, TransactionUUID::kBytesSize, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::insert main payment: "
                          "Bad binding of commandUUID; sqlite error: " + to_string(rc));
    }

    rc = sqlite3_step(stmt);
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    if (rc == SQLITE_DONE) {
#ifdef STORAGE_HANDLER_DEBUG_LOG
        info() << "prepare inserting of outgoing main payment is completed successfully";
#endif
    } else {
        throw IOError("HistoryStorage::insert main payment: "
                          "Run query; sqlite error: " + to_string(rc));
    }
}

void HistoryStorage::savePaymentMainIncomingRecord(
    PaymentRecord::Shared record,
    const SerializedEquivalent equivalent)
{
    string query = "INSERT INTO " + mMainTableName
                   + "(operation_uuid, operation_timestamp, equivalent, "
                     "record_type, record_body, record_body_bytes_count) "
                       "VALUES(?, ?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(mDataBase, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::insert main payment: "
                          "Bad query; sqlite error: " + to_string(rc));
    }

    rc = sqlite3_bind_blob(stmt, 1, record->operationUUID().data, TransactionUUID::kBytesSize, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::insert main payment: "
                          "Bad binding of OperationUUID; sqlite error: " + to_string(rc));
    }

    GEOEpochTimestamp timestamp = microsecondsSinceGEOEpoch(record->timestamp());
    rc = sqlite3_bind_int64(stmt, 2, timestamp);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::insert main payment: "
                          "Bad binding of Timestamp; sqlite error: " + to_string(rc));
    }

    rc = sqlite3_bind_int(stmt, 3, equivalent);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::insert main payment: "
                          "Bad binding of Equivalent; sqlite error: " + to_string(rc));
    }

    rc = sqlite3_bind_int(stmt, 4, record->recordType());
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::insert main payment: "
                          "Bad binding of RecordType; sqlite error: " + to_string(rc));
    }

    auto serializedPaymentRecordAndSize = record->serializedHistoryRecordBody();
    rc = sqlite3_bind_blob(stmt, 5, serializedPaymentRecordAndSize.first.get(),
                           (int) serializedPaymentRecordAndSize.second, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::insert main payment: "
                          "Bad binding of RecordBody; sqlite error: " + to_string(rc));
    }

    rc = sqlite3_bind_int(stmt, 6, (int) serializedPaymentRecordAndSize.second);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::insert main payment: "
                          "Bad binding of RecordBody bytes count; sqlite error: " + to_string(rc));
    }

    rc = sqlite3_step(stmt);
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    if (rc == SQLITE_DONE) {
#ifdef STORAGE_HANDLER_DEBUG_LOG
        info() << "prepare inserting of incoming main payment is completed successfully";
#endif
    } else {
        throw IOError("HistoryStorage::insert main payment: "
                          "Run query; sqlite error: " + to_string(rc));
    }
}

void HistoryStorage::savePaymentAdditionalRecord(
    PaymentAdditionalRecord::Shared record,
    const SerializedEquivalent equivalent)
{
    string query = "INSERT INTO " + mAdditionalTableName
                   + "(operation_uuid, operation_timestamp, equivalent, record_type, record_body, record_body_bytes_count) "
                           "VALUES(?, ?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(mDataBase, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::insert additional payment: "
                          "Bad query; sqlite error: " + to_string(rc));
    }
    rc = sqlite3_bind_blob(stmt, 1, record->operationUUID().data, TransactionUUID::kBytesSize, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::insert additional payment: "
                          "Bad binding of OperationUUID; sqlite error: " + to_string(rc));
    }
    GEOEpochTimestamp timestamp = microsecondsSinceGEOEpoch(record->timestamp());
    rc = sqlite3_bind_int64(stmt, 2, timestamp);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::insert additional payment: "
                          "Bad binding of Timestamp; sqlite error: " + to_string(rc));
    }
    rc = sqlite3_bind_int(stmt, 3, equivalent);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::insert additional payment: "
                          "Bad binding of Equivalent; sqlite error: " + to_string(rc));
    }
    rc = sqlite3_bind_int(stmt, 4, record->recordType());
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::insert additional payment: "
                          "Bad binding of RecordType; sqlite error: " + to_string(rc));
    }
    auto serializedPaymentRecordAndSize = record->serializedHistoryRecordBody();
    rc = sqlite3_bind_blob(stmt, 5, serializedPaymentRecordAndSize.first.get(),
                           (int) serializedPaymentRecordAndSize.second, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::insert additional payment: "
                          "Bad binding of RecordBody; sqlite error: " + to_string(rc));
    }
    rc = sqlite3_bind_int(stmt, 6, (int) serializedPaymentRecordAndSize.second);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::insert additional payment: "
                          "Bad binding of RecordBody bytes count; sqlite error: " + to_string(rc));
    }
    rc = sqlite3_step(stmt);
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    if (rc == SQLITE_DONE) {
#ifdef STORAGE_HANDLER_DEBUG_LOG
        info() << "prepare inserting of additional payment is completed successfully";
#endif
    } else {
        throw IOError("HistoryStorage::insert additional payment: "
                          "Run query; sqlite error: " + to_string(rc));
    }
}

vector<TrustLineRecord::Shared> HistoryStorage::allTrustLineRecords(
    const SerializedEquivalent equivalent,
    size_t recordsCount,
    size_t fromRecord,
    DateTime timeFrom,
    bool isTimeFromPresent,
    DateTime timeTo,
    bool isTimeToPresent)
{
    vector<TrustLineRecord::Shared> result;
    string query = "SELECT operation_uuid, operation_timestamp, record_body, record_body_bytes_count FROM "
                   + mMainTableName + " WHERE equivalent = ? AND record_type = ? ";
    if (isTimeFromPresent) {
        query += " AND operation_timestamp >= ? ";
    }
    if (isTimeToPresent) {
        query += " AND operation_timestamp <= ? ";
    }
    query += " ORDER BY operation_timestamp DESC LIMIT ? OFFSET ?;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(mDataBase, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::allTrustLineRecords: "
                          "Bad query; sqlite error: " + to_string(rc));
    }
    int idxParam = 1;
    rc = sqlite3_bind_int(stmt, idxParam++, equivalent);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::allTrustLineRecords: "
                          "Bad binding of Equivalent; sqlite error: " + to_string(rc));
    }
    rc = sqlite3_bind_int(stmt, idxParam++, Record::TrustLineRecordType);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::allTrustLineRecords: "
                          "Bad binding of RecordType; sqlite error: " + to_string(rc));
    }
    if (isTimeFromPresent) {
        GEOEpochTimestamp timestamp = microsecondsSinceGEOEpoch(timeFrom);
        rc = sqlite3_bind_int64(stmt, idxParam++, timestamp);
        if (rc != SQLITE_OK) {
            throw IOError("HistoryStorage::allTrustLineRecords: "
                              "Bad binding of TimeFrom; sqlite error: " + to_string(rc));
        }
    }
    if (isTimeToPresent) {
        GEOEpochTimestamp timestamp = microsecondsSinceGEOEpoch(timeTo);
        rc = sqlite3_bind_int64(stmt, idxParam++, timestamp);
        if (rc != SQLITE_OK) {
            throw IOError("HistoryStorage::allTrustLineRecords: "
                              "Bad binding of TimeTo; sqlite error: " + to_string(rc));
        }
    }
    rc = sqlite3_bind_int(stmt, idxParam++, (int)recordsCount);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::allTrustLineRecords: "
                          "Bad binding of recordsCount; sqlite error: " + to_string(rc));
    }
    rc = sqlite3_bind_int(stmt, idxParam, (int)fromRecord);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::allTrustLineRecords: "
                          "Bad binding of fromRecord; sqlite error: " + to_string(rc));
    }

    while (sqlite3_step(stmt) == SQLITE_ROW ) {
        result.push_back(
            deserializeTrustLineRecord(
                stmt));
    }

    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    return result;
}

vector<PaymentRecord::Shared> HistoryStorage::allPaymentRecords(
    const SerializedEquivalent equivalent,
    size_t recordsCount,
    size_t fromRecord,
    DateTime timeFrom,
    bool isTimeFromPresent,
    DateTime timeTo,
    bool isTimeToPresent)
{
    vector<PaymentRecord::Shared> result;
    string query = "SELECT operation_uuid, operation_timestamp, record_body, record_body_bytes_count FROM "
                   + mMainTableName + " WHERE equivalent = ? AND record_type = ? ";
    if (isTimeFromPresent) {
        query += " AND operation_timestamp >= ? ";
    }
    if (isTimeToPresent) {
        query += " AND operation_timestamp <= ? ";
    }
    query += " ORDER BY operation_timestamp DESC LIMIT ? OFFSET ?;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(mDataBase, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::allPaymentRecords: "
                          "Bad query; sqlite error: " + to_string(rc));
    }
    int idxParam = 1;
    rc = sqlite3_bind_int(stmt, idxParam++, equivalent);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::allPaymentRecords: "
                          "Bad binding of Equivalent; sqlite error: " + to_string(rc));
    }
    rc = sqlite3_bind_int(stmt, idxParam++, Record::PaymentRecordType);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::allPaymentRecords: "
                          "Bad binding of RecordType; sqlite error: " + to_string(rc));
    }
    if (isTimeFromPresent) {
        GEOEpochTimestamp timestamp = microsecondsSinceGEOEpoch(timeFrom);
        rc = sqlite3_bind_int64(stmt, idxParam++, timestamp);
        if (rc != SQLITE_OK) {
            throw IOError("HistoryStorage::allPaymentRecords: "
                              "Bad binding of TimeFrom; sqlite error: " + to_string(rc));
        }
    }
    if (isTimeToPresent) {
        GEOEpochTimestamp timestamp = microsecondsSinceGEOEpoch(timeTo);
        rc = sqlite3_bind_int64(stmt, idxParam++, timestamp);
        if (rc != SQLITE_OK) {
            throw IOError("HistoryStorage::allPaymentRecords: "
                              "Bad binding of TimeTo; sqlite error: " + to_string(rc));
        }
    }
    rc = sqlite3_bind_int(stmt, idxParam++, (int)recordsCount);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::allPaymentRecords: "
                          "Bad binding of recordsCount; sqlite error: " + to_string(rc));
    }
    rc = sqlite3_bind_int(stmt, idxParam, (int)fromRecord);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::allPaymentRecords: "
                          "Bad binding of fromRecord; sqlite error: " + to_string(rc));
    }

    while (sqlite3_step(stmt) == SQLITE_ROW ) {
        result.push_back(
            deserializePaymentRecord(
                stmt));
    }

    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    return result;
}

size_t HistoryStorage::countRecordsByType(
    Record::RecordType recordType,
    const SerializedEquivalent equivalent)
{
    string query = "SELECT count(*) FROM "
                   + mMainTableName + " WHERE equivalent = ? AND record_type = ? ";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(mDataBase, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::countRecordsByType: "
                          "Bad query; sqlite error: " + to_string(rc));
    }
    rc = sqlite3_bind_int(stmt, 1, equivalent);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::countRecordsByType: "
                          "Bad binding of Equivalent; sqlite error: " + to_string(rc));
    }
    rc = sqlite3_bind_int(stmt, 2, recordType);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::countRecordsByType: "
                          "Bad binding of RecordType; sqlite error: " + to_string(rc));
    }
    sqlite3_step(stmt);
    auto result = (size_t)sqlite3_column_int(stmt, 0);
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    return result;
}

vector<PaymentRecord::Shared> HistoryStorage::allPaymentRecords(
    const SerializedEquivalent equivalent,
    size_t recordsCount,
    size_t fromRecord,
    DateTime timeFrom,
    bool isTimeFromPresent,
    DateTime timeTo,
    bool isTimeToPresent,
    const TrustLineAmount& lowBoundaryAmount,
    bool isLowBoundaryAmountPresent,
    const TrustLineAmount& highBoundaryAmount,
    bool isHighBoundaryAmountPresent)
{
    if (!isLowBoundaryAmountPresent && !isHighBoundaryAmountPresent) {
        return allPaymentRecords(
            equivalent,
            recordsCount,
            fromRecord,
            timeFrom,
            isTimeFromPresent,
            timeTo,
            isTimeToPresent);
    }
    vector<PaymentRecord::Shared> result;
    size_t paymentRecordsCount = countRecordsByType(
        Record::PaymentRecordType,
        equivalent);
    size_t currentOffset = 0;
    size_t countRecordsUnderConditions = 0;
    while (result.size() < recordsCount && currentOffset < paymentRecordsCount) {
        auto paymentRecords = allPaymentRecords(
            equivalent,
            kPortionRequestSize,
            currentOffset,
            timeFrom,
            isTimeFromPresent,
            timeTo,
            isTimeToPresent);
        for (auto &paymentRecord : paymentRecords) {
            bool recordUnderConditions = true;
            if (isLowBoundaryAmountPresent) {
                recordUnderConditions = recordUnderConditions &&
                    (paymentRecord->amount() >= lowBoundaryAmount);
            }
            if (isHighBoundaryAmountPresent) {
                recordUnderConditions = recordUnderConditions &&
                    (paymentRecord->amount() <= highBoundaryAmount);
            }
            if (recordUnderConditions) {
                countRecordsUnderConditions++;
                if (countRecordsUnderConditions > fromRecord) {
                    result.push_back(paymentRecord);
                }
            }
            if (result.size() >= recordsCount) {
                break;
            }
        }
        currentOffset += kPortionRequestSize;
    }
    return result;
}

vector<PaymentAdditionalRecord::Shared> HistoryStorage::allPaymentAdditionalRecords(
    const SerializedEquivalent equivalent,
    size_t recordsCount,
    size_t fromRecord,
    DateTime timeFrom,
    bool isTimeFromPresent,
    DateTime timeTo,
    bool isTimeToPresent,
    const TrustLineAmount& lowBoundaryAmount,
    bool isLowBoundaryAmountPresent,
    const TrustLineAmount& highBoundaryAmount,
    bool isHighBoundaryAmountPresent)
{
    if (!isLowBoundaryAmountPresent && !isHighBoundaryAmountPresent) {
        return allPaymentAdditionalRecords(
            equivalent,
            recordsCount,
            fromRecord,
            timeFrom,
            isTimeFromPresent,
            timeTo,
            isTimeToPresent);
    }
    vector<PaymentAdditionalRecord::Shared> result;
    size_t paymentRecordsCount = countRecordsByType(
        Record::PaymentAdditionalRecordType,
        equivalent);
    size_t currentOffset = 0;
    size_t countRecordsUnderConditions = 0;
    while (result.size() < recordsCount && currentOffset < paymentRecordsCount) {
        auto paymentAdditionalRecords = allPaymentAdditionalRecords(
            equivalent,
            kPortionRequestSize,
            currentOffset,
            timeFrom,
            isTimeFromPresent,
            timeTo,
            isTimeToPresent);
        for (auto &paymentAdditionalRecord : paymentAdditionalRecords) {
            bool recordUnderConditions = true;
            if (isLowBoundaryAmountPresent) {
                recordUnderConditions = recordUnderConditions &&
                        (paymentAdditionalRecord->amount() >= lowBoundaryAmount);
            }
            if (isHighBoundaryAmountPresent) {
                recordUnderConditions = recordUnderConditions &&
                        (paymentAdditionalRecord->amount() <= highBoundaryAmount);
            }
            if (recordUnderConditions) {
                countRecordsUnderConditions++;
                if (countRecordsUnderConditions > fromRecord) {
                    result.push_back(paymentAdditionalRecord);
                }
            }
            if (result.size() >= recordsCount) {
                break;
            }
        }
        currentOffset += kPortionRequestSize;
    }
    return result;
}

vector<PaymentAdditionalRecord::Shared> HistoryStorage::allPaymentAdditionalRecords(
    const SerializedEquivalent equivalent,
    size_t recordsCount,
    size_t fromRecord,
    DateTime timeFrom,
    bool isTimeFromPresent,
    DateTime timeTo,
    bool isTimeToPresent)
{
    vector<PaymentAdditionalRecord::Shared> result;
    string query = "SELECT operation_uuid, operation_timestamp, record_body, record_body_bytes_count FROM "
                   + mAdditionalTableName + " WHERE equivalent = ? AND record_type = ? ";
    if (isTimeFromPresent) {
        query += " AND operation_timestamp >= ? ";
    }
    if (isTimeToPresent) {
        query += " AND operation_timestamp <= ? ";
    }
    query += " ORDER BY operation_timestamp DESC LIMIT ? OFFSET ?;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(mDataBase, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::allAdditionalPaymentRecords: "
                          "Bad query; sqlite error: " + to_string(rc));
    }
    int idxParam = 1;
    rc = sqlite3_bind_int(stmt, idxParam++, equivalent);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::allAdditionalPaymentRecords: "
                          "Bad binding of Equivalent; sqlite error: " + to_string(rc));
    }
    rc = sqlite3_bind_int(stmt, idxParam++, Record::PaymentAdditionalRecordType);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::allAdditionalPaymentRecords: "
                          "Bad binding of RecordType; sqlite error: " + to_string(rc));
    }
    if (isTimeFromPresent) {
        GEOEpochTimestamp timestamp = microsecondsSinceGEOEpoch(timeFrom);
        rc = sqlite3_bind_int64(stmt, idxParam++, timestamp);
        if (rc != SQLITE_OK) {
            throw IOError("HistoryStorage::allAdditionalPaymentRecords: "
                              "Bad binding of TimeFrom; sqlite error: " + to_string(rc));
        }
    }
    if (isTimeToPresent) {
        GEOEpochTimestamp timestamp = microsecondsSinceGEOEpoch(timeTo);
        rc = sqlite3_bind_int64(stmt, idxParam++, timestamp);
        if (rc != SQLITE_OK) {
            throw IOError("HistoryStorage::allAdditionalPaymentRecords: "
                              "Bad binding of TimeTo; sqlite error: " + to_string(rc));
        }
    }
    rc = sqlite3_bind_int(stmt, idxParam++, (int)recordsCount);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::allAdditionalPaymentRecords: "
                          "Bad binding of recordsCount; sqlite error: " + to_string(rc));
    }
    rc = sqlite3_bind_int(stmt, idxParam, (int)fromRecord);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::allAdditionalPaymentRecords: "
                          "Bad binding of fromRecord; sqlite error: " + to_string(rc));
    }

    while (sqlite3_step(stmt) == SQLITE_ROW ) {
        result.push_back(
            deserializePaymentAdditionalRecord(
                stmt));
    }

    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    return result;
}

vector<Record::Shared> HistoryStorage::recordsPortionWithContractor(
    const SerializedEquivalent equivalent,
    size_t recordsCount,
    size_t fromRecord)
{
    vector<Record::Shared> result;
    string query = "SELECT operation_uuid, operation_timestamp, record_body, record_body_bytes_count, record_type"
                   " FROM " + mMainTableName + " WHERE equivalent = ? "
                   "ORDER BY operation_timestamp DESC LIMIT ? OFFSET ?;";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(mDataBase, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::recordsPortionWithContractor: "
                          "Bad query; sqlite error: " + to_string(rc));
    }
    rc = sqlite3_bind_int(stmt, 1, equivalent);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::recordsPortionWithContractor: "
                          "Bad binding of Equivalent; sqlite error: " + to_string(rc));
    }
    rc = sqlite3_bind_int(stmt, 2, (int)recordsCount);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::recordsPortionWithContractor: "
                          "Bad binding of recordsCount; sqlite error: " + to_string(rc));
    }
    rc = sqlite3_bind_int(stmt, 3, (int)fromRecord);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::recordsPortionWithContractor: "
                          "Bad binding of fromRecord; sqlite error: " + to_string(rc));
    }

    while (sqlite3_step(stmt) == SQLITE_ROW ) {
        int recordType = sqlite3_column_int(stmt, 4);
        switch (recordType) {
            case Record::TrustLineRecordType:
                result.push_back(
                    deserializeTrustLineRecord(
                        stmt));
                break;
            case Record::PaymentRecordType:
                result.push_back(
                    deserializePaymentRecord(
                        stmt));
                break;
            default:
                throw ValueError("HistoryStorage::recordsPortionWithContractor: "
                                     "invalid record type");
        }
    }

    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    return result;
}

vector<Record::Shared> HistoryStorage::recordsWithContractor(
    vector<BaseAddress::Shared> contractorAddresses,
    const SerializedEquivalent equivalent,
    size_t recordsCount,
    size_t fromRecord)
{
    vector<Record::Shared> result;

    string query = "SELECT count(*) FROM " + mMainTableName + " WHERE equivalent = ?";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(mDataBase, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::recordsWithContractor: "
                          "Bad query; sqlite error: " + to_string(rc));
    }
    rc = sqlite3_bind_int(stmt, 1, equivalent);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::recordsWithContractor: "
                          "Bad binding of Equivalent; sqlite error: " + to_string(rc));
    }
    sqlite3_step(stmt);
    auto allRecordsCount = (size_t)sqlite3_column_int(stmt, 0);
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
#ifdef STORAGE_HANDLER_DEBUG_LOG
    debug() << "all records count: " << allRecordsCount;
#endif
    size_t currentOffset = 0;
    size_t countRecordsUnderConditions = 0;
    while (result.size() < recordsCount && currentOffset < allRecordsCount) {
        auto records = recordsPortionWithContractor(
            equivalent,
            kPortionRequestSize,
            currentOffset);
        for (auto &record : records) {
            if (record->contractor()->containsAddresses(contractorAddresses)) {
                countRecordsUnderConditions++;
                if (countRecordsUnderConditions > fromRecord) {
                    result.push_back(record);
                }
            }
            if (result.size() >= recordsCount) {
                break;
            }
        }
        currentOffset += kPortionRequestSize;
    }
    return result;
}

vector<PaymentRecord::Shared> HistoryStorage::paymentRecordsByCommandUUID(
    const CommandUUID &commandUUID)
{
    vector<PaymentRecord::Shared> result;
    string query = "SELECT operation_uuid, operation_timestamp, record_body, record_body_bytes_count FROM "
                   + mMainTableName + " WHERE record_type = ? AND command_uuid = ?";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(mDataBase, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::paymentRecordsByCommandUUID: "
                          "Bad query; sqlite error: " + to_string(rc));
    }

    rc = sqlite3_bind_int(stmt, 1, Record::PaymentRecordType);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::paymentRecordsByCommandUUID: "
                          "Bad binding of RecordType; sqlite error: " + to_string(rc));
    }

    rc = sqlite3_bind_blob(stmt, 2, commandUUID.data, CommandUUID::kBytesSize, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::paymentRecordsByCommandUUID: "
                          "Bad binding of commandUUID; sqlite error: " + to_string(rc));
    }

    while (sqlite3_step(stmt) == SQLITE_ROW ) {
        result.push_back(
            deserializePaymentRecord(
                stmt));
        return result;
    }

    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    return result;
}

bool HistoryStorage::whetherOperationWasConducted(
    const TransactionUUID &transactionUUID)
{
    string query = "SELECT operation_uuid FROM "
                   + mMainTableName + " WHERE operation_uuid = ? LIMIT 1";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(mDataBase, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::whetherOperationWasConducted: "
                          "Bad query; sqlite error: " + to_string(rc));
    }

    rc = sqlite3_bind_blob(stmt, 1, transactionUUID.data, TransactionUUID::kBytesSize, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::whetherOperationWasConducted: "
                          "Bad binding of transactionUUID; sqlite error: " + to_string(rc));
    }

    bool result = (sqlite3_step(stmt) == SQLITE_ROW);
    
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    return result;
}

TrustLineRecord::Shared HistoryStorage::deserializeTrustLineRecord(
    sqlite3_stmt *stmt)
{
    TransactionUUID operationUUID((uint8_t *)sqlite3_column_blob(stmt, 0));
    auto timestamp = (GEOEpochTimestamp)sqlite3_column_int64(stmt, 1);
    auto recordBodyBytesCount = (size_t)sqlite3_column_int(stmt, 3);
    BytesShared recordBody = tryMalloc(recordBodyBytesCount);
    memcpy(
        recordBody.get(),
        sqlite3_column_blob(stmt, 2),
        recordBodyBytesCount);

    return make_shared<TrustLineRecord>(
        operationUUID,
        timestamp,
        recordBody);
}

PaymentRecord::Shared HistoryStorage::deserializePaymentRecord(
    sqlite3_stmt *stmt)
{
    TransactionUUID operationUUID((uint8_t *)sqlite3_column_blob(stmt, 0));
    auto timestamp = (GEOEpochTimestamp)sqlite3_column_int64(stmt, 1);
    auto recordBodyBytesCount = (size_t)sqlite3_column_int(stmt, 3);
    BytesShared recordBody = tryMalloc(recordBodyBytesCount);
    memcpy(
        recordBody.get(),
        sqlite3_column_blob(stmt, 2),
        recordBodyBytesCount);

    return make_shared<PaymentRecord>(
        operationUUID,
        timestamp,
        recordBody);
}

PaymentAdditionalRecord::Shared HistoryStorage::deserializePaymentAdditionalRecord(
    sqlite3_stmt *stmt)
{
    TransactionUUID operationUUID((uint8_t *)sqlite3_column_blob(stmt, 0));
    auto timestamp = (GEOEpochTimestamp)sqlite3_column_int64(stmt, 1);
    auto recordBodyBytesCount = (size_t)sqlite3_column_int(stmt, 3);
    BytesShared recordBody = tryMalloc(recordBodyBytesCount);
    memcpy(
        recordBody.get(),
        sqlite3_column_blob(stmt, 2),
        recordBodyBytesCount);

    return make_shared<PaymentAdditionalRecord>(
        operationUUID,
        timestamp,
        recordBody);
}

void HistoryStorage::deleteAllMainHistory(
    const SerializedEquivalent equivalent)
{
    string query = "DELETE FROM " + mMainTableName + " WHERE equivalent = ?";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2( mDataBase, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw IOError("AddressHandler::deleteAllMainHistory: "
                      "Bad query; sqlite error: " + to_string(rc));
    }
    rc = sqlite3_bind_int(stmt, 1, equivalent);
    if (rc != SQLITE_OK) {
        throw IOError("AddressHandler::deleteAllMainHistory: "
                      "Bad binding of Equivalent; sqlite error: " + to_string(rc));
    }

    rc = sqlite3_step(stmt);
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    if (rc == SQLITE_DONE) {
#ifdef STORAGE_HANDLER_DEBUG_LOG
        info() << "deleting is completed successfully";
#endif
    } else {
        throw IOError("AddressHandler::deleteAllMainHistory: "
                      "Run query; sqlite error: " + to_string(rc));
    }
}

void HistoryStorage::deleteAllAdditionalHistory(
    const SerializedEquivalent equivalent)
{
    string query = "DELETE FROM " + mAdditionalTableName + " WHERE equivalent = ?";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2( mDataBase, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw IOError("AddressHandler::deleteAllAdditionalHistory: "
                      "Bad query; sqlite error: " + to_string(rc));
    }
    rc = sqlite3_bind_int(stmt, 1, equivalent);
    if (rc != SQLITE_OK) {
        throw IOError("AddressHandler::deleteAllAdditionalHistory: "
                      "Bad binding of Equivalent; sqlite error: " + to_string(rc));
    }

    rc = sqlite3_step(stmt);
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    if (rc == SQLITE_DONE) {
#ifdef STORAGE_HANDLER_DEBUG_LOG
        info() << "deleting is completed successfully";
#endif
    } else {
        throw IOError("AddressHandler::deleteAllAdditionalHistory: "
                      "Run query; sqlite error: " + to_string(rc));
    }
}

vector<SerializedEquivalent> HistoryStorage::allMainEquivalents()
{
    vector<SerializedEquivalent> result;
    string query = "SELECT DISTINCT equivalent FROM " + mMainTableName;

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(mDataBase, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::allEquivalents: "
                      "Bad query; sqlite error: " + to_string(rc));
    }

    while (sqlite3_step(stmt) == SQLITE_ROW ) {
        auto equivalent = (SerializedEquivalent)sqlite3_column_int(stmt, 0);
        result.push_back(equivalent);
    }

    return result;
}

vector<SerializedEquivalent> HistoryStorage::allAdditionalEquivalents()
{
    vector<SerializedEquivalent> result;
    string query = "SELECT DISTINCT equivalent FROM " + mAdditionalTableName;

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(mDataBase, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw IOError("HistoryStorage::allEquivalents: "
                      "Bad query; sqlite error: " + to_string(rc));
    }

    while (sqlite3_step(stmt) == SQLITE_ROW ) {
        auto equivalent = (SerializedEquivalent)sqlite3_column_int(stmt, 0);
        result.push_back(equivalent);
    }

    return result;
}

LoggerStream HistoryStorage::info() const
{
    return mLog.info(logHeader());
}

LoggerStream HistoryStorage::debug() const
{
    return mLog.debug(logHeader());
}

LoggerStream HistoryStorage::warning() const
{
    return mLog.warning(logHeader());
}

const string HistoryStorage::logHeader() const
{
    stringstream s;
    s << "[HistoryStorage]";
    return s.str();
}
