#ifndef NONAME_CHUNK_H
#define NONAME_CHUNK_H


#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <vector>
#include <chrono>
#include <zconf.h>
#include "../../../deps/json.h"
#include "../../entity/Entity.h"
#include "../../utils/Buffer.h"


//! Representation of a tile in a chunk
class ChunkTile {

public:
    static constexpr auto TAG = "ChunkTile";

    //! Tile id pointing to data in TileDatabase
    short tileId;
    //! Amount of material in one tile
    uint amount;

    //! Id of object in tile. -1 if empty
    short objectId = -1;

    void serialize(Buffer&);

    ushort node = 0;

    ushort node0 = 0;
    ushort node1 = 0;

    ChunkTile() = default;
    ChunkTile(short tileId, uint amount, short objectId = -1);
    explicit ChunkTile(nlohmann::json);
};

class Chunk {
public:
    static constexpr auto TAG = "Chunk";

    //! Size of chunk in tiles
    static const int SIDE_LENGTH = 32;
    //! Size of tile in pixels
    static const int TILE_SIZE = 16;

    //! Vector of all objects inside a chunk
    std::vector<std::shared_ptr<Entity>> objects;

    //! Method used to render Chunk
    /*!
     * render() renders Chunk using given transformation parameters
     * @param window reference to target window
     * @param translation offset in world coordinates
     * @param scale scaling vector
     */
    void render(sf::RenderWindow& window, const sf::Vector2f& translation, const sf::Vector2f& scale);

    //! Method used to update chunk
    /*!
     * update() updates chunk and all of it's objects
     * @param deltaTime time since last update
     */
    void update(std::chrono::microseconds deltaTime);

    //! Method used for saving Chunk to files
    /*!
     * save() saves chunk and all of objects into files
     * @param path to files (root directory is chunk/) without extension
     */
    void save(const std::string& filename);

    //! Method used for loading Chunk from files
    /*!
     * load() loads chunk and all of objects from files
     * @param path to files (root directory is chunk/) without extension
     */
    bool load(const std::string& filename);

    //! Get tile from chunk
    /*!
     * getTile() return a struct representation of chunk located at given x and y coordinates
     * @param x X coordinate (in chunk coordinates)
     * @param y Y coordinate (in chunk coordinates)
     * @return chunkTile
     */
    ChunkTile getTile(int x, int y);

    //! Set tile to given ID
    /*!
     * setTile() sets a tile located at
     * given x and y coordinates to given value
     * @param x X coordinate (in chunk coordinates)
     * @param y Y coordinate (in chunk coordinates)
     * @param value a new value of the tile
     */
    void setTile(int x, int y, ChunkTile value);
    //! Set tile objectId at given coordinates to supplied objectId
    void setTileObject(int x, int y, short objectId);
    void setTileNode(int x, int y, ushort node);
    void setTilePath(int x, int y, ushort node0, ushort node1);

    Chunk();
    Chunk(Chunk const&) = delete;
    void operator=(Chunk const&) = delete;
    explicit Chunk(const std::array<ChunkTile, SIDE_LENGTH * SIDE_LENGTH>& tiles);

private:
    std::array<ChunkTile, SIDE_LENGTH * SIDE_LENGTH> tiles;

    void generateVertices();
    void changeQuad(int x, int y);
    void updateQuads();
    sf::VertexArray vertices = sf::VertexArray(sf::Quads, SIDE_LENGTH * SIDE_LENGTH * 4);

    //! Returns true if tile is inside a chunk. Warns if otherwise
    bool isInsideBoundaries(int x, int y);

};


#endif //NONAME_CHUNK_H
