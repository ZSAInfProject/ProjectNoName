#include "TileDatabase.h"

#include <fstream>
#include <utility>
#include <iostream>
#include "Tile.h"
#include "../utils/json.hpp"

void TileDatabase::loadTiles(std::string file) {
    tiles.clear();
    tiles.emplace_back("Default", 1.0, "X", 0, 0);
    std::ifstream ifs("res/" + file);
    nlohmann::json j = nlohmann::json::parse(ifs);
    std::vector<nlohmann::json> vect = j;
    for (const auto &i : vect) {
        loadTile(i);
    }
}

void TileDatabase::loadTile(nlohmann::json json) {
    if (json.find("id") == json.end()) {
        std::cout<<"Tile in file does not hove an id. Exiting"<<std::endl;
        exit(1);
    }
    else {
        int id = json["id"];
        Tile tile(std::move(json));
        if (tiles.size() < id + 1) {
            tiles.resize(id + 1, Tile("Default", 1.0, "X", 0, 0));
            tiles[id] = tile;
        }
    }
}

Tile &TileDatabase::operator[](int index){
    if (index >= tiles.size()) {
        std::cout<<"Id too big. Returning default tile"<<std::endl;
        return tiles[0];
    }
    else {
        return tiles[index];
    }
}

void TileDatabase::loadTexture(std::string file) {
    if (!texture.loadFromFile("res/" + file)) {
        std::cout<<"Could not load textures"<<"\n";
    }
}
