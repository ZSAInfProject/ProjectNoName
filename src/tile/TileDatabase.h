#ifndef NONAME_TILEDATABASE_H
#define NONAME_TILEDATABASE_H

#include <SFML/Graphics/Texture.hpp>
#include "../utils/json.hpp"

#include "Tile.h"

//! Singleton mapping ids to tiles
/*!
 * Contains vector of reference tiles. Loaded from json file using loadTiles().
 */
class TileDatabase {

public:
    static TileDatabase& get() {
        static TileDatabase instance;
        return instance;
    }

    TileDatabase(TileDatabase const&) = delete;
    void operator= (TileDatabase const&) = delete;

private:
    TileDatabase() = default;

public:
    //! Loads tiles from JSON file.
    /*!
     * File should be a JSON vector of tiles.
     * @param file - path to file (root directory is res/)
     */
    void loadTiles(std::string file);
    //! Loads texture map for tiles from file
    void loadTexture(std::string file);
    Tile& operator[] (int index);

    //! Texture map for tiles
    sf::Texture texture;

private:
    std::vector<Tile> tiles;
    void loadTile(nlohmann::json json);

    static constexpr auto TAG = "TileDatabase";
};


#endif //NONAME_TILEDATABASE_H
