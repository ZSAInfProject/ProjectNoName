//
// Created by zakrent on 4/6/18.
//

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "world/chunk/ChunkDatabase.h"

int main()
{
    ChunkDatabase db(std::make_unique<ChunkGenerator>(0));
    db.getChunk(0, 2);
    db.getChunk(0, 3);
    db.getChunk(0, 4);
    db.getChunk(0, 5);
    db.getChunk(0, 6);
    db.getChunk(0, 7);
    db.getChunk(0, 8);
    db.getChunk(0, 9);
    db.getChunk(0, 10);
    db.getChunk(0, 11);
    db.getChunk(0, 12);
    db.getChunk(0, 13);
    db.getChunk(0, 14);
    db.getChunk(0, 15);
    db.getChunk(0, 16);
    while(true) {
        db.getChunk(0, 10);
    }
    //Game::get().run();
    return EXIT_SUCCESS;
}

