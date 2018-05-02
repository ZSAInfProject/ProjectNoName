#include <cmath>
#include "ChunkGenerator.h"
#include "../../utils/Log.h"
#include "../../utils/PerlinNoise.h"
#include "../../utils/json.hpp"

const float ChunkGenerator::getWorldHeight(float x) {
    return static_cast<const float>(heightNoise.noise(x / 100.5f) * 50.5f +
                 detailNoise.noise(x / 10.5) *  detailNoise2.noise0_1(x / 1000.5) * 10.5f);
}

const float ChunkGenerator::getMaterialNoise(int materialType, float x, float y) {
    std::unique_lock<std::mutex> lock(mappedNoisesMut);
    auto it = materialNoises.find(materialType);
    if(it == materialNoises.end()){
        it = materialNoises.emplace(std::pair<int,siv::PerlinNoise>(materialType,
                                                               siv::PerlinNoise((uint32_t)seed*10+materialType))).first;
    }
    return static_cast<const float>(it->second.noise0_1(x/10.5, y/10.5));
}

const float ChunkGenerator::getOreNoise(int oreType, float x, float y) {
    std::unique_lock<std::mutex> lock(mappedNoisesMut);
    auto it = oreNoises.find(oreType);
    if(it == oreNoises.end()){
        it = oreNoises.emplace(std::pair<int,siv::PerlinNoise>(oreType,
                                                               siv::PerlinNoise((uint32_t)seed*20+oreType))).first;
    }
    auto oreNoiseVal = (float) (1 - fabs(it->second.octaveNoise(x / 80.5, y / 80.5, 6)));
    oreNoiseVal *= 1 - it->second.noise0_1(x / 1000.5, y / 1000.5) / 3;
    return oreNoiseVal;
}

Biome ChunkGenerator::getBiome(float worldX) {
    return biomes.front();
}

ChunkTile ChunkGenerator::getTile(float worldX, float worldY, float worldHeight, Biome biome) {
    if(worldY >= worldHeight){
        //Above ground
        return {1,1};
    }
    float depth = worldHeight-worldY;
    for(const auto& material : biome.secondaryMaterials){
        float depthMul = 1;
        if(material.hasMaxDepth){
            if(depth > material.maxDepth)
                continue;
            if(material.maxDepth - depth < 20){
                depthMul = (material.maxDepth-depth)/20;
            }
        }
        if(material.hasMinDepth){
            if(depth < material.minDepth)
                continue;
            if(depth - material.minDepth < 20){
                depthMul = (depth-material.minDepth)/20;
            }

        }
        float noiseVal = material.isOre ?
                         getOreNoise(material.tile.tileId, worldX, worldY) :
                         getMaterialNoise(material.tile.tileId, worldX, worldY);
        noiseVal *= material.noiseMul;
        noiseVal *= depthMul;
        if(noiseVal > 0.8)
            return material.tile;
    }

    if(worldHeight - worldY < 1){
        return biome.surfaceTile;
    }
    if(worldHeight - worldY < 4){
        return biome.subSurfaceTile;
    }

    return biome.primaryTile;
}

std::unique_ptr<Chunk> ChunkGenerator::generateChunk(int x, int y) {
    std::array<ChunkTile, Chunk::SIDE_LENGTH*Chunk::SIDE_LENGTH> tiles{};
    std::mutex tileMut;

    auto rowFunc = [this, x, y, &tiles, &tileMut](float tileX) {
        float worldX = tileX + x * Chunk::SIDE_LENGTH;
        Biome biome = getBiome(worldX);
        float worldHeight = getWorldHeight(worldX);

        for (int tileY = 0; tileY < Chunk::SIDE_LENGTH; tileY++) {
            float worldY = tileY + y * Chunk::SIDE_LENGTH;

            ChunkTile tile = getTile(worldX, worldY, worldHeight, biome);

            tileMut.lock();
            tiles[Chunk::SIDE_LENGTH * tileY + tileX] = tile;
            tileMut.unlock();
        }
    };

    for(int tileX = 0; tileX < Chunk::SIDE_LENGTH; tileX++){
        threadPool.addJob([tileX, &rowFunc](){rowFunc(tileX);});
    }

    threadPool.wait();

    Log::verbose(TAG, "Generated new chunk at X = " + std::to_string(x) + " Y = " + std::to_string(y));
    return std::make_unique<Chunk>(tiles);
}

ChunkGenerator::ChunkGenerator(int _seed) : threadPool(Chunk::SIDE_LENGTH) {
    seed = _seed;
    loadBiomes("biomes.json");
    biomeNoise.reseed(static_cast<uint32_t>(seed / 2));
    heightNoise.reseed(static_cast<uint32_t>(seed));
    detailNoise.reseed(static_cast<uint32_t>(seed * 2));
    detailNoise2.reseed(static_cast<uint32_t>(seed * 3));
}

void ChunkGenerator::loadBiomes(std::string filename) {
    std::ifstream biomesData("res/"+filename);
    if(biomesData.is_open()) {
        nlohmann::json json = nlohmann::json::parse(biomesData);
        std::vector<nlohmann::json> biomes = json;
        for(const auto& biome : biomes){
            loadBiome(biome);
        }
    }
    else{
        Log::error(TAG, "Failed to open res/"+filename);
    }
}

void ChunkGenerator::loadBiome(nlohmann::json json) {
    biomes.emplace_back(json);
}


Biome::Biome(nlohmann::json json) {
    if(json.find("id") != json.end()){
        biomeId = json["id"].get<int>();
    }

    if(json.find("primaryTile") != json.end()){
        primaryTile = ChunkTile(json["primaryTile"]);
    }

    if(json.find("surfaceTile") != json.end()){
        surfaceTile = ChunkTile(json["surfaceTile"]);
    }

    if(json.find("subSurfaceTile") != json.end()){
        subSurfaceTile = ChunkTile(json["subSurfaceTile"]);
    }

    if(json.find("secondaryTiles") != json.end()){
        nlohmann::json secondaries = json["secondaryTiles"];
        for (auto& secondary : secondaries) {
            secondaryMaterials.emplace_back(secondary);
        }
    }

}

SecondaryMaterial::SecondaryMaterial(nlohmann::json json) {
    if(json.find("tile") != json.end()){
        tile = ChunkTile(json["tile"]);
    }

    if(json.find("minDepth") != json.end()){
        minDepth = json["minDepth"].get<float>();
    }

    if(json.find("maxDepth") != json.end()){
        maxDepth = json["maxDepth"].get<float>();
    }

    if(json.find("hasMaxDepth") != json.end()){
        hasMaxDepth = json["hasMaxDepth"].get<bool>();
    }

    if(json.find("hasMinDepth") != json.end()){
        hasMinDepth = json["hasMinDepth"].get<bool>();
    }

    if(json.find("isOre") != json.end()){
        isOre = json["isOre"].get<bool>();
    }

    if(json.find("noiseMul") != json.end()){
        noiseMul = json["noiseMul"].get<float>();
    }
}
