#ifndef NONAME_CHUNKGENERATOR_H
#define NONAME_CHUNKGENERATOR_H


#include "Chunk.h"

class ChunkGenerator {
    int seed;
public:
    Chunk generateChunk(int x, int y);

    explicit ChunkGenerator(int _seed);
};


#endif //NONAME_CHUNKGENERATOR_H
