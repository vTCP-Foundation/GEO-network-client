#include "Logger.h"
#include <sys/ioctl.h>

// Initialize static member
Logger* Logger::instance = nullptr;

/**
 * @brief Stream class for handling log message construction and output
 * @param logger Pointer to the Logger instance
 * @param group Log level group (e.g., "INFO", "ERROR")
 * @param subsystem Name of the subsystem generating the log
 * @param type Type of logging stream (Standard, Transaction, or Dummy)
 */
LoggerStream::LoggerStream(
    Logger *logger,
    const string &group,
    const string &subsystem,
    const StreamType type) :

    mLogger(logger),
    mGroup(group),
    mSubsystem(subsystem),
    mType(type)
{}

LoggerStream::~LoggerStream()
{
    if (mType == Dummy) {
        // No output must be generated.
        return;
    }

    if (mType == Transaction) {
        // If this message was received from the transaction,
        // but transactions log was disabled - ignore this.
#ifndef TRANSACTIONS_LOG
        return;
#endif
    }

    auto message = this->str();
    mLogger->logRecord(
        mGroup,
        mSubsystem,
        message);
}

/**
 * Returns logger stream, that would collect information, but would never outputs it.
 */
LoggerStream LoggerStream::dummy()
{
    return LoggerStream(nullptr, "", "", Dummy);
}

LoggerStream::LoggerStream(
    const LoggerStream &other) :

    mLogger(other.mLogger),
    mGroup(other.mGroup),
    mSubsystem(other.mSubsystem),
    mType(other.mType)
{}

/**
 * @brief Initializes the Logger singleton with appropriate file handling and signal management
 * @note Sets up buffering, signal handlers, and log file rotation
 */
Logger::Logger():
    mOperationLogFileName("operations.log"),
    mOperationsLogFileLinesNumber(0),
    mIsTTYMode(isatty(STDOUT_FILENO)),
    mLastFlushTime(std::chrono::steady_clock::now())
{
    instance = this;
    std::cout.setf(std::ios::unitbuf);

    calculateOperationsLogFileLinesNumber();

#ifdef DEBUG
    mOperationsLogFile.open(mOperationLogFileName, std::fstream::out | std::fstream::trunc);
#else
    mOperationsLogFile.open(mOperationLogFileName, std::fstream::out | std::fstream::app);
#endif

    if (!mOperationsLogFile.is_open()) {
        throw std::runtime_error("Failed to open operations log file");
    }

    setupSignalHandlers();
}

Logger::~Logger()
{
    cleanup();
}

void Logger::setupSignalHandlers()
{
    struct sigaction sa;
    sa.sa_handler = signalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, nullptr);
    sigaction(SIGTERM, &sa, nullptr);
}

void Logger::signalHandler(int signal)
{
    if (instance) {
        instance->cleanup();
    }

    // Re-raise the signal after cleanup
    struct sigaction sa;
    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(signal, &sa, nullptr);
    raise(signal);
}

void Logger::cleanup()
{
    // Flush cout
    std::cout.flush();

    // Flush and close the operations log file
    if (mOperationsLogFile.is_open()) {
        mOperationsLogFile.flush();
        mOperationsLogFile.close();
    }
}

void Logger::logException(
    const string &subsystem,
    const exception &e)
{
    auto m = string(e.what());
    logRecord("EXCEPT", subsystem, m);
}

LoggerStream Logger::info(
    const string &subsystem)
{
    return LoggerStream(this, "INFO", subsystem);
}

LoggerStream Logger::warning(
    const string &subsystem)
{
    return LoggerStream(this, "WARN", subsystem);
}

LoggerStream Logger::debug(
    const string &subsystem)
{
    return LoggerStream(this, "DBG ", subsystem);
}

LoggerStream Logger::error(
    const string &subsystem)
{
    return LoggerStream(this, "ERR ", subsystem);
}

/**
 * @brief Calculates the number of lines in the operations log file
 * @note Used for log rotation management
 */
void Logger::calculateOperationsLogFileLinesNumber()
{
    std::ifstream operationLogFile(mOperationLogFileName);
    std::string line;
    mOperationsLogFileLinesNumber = 0;

    if (!operationLogFile.is_open()) {
        return;
    }

    while (std::getline(operationLogFile, line)) {
        ++mOperationsLogFileLinesNumber;
    }
    operationLogFile.close();
}

/**
 * @brief Rotates the log file when it reaches the maximum size limit
 * @note Creates a new log file with timestamp and archives the old one
 */
void Logger::rotate()
{
    if (!mOperationsLogFile.is_open()) {
        return;
    }

    mOperationsLogFile.close();

    std::stringstream rotateFileName;
    rotateFileName << "archived_operation_" << utc_now() << ".log";

    std::string archiveFileName = rotateFileName.str();
    formatLogFileName(archiveFileName, " ", "_");

    if (std::rename(mOperationLogFileName.c_str(), archiveFileName.c_str()) != 0) {
        // If rename fails, try to reopen the original file
        mOperationsLogFile.open(mOperationLogFileName, std::fstream::out | std::fstream::app);
        return;
    }

    mOperationsLogFile.open(mOperationLogFileName, std::fstream::out | std::fstream::trunc);
    if (!mOperationsLogFile.is_open()) {
        throw std::runtime_error("Failed to create new log file after rotation");
    }
}

/**
 * @brief Formats a log message for output
 * @param message The raw message to format
 * @return Formatted message with proper punctuation
 */
const string Logger::formatMessage(
    const string &message) const
{
    if (message.empty()) {
        return message;
    }

    string m = message;
    const size_t lastChar = m.size() - 1;

    // Remove trailing newline if present
    if (m[lastChar] == '\n') {
        m.pop_back();
    }

    return m;
}

const string Logger::recordPrefix(
    const string &group)
{
    stringstream s;
    s << utc_now() << " : ";

    if (isatty(STDOUT_FILENO)) {
        // Terminal output - use formatting
        if (group == "INFO") {
            s << "\033[32m" << group << "\t" << "\033[0m";
        } else if (group == "WARN") {
            s << "\033[33m" << group << "\t" << "\033[0m";
        } else if (group == "ERR") {
            s << "\033[31m" << group << "\t" << "\033[0m";
        } else {
            s << group << "\t";
        }

    } else {
        // File output - no formatting needed
        s << group << "\t";
    }


    return s.str();
}

/**
 * @brief Logs a record with proper formatting and output handling
 * @param group Log level group
 * @param subsystem Name of the subsystem
 * @param message Log message content
 */
void Logger::logRecord(
    const string &group,
    const string &subsystem,
    const string &message)
{
    const string formattedMessage = formatMessage(message);

    // Prepare JSON output
    stringstream jsonStream;
    jsonStream << "{"
               << "\"timestamp\":\"" << utc_now() << "\","
               << "\"level\":\"" << group << "\","
               << "\"subsystem\":\"" << subsystem << "\","
               << "\"message\":\"" << escapeJsonString(formattedMessage) << "\""
               << "}" << endl;

    // Write to log file
    mOperationsLogFile << jsonStream.str();

    // Console output handling
    if (mIsTTYMode) {
        outputToTTY(group, subsystem, formattedMessage);
    } else {
        std::cout << jsonStream.str();
    }

    // Periodic flush and rotation check
    auto now = std::chrono::steady_clock::now();
    if (now - mLastFlushTime >= FLUSH_INTERVAL) {
        mOperationsLogFile.flush();
        mLastFlushTime = now;
    }

    if (++mOperationsLogFileLinesNumber >= maxRotateLimit) {
        rotate();
        mOperationsLogFileLinesNumber = 0;
    }
}

/**
 * @brief Handles TTY-specific output formatting
 * @param group Log level group
 * @param subsystem Name of the subsystem
 * @param message Formatted message
 */
void Logger::outputToTTY(
    const string &group,
    const string &subsystem,
    const string &message)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    const int terminal_width = w.ws_col;

    stringstream recordStream;
    recordStream << recordPrefix(group) << message;

    // Calculate plain text length (without ANSI sequences)
    string currentLine = recordStream.str();
    string plainLine = stripAnsiSequences(currentLine);

    // Right-align subsystem tag if it fits
    int spaces = terminal_width - plainLine.length() - subsystem.length() - 5;
    if (spaces > 0) {
        recordStream << string(spaces, ' ');
    }
    recordStream << "\033[2m" << subsystem << "\033[0m" << endl;

    std::cout << recordStream.str();
}

/**
 * @brief Removes ANSI escape sequences from a string
 * @param input String containing ANSI sequences
 * @return Clean string without ANSI sequences
 */
string Logger::stripAnsiSequences(const string& input)
{
    string result = input;
    size_t start_pos = 0;
    while ((start_pos = result.find("\033[", start_pos)) != string::npos) {
        size_t end_pos = result.find("m", start_pos);
        if (end_pos == string::npos) break;
        result.erase(start_pos, end_pos - start_pos + 1);
    }
    return result;
}

bool Logger::formatLogFileName(
    string& str,
    const string& from,
    const string& to)
{
    size_t start_pos = str.find(from);
    if(start_pos == string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

string Logger::escapeJsonString(const string& input)
{
    stringstream escaped;
    for (char c : input) {
        switch (c) {
        case '\"':
            escaped << "\\\"";
            break;
        case '\\':
            escaped << "\\\\";
            break;
        case '\b':
            escaped << "\\b";
            break;
        case '\f':
            escaped << "\\f";
            break;
        case '\n':
            escaped << "\\n";
            break;
        case '\r':
            escaped << "\\r";
            break;
        case '\t':
            escaped << "\\t";
            break;
        default:
            if ('\x00' <= c && c <= '\x1f') {
                escaped << "\\u" << std::hex << std::setw(4) << std::setfill('0') << (int)c;
            } else {
                escaped << c;
            }
        }
    }
    return escaped.str();
}
