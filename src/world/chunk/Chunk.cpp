#include "Chunk.h"

void Chunk::render(sf::RenderWindow) {

}

void Chunk::update(std::chrono::microseconds deltaTime) {

}

Chunk::Chunk() {

}

Chunk::Chunk(const std::vector<int>& _tiles, const std::vector<Object>& _objects) {
    tiles = _tiles;
    objects = _objects;
}
