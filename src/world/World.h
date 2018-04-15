#ifndef NONAME_WORLD_H
#define NONAME_WORLD_H


#include "chunk/ChunkDatabase.h"
#include "../tile/Tile.h"

class World {
    ChunkDatabase chunkDatabase;
public:
    void render(sf::RenderWindow& window, sf::View camera);

    //!Gets tile id using global coordinates of the world
    int getTile(int x, int y);
    //!Sets tile id using global coordinates of the world
    void setTile(int x, int y, int id);

    explicit World(int seed);

    static constexpr auto TAG = "World";
};


#endif //NONAME_WORLD_H
