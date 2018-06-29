#include <iostream>
#include <vector>
#include "World.h"
#include "../tile/Tile.h"
#include "../utils/Log.h"
#include "../Game.h"
#include "../entity/components/ObjectPosition.h"
#include "../tile/TileDatabase.h"
#include "../state/GameState.h"

World::World(int seed) : chunkDatabase(std::make_unique<ChunkGenerator>(seed)) {
}

void World::render(sf::View camera) {
    int chunkSideLengthInWorldCord = Chunk::SIDE_LENGTH * Chunk::TILE_SIZE;
    auto minXChunk = static_cast<int>(camera.getCenter().x - camera.getSize().x / 2) / chunkSideLengthInWorldCord - 1;
    auto maxXChunk = static_cast<int>(camera.getCenter().x + camera.getSize().x / 2) / chunkSideLengthInWorldCord + 1;
    auto minYChunk = static_cast<int>(camera.getCenter().y + camera.getSize().y / 2) / chunkSideLengthInWorldCord - 2;
    auto maxYChunk = static_cast<int>(camera.getCenter().y - camera.getSize().y / 2) / chunkSideLengthInWorldCord;
    for(int x = minXChunk; x <= maxXChunk; x++) {
        for(int y = minYChunk; y <= maxYChunk; y++) {
            chunkDatabase.getChunk(x, y)->render(Game::getRenderWindow(),
                                                 sf::Vector2f(chunkSideLengthInWorldCord * x,
                                                              chunkSideLengthInWorldCord * y),
                                                 sf::Vector2f(1.0, 1.0));
        }
    }
}

ChunkTile World::getTile(int x, int y) {
    auto coordinates = getTileAndChunkCoordinates(x, y);
    auto tile = coordinates.first;
    auto chunk = coordinates.second;

    return chunkDatabase.getChunk(chunk.x, chunk.y)->getTile(tile.x, tile.y);
}

ChunkTile World::getLoadedTile(int x, int y) {
    sf::Vector2i tile;
    sf::Vector2i chunk;
    chunk.x = static_cast<int>(floor(1.0f * x / Chunk::SIDE_LENGTH));
    chunk.y = static_cast<int>(floor(1.0f * y / Chunk::SIDE_LENGTH));
    auto mod = [](int a, int b) -> int {
        int ret = a % b;
        return ret >= 0 ? ret : ret + b;
    };
    tile.x = mod(x, Chunk::SIDE_LENGTH);
    tile.y = mod(y, Chunk::SIDE_LENGTH);
    if(!chunkDatabase.isChunkGenerated(chunk.x, chunk.y)) {
        ChunkTile chunkTile(0, 0);
        chunkTile.tileId = 0;
        return chunkTile;
    }
    return chunkDatabase.getChunk(chunk.x, chunk.y)->getTile(tile.x, tile.y);
}

void World::setTile(int x, int y, ChunkTile value) {
    Log::verbose(TAG, "Tile set at: " + std::to_string(x) + " " +
                      std::to_string(y) + " to: " + std::to_string(value.tileId));

    auto coordinates = getTileAndChunkCoordinates(x, y);
    auto tile = coordinates.first;
    auto chunk = coordinates.second;

    if(TileDatabase::get()[value.tileId].isSolid != TileDatabase::get()[getTile(x, y).tileId].isSolid) {
        chunkDatabase.getChunk(chunk.x, chunk.y)->setTile(tile.x, tile.y, value);

        if(getTile(x, y).node0 != 0) gameState->pathfinder.update_branch(x, y);
        if(getTile(x + 1, y).node0 != 0) gameState->pathfinder.update_branch(x + 1, y);
        if(getTile(x - 1, y).node0 != 0) gameState->pathfinder.update_branch(x - 1, y);
        if(getTile(x, y + 1).node0 != 0) gameState->pathfinder.update_branch(x, y + 1);
        if(getTile(x, y - 1).node0 != 0) gameState->pathfinder.update_branch(x, y - 1);
    }
    else {
        chunkDatabase.getChunk(chunk.x, chunk.y)->setTile(tile.x, tile.y, value);
    }
}

short World::mineTile(int x, int y) {
    ChunkTile tile = getTile(x, y);
    short id = tile.tileId;
    if(tile.tileId != 1) {
        tile.amount--;
        if(tile.amount <= 0) {
            tile.tileId = 1;
        }
        setTile(x, y, tile);
    }
    return id;
}

std::vector<std::shared_ptr<Entity>> World::getObjectsForUpdate() {
    std::vector<std::shared_ptr<Entity>> vector;
    for(auto& chunk : chunkDatabase.chunkCache) {
        for(auto& object : chunk.second.chunk->objects) {
            vector.push_back(object);
        }
    }
    return vector;
}

void World::addObject(std::shared_ptr<Entity> object) {
    auto* opc = object->getComponent<ObjectPositionComponent>();
    if(!opc) {
        Log::warn(TAG, "Object added without object position");
        object->addComponent<ObjectPositionComponent>(std::make_unique<ObjectPositionComponent>());
        opc = object->getComponent<ObjectPositionComponent>();
    }

    auto coordinates = getTileAndChunkCoordinates(opc->position.x, opc->position.y);
    auto tile = coordinates.first;
    auto chunk = coordinates.second;

    if(getTile(tile.x, tile.y).objectId != -1) {
        Log::info(TAG, "Added object in place of another. Aborting");
        return;
    }

    chunkDatabase.getChunk(chunk.x, chunk.y)->objects.push_back(object);
    auto id = static_cast<short>(chunkDatabase.getChunk(chunk.x, chunk.y)->objects.size() - 1);
    chunkDatabase.getChunk(chunk.x, chunk.y)->setTileObject(tile.x, tile.y, id);
}

void World::removeObject(int x, int y) {
    auto coordinates = getTileAndChunkCoordinates(x, y);
    auto tile = coordinates.first;
    auto chunk = coordinates.second;

    auto index = chunkDatabase.getChunk(chunk.x, chunk.y)->getTile(tile.x, tile.y).objectId;
    chunkDatabase.getChunk(chunk.x, chunk.y)->setTileObject(tile.x, tile.y, -1);
    chunkDatabase.getChunk(chunk.x, chunk.y)->objects[index] = std::make_shared<Entity>();
}

std::pair<sf::Vector2i, sf::Vector2i> World::getTileAndChunkCoordinates(int x, int y) {
    sf::Vector2i tile;
    sf::Vector2i chunk;
    chunk.x = static_cast<int>(std::floor(1.0f * x / Chunk::SIDE_LENGTH));
    chunk.y = static_cast<int>(std::floor(1.0f * y / Chunk::SIDE_LENGTH));
    auto mod = [](int a, int b) -> int {
        int ret = a % b;
        return ret >= 0 ? ret : ret + b;
    };
    tile.x = mod(x, Chunk::SIDE_LENGTH);
    tile.y = mod(y, Chunk::SIDE_LENGTH);
    return std::make_pair(tile, chunk);
}

void World::setTileNode(int x, int y, short node) {
    sf::Vector2i tile;
    sf::Vector2i chunk;
    chunk.x = static_cast<int>(floor(1.0f * x / Chunk::SIDE_LENGTH));
    chunk.y = static_cast<int>(floor(1.0f * y / Chunk::SIDE_LENGTH));
    auto mod = [](int a, int b) -> int {
        int ret = a % b;
        return ret >= 0 ? ret : ret + b;
    };
    tile.x = mod(x, Chunk::SIDE_LENGTH);
    tile.y = mod(y, Chunk::SIDE_LENGTH);
    chunkDatabase.getChunk(chunk.x, chunk.y)->setTileNode(tile.x, tile.y, node);
}

void World::setTilePath(int x, int y, short node0, short node1) {
    sf::Vector2i tile;
    sf::Vector2i chunk;
    chunk.x = static_cast<int>(floor(1.0f * x / Chunk::SIDE_LENGTH));
    chunk.y = static_cast<int>(floor(1.0f * y / Chunk::SIDE_LENGTH));
    auto mod = [](int a, int b) -> int {
        int ret = a % b;
        return ret >= 0 ? ret : ret + b;
    };
    tile.x = mod(x, Chunk::SIDE_LENGTH);
    tile.y = mod(y, Chunk::SIDE_LENGTH);
    chunkDatabase.getChunk(chunk.x, chunk.y)->setTilePath(tile.x, tile.y, node0, node1);
}

std::shared_ptr<Entity> World::getObject(int x, int y) {
    sf::Vector2i tile;
    sf::Vector2i chunk;
    chunk.x = static_cast<int>(floor(1.0f * x / Chunk::SIDE_LENGTH));
    chunk.y = static_cast<int>(floor(1.0f * y / Chunk::SIDE_LENGTH));
    auto mod = [](int a, int b) -> int {
        int ret = a % b;
        return ret >= 0 ? ret : ret + b;
    };
    tile.x = mod(x, Chunk::SIDE_LENGTH);
    tile.y = mod(y, Chunk::SIDE_LENGTH);
    if(!chunkDatabase.isChunkGenerated(chunk.x, chunk.y)) {
        return nullptr;
    }
    auto index = chunkDatabase.getChunk(chunk.x, chunk.y)->getTile(tile.x, tile.y).objectId;
    if(index == -1) {
        return nullptr;
    }
    return chunkDatabase.getChunk(chunk.x, chunk.y)->objects[index];
}

void World::setGameState(GameState* gameState) {
    this->gameState = gameState;
}

void World::clearCache() {
    chunkDatabase.saveCache(true);
    chunkDatabase.chunkCache.clear();

}
