#include <iostream>
#include "World.h"

World::World(int seed) : chunkDatabase(std::make_unique<ChunkGenerator>(seed)){

}

void World::render(sf::RenderWindow &window, sf::View camera) {
    int chunkSideLengthInWorldCord = Chunk::SIDE_LENGTH*Chunk::TILE_SIZE;
    auto minXChunk = static_cast<int>(camera.getCenter().x - camera.getSize().x / 2)/chunkSideLengthInWorldCord-1;
    auto maxXChunk = static_cast<int>(camera.getCenter().x + camera.getSize().x / 2)/chunkSideLengthInWorldCord+1;
    auto minYChunk = static_cast<int>(camera.getCenter().y - camera.getSize().y / 2)/chunkSideLengthInWorldCord-1;
    auto maxYChunk = static_cast<int>(camera.getCenter().y + camera.getSize().y / 2)/chunkSideLengthInWorldCord+1;
    for(int x = minXChunk; x <= maxXChunk;x++){
        for(int y = minYChunk; y <= maxYChunk; y++){
            chunkDatabase.getChunk(x, y)->render(window,
                                                 sf::Vector2f(chunkSideLengthInWorldCord*x,
                                                              chunkSideLengthInWorldCord*y),
                                                 sf::Vector2f(1.0, 1.0));
        }
    }
    ChunkGenerator chunkGenerator(10);
    chunkGenerator.generateChunk(0,0)->render(window, sf::Vector2f(0,0), sf::Vector2f(1,1));
}
