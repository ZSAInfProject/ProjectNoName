#include <SFML/Graphics/RectangleShape.hpp>
#include "Pathfinder.h"
#include "../chunk/Chunk.h"
#include "../../tile/TileDatabase.h"
#include "../../utils/Log.h"

void Pathfinder::render(sf::RenderWindow& window) {
    for (int i=1; i<nodes.size(); i++) {
        if (nodes[i].enabled) {
            sf::RectangleShape rectangle;
            rectangle.setPosition(sf::Vector2f(nodes[i].x*Chunk::TILE_SIZE, nodes[i].y*Chunk::TILE_SIZE));
            rectangle.setSize(sf::Vector2f(Chunk::TILE_SIZE, Chunk::TILE_SIZE));
            rectangle.setFillColor(sf::Color::Blue);
            window.draw(rectangle);
        }
    }
}

Pathfinder::Pathfinder() {
    nodes.push_back({false, 0, 0, {0, 0, 0, 0}, {0, 0, 0, 0}});
}

void Pathfinder::generate(int x, int y) {
    nodes.push_back({true, x, y, {0, 0, 0, 0}, {0, 0, 0, 0}});
    world->setTileNodes(x, y, static_cast<short>(nodes.size() - 1), 0, 0);
    branch_node(1);
}

void Pathfinder::branch_node(short id) {
    if (!nodes[id].enabled)
        return;
    int x = nodes[id].x;
    int y = nodes[id].y;
    //    0 1 2
    //    3   4
    //    5 6 7
    bool solid[8] = {TileDatabase::get()[world->getLoadedTile(x-1, y+1).tileId].isSolid,
                     TileDatabase::get()[world->getLoadedTile(x, y+1).tileId].isSolid,
                     TileDatabase::get()[world->getLoadedTile(x+1, y+1).tileId].isSolid,
                     TileDatabase::get()[world->getLoadedTile(x-1, y).tileId].isSolid,
                     TileDatabase::get()[world->getLoadedTile(x+1, y).tileId].isSolid,
                     TileDatabase::get()[world->getLoadedTile(x-1, y-1).tileId].isSolid,
                     TileDatabase::get()[world->getLoadedTile(x, y-1).tileId].isSolid,
                     TileDatabase::get()[world->getLoadedTile(x+1, y-1).tileId].isSolid
    };
    if (!solid[4] && nodes[id].connections[Right] == 0) {
        branch(Right, nodes[id].x+1, nodes[id].y, id);

    }
    if (!solid[3] && nodes[id].connections[Left] == 0) {
        branch(Left, nodes[id].x-1, nodes[id].y, id);
    }
    if (!solid[6]) {
        branch(Down, nodes[id].x, nodes[id].y - 1, id);
    }
    if (!solid[1] && ((!solid[0] && solid[3])||(!solid[2] && solid[4])) && nodes[id].connections[Up] == 0) {
        branch(Up, nodes[id].x, nodes[id].y + 1, id);
    }
}

void Pathfinder::branch(Direction direction, int x, int y, short id) {

    bool end = false;
    Direction dir = direction;

    while (!end) {
        //    0 1 2
        //    3   4
        //    5 6 7
        bool solid[8] = {TileDatabase::get()[world->getLoadedTile(x-1, y+1).tileId].isSolid,
                         TileDatabase::get()[world->getLoadedTile(x, y+1).tileId].isSolid,
                         TileDatabase::get()[world->getLoadedTile(x+1, y+1).tileId].isSolid,
                         TileDatabase::get()[world->getLoadedTile(x-1, y).tileId].isSolid,
                         TileDatabase::get()[world->getLoadedTile(x+1, y).tileId].isSolid,
                         TileDatabase::get()[world->getLoadedTile(x-1, y-1).tileId].isSolid,
                         TileDatabase::get()[world->getLoadedTile(x, y-1).tileId].isSolid,
                         TileDatabase::get()[world->getLoadedTile(x+1, y-1).tileId].isSolid
        };

        int temp_x = x;
        int temp_y = y;
        Direction temp_dir = direction;
        int count = 0;

        if (!solid[4] && (solid[6] || solid[7]) && direction != Left) {
            count++;
            temp_dir = Right;
            temp_x++;
        }
        if (!solid[3] && (solid[6] || solid[5]) && direction != Right) {
            count++;
            temp_dir = Left;
            temp_x--;
        }
        if (!solid[6] && direction != Up) {
            count++;
            temp_dir = Down;
            temp_y--;
        }
        if (!solid[1] && ((!solid[0] && solid[3]) || (!solid[2] && solid[4])) && direction != Down) {
            count++;
            temp_dir = Up;
            temp_y++;
        }
        if (count > 1 || count == 0) {
            end = true;

            if (world->getTile(x, y).node == 0) {
                short arr1[4] = {0, 0, 0, 0};
                short arr2[4] = {0, 0, 0, 0};
                switch (direction) {
                    case Up:
                        arr1[Down] = id;
                        break;
                    case Down:
                        arr1[Up] = id;
                        break;
                    case Left:
                        arr1[Right] = id;
                        break;
                    case Right:
                        arr1[Left] = id;
                        break;
                }
                Node n = {true, x, y, {arr1[0], arr1[1], arr1[2], arr1[3]}, {arr2[0], arr2[1], arr2[2], arr2[3]}};
                nodes.push_back(n);
                Log::debug(TAG, std::to_string(static_cast<short>(nodes.size() - 1)));
                world->setTileNodes(x, y, static_cast<short>(nodes.size() - 1), 0, 0);
                branch_node(static_cast<short>(nodes.size() - 1));
            } else {
                switch (direction) {
                    case Up:
                        nodes[world->getTile(x, y).node].connections[Down] = id;
                        break;
                    case Down:
                        nodes[world->getTile(x, y).node].connections[Up] = id;
                        break;
                    case Left:
                        nodes[world->getTile(x, y).node].connections[Right] = id;
                        break;
                    case Right:
                        nodes[world->getTile(x, y).node].connections[Left] = id;
                        break;
                }
                branch_node(world->getTile(x, y).node);
            }
            nodes[id].connections[dir] = static_cast<short>(nodes.size() - 1);


        } else {
            x = temp_x;
            y = temp_y;
            direction = temp_dir;
        }
    }
}

void Pathfinder::setWorld(World *world) {
    this->world = world;
}

