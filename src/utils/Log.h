#ifndef NONAME_LOG_H
#define NONAME_LOG_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

enum Mode { VERBOSE = 0, DEBUG = 1, INFO = 2, WARN = 3, ERROR = 4, NOLOG = 5 };

class LogSubscriber {
public:
    //! Notify about a new message
    virtual void notify(enum Mode, std::string tag, std::string message) = 0;
};

//! Singleton responsible for logging and saving logs.
class Log {

public:
    static constexpr auto TAG = "Log";

    static Log& get() {
        static Log instance;
        return instance;
    }

    Log(Log const&) = delete;
    void operator=(Log const&) = delete;

    static void error(std::string tag, std::string message);
    static void warn(std::string tag, std::string message);
    static void info(std::string tag, std::string message);
    static void debug(std::string tag, std::string message);
    static void verbose(std::string tag, std::string message);

    //! Sets file to save logs to and logging level.
    void setLogFile(std::string);
    void getMode();

    void setMode(std::string mode);

    //! Subscribe to receive debug messages
    void subscribe(LogSubscriber& subscriber);

private:
    std::vector<std::reference_wrapper<LogSubscriber>> subscribers;
    void notifyAll(enum Mode, std::string, std::string);

    std::string logFileName;
    std::ofstream logFile;
    Mode loggingMode = VERBOSE;

    Log() = default;
    ~Log();

};


#endif //NONAME_LOG_H
