#include "Chunk.h"

#include <fstream>
#include <iostream>

void Chunk::render(sf::RenderWindow) {

}

void Chunk::update(std::chrono::microseconds deltaTime) {

}

Chunk::Chunk() = default;

Chunk::Chunk(const std::array<int, SIDE_LENGTH*SIDE_LENGTH> &_tiles) {
    tiles = _tiles;
}

int Chunk::getTile(int x, int y) {
    return tiles[y*SIDE_LENGTH + x];
}

void Chunk::setTile(int x, int y, int value) {
    tiles[y*SIDE_LENGTH + x] = value;
}

void Chunk::save(const std::string &fileName) {
    //TODO: save objects to another file.

    //Save tiles
    std::ofstream tileData(fileName+".td");
    for(int tile : tiles) {
        tileData.write(reinterpret_cast<const char *>(&tile), sizeof(int));
        if(tileData.fail()){
            std::cout<<"Saving to file failed!\n";
        }
    }
    tileData.close();
}

bool Chunk::load(const std::string &filename) {
    std::ifstream tileData(filename+".td");
    if(!tileData.is_open()){
        tileData.close();
        return false;
    }
    //TODO: load objects

    //Load tiles
    for(int& tile : tiles){
        tileData.read(reinterpret_cast<char *>(&tile), sizeof(int));
        if(tileData.fail()){
            std::cout<<"Reading from file failed!\n";
        }
    }
    tileData.close();
    return true;
}


