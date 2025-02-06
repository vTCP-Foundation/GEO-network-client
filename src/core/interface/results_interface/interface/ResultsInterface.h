#ifndef VTCPD_RESULTSINTERFACE_H
#define VTCPD_RESULTSINTERFACE_H

#include "../../BaseFIFOInterface.h"
#include "../../../logger/Logger.h"
#include "../../../common/exceptions/IOError.h"
#include "../../../common/exceptions/MemoryError.h"

#include <string>

using namespace std;

class ResultsInterface: public BaseFIFOInterface
{

public:
    explicit ResultsInterface(
        Logger &logger);

    ~ResultsInterface();

    void writeResult(
        const char* bytes,
        const size_t bytesCount);

private:
    virtual const char* FIFOName() const;

public:
    static const constexpr char* kFIFOName = "results.fifo";
    static const constexpr unsigned int kPermissionsMask = 0755;

private:
    Logger &mLog;
};

#endif //VTCPD_RESULTSINTERFACE_H
