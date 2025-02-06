#ifndef VTCPD_FEATURESHANDLER_H
#define VTCPD_FEATURESHANDLER_H

#include "../../common/Types.h"
#include "../../logger/Logger.h"
#include "../../common/memory/MemoryUtils.h"
#include "../../common/exceptions/IOError.h"
#include "../../common/exceptions/NotFoundError.h"
#include "../../../libs/sqlite3/sqlite3.h"

#include <string>

class FeaturesHandler
{

public:
    FeaturesHandler(
        sqlite3 *dbConnection,
        const string &tableName,
        Logger &logger);

    void saveFeature(
        const string& featureName,
        const string& featureValue);

    string getFeature(
        const string& featureName);

private:
    LoggerStream info() const;

    LoggerStream warning() const;

    const string logHeader() const;

private:
    sqlite3 *mDataBase = nullptr;
    string mTableName;
    Logger &mLog;
};


#endif //VTCPD_FEATURESHANDLER_H
