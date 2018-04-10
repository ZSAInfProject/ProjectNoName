#ifndef NONAME_TILEDATABASE_H
#define NONAME_TILEDATABASE_H

#include "../utils/json.hpp"

#include "Tile.h"

class TileDatabase {
public:
    void loadTiles(std::string file);
    Tile& operator[] (int index);

private:
    std::vector<Tile> tiles;
    void loadTile(nlohmann::json json);
};


#endif //NONAME_TILEDATABASE_H
