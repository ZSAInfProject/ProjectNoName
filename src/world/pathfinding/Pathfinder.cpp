#include <SFML/Graphics/RectangleShape.hpp>
#include "Pathfinder.h"
#include "../chunk/Chunk.h"
#include "../../tile/TileDatabase.h"
#include "../../utils/Log.h"
#include "../../utils/Settings.h"
#include "../../entity/components/Ladder.h"

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
    world->setTileNode(x, y, static_cast<short>(nodes.size() - 1));
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
    //    8 9 10
    bool solid[11] = {TileDatabase::get()[world->getLoadedTile(x-1, y+1).tileId].isSolid,
                      TileDatabase::get()[world->getLoadedTile(x, y+1).tileId].isSolid,
                      TileDatabase::get()[world->getLoadedTile(x+1, y+1).tileId].isSolid,
                      TileDatabase::get()[world->getLoadedTile(x-1, y).tileId].isSolid,
                      TileDatabase::get()[world->getLoadedTile(x+1, y).tileId].isSolid,
                      TileDatabase::get()[world->getLoadedTile(x-1, y-1).tileId].isSolid,
                      TileDatabase::get()[world->getLoadedTile(x, y-1).tileId].isSolid,
                      TileDatabase::get()[world->getLoadedTile(x+1, y-1).tileId].isSolid,
                      TileDatabase::get()[world->getLoadedTile(x-1, y-2).tileId].isSolid,
                      TileDatabase::get()[world->getLoadedTile(x, y-2).tileId].isSolid,
                      TileDatabase::get()[world->getLoadedTile(x+1, y-2).tileId].isSolid
    };

    //
    //       0
    //     1 2 3
    bool ladder[4] = {
            world->getObject(x, y) == nullptr ? false : world->getObject(x, y)->getComponent<LadderComponent>() != nullptr,
            world->getObject(x-1, y-1) == nullptr ? false : world->getObject(x-1, y-1)->getComponent<LadderComponent>() != nullptr,
            world->getObject(x, y-1) == nullptr ? false : world->getObject(x, y-1)->getComponent<LadderComponent>() != nullptr,
            world->getObject(x+1, y-1) == nullptr ? false : world->getObject(x+1, y-1)->getComponent<LadderComponent>() != nullptr
    };


    if (!solid[4] && (solid[7] || solid[10] || ladder[3]) && nodes[id].connections[Right] == 0) {
        branch(Right, nodes[id].x+1, nodes[id].y, id);

    }
    if (!solid[3] && (solid[5] || solid[8] || ladder[1]) && nodes[id].connections[Left] == 0) {
        branch(Left, nodes[id].x-1, nodes[id].y, id);
    }
    if (!solid[6] && (solid[9] || ladder[2])) {
        branch(Down, nodes[id].x, nodes[id].y - 1, id);
    }
    if (!solid[1] && ((!solid[0] && solid[3]) || (!solid[2] && solid[4]) || ladder[0]) && nodes[id].connections[Up] == 0) {
        branch(Up, nodes[id].x, nodes[id].y + 1, id);
    }
}

void Pathfinder::branch(Direction direction, int x, int y, short id) {

    std::vector<sf::Vector2i> tiles;

    bool end = false;
    Direction dir = direction;

    while (!end) {
        //    0 1 2
        //    3   4
        //    5 6 7
        //    8 9 10
        bool solid[11] = {TileDatabase::get()[world->getLoadedTile(x-1, y+1).tileId].isSolid,
                         TileDatabase::get()[world->getLoadedTile(x, y+1).tileId].isSolid,
                         TileDatabase::get()[world->getLoadedTile(x+1, y+1).tileId].isSolid,
                         TileDatabase::get()[world->getLoadedTile(x-1, y).tileId].isSolid,
                         TileDatabase::get()[world->getLoadedTile(x+1, y).tileId].isSolid,
                         TileDatabase::get()[world->getLoadedTile(x-1, y-1).tileId].isSolid,
                         TileDatabase::get()[world->getLoadedTile(x, y-1).tileId].isSolid,
                         TileDatabase::get()[world->getLoadedTile(x+1, y-1).tileId].isSolid,
                         TileDatabase::get()[world->getLoadedTile(x-1, y-2).tileId].isSolid,
                         TileDatabase::get()[world->getLoadedTile(x, y-2).tileId].isSolid,
                         TileDatabase::get()[world->getLoadedTile(x+1, y-2).tileId].isSolid
        };
        //
        //       0
        //     1 2 3
        bool ladder[4] = {
                world->getObject(x, y) == nullptr ? false : world->getObject(x, y)->getComponent<LadderComponent>() != nullptr,
                world->getObject(x-1, y-1) == nullptr ? false : world->getObject(x-1, y-1)->getComponent<LadderComponent>() != nullptr,
                world->getObject(x, y-1) == nullptr ? false : world->getObject(x, y-1)->getComponent<LadderComponent>() != nullptr,
                world->getObject(x+1, y-1) == nullptr ? false : world->getObject(x+1, y-1)->getComponent<LadderComponent>() != nullptr
        };


        int temp_x = x;
        int temp_y = y;
        Direction temp_dir = direction;
        tiles.push_back({x, y});

        int count = 0;

        if (!solid[4] && (solid[6] || ladder[2] || direction == Up) && (solid[7] || (solid[10] && direction != Up) || ladder[3]) && direction != Left) {
            count++;
            temp_dir = Right;
            temp_x++;
        }
        if (!solid[3] && (solid[6] || ladder[2] || direction == Up) && (solid[5] || (solid[8] && direction != Up) || ladder[1]) && direction != Right) {
            count++;
            temp_dir = Left;
            temp_x--;
        }
        if (!solid[6] && (solid[9] || ladder[2]) && direction != Up) {
            count++;
            temp_dir = Down;
            temp_y--;
        }
        if (!solid[1] && ((!solid[0] && solid[3]) || (!solid[2] && solid[4]) || ladder[0]) && direction != Down) {
            count++;
            temp_dir = Up;
            temp_y++;
        }

        Log::verbose(TAG, "Step " + std::to_string(x) + " " + std::to_string(y));

        if (count > 1 || count == 0) {
            end = true;
            short newId;

            if (world->getTile(x, y).node != 0 && world->getTile(x, y).node < nodes.size()) {
                newId = world->getTile(x, y).node;
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
            } else {
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
                Log::verbose(TAG, "Node created " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(id));
                Node n = {true, x, y, {arr1[0], arr1[1], arr1[2], arr1[3]}, {arr2[0], arr2[1], arr2[2], arr2[3]}};
                nodes.push_back(n);
                newId = nodes.size() - 1;

                world->setTileNode(x, y, static_cast<short>(nodes.size() - 1));
                nodes[id].connections[dir] = static_cast<short>(nodes.size() - 1);
                if (id != static_cast<short>(nodes.size() - 1)) {
                    branch_node(static_cast<short>(nodes.size() - 1));
                }
            }

            for (auto tile : tiles) {
                world->setTilePath(tile.x, tile.y, id, newId);
            }

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

void Pathfinder::save() {
    std::ofstream file(Settings::get<std::string>("save_path") + "nodes.td");
    if(file.good()) {
        std::string output;
        for (auto node : nodes) {
            output += std::to_string(node.enabled) + "\n";
            output += std::to_string(node.x) + "\n";
            output += std::to_string(node.y) + "\n";
            for (auto connection : node.connections) {
                output += std::to_string(connection) + "\n";
            }
            for (auto length : node.lenghts) {
                output += std::to_string(length) + "\n";
            }
            output += "\n";
        }
        file << output;
        file.close();
    }
}

bool Pathfinder::load() {
    std::ifstream file(Settings::get<std::string>("save_path") + "nodes.td");
    if(file.good()) {
        std::string line;
        while(getline(file, line)) {
            Node node{};
            node.enabled = (bool)std::stoi(line);
            getline(file, line);
            node.x = std::stoi(line);
            getline(file, line);
            node.y = std::stoi(line);
            getline(file, line);
            for (short &connection : node.connections) {
                connection = (short)std::stoi(line);
                getline(file, line);
            }
            for (short &lenght : node.lenghts) {
                lenght = (short)std::stoi(line);
                getline(file, line);
            }
            nodes.push_back(node);
        }
        return true;
    }
    else {
        Log::debug(TAG, "Could not load graph file");
        return false;
    }
}

Pathfinder::~Pathfinder() {
   save();
   Log::debug(TAG, "Saving graph");
}

