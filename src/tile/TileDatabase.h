#ifndef NONAME_TILEDATABASE_H
#define NONAME_TILEDATABASE_H

#include <SFML/Graphics/Texture.hpp>
#include "../utils/json.hpp"

#include "Tile.h"

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
    void loadTiles(std::string file);
    void loadTexture(std::string file);
    Tile& operator[] (int index);

    sf::Texture texture;

private:
    std::vector<Tile> tiles;
    void loadTile(nlohmann::json json);
};


#endif //NONAME_TILEDATABASE_H
