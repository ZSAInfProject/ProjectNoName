#include "Game.h"
#include "utils/Log.h"
#include "utils/Settings.h"

int main() {
    Settings::get().setFile("settings.json");
    Log::get().setLogFile(Settings::get<std::string>("logfile"));
    Log::get().setMode(Settings::get<std::string>("log_mode"));
    try {
        Log::info("Main", "Program started");
        Game::get().run();
        return EXIT_SUCCESS;
    }
    catch(...) {
        Log::error("Main", "Fatal error. Exiting");
        return EXIT_FAILURE;
    }
}

