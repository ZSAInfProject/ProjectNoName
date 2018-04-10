#include <iostream>
#include <utility>

#include "Tile.h"

Tile::Tile(nlohmann::json json) {
    if (json.find("name") != json.end()) {
        name = json["name"].get<std::string>();
    }
    else {
        std::cout<<"No tile name specified. Setting to NoName"<<std::endl;
        name = "NoName";
    }

    if (json.find("terminal") != json.end()) {
        terminal_representation = json["terminal"].get<std::string>();
    }
    else {
        terminal_representation = " ";
    }

    if (json.find("hardness") != json.end()) {
        hardness = json["hardness"].get<float>();
    }
    else {
        hardness = 1;
    }
}

Tile::Tile(std::string name, float hardness, std::string terminal_representation) : name(std::move(name)),
                                                                                 hardness(hardness),
                                                                                 terminal_representation(std::move(
                                                                                         terminal_representation)) {}
