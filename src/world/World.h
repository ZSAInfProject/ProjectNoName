#ifndef NONAME_WORLD_H
#define NONAME_WORLD_H


#include <vector>
#include "chunk/ChunkDatabase.h"
#include "../tile/Tile.h"

class GameState;

class World {

public:
    static constexpr auto TAG = "World";

    //! Renders all chunks visible by the camera
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

    //! Adds object to world
    void addObject(std::shared_ptr<Entity> object);

    //! Removes object at supplied coordinates
    void removeObject(int x, int y);

    std::shared_ptr<Entity> getObject(int x, int y);

    //! Returns objects to be processed by systems
    std::vector<std::shared_ptr<Entity>> getObjectsForUpdate();
    void setTileNode(int x, int y, short node);
    void setTilePath(int x, int y, short node0, short node1);

    ChunkTile getLoadedTile(int x, int y);

    void clearCache();

    void setGameState(GameState* gameState);

    explicit World(int seed);

private:
    ChunkDatabase chunkDatabase;

    GameState* gameState;

    //! Returns coordinates of tile (0,0 at bottom-left chunk corner) and chunk coordinates
    std::pair<sf::Vector2i, sf::Vector2i> getTileAndChunkCoordinates(int x, int y);
};


#endif //NONAME_WORLD_H
