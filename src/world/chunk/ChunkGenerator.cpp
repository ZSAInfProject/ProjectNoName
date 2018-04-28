#include <cmath>
#include "ChunkGenerator.h"
#include "../../utils/Log.h"
#include "../../utils/PerlinNoise.h"

chunkTile ChunkGenerator::getTile(float tileHeight, float height, float carvingNoise, float materialNoise) {
    chunkTile retTile = {2,1};
    bool isAboveGround = tileHeight > height;
    if(isAboveGround){
        retTile = {1, 1};
    }
    else{
        uint amount = 5;
        short material = 2;
        if(height - tileHeight < 1){
            //Surface grass
            amount = 1;
            material = 4;
        }
        else if(height - tileHeight < materialNoise*4+1){
            //Surface dirt
            amount = 1;
            material = 3;
        }
        else if(materialNoise < 0.3){
            //Underground dirt
            amount = 2;
            material = 3;
        }

        bool isSmallCave = carvingNoise < 0.3;
        bool isLargeCave = carvingNoise < 0.4 && tileHeight < height - 50;
        if(isSmallCave || isLargeCave){
            material = 1;
            amount = 1;
        }

        retTile = {material, amount};
    }
    return retTile;
}

std::unique_ptr<Chunk> ChunkGenerator::generateChunk(int x, int y) {
    siv::PerlinNoise heightNoise((uint32_t)seed);
    siv::PerlinNoise detailNoise((uint32_t)(seed / 2));
    siv::PerlinNoise detailNoise2((uint32_t)(seed / 3));
    siv::PerlinNoise carvingNoise((uint32_t)seed*2);
    siv::PerlinNoise materialNoise((uint32_t)(seed * 3));

    std::array<chunkTile, Chunk::SIDE_LENGTH*Chunk::SIDE_LENGTH> tiles{};
    tiles.fill({2,1});
    for(int tileX = 0; tileX < Chunk::SIDE_LENGTH; tileX++){
        float worldX = tileX+x*Chunk::SIDE_LENGTH;

        auto worldHeightAtX =
                (float)heightNoise.noise(worldX/100.5f)*50.5f +
                (float)detailNoise.noise(worldX/10.5)*(float)detailNoise2.noise0_1(worldX/1000.5)*10.5f;

        for(int tileY = 0; tileY < Chunk::SIDE_LENGTH; tileY++){
            float worldY = tileY+y*Chunk::SIDE_LENGTH;

            int currentHeight = Chunk::SIDE_LENGTH*y + tileY;
            auto carvingNoiseVal = (float)carvingNoise.noise0_1(worldX/20.5, worldY/10.5);
            auto materialNoiseVal = (float)materialNoise.noise0_1(worldX/10.5, worldY/10.5);

            tiles[Chunk::SIDE_LENGTH*tileY+tileX] = getTile(currentHeight, worldHeightAtX,
                                                                carvingNoiseVal,
                                                                materialNoiseVal);
        }
    }
    Log::verbose(TAG, "Generated new chunk at X = " + std::to_string(x) + " Y = " + std::to_string(y));
    return std::make_unique<Chunk>(tiles);
}

ChunkGenerator::ChunkGenerator(int _seed) {
    seed = _seed;
}

