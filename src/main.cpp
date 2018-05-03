#include "Game.h"
#include "utils/Log.h"
#include "utils/Settings.h"

int main()
{
    Settings::get().setFile("settings.json");
    Log::get().setLogFile(Settings::get<std::string>("logfile"), DEBUG);
    Log::info("Main", "Program started");
    Game::get().run();
    return EXIT_SUCCESS;
}

