#include "TileDatabase.h"

#include <fstream>
#include <utility>
#include <iostream>
#include "Tile.h"
#include "../../deps/json.h"
#include "../utils/Log.h"

void TileDatabase::loadTiles(std::string file) {
    tiles.clear();
    tiles.emplace_back();
    std::ifstream ifs("res/" + file);
    if (ifs.is_open()) {
        nlohmann::json j = nlohmann::json::parse(ifs);
        std::vector<nlohmann::json> vect = j;
        for (const auto &i : vect) {
            loadTile(i);
        }
    } else {
        Log::error(TAG, "Could not open Tiles file");
    }
}

void TileDatabase::loadTile(nlohmann::json json) {
    if (json.find("id") == json.end()) {
        Log::error(TAG, "Tile in file does not have an id. Tile not added");
    }
    else {
        int id = json["id"];
        Tile tile(std::move(json));
        if (tiles.size() < id + 1) {
            tiles.resize(id + 1, Tile());
            tiles[id] = tile;
        }
    }
}

Tile &TileDatabase::operator[](int index){
    if (index >= tiles.size()) {
        Log::debug(TAG, "Id too big. Returning default tile");
        return tiles[0];
    }
    else {
        return tiles[index];
    }
}

void TileDatabase::loadTexture(std::string file) {
    if (!texture.loadFromFile("res/" + file)) {
        Log::warn(TAG, "Could not load textures");
    }
}
