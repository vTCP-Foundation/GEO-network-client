#ifndef VTCPD_LOGGER_H
#define VTCPD_LOGGER_H

#include "../common/exceptions/Exception.h"
#include "../common/time/TimeUtils.h"

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <csignal>
#include <chrono>
#include <fstream>

using namespace std;


class Logger;

/**
 * Logger stream is used as interface for the logger.
 * It collects information
 */
class LoggerStream:
    public stringstream
{

public:
    enum StreamType
    {
        Standard = 0,

        // Used into the transactions.
        // Helps distinquish transactions log from other logs flows.
        Transaction,

        //...
        // Other logs types must be located here

        // Dummy logger is used in configurations,
        // where no log records should be issued,
        // but the logger structure is required by some type of signature(s).
        Dummy,
    };

public:
    explicit LoggerStream(
        Logger *logger,
        const string &group,
        const string &subsystem,
        const StreamType type = Standard);

    LoggerStream(
        const LoggerStream &other);

    ~LoggerStream();

    static LoggerStream dummy();

private:
    Logger *mLogger;
    const string mGroup;
    const string mSubsystem;
    const StreamType mType;
};


class Logger
{
    friend class LoggerStream;

public:
    Logger();
    ~Logger();

    static void setupSignalHandlers();
    static void signalHandler(int signal);
    static Logger* getInstance()
    {
        return instance;
    }

    void cleanup();

    void logException(
        const string &subsystem,
        const exception &e);

    LoggerStream info(
        const string &subsystem);

    LoggerStream error(
        const string &subsystem);

    LoggerStream debug(
        const string &subsystem);

    LoggerStream warning(
        const string &subsystem);

protected:
    const uint32_t maxRotateLimit = 500000;
    const std::chrono::seconds FLUSH_INTERVAL{3}; // Flush every 3 seconds

protected:
    const string formatMessage(
        const string &message) const;

    const string recordPrefix(
        const string &group);

    void logRecord(
        const string &group,
        const string &subsystem,
        const string &message);

    void rotate();

    bool formatLogFileName(
        string& str,
        const string& from,
        const string& to);

    void calculateOperationsLogFileLinesNumber();

    string escapeJsonString(const string& input);

    /**
     * @brief Handles TTY-specific output formatting
     */
    void outputToTTY(
        const string &group,
        const string &subsystem,
        const string &message);

    /**
     * @brief Removes ANSI escape sequences from a string
     */
    string stripAnsiSequences(const string& input);

private:
    static Logger* instance;
    std::ofstream mOperationsLogFile;
    uint32_t mOperationsLogFileLinesNumber;
    string mOperationLogFileName;
    bool mIsTTYMode;
    std::chrono::steady_clock::time_point mLastFlushTime;
};
#endif //VTCPD_LOGGER_H