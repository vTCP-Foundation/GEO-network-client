#ifndef VTCPD_ADDRESSHANDLER_H
#define VTCPD_ADDRESSHANDLER_H

#include "../../logger/Logger.h"
#include "../../common/exceptions/IOError.h"
#include "../../common/multiprecision/MultiprecisionUtils.h"

#include "../../../libs/sqlite3/sqlite3.h"

#include <vector>

class AddressHandler
{

public:
    AddressHandler(
        sqlite3 *dbConnection,
        const string &tableName,
        Logger &logger);

    void saveAddress(
        ContractorID contractorID,
        BaseAddress::Shared address);

    vector<BaseAddress::Shared> contractorAddresses(
        ContractorID contractorID);

    void removeAddresses(
        ContractorID contractorID);

private:
    LoggerStream info() const;

    LoggerStream warning() const;

    const string logHeader() const;

private:
    sqlite3 *mDataBase = nullptr;
    string mTableName;
    Logger &mLog;
};


#endif //VTCPD_ADDRESSHANDLER_H
