#include "Chunk.h"
#include "../../tile/TileDatabase.h"

#include <fstream>
#include <iostream>

void Chunk::render(sf::RenderWindow& window, const sf::Vector2f& translation, const sf::Vector2f& scale) {
    sf::RenderStates states;
    states.transform.translate(translation);
    states.transform.scale(scale);
    states.texture = &TileDatabase::get().texture;

    window.draw(vertices, states);
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
    changeQuad(x, y);
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

    generateVertices();

    return true;
}

void Chunk::generateVertices() {

    vertices.setPrimitiveType(sf::Quads);
    vertices.resize(SIDE_LENGTH*SIDE_LENGTH*4);

    for (unsigned int i=0; i<SIDE_LENGTH; i++) {
        for (unsigned int j=0; j<SIDE_LENGTH; j++) {
            sf::Vertex* quad = &vertices[(i+j*SIDE_LENGTH) * 4];

            quad[0].position = sf::Vector2f(i*TILE_SIZE, j*TILE_SIZE);
            quad[1].position = sf::Vector2f((i+1)*TILE_SIZE, j*TILE_SIZE);
            quad[2].position = sf::Vector2f((i+1)*TILE_SIZE, (j+1)*TILE_SIZE);
            quad[3].position = sf::Vector2f(i*TILE_SIZE, (j+1)*TILE_SIZE);

            changeQuad(i, j);
        }
    }
}

void Chunk::changeQuad(int x, int y) {
    if (vertices.getVertexCount() == 0) {
        generateVertices();
    }
    else {
        sf::Vertex *quad = &vertices[(x + y * SIDE_LENGTH) * 4];

        int tile_id = getTile(x, y);

        int tx = TileDatabase::get()[tile_id].texture_x;
        int ty = TileDatabase::get()[tile_id].texture_y;

        quad[0].texCoords = sf::Vector2f(tx*TILE_SIZE, ty*TILE_SIZE);
        quad[1].texCoords = sf::Vector2f((tx+1)*TILE_SIZE, ty*TILE_SIZE);
        quad[2].texCoords = sf::Vector2f((tx+1)*TILE_SIZE, (ty+1)*TILE_SIZE);
        quad[3].texCoords = sf::Vector2f(tx*TILE_SIZE, (ty+1)*TILE_SIZE);
    }
}


