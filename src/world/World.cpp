#include <iostream>
#include "World.h"
#include "../tile/Tile.h"
#include "../utils/Log.h"

World::World(int seed) : chunkDatabase(std::make_unique<ChunkGenerator>(seed)){

}

void World::render(sf::RenderWindow &window, sf::View camera) {
    int chunkSideLengthInWorldCord = Chunk::SIDE_LENGTH*Chunk::TILE_SIZE;
    auto minXChunk = static_cast<int>(camera.getCenter().x - camera.getSize().x / 2)/chunkSideLengthInWorldCord-1;
    auto maxXChunk = static_cast<int>(camera.getCenter().x + camera.getSize().x / 2)/chunkSideLengthInWorldCord+1;
    auto minYChunk = static_cast<int>(camera.getCenter().y + camera.getSize().y / 2)/chunkSideLengthInWorldCord-2;
    auto maxYChunk = static_cast<int>(camera.getCenter().y - camera.getSize().y / 2)/chunkSideLengthInWorldCord;
    for(int x = minXChunk; x <= maxXChunk;x++){
        for(int y = minYChunk; y <= maxYChunk; y++){
            chunkDatabase.getChunk(x, y)->render(window,
                                                 sf::Vector2f(chunkSideLengthInWorldCord*x,
                                                              chunkSideLengthInWorldCord*y),
                                                 sf::Vector2f(1.0, 1.0));
        }
    }
    ChunkGenerator chunkGenerator(10);
}

int World::getTile(int x, int y) {
    sf::Vector2i tile;
    tile.x = abs(x);
    tile.y = abs(y);
    sf::Vector2i chunk = tile / Chunk::SIDE_LENGTH;
    tile.x = tile.x % Chunk::SIDE_LENGTH;
    tile.y = tile.y % Chunk::SIDE_LENGTH;
    if (x<0) {
        chunk.x = -chunk.x - 1;
        tile.x = Chunk::SIDE_LENGTH - tile.x;
    }
    if (y<0) {
        chunk.y = -chunk.y - 1;
        tile.y = Chunk::SIDE_LENGTH - tile.y;
    }
    return chunkDatabase.getChunk(chunk.x, chunk.y)->getTile(tile.x, tile.y);
}

void World::setTile(int x, int y, int id) {
    Log::verbose(TAG, "Tile set at: " + std::to_string(x) + " " + std::to_string(y) + " to: " + std::to_string(id));
    sf::Vector2i tile;
    tile.x = abs(x);
    tile.y = abs(y);
    sf::Vector2i chunk = tile / Chunk::SIDE_LENGTH;
    tile.x = tile.x % Chunk::SIDE_LENGTH;
    tile.y = tile.y % Chunk::SIDE_LENGTH;
    if (x<0) {
        chunk.x = -chunk.x - 1;
        tile.x = Chunk::SIDE_LENGTH - tile.x;
    }
    if (y<0) {
        chunk.y = -chunk.y - 1;
        tile.y = Chunk::SIDE_LENGTH - tile.y;
    }
    chunkDatabase.getChunk(chunk.x, chunk.y)->setTile(tile.x, tile.y, id);
}
