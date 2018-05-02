#ifndef NONAME_CHUNKGENERATOR_H
#define NONAME_CHUNKGENERATOR_H


#include <map>
#include "Chunk.h"
#include "../../utils/PerlinNoise.h"
#include "../../utils/ThreadPool.h"

struct SecondaryMaterial{
    ChunkTile tile;
    float minDepth;
    float maxDepth;
    bool hasMinDepth;
    bool hasMaxDepth;
    bool isOre;
    float noiseMul;
};

struct Biome{
    int biomeId;
    ChunkTile primaryTile;
    ChunkTile surfaceTile;
    ChunkTile subSurfaceTile;
    std::vector<SecondaryMaterial> secondaryMaterials;
};

//! Chunk generator class
/*!
 * This class generates new chunks based on chunk's position and world seed.
 */
class ChunkGenerator {
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

    inline const float getWorldHeight(float x);

    inline const float getMaterialNoise(int materialType, float x, float y);
    inline const float getOreNoise(int oreType, float x, float y);

    Biome getBiome(float worldX);
    ChunkTile getTile(float worldX, float worldY, float worldHeight, Biome biome);

    static constexpr auto TAG = "ChunkGenerator";
public:
    //! Method used to generate new chunks
    /*!
     * generateChunk() is used to procedurally generate new chunks based on it's world cords and seed
     * @param x X coordinate of new chunk
     * @param y Y coordinate of new chunk
     * @return a newly created chunk
     */
    std::unique_ptr<Chunk> generateChunk(int x, int y);

    explicit ChunkGenerator(int _seed);
};


#endif //NONAME_CHUNKGENERATOR_H
