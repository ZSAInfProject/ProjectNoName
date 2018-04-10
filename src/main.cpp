//
// Created by zakrent on 4/6/18.
//

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "world/chunk/ChunkDatabase.h"

int main()
{
    Game::get().run();
    return EXIT_SUCCESS;
}

