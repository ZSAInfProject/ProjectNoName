#ifndef NONAME_WORLD_H
#define NONAME_WORLD_H


#include "chunk/ChunkDatabase.h"

class World {
    ChunkDatabase chunkDatabase;
public:
    void render(sf::RenderWindow& window, sf::View camera);

    explicit World(int seed);
};


#endif //NONAME_WORLD_H
