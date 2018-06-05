#include <iostream>
#include <vector>
#include "World.h"
#include "../tile/Tile.h"
#include "../utils/Log.h"
#include "../Game.h"

World::World(int seed) : chunkDatabase(std::make_unique<ChunkGenerator>(seed)){

}

void World::render(sf::View camera) {
    int chunkSideLengthInWorldCord = Chunk::SIDE_LENGTH*Chunk::TILE_SIZE;
    auto minXChunk = static_cast<int>(camera.getCenter().x - camera.getSize().x / 2)/chunkSideLengthInWorldCord-1;
    auto maxXChunk = static_cast<int>(camera.getCenter().x + camera.getSize().x / 2)/chunkSideLengthInWorldCord+1;
    auto minYChunk = static_cast<int>(camera.getCenter().y + camera.getSize().y / 2)/chunkSideLengthInWorldCord-2;
    auto maxYChunk = static_cast<int>(camera.getCenter().y - camera.getSize().y / 2)/chunkSideLengthInWorldCord;
    for(int x = minXChunk; x <= maxXChunk;x++){
        for(int y = minYChunk; y <= maxYChunk; y++){
            chunkDatabase.getChunk(x, y)->render(Game::getRenderWindow(),
                                                 sf::Vector2f(chunkSideLengthInWorldCord*x,
                                                              chunkSideLengthInWorldCord*y),
                                                 sf::Vector2f(1.0, 1.0));
        }
    }
}

ChunkTile World::getTile(int x, int y) {
    sf::Vector2i tile;
    sf::Vector2i chunk;
    chunk.x = static_cast<int>(floor(1.0f * x / Chunk::SIDE_LENGTH));
    chunk.y = static_cast<int>(floor(1.0f * y / Chunk::SIDE_LENGTH));
    auto mod = [](int a, int b)->int{int ret = a%b; return ret>=0? ret: ret+b;};
    tile.x = mod(x, Chunk::SIDE_LENGTH);
    tile.y = mod(y, Chunk::SIDE_LENGTH);
    return chunkDatabase.getChunk(chunk.x, chunk.y)->getTile(tile.x, tile.y);
}

void World::setTile(int x, int y, ChunkTile value) {
    Log::verbose(TAG, "Tile set at: " + std::to_string(x) + " " + std::to_string(y) + " to: " + std::to_string(value.tileId));
    sf::Vector2i tile;
    sf::Vector2i chunk;
    chunk.x = static_cast<int>(floor(1.0f * x / Chunk::SIDE_LENGTH));
    chunk.y = static_cast<int>(floor(1.0f * y / Chunk::SIDE_LENGTH));
    auto mod = [](int a, int b)->int{int ret = a%b; return ret>=0? ret: ret+b;};
    tile.x = mod(x, Chunk::SIDE_LENGTH);
    tile.y = mod(y, Chunk::SIDE_LENGTH);
    chunkDatabase.getChunk(chunk.x, chunk.y)->setTile(tile.x, tile.y, value);
}

short World::mineTile(int x, int y) {
    ChunkTile tile = getTile(x,y);
    short id = tile.tileId;
    if(tile.tileId != 1){
        tile.amount--;
        if(tile.amount <= 0){
            tile.tileId = 1;
        }
        setTile(x, y, tile);
    }
    return id;
}

std::vector<std::shared_ptr<Object>> World::getObjectsForUpdate() {
    std::vector<std::shared_ptr<Object>> vector;
    for (auto& chunk : chunkDatabase.chunkCache) {
        for (auto& object : chunk.second.chunk->objects) {
            vector.push_back(object);
        }
    }
    return vector;
}
