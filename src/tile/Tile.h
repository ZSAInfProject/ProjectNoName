#ifndef NONAME_TILE_H
#define NONAME_TILE_H

#include <string>

#include "../utils/json.hpp"

class Tile {
public:
    explicit Tile(nlohmann::json json);
    Tile(std::string name, float hardness, std::string terminal_representation, int texture_x, int texture_y);
    Tile() = delete;


    std::string name;
    float hardness;
    std::string terminal_representation;
    int texture_x;
    int texture_y;
};


#endif //NONAME_TILE_H
