#include <iostream>
#include <utility>

#include "Tile.h"
#include "../utils/Log.h"

Tile::Tile(nlohmann::json json) {
    if (json.find("name") != json.end()) {
        name = json["name"].get<std::string>();
    }
    else {
        Log::warn(TAG, "No name specified for tile");
        name = "Default";
    }

    if (json.find("terminal") != json.end()) {
        terminal_representation = json["terminal"].get<std::string>();
    }
    else {
        Log::debug(TAG, "No terminal representation for tile specified");
        terminal_representation = "?";
    }

    if (json.find("hardness") != json.end()) {
        hardness = json["hardness"].get<float>();
    }
    else {
        Log::warn(TAG, "No hardness for tile specified");
        hardness = 1;
    }

    if (json.find("texture") != json.end()) {
        texture_x = json["texture"][0].get<int>();
        texture_y = json["texture"][1].get<int>();
    }
    else {
        Log::warn(TAG, "No texture coordinates for tile specified");
        texture_x = 0;
        texture_y = 0;
    }

    if (json.find("drop") != json.end()) {
        std::string type = json["drop"]["type"];
        drop.type = getResourceType(type);
        drop.amount = json["drop"]["amount"];
    }
    else{
        drop.type = resourceType::unknown;
        drop.amount = 0;
    }

    if (json.find("isSolid") != json.end()) {
        isSolid = json["isSolid"].get<bool>();
    }
    else {
        Log::warn(TAG, "No isSolid for tile specified");
        isSolid = true;
    }
}

Tile::Tile(std::string name, float hardness, std::string terminal_representation, int texture_x, int texture_y, bool isSolid)
        : name(std::move(name)), hardness(hardness), terminal_representation(std::move(terminal_representation)), texture_x(texture_x), texture_y(texture_y), isSolid(isSolid) {}

Tile::Tile() {
    name = "Default";
    hardness = 1;
    terminal_representation = "?";
    texture_x = 0;
    texture_y = 0;
    isSolid = true;
}
