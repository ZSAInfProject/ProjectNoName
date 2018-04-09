#ifndef NONAME_CHUNKDATABASE_H
#define NONAME_CHUNKDATABASE_H


#include <unordered_map>
#include "Chunk.h"
#include "ChunkGenerator.h"

class ChunkDatabase {
    std::unordered_map<std::tuple<int, int>, Chunk> chunkCache;
    std::unique_ptr<ChunkGenerator> chunkGenerator;
public:
    Chunk* getChunk(int x, int y);

    explicit ChunkDatabase(std::unique_ptr<ChunkGenerator>);
};


#endif //NONAME_CHUNKDATABASE_H
