#ifndef NONAME_TILE_H
#define NONAME_TILE_H

#include <string>

#include "../utils/json.hpp"

class Tile {
public:
    explicit Tile(nlohmann::json json);
    Tile(int id, std::string name, float hardness, std::string terminal_representation);
    Tile() = delete;

    Tile(Tile const&) = delete;
    void operator= (Tile const&) = delete;

    int id;
    std::string name;
    float hardness;
    std::string terminal_representation;
};


#endif //NONAME_TILE_H
