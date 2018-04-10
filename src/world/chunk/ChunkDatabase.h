#ifndef NONAME_CHUNKDATABASE_H
#define NONAME_CHUNKDATABASE_H


#include <map>
#include "Chunk.h"
#include "ChunkGenerator.h"

struct CacheEntry{
    std::unique_ptr<Chunk> chunk;
    std::chrono::system_clock::time_point lastHit;
};

class ChunkDatabase {
    std::map<std::tuple<int, int>, CacheEntry> chunkCache;
    std::unique_ptr<ChunkGenerator> chunkGenerator;
public:
    Chunk* getChunk(int x, int y);

    explicit ChunkDatabase(std::unique_ptr<ChunkGenerator>);

    //Returns string representation of cache (what chunks are loaded) x and y are used to set top-left corner chunk coordinates
    std::string cacheDebug(int x, int y);
private:
    std::map<std::tuple<int, int>, CacheEntry>::iterator insertChunkIntoCache(int x, int y, std::unique_ptr<Chunk> chunk);

    std::chrono::system_clock::time_point lastCacheCleanUp = std::chrono::system_clock::now();
    std::chrono::microseconds cleanUpPeriod = std::chrono::microseconds(50000);
    std::chrono::microseconds minimumCacheTime = std::chrono::microseconds(100000);
    const int maxNumOfChunksBeforeCleanup = 10;
    void cleanUpCache();

    std::string getChunkFilename(int x, int y);
};


#endif //NONAME_CHUNKDATABASE_H
