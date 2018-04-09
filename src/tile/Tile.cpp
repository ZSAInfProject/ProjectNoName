#include <iostream>
#include <utility>

#include "Tile.h"

Tile::Tile(nlohmann::json json) {
    if (json.find("id") != json.end()) {
        id = json["id"].get<int>();
    }
    else {
        std::cout<<"No tile id specified in JSON"<<std::endl;
        exit(1);
    }

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

Tile::Tile(int id, std::string name, float hardness, std::string terminal_representation) : id(id), name(std::move(name)),
                                                                                 hardness(hardness),
                                                                                 terminal_representation(std::move(
                                                                                         terminal_representation)) {}
