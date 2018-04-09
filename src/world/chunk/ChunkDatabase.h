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

private:
    std::map<std::tuple<int, int>, CacheEntry>::iterator insertChunkIntoCache(int x, int y, std::unique_ptr<Chunk> chunk);
};


#endif //NONAME_CHUNKDATABASE_H
