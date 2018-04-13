

#include "Log.h"

void Log::setLogFile(std::string fileName, Mode mode) {
    logFileName = fileName;
    logFile.open(logFileName.c_str());
    loggingMode = mode;
}

void Log::getMode(){
    std::cout<< loggingMode<<std::endl;
}

void Log::error(std::string message, std::string tag) {
    if(loggingMode != nolog){
        std::cerr<< "ERROR  in "+tag +" "+message+'\n';
        logFile << "ERROR in "+tag+" "+message+'\n';
    }
}

void Log::debug(std::string message, std::string tag) {
    if(loggingMode == verbose || loggingMode == Mode::debug){
        std::cerr<< "DEBUG from "+tag+" "+message+'\n';
        logFile << "DEBUG from "+tag+" "+message+'\n';
    }
}

void Log::info(std::string message,std::string tag) {
    if(loggingMode == verbose){
        std::cerr<< "INFO from "+tag+" "+message+'\n';
        logFile << "INFO from "+tag+" "+message+'\n';
    }
}


Log::~Log() {
    logFile.close();
}

