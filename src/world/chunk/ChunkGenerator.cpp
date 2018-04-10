#include "ChunkGenerator.h"

std::unique_ptr<Chunk> ChunkGenerator::generateChunk(int x, int y) {
    std::array<int, Chunk::SIDE_LENGTH*Chunk::SIDE_LENGTH> tiles{};
    tiles.fill(2);
    return std::make_unique<Chunk>(tiles);
}

ChunkGenerator::ChunkGenerator(int _seed) {
    seed = _seed;
}
