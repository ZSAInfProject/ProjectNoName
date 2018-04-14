#ifndef NONAME_CHUNKGENERATOR_H
#define NONAME_CHUNKGENERATOR_H


#include "Chunk.h"

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
public:
    //! Return chunk generated for given coordinates.
    std::unique_ptr<Chunk> generateChunk(int x, int y);

    explicit ChunkGenerator(int _seed);
};


#endif //NONAME_CHUNKGENERATOR_H
