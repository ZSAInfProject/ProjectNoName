#ifndef NONAME_CHUNK_H
#define NONAME_CHUNK_H


#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>
#include <chrono>
#include "../object/Object.h"

class Chunk {
public:
    static const int SIDE_LENGTH = 32;
private:
    std::array<int, SIDE_LENGTH*SIDE_LENGTH> tiles;
    std::vector<std::unique_ptr<Object>> objects;
public:
    Chunk(Chunk const&) = delete;
    void operator= (Chunk const&) = delete;

    void render(sf::RenderWindow);
    void update(std::chrono::microseconds deltaTime);

    void save(const std::string& filename);
    bool load(const std::string& filename);

    int getTile(int x, int y);
    void setTile(int x, int y, int value);

    Chunk();
    explicit Chunk(const std::array<int, SIDE_LENGTH*SIDE_LENGTH>& _tiles);
};


#endif //NONAME_CHUNK_H
