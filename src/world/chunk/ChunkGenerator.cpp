#include <cmath>
#include "ChunkGenerator.h"
#include "../../utils/Log.h"

std::unique_ptr<Chunk> ChunkGenerator::generateChunk(int x, int y) {
    std::array<int, Chunk::SIDE_LENGTH*Chunk::SIDE_LENGTH> tiles{};
    tiles.fill(2);
    for(int tileX = 0; tileX < Chunk::SIDE_LENGTH; tileX++){
        //Debug, will be replaced by some noise
        auto worldHeightAtX = static_cast<int>(5*sin((tileX + Chunk::SIDE_LENGTH * x * 1.0) / 16 * M_PI));
        for(int tileY = 0; tileY < Chunk::SIDE_LENGTH; tileY++){
            int currentHeight = Chunk::SIDE_LENGTH*y + tileY;
            if(currentHeight > worldHeightAtX)
                tiles[Chunk::SIDE_LENGTH*tileY+tileX] = 0;
        }
    }
    Log::verbose(TAG, "Generated new chunk");
    return std::make_unique<Chunk>(tiles);
}

ChunkGenerator::ChunkGenerator(int _seed) {
    seed = _seed;
}
