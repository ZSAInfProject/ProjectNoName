#ifndef NONAME_LOG_H
#define NONAME_LOG_H

#include <iostream>
#include <fstream>
#include <string>

enum Mode{ VERBOSE = 0, DEBUG = 1, INFO = 2, WARN = 3, ERROR = 4, NOLOG = 5};

class Log {
public:
    static Log& get(){
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

    void setLogFile(std::string, Mode);
    void getMode();

private:

    std::string logFileName;
    std::ofstream logFile;
    Mode loggingMode = VERBOSE;

    Log() = default;
    ~Log();

};


#endif //NONAME_LOG_H
