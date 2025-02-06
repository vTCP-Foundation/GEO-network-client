#ifndef VTCPD_EVENTSINTERFACE_H
#define VTCPD_EVENTSINTERFACE_H

#include "../../BaseFIFOInterface.h"
#include "../events/Event.h"
#include "../../../logger/Logger.h"

class EventsInterface : public BaseFIFOInterface
{

public:
    explicit EventsInterface(
        string fifoName,
        bool isBlocked,
        Logger &logger);

    ~EventsInterface();

    void writeEvent(
        Event::Shared event);

private:
    virtual const char* FIFOName() const;

public:
    static const constexpr unsigned int kPermissionsMask = 0755;

private:
    string mFIFOName;
    bool mIsBlocked;
    Logger &mLogger;
};


#endif //VTCPD_EVENTSINTERFACE_H
