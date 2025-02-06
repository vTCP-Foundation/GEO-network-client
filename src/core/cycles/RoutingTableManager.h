#ifndef VTCPD_ROUTINGTABLESBLUERAYEDITION_H
#define VTCPD_ROUTINGTABLESBLUERAYEDITION_H

#include "../contractors/addresses/BaseAddress.h"
#include "../logger/Logger.h"
#include "../common/Types.h"

#include <map>
#include <vector>

class RoutingTableManager
{

public:
    RoutingTableManager(
        const SerializedEquivalent equivalent,
        Logger &logger);

    void updateMapAddSeveralNeighbors(
        ContractorID firstLevelContractor,
        vector<BaseAddress::Shared> secondLevelContractors);

    void clearMap();

    vector<BaseAddress::Shared> secondLevelContractorsForNode(
        ContractorID contractorID);

    // TODO remove after testing
    void printRT();

protected:
    string logHeader() const;

    LoggerStream warning() const;

    LoggerStream error() const;

    LoggerStream info() const;

    LoggerStream debug() const;

protected:
    SerializedEquivalent mEquivalent;
    map<ContractorID, vector<BaseAddress::Shared>> mRoutingTable;
    Logger &mLog;
};

#endif //VTCPD_ROUTINGTABLESBLUERAYEDITION_H
