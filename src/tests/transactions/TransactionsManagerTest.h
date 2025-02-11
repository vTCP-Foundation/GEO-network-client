#ifndef VTCPD_TRANSACTIONSSCHEDULERTEST_H
#define VTCPD_TRANSACTIONSSCHEDULERTEST_H


#include "../../core/transactions/manager/TransactionsManager.h"

class TransactionsManagerTest
{

public:
    TransactionsManager *mTransactionsManager;
    Logger *mLog;

public:
    TransactionsManagerTest(
        as::io_context &IOCtx,
        TrustLinesManager *trustLinesManager,
        ResultsInterface *resultsInterface,
        Logger *logger);

    ~TransactionsManagerTest();

    void runFewTransactionsWithSuccessResultTestCase();

    void runSingleTransactionWithFailureStateAndAfterRelaunchingWithSuccessResultTestCase();

    void runSingleTransactionWhichCrashWhileRunning();

    void runFewTransactionsWhichReturnsFailureStateAndAfterRelaunchingWithSuccessResultTestCase();

    void runAllCases();

    void run();
};


#endif //VTCPD_TRANSACTIONSSCHEDULERTEST_H
