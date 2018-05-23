#ifndef NONAME_PATHFINDER_H
#define NONAME_PATHFINDER_H


#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../World.h"

struct Node {
    bool enabled;
    int x;
    int y;
    short connections[4];
    short lenghts[4];
};

enum Direction {
    Up,
    Down,
    Left,
    Right
};

class Pathfinder {
public:

    Pathfinder();
    void render(sf::RenderWindow& window);
    void generate(int x, int y);
    void setWorld(World* world);

private:
    void branch_node(short id);
    void branch(Direction direction, int x, int y, short id);
    std::vector<Node> nodes;
    World* world;

    static constexpr auto TAG = "Pathfinder";
};


#endif //NONAME_PATHFINDER_H
