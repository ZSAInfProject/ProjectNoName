#ifndef NONAME_CHUNK_H
#define NONAME_CHUNK_H


#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>
#include <chrono>
#include "../object/Object.h"

class Chunk {
    std::array<int, 4096> tiles;
    std::vector<std::unique_ptr<Object>> objects;

    Chunk(Chunk const&) = delete;
    void operator= (Chunk const&) = delete;

public:
    void render(sf::RenderWindow);
    void update(std::chrono::microseconds deltaTime);

    Chunk();
    Chunk(const std::array<int, 4096>& _tiles, std::vector<std::unique_ptr<Object>> _objects);
};


#endif //NONAME_CHUNK_H
