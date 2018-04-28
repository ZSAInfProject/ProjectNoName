#ifndef NONAME_CHUNKGENERATOR_H
#define NONAME_CHUNKGENERATOR_H


#include "Chunk.h"
#include "../../utils/PerlinNoise.h"

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

    siv::PerlinNoise heightNoise;
    siv::PerlinNoise detailNoise;
    siv::PerlinNoise detailNoise2;
    siv::PerlinNoise carvingNoise;
    siv::PerlinNoise materialNoise;
    siv::PerlinNoise oreNoise;
    siv::PerlinNoise oreNoise2;
    siv::PerlinNoise oreTypeNoise;

    static constexpr auto TAG = "ChunkGenerator";

    const chunkTile getTile(float tileHeight, float height, float carvingNoise, float materialNoise, float oreNoise, float oreTypeNoise);
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
