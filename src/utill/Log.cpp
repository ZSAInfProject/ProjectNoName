

#include "Log.h"

void Log::setLogFile(std::string fileName, int mode) {
    logFileName = fileName;
    logFile.open(logFileName.c_str());
    loggingMode = mode;
}

void Log::getMode(){
    std::cout<< loggingMode<<std::endl;
}

void Log::error(std::string message) {
    if(loggingMode != MODE_NOLOG){
        std::cerr<< "ERROR "+message+'\n';
        logFile << "ERROR "+message+'\n';
    }
}

void Log::debug(std::string message) {
    if(loggingMode == MODE_DEBUG || loggingMode == MODE_VERBOSE){
        std::cerr<< "DEBUG "+message+'\n';
        logFile << "DEBUG "+message+'\n';
    }
}

void Log::info(std::string message) {
    if(loggingMode == MODE_VERBOSE){
        std::cerr<< "DEBUG "+message+'\n';
        logFile << "DEBUG "+message+'\n';
    }
}


Log::~Log() {
    logFile.close();
}

