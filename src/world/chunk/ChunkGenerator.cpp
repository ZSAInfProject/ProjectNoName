#include "ChunkGenerator.h"

std::unique_ptr<Chunk> ChunkGenerator::generateChunk(int x, int y) {
    std::array<int, Chunk::SIDE_LENGTH*Chunk::SIDE_LENGTH> tiles{};
    tiles.fill(2);
    for(int x = 0; x < Chunk::SIDE_LENGTH; x++){
        for(int y = 0; y < Chunk::SIDE_LENGTH; y++){
            if(x == 0 || x == Chunk::SIDE_LENGTH-1 || y == 0 || y == Chunk::SIDE_LENGTH-1)
                tiles[Chunk::SIDE_LENGTH*y+x] = 0;
        }
    }
    return std::make_unique<Chunk>(tiles);
}

ChunkGenerator::ChunkGenerator(int _seed) {
    seed = _seed;
}
