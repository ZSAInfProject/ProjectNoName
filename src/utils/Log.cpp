#include "Log.h"
#include <utility>
#include <functional>

void Log::setLogFile(std::string fileName) {
    logFileName = std::move(fileName);
    logFile.open(logFileName);
    if(!logFile.is_open()) {
        Log::warn("Log", "Could not open log file");
    }
}

void Log::getMode() {
    std::cout << loggingMode << std::endl;
}

Log::~Log() {
    logFile.close();
}

void Log::error(std::string tag, std::string message) {
    if(get().loggingMode <= Mode::ERROR) {
        get().notifyAll(ERROR, tag, message);
        std::cout << "\033[31m[ERROR] " + tag + ": " + message + "\033[0m\n";
        get().logFile << "[ERROR] " + tag + ": " + message + "\n";
    }
}

void Log::warn(std::string tag, std::string message) {
    if(get().loggingMode <= Mode::WARN) {
        get().notifyAll(WARN, tag, message);
        std::cout << "\033[33m[WARN] " + tag + ": " + message + "\033[0m\n";
        get().logFile << "[WARN] " + tag + ": " + message + "\n";
    }
}

void Log::info(std::string tag, std::string message) {
    if(get().loggingMode <= Mode::INFO) {
        get().notifyAll(INFO, tag, message);
        std::cout << "[INFO] " + tag + ": " + message + "\n";
        get().logFile << "[INFO] " + tag + ": " + message + "\n";
    }
}

void Log::debug(std::string tag, std::string message) {
    if(get().loggingMode <= Mode::DEBUG) {
        get().notifyAll(DEBUG, tag, message);
        std::cout << "[DEBUG] " + tag + ": " + message + "\n";
        get().logFile << "[DEBUG] " + tag + ": " + message + "\n";
    }
}

void Log::verbose(std::string tag, std::string message) {
    if(get().loggingMode <= Mode::VERBOSE) {
        get().notifyAll(VERBOSE, tag, message);
        std::cerr << "[VERBOSE] " + tag + ": " + message + "\n";
        get().logFile << "[VERBOSE] " + tag + ": " + message + "\n";
    }
}

void Log::setMode(std::string mode) {
    if(mode == "ERROR") loggingMode = Mode::ERROR;
    else if(mode == "WARN") loggingMode = Mode::WARN;
    else if(mode == "INFO") loggingMode = Mode::INFO;
    else if(mode == "DEBUG") loggingMode = Mode::DEBUG;
    else if(mode == "VERBOSE") loggingMode = Mode::VERBOSE;

}

void Log::subscribe(LogSubscriber& sub) {
    std::reference_wrapper<LogSubscriber> ref = sub;
    get().subscribers.push_back(ref);
}

void Log::notifyAll(enum Mode mode, std::string tag, std::string message) {
    for(auto& subscriber : subscribers) {
        subscriber.get().notify(mode, tag, message);
    }
}

