#ifndef NONAME_TILE_H
#define NONAME_TILE_H

#include <string>

#include "../utils/json.hpp"

//! Represents Tile - 1x1 game block
/*!
 * Tile can be created passing all fields (for debug) or using a JSON object
 */
class Tile {
public:
    explicit Tile(nlohmann::json json);
    Tile(std::string name, float hardness, std::string terminal_representation, int texture_x, int texture_y);
    Tile();


    //! Tile name showed to player
    std::string name;
    //! Hardness of a material. Correlates to time needed to destroy.
    float hardness;
    //! String used to represent tile in terminal for debug.
    std::string terminal_representation;
    //! Integer x coordinate of texture on texture map
    int texture_x;
    //! Integer y coordinate of texture on texture map
    int texture_y;

private:
    static constexpr auto TAG = "Tile";
};


#endif //NONAME_TILE_H