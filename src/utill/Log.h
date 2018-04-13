#ifndef NONAME_LOG_H
#define NONAME_LOG_H

#include <iostream>
#include <fstream>
#include <string>



#define ERRORLOG(message) Log::get().error(message, __PRETTY_FUNCTION__)
#define LOG(message) Log::get().info(message, __PRETTY_FUNCTION__)

enum Mode{ DEBUG, VERBOSE, NOLOG, ERROR};

class Log {
public:
    static Log& get(){
        static Log instance;
        return instance;
    }

    Log(Log const&) = delete;
    void operator=(Log const&) = delete;

    void setLogFile(std::string, Mode);
    void error(std::string, std::string);
    void info(std::string, std::string);
    void debug(std::string, std::string);
    void getMode();

private:

    std::string logFileName;
    std::fstream logFile;
    Mode loggingMode = NOLOG;

    Log() = default;
    ~Log();

};


#endif //NONAME_LOG_H
