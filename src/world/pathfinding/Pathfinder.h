#ifndef NONAME_PATHFINDER_H
#define NONAME_PATHFINDER_H


#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../World.h"

struct Node {
    bool enabled;
    int x;
    int y;
    std::array<ushort, 4> connections;
    std::array<ushort, 4> lengths;
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
    void generate(int x, int y);
    void setWorld(World* world);
    void save();
    bool load();
    void updated_block(int x, int y);
    ~Pathfinder();

private:
    void delete_branch(int x, int y, ushort node0, ushort node1);
    void branch_node(ushort id);
    void branch(Direction direction, int x, int y, ushort id);
    std::vector<Node> nodes;
    World* world;

    static constexpr auto TAG = "Pathfinder";
};


#endif //NONAME_PATHFINDER_H
