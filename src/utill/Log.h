#ifndef NONAME_LOG_H
#define NONAME_LOG_H

#include <iostream>
#include <fstream>
#include <string>

#define MODE_ERROR 0
#define MODE_DEBUG 1
#define MODE_VERBOSE 2
#define MODE_NOLOG 3

#define ERRORLOG(message) std::cerr<<"ERROR from "<<__PRETTY_FUNCTION__<<" ["<<__LINE__<<"] "<<message<<std::endl
#define LOG(message) std::clog<<"INFO from "<<__PRETTY_FUNCTION__<<" ["<<__LINE__<<"] "<<message<<std::endl

class Log {
public:
    static Log& get(){
        static Log instance;
        return instance;
    }

    Log(Log const&) = delete;
    void operator=(Log const&) = delete;

    void setLogFile(std::string, int);
    void error(std::string);
    void info(std::string);
    void debug(std::string);
    void getMode();

private:

    std::string logFileName;
    std::fstream logFile;
    int loggingMode = MODE_NOLOG;

    Log() = default;
    ~Log();

};


#endif //NONAME_LOG_H
