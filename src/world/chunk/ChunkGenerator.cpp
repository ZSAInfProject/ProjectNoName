#include "ChunkGenerator.h"

std::unique_ptr<Chunk> ChunkGenerator::generateChunk(int x, int y) {
    return std::make_unique<Chunk>();
}

ChunkGenerator::ChunkGenerator(int _seed) {
    seed = _seed;
}
