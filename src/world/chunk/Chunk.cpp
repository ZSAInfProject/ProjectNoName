#include "Chunk.h"
#include "../../tile/TileDatabase.h"
#include "../../utils/Log.h"

#include <fstream>
#include <iostream>

void Chunk::render(sf::RenderWindow& window, const sf::Vector2f& translation, const sf::Vector2f& scale) {
    if(vertices.getVertexCount() == 0){
        generateVertices();
        updateQuads();
    }
    sf::RenderStates states;
    states.transform.translate(translation);
    states.transform.scale(scale);
    states.texture = &TileDatabase::get().texture;

    window.draw(vertices, states);
}

void Chunk::update(std::chrono::microseconds deltaTime) {

}

Chunk::Chunk() {
    generateVertices();
}

Chunk::Chunk(const std::array<ChunkTile, SIDE_LENGTH*SIDE_LENGTH> &_tiles) {
    tiles = _tiles;
    generateVertices();
    updateQuads();
}

ChunkTile Chunk::getTile(int x, int y) {
    if(x >= SIDE_LENGTH || y >= SIDE_LENGTH){
        Log::error(TAG, "Tile get out of bounds at x = " + std::to_string(x) + " y = " + std::to_string(y));
        return tiles[0];
    }
    return tiles[y*SIDE_LENGTH + x];
}

void Chunk::setTile(int x, int y, ChunkTile value) {
    if(x >= SIDE_LENGTH || y >= SIDE_LENGTH){
        Log::error(TAG, "Tile set out of bounds at x = " + std::to_string(x) + " y = " + std::to_string(y));
        return;
    }
    tiles[y*SIDE_LENGTH + x] = value;
    changeQuad(x, y);
}

void Chunk::save(const std::string &fileName) {
    //TODO: save objects to another file.

    //Save tiles
    std::ofstream tileData(fileName+".td");
    for(ChunkTile tile : tiles) {
        if(tileData.good()) {
            tileData.write(reinterpret_cast<const char *>(&tile), sizeof(typeof(tile)));
        }
        else{
            Log::error(TAG, "Saving to " + fileName + ".td file failed!");
        }
    }
    tileData.close();
}

bool Chunk::load(const std::string &filename) {
    std::ifstream tileData;
    tileData.open(filename+".td", std::ifstream::in | std::ifstream::binary);
    if(!tileData.is_open()){
        return false;
    }
    //TODO: load objects

    //Load tiles
    for(ChunkTile& tile : tiles){
        if(tileData.good()) {
            tileData.read(reinterpret_cast<char *>(&tile), sizeof(typeof(tile)));
        }
        else{
            Log::error(TAG, "Loading from " + filename + ".td failed!");
            return false;
        }
    }
    tileData.close();

    updateQuads();

    return true;
}

void Chunk::generateVertices() {
    for (unsigned int i=0; i<SIDE_LENGTH; i++) {
        for (unsigned int j=0; j<SIDE_LENGTH; j++) {
            sf::Vertex* quad = &vertices[(i+j*SIDE_LENGTH) * 4];

            quad[2].position = sf::Vector2f(i*TILE_SIZE, j*TILE_SIZE);
            quad[3].position = sf::Vector2f((i+1)*TILE_SIZE, j*TILE_SIZE);
            quad[0].position = sf::Vector2f((i+1)*TILE_SIZE, (j+1)*TILE_SIZE);
            quad[1].position = sf::Vector2f(i*TILE_SIZE, (j+1)*TILE_SIZE);
        }
    }
    Log::verbose(TAG, "Generated new vertices");
}

void Chunk::updateQuads() {
    for (unsigned int i=0; i<SIDE_LENGTH; i++) {
        for (unsigned int j=0; j<SIDE_LENGTH; j++) {
            changeQuad(i, j);
        }
    }
}

void Chunk::changeQuad(int x, int y) {
    sf::Vertex *quad = &vertices[(x + y * SIDE_LENGTH) * 4];

    int tile_id = getTile(x, y).tileId;

    int tx = TileDatabase::get()[tile_id].texture_x;
    int ty = TileDatabase::get()[tile_id].texture_y;

    quad[0].texCoords = sf::Vector2f(tx*TILE_SIZE, ty*TILE_SIZE);
    quad[1].texCoords = sf::Vector2f((tx+1)*TILE_SIZE - 0.0075, ty*TILE_SIZE);
    quad[2].texCoords = sf::Vector2f((tx+1)*TILE_SIZE - 0.0075, (ty+1)*TILE_SIZE - 0.0075);
    quad[3].texCoords = sf::Vector2f(tx*TILE_SIZE, (ty+1)*TILE_SIZE - 0.0075);
}


ChunkTile::ChunkTile(nlohmann::json json) {
    if(json.find("id") != json.end()){
        tileId = json["id"].get<short>();
    }
    else{
        Log::error(TAG, "No id data in json");
    }

    if(json.find("amount") != json.end()){
        amount = json["amount"].get<uint>();
    }
    else{
        Log::error(TAG, "No amount data in json");
    }
}

ChunkTile::ChunkTile(short tileId_, uint amount_) {
    tileId = tileId_;
    amount = amount_;
}

