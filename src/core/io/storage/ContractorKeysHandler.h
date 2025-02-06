#ifndef VTCPD_CONTRACTORKEYSHANDLER_H
#define VTCPD_CONTRACTORKEYSHANDLER_H

#include "../../logger/Logger.h"
#include "../../common/memory/MemoryUtils.h"
#include "../../common/exceptions/IOError.h"
#include "../../common/exceptions/NotFoundError.h"
#include "../../common/exceptions/ValueError.h"
#include "../../crypto/lamportkeys.h"

#include "../../../libs/sqlite3/sqlite3.h"

using namespace crypto::lamport;

class ContractorKeysHandler
{

public:
    ContractorKeysHandler(
        sqlite3 *dbConnection,
        const string &tableName,
        Logger &logger);

    void saveKey(
        const TrustLineID trustLineID,
        const KeyNumber keysSetSequenceNumber,
        const PublicKey::Shared publicKey,
        const KeyNumber number);

    const KeyNumber maxKeySetSequenceNumber(
        const TrustLineID trustLineID);

    void invalidKey(
        const TrustLineID trustLineID,
        const KeyNumber number);

    void invalidKeyByHash(
        const TrustLineID trustLineID,
        const KeyHash::Shared keyHash);

    PublicKey::Shared keyByNumber(
        const TrustLineID trustLineID,
        const KeyNumber number);

    PublicKey::Shared keyByHash(
        const TrustLineID trustLineID,
        const KeyHash::Shared keyHash);

    const KeyHash::Shared keyHashByNumber(
        const TrustLineID trustLineID,
        const KeyNumber number);

    KeysCount availableKeysCnt(
        const TrustLineID trustLineID);

    void removeUnusedKeys(
        const TrustLineID trustLineID);

    vector<PublicKey::Shared> publicKeysBySetNumber(
        const TrustLineID trustLineID,
        const KeyNumber keysSetSequenceNumber) const;

    void deleteKeysByTrustLineID(
        const TrustLineID trustLineID);

    void deleteKeyByHashExceptSequenceNumber(
        KeyHash::Shared keyHash,
        const KeyNumber keysSetSequenceNumber);

    vector<KeyHash::Shared> publicKeyHashesLessThanSetNumber(
        const TrustLineID trustLineID,
        const KeyNumber keysSetSequenceNumber) const;

private:
    LoggerStream info() const;

    LoggerStream warning() const;

    const string logHeader() const;

private:
    sqlite3 *mDataBase = nullptr;
    string mTableName;
    Logger &mLog;
};


#endif //VTCPD_CONTRACTORKEYSHANDLER_H
