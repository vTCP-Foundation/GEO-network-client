#ifndef VTCPD_TRUSTLINESMANAGERTEST_H
#define VTCPD_TRUSTLINESMANAGERTEST_H

#include <iostream>
#include <string>
#include "../../core/trust_lines/manager/TrustLinesManager.h"

using namespace std;

class TrustLinesManagerTest
{

public:
    NodeUUID contractor1;
    NodeUUID contractor2;
    NodeUUID contractor3;
    NodeUUID contractor4;

    TrustLinesManager *mTrustLinesManager;

public:

    TrustLinesManagerTest();

    void openSuccessTestCase();

    void acceptSuccessTestCase();

    void closeSuccessTestCase();

    void rejectSuccessTestCase();

    void deletePointer();

    void showAllTrustLines();

    void run();
};


#endif //VTCPD_TRUSTLINESMANAGERTEST_H
