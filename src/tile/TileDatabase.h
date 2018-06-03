#ifndef NONAME_TILEDATABASE_H
#define NONAME_TILEDATABASE_H

#include <SFML/Graphics/Texture.hpp>
#include "../../deps/json.h"

#include "Tile.h"

//!order of categories, do not change it
enum categoriesEnum{
    unusable = 0,
    plainBlocks,
    ores
};

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
    //! Returns size of tiles
    int size();

    //! Texture map for tiles
    sf::Texture texture;
    std::shared_ptr<std::vector<std::string>> getCategoryNames();

private:
    std::vector<Tile> tiles;
    void loadTile(nlohmann::json json);
    //! order of category names corresponds to categoriesEnum's order
    const std::shared_ptr<std::vector<std::string>> categoryNames = std::make_shared<std::vector<std::string>>
            (std::vector<std::string> {"unusable", "plain blocks", "ores"});

    static constexpr auto TAG = "TileDatabase";
};


#endif //NONAME_TILEDATABASE_H
