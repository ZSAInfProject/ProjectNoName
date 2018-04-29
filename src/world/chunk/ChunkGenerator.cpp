#include <cmath>
#include <future>
#include "ChunkGenerator.h"
#include "../../utils/Log.h"
#include "../../utils/PerlinNoise.h"

#define COPPER 5
#define COAL 6
#define IRON 7

const short ChunkGenerator::getOreType(float depth, float oreTypeNoise) {
    if(oreTypeNoise < 0.6){
        return COAL;
    }
    else if(oreTypeNoise<0.8){
        return IRON;
    }
    else{
        return COPPER;
    }
}

const chunkTile ChunkGenerator::getTile(float tileHeight, float height, float carvingNoise, float materialNoise, float oreNoise, float oreTypeNoise) {
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

        bool isOreVein = oreNoise > 0.84;
        if(isOreVein){
            material = getOreType(height-tileHeight, oreTypeNoise);
            amount = 10;
        }

        bool isSmallCave = carvingNoise < 0.2;
        bool isLargeCave = carvingNoise < 0.3 && tileHeight < height - 50;
        if(isSmallCave || isLargeCave){
            material = 1;
            amount = 1;
        }

        retTile = {material, amount};
    }
    return retTile;
}

std::unique_ptr<Chunk> ChunkGenerator::generateChunk(int x, int y) {
    std::array<chunkTile, Chunk::SIDE_LENGTH*Chunk::SIDE_LENGTH> tiles{};
    std::mutex tileMut;

    auto rowFunc = [this, x, y, &tiles, &tileMut](float tileX) {
        float worldX = tileX + x * Chunk::SIDE_LENGTH;

        auto worldHeightAtX =
                (float) heightNoise.noise(worldX / 100.5f) * 50.5f +
                (float) detailNoise.noise(worldX / 10.5) * (float) detailNoise2.noise0_1(worldX / 1000.5) * 10.5f;
        auto xOreTypeNoiseVal = (float) oreTypeNoise.noise(worldX / 2000.5);


        for (int tileY = 0; tileY < Chunk::SIDE_LENGTH; tileY++) {
            float worldY = tileY + y * Chunk::SIDE_LENGTH;

            int currentHeight = Chunk::SIDE_LENGTH * y + tileY;
            auto carvingNoiseVal = (float) carvingNoise.noise0_1(worldX / 20.5, worldY / 10.5);
            auto materialNoiseVal = (float) materialNoise.noise0_1(worldX / 10.5, worldY / 10.5);

            auto oreNoiseVal = (float) (1 - fabs(oreNoise.octaveNoise(worldX / 80.5, worldY / 80.5, 6)));
            oreNoiseVal *= 1 - oreNoise2.noise0_1(worldX / 1000.5, worldY / 1000.5) / 3;

            auto oreTypeNoiseVal = (float)oreTypeNoise2.noise0_1(tileY/2000.5)+xOreTypeNoiseVal;

            chunkTile tile = getTile(currentHeight, worldHeightAtX,
                                     carvingNoiseVal,
                                     materialNoiseVal,
                                     oreNoiseVal,
                                     oreTypeNoiseVal);
            tileMut.lock();
            tiles[Chunk::SIDE_LENGTH * tileY + tileX] = tile;
            tileMut.unlock();
        }
    };


    for(int tileX = 0; tileX < Chunk::SIDE_LENGTH; tileX++){
        auto job = [tileX, &rowFunc](){rowFunc(tileX);};
        threadPool.addJob(job);
    }

    threadPool.wait();

    Log::verbose(TAG, "Generated new chunk at X = " + std::to_string(x) + " Y = " + std::to_string(y));
    return std::make_unique<Chunk>(tiles);
}

ChunkGenerator::ChunkGenerator(int _seed) : threadPool(Chunk::SIDE_LENGTH) {
    seed = _seed;
    heightNoise.reseed(static_cast<uint32_t>(seed));
    detailNoise.reseed(static_cast<uint32_t>(seed * 2));
    detailNoise2.reseed(static_cast<uint32_t>(seed * 3));
    carvingNoise.reseed(static_cast<uint32_t>(seed * 4));
    materialNoise.reseed(static_cast<uint32_t>(seed * 5));
    oreNoise.reseed(static_cast<uint32_t>(seed * 6));
    oreNoise2.reseed(static_cast<uint32_t>(seed * 7));
    oreTypeNoise.reseed(static_cast<uint32_t>(seed * 8));
    oreTypeNoise2.reseed(static_cast<uint32_t>(seed * 9));
}


