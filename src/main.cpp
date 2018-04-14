#include "Game.h"
#include "utils/Log.h"

int main()
{
    Log::get().setLogFile("Log.txt", DEBUG);
    Log::info("Main", "Program started");
    Game::get().run();
    return EXIT_SUCCESS;
}

