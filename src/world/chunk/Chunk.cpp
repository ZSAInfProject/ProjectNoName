#include "Chunk.h"
#include "../../tile/TileDatabase.h"
#include "../../utils/Log.h"
#include "../../utils/Buffer.h"

#include <fstream>
#include <iostream>

void Chunk::render(sf::RenderWindow& window, const sf::Vector2f& translation, const sf::Vector2f& scale) {
    if(vertices.getVertexCount() == 0) {
        generateVertices();
        updateQuads();
    }
    sf::RenderStates states;
    states.transform.translate(translation);
    states.transform.scale(scale);
    states.texture = &TileDatabase::get().texture;

    window.draw(vertices, states);
}

Chunk::Chunk() {
    generateVertices();
}

Chunk::Chunk(const std::array<ChunkTile, SIDE_LENGTH * SIDE_LENGTH>& tiles) {
    this->tiles = tiles;
    generateVertices();
    updateQuads();
}

ChunkTile Chunk::getTile(int x, int y) {
    if(isInsideBoundaries(x, y)) {
        return tiles[y * SIDE_LENGTH + x];
    }
    else {
        return tiles[0];
    };
}

void Chunk::setTile(int x, int y, ChunkTile value) {
    if(isInsideBoundaries(x, y)) {
        tiles[y * SIDE_LENGTH + x] = value;
        changeQuad(x, y);
    }
}

void Chunk::setTileObject(int x, int y, short objectId) {
    if(isInsideBoundaries(x, y)) {
        tiles[y * SIDE_LENGTH + x].objectId = objectId;
    }
}

void Chunk::save(const std::string& fileName) {

    //Save tiles
    Buffer buffer(bufferMode::store);

    for(ChunkTile tile : tiles) {
        tile.serialize(buffer);
    }
    buffer.save(fileName + ".td");

    //Save objects
    std::ofstream objectData(fileName + ".json");
    if(objectData.good()) {
        nlohmann::json json;
        json["objects"] = nlohmann::json::array();
        for(uint i = 0; i < objects.size(); i++) {
            json["objects"][i] = objects[i]->serialize();
        }
        std::string data = json.dump();
        objectData.write(data.c_str(), data.size());
    }
    else {
        Log::error(TAG, "Saving to " + fileName + ".json file failed!");
    }
    objectData.close();
}

bool Chunk::load(const std::string& filename) {

    Buffer buffer(bufferMode::load);
    if(!buffer.load(filename + ".td"))
        return false;

    //Load tiles
    for(ChunkTile& tile : tiles) {
        tile.serialize(buffer);
    }

    updateQuads();

    //Load objects
    std::ifstream objectData(filename + ".json");
    if(objectData.is_open()) {
        nlohmann::json j = nlohmann::json::parse(objectData);
        std::vector<nlohmann::json> objectVector = j["objects"];
        for(auto object : objectVector) {
            objects.push_back(std::make_shared<Entity>(object));
        }
    }
    return true;
}

void Chunk::generateVertices() {
    for(unsigned int i = 0; i < SIDE_LENGTH; i++) {
        for(unsigned int j = 0; j < SIDE_LENGTH; j++) {
            sf::Vertex* quad = &vertices[(i + j * SIDE_LENGTH) * 4];

            quad[2].position = sf::Vector2f(i * TILE_SIZE, j * TILE_SIZE);
            quad[3].position = sf::Vector2f((i + 1) * TILE_SIZE, j * TILE_SIZE);
            quad[0].position = sf::Vector2f((i + 1) * TILE_SIZE, (j + 1) * TILE_SIZE);
            quad[1].position = sf::Vector2f(i * TILE_SIZE, (j + 1) * TILE_SIZE);
        }
    }
    Log::verbose(TAG, "Generated new vertices");
}

void Chunk::updateQuads() {
    for(unsigned int i = 0; i < SIDE_LENGTH; i++) {
        for(unsigned int j = 0; j < SIDE_LENGTH; j++) {
            changeQuad(i, j);
        }
    }
}

void Chunk::changeQuad(int x, int y) {
    sf::Vertex* quad = &vertices[(x + y * SIDE_LENGTH) * 4];

    int tile_id = getTile(x, y).tileId;
    if (getTile(x, y).node0 != 0) {
        tile_id = 0;
    }

    int tx = TileDatabase::get()[tile_id].texture_x;
    int ty = TileDatabase::get()[tile_id].texture_y;

    quad[0].texCoords = sf::Vector2f(tx * TILE_SIZE, ty * TILE_SIZE);
    quad[1].texCoords = sf::Vector2f(static_cast<float>((tx + 1) * TILE_SIZE - 0.0075), ty * TILE_SIZE);
    quad[2].texCoords = sf::Vector2f(static_cast<float>((tx + 1) * TILE_SIZE - 0.0075),
                                     static_cast<float>((ty + 1) * TILE_SIZE - 0.0075));
    quad[3].texCoords = sf::Vector2f(tx * TILE_SIZE, static_cast<float>((ty + 1) * TILE_SIZE - 0.0075));
}

void Chunk::update(std::chrono::microseconds deltaTime) {

}

bool Chunk::isInsideBoundaries(int x, int y) {
    if(x >= SIDE_LENGTH || y >= SIDE_LENGTH) {
        Log::warn(TAG, "Tile access out of bounds at x = " + std::to_string(x) + " y = " + std::to_string(y));
        return false;
    }
    return true;
}

void Chunk::setTileNode(int x, int y, short node) {
    if(x >= SIDE_LENGTH || y >= SIDE_LENGTH){
        Log::error(TAG, "Tile set out of bounds at x = " + std::to_string(x) + " y = " + std::to_string(y));
        return;
    }
    tiles[y*SIDE_LENGTH + x].node = node;
}

void Chunk::setTilePath(int x, int y, short node0, short node1) {
    if(x >= SIDE_LENGTH || y >= SIDE_LENGTH){
        Log::error(TAG, "Tile set out of bounds at x = " + std::to_string(x) + " y = " + std::to_string(y));
        return;
    }
    tiles[y*SIDE_LENGTH + x].node0 = node0;
    tiles[y*SIDE_LENGTH + x].node1 = node1;
}

ChunkTile::ChunkTile(nlohmann::json json) {
    if(json.find("id") != json.end()) {
        tileId = json["id"].get<short>();
    }
    else {
        Log::error(TAG, "No id data in json");
    }

    if(json.find("amount") != json.end()) {
        amount = json["amount"].get<uint>();
    }
    else {
        Log::error(TAG, "No amount data in json");
    }
    objectId = -1;
}

ChunkTile::ChunkTile(short tileId_, uint amount_, short objectId_) {
    tileId = tileId_;
    amount = amount_;
    objectId = objectId_;
}

void ChunkTile::serialize(Buffer& buffer) {
    buffer.io<short>(&tileId);
    buffer.io<uint>(&amount);
    buffer.io<short>(&objectId);
    buffer.io<short>(&node);
    buffer.io<short>(&node0);
    buffer.io<short>(&node1);
}

