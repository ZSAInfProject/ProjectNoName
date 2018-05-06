#ifndef NONAME_WORLD_H
#define NONAME_WORLD_H


#include "chunk/ChunkDatabase.h"
#include "../tile/Tile.h"

class World {
    ChunkDatabase chunkDatabase;
public:
    void render(sf::View camera);

    //!Gets chunkTile using global coordinates of the world
    ChunkTile getTile(int x, int y);
    //!Sets tile to given value using global coordinates of the world
    void setTile(int x, int y, ChunkTile value);

    //!Mines given tile
    /*!
     * Mines tile at given coord
     * @param x X world coordinate
     * @param y Y world coordinate
     * @return tile id BEFORE mining
     */
    short mineTile(int x, int y);

    explicit World(int seed);

    static constexpr auto TAG = "World";
};


#endif //NONAME_WORLD_H
