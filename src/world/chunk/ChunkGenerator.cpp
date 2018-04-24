#include <cmath>
#include "ChunkGenerator.h"
#include "../../utils/Log.h"
#include "../../utils/PerlinNoise.h"

int ChunkGenerator::getTileType(float tileHeight, float height, float carvingNoise, float materialNoise) {
    if(tileHeight > height){
        return 1;
    }
    else{
        //Basic ground material
        int material = 2;
        if(height - tileHeight < 1){
            //Surface grass
            material = 4;
        }
        else if(height - tileHeight < materialNoise*4+1){
            //Surface dirt
            material = 3;
        }
        else if(materialNoise < 0.3){
            //Underground dirt
            material = 3;
        }

        //Small caves and overhangs
        if(carvingNoise < 0.3){
            return 1;
        }

        //Large caves
        if(tileHeight < height-50 && carvingNoise < 0.4){
            return 1;
        }

        return material;
    }
}

std::unique_ptr<Chunk> ChunkGenerator::generateChunk(int x, int y) {
    siv::PerlinNoise heightNoise((uint32_t)seed);
    siv::PerlinNoise detailNoise((uint32_t)(seed / 2));
    siv::PerlinNoise detailNoise2((uint32_t)(seed / 3));
    siv::PerlinNoise carvingNoise((uint32_t)seed*2);
    siv::PerlinNoise materialNoise((uint32_t)(seed * 3));

    std::array<int, Chunk::SIDE_LENGTH*Chunk::SIDE_LENGTH> tiles{};
    tiles.fill(2);
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

            tiles[Chunk::SIDE_LENGTH*tileY+tileX] = getTileType(currentHeight, worldHeightAtX,
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

