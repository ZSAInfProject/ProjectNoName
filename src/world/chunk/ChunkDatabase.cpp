#include "ChunkDatabase.h"

ChunkDatabase::ChunkDatabase(std::unique_ptr<ChunkGenerator> _chunkGenerator) {
    chunkGenerator = std::move(_chunkGenerator);
}

Chunk *ChunkDatabase::getChunk(int x, int y) {
    //Try to load chunk from cache
    auto chunkIt = chunkCache.find(std::make_tuple(x, y));
    if(chunkIt != chunkCache.end()){
        chunkIt->second.lastHit = std::chrono::system_clock::now();
    }
    else {
        //If chunk is not in cache load it from file
        //TODO: load from file

        //if chunk does not exist generate one
        chunkIt = insertChunkIntoCache(x, y, chunkGenerator->generateChunk(x, y));
    }
    return chunkIt->second.chunk.get();
}
std::map<std::tuple<int, int>, CacheEntry>::iterator ChunkDatabase::insertChunkIntoCache(int x, int y, std::unique_ptr<Chunk> chunk) {
    return chunkCache.insert(std::pair<std::tuple<int, int>, CacheEntry>(
            std::make_tuple(x, y), CacheEntry{std::move(chunk), std::chrono::system_clock::now()})).first;
}
