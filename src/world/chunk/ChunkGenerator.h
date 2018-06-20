#ifndef NONAME_CHUNKGENERATOR_H
#define NONAME_CHUNKGENERATOR_H


#include <map>
#include "Chunk.h"
#include "../../../deps/PerlinNoise.h"
#include "../../utils/ThreadPool.h"
#include "../../../deps/json.h"

class SecondaryMaterial {
public:
    static constexpr auto TAG = "SecondaryMaterial";

    ChunkTile tile;
    float minDepth;
    float maxDepth;
    bool hasMinDepth;
    bool hasMaxDepth;
    bool isOre;
    float noiseMul;
    explicit SecondaryMaterial(nlohmann::json);
};

class Biome {
public:
    static constexpr auto TAG = "Biome";

    int biomeId;
    ChunkTile primaryTile;
    ChunkTile surfaceTile;
    ChunkTile subSurfaceTile;
    std::vector<SecondaryMaterial> secondaryMaterials;
    explicit Biome(nlohmann::json);
};

//! Chunk generator class
/*!
 * This class generates new chunks based on chunk's position and world seed.
 */
class ChunkGenerator {

public:
    static constexpr auto TAG = "ChunkGenerator";

    //! Method used to generate new chunks
    /*!
     * generateChunk() is used to procedurally generate new chunks based on it's world cords and seed
     * @param x X coordinate of new chunk
     * @param y Y coordinate of new chunk
     * @return a newly created chunk
     */
    std::unique_ptr<Chunk> generateChunk(int x, int y);

    //! Loads biomes from file using supplied file name
    void loadBiomes(std::string filename);

    explicit ChunkGenerator(int seed);

private:
    //! World seed
    /*!
     * generateChunk() will use this value for chunk generation
     */
    int seed;

    siv::PerlinNoise biomeNoise;
    siv::PerlinNoise heightNoise;
    siv::PerlinNoise detailNoise;
    siv::PerlinNoise detailNoise2;

    ThreadPool threadPool;

    std::mutex mappedNoisesMut;
    std::map<int, siv::PerlinNoise> oreNoises;
    std::map<int, siv::PerlinNoise> materialNoises;

    std::vector<Biome> biomes;

    //! Returns terrain height at x coordinate
    inline float getWorldHeight(float x);
    //! Returns noise value for selected material at specified coordinates
    inline float getMaterialNoise(int materialType, float x, float y);
    //! Returns noise value for selected ore at specified coordinates
    inline float getOreNoise(int oreType, float x, float y);

    //! Returns biome at specified x coordinate
    Biome getBiome(float worldX);
    //! Returns generated tile at specified coordinate using biome and height
    ChunkTile getTile(float worldX, float worldY, float worldHeight, Biome biome);

    //! Loads biome definition from json
    void loadBiome(nlohmann::json json);
};


#endif //NONAME_CHUNKGENERATOR_H
