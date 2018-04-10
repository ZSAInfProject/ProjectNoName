#include "Chunk.h"

void Chunk::render(sf::RenderWindow) {

}

void Chunk::update(std::chrono::microseconds deltaTime) {

}

Chunk::Chunk() {

}

Chunk::Chunk(const std::array<int, SIDE_LENGTH*SIDE_LENGTH>& _tiles, std::vector<std::unique_ptr<Object>> _objects) {
    tiles = _tiles;
    objects = std::move(_objects);
}

int Chunk::getTile(int x, int y) {
    return tiles[y*SIDE_LENGTH + x];
}

void Chunk::setTile(int x, int y, int value) {
    tiles[y*SIDE_LENGTH + x] = value;
}
