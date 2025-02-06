#ifndef VTCPD_TRUSTLINETESTS_H
#define VTCPD_TRUSTLINETESTS_H
#include <iostream>
#include <string>
#include "../../core/common/Types.h"
#include "../../core/trust_lines/TrustLine.h"
#include "../../core/logger/Logger.h"

using namespace std;

class TrustLineTests
{

public:
    NodeUUID contractor1;
    NodeUUID contractor2;

public:
    void create_trustlineCase();
    void check_balanceCase();
    void check_serializeCase();
    void change_parametersCase();
    void run();
    Logger *mLogger;
};

#endif //VTCPD_TRUSTLINETESTS_H
