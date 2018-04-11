#include "World.h"

World::World(int seed) : chunkDatabase(std::make_unique<ChunkGenerator>(seed)){

}
