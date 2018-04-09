#include "ChunkDatabase.h"

ChunkDatabase::ChunkDatabase(std::unique_ptr<ChunkGenerator> _chunkGenerator) {
    chunkGenerator = std::move(_chunkGenerator);
}

Chunk *ChunkDatabase::getChunk(int x, int y) {
    return nullptr;
}
