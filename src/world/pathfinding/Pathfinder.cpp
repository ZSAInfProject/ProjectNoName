#include <SFML/Graphics/RectangleShape.hpp>
#include <set>
#include <zconf.h>
#include "Pathfinder.h"
#include "../chunk/Chunk.h"
#include "../../tile/TileDatabase.h"
#include "../../utils/Log.h"
#include "../../utils/Settings.h"
#include "../../entity/components/Ladder.h"

Pathfinder::Pathfinder() {
}

void Pathfinder::generate(int x, int y) {
    nodes.push_back({false, 0, 0, {0, 0, 0, 0}, {0, 0, 0, 0}});
    nodes.push_back({true, x, y, {0, 0, 0, 0}, {0, 0, 0, 0}});
    world->setTileNode(x, y, static_cast<ushort>(nodes.size() - 1));
    branch_node(1);
}

void Pathfinder::branch_node(ushort id) {
    if(!nodes[id].enabled)
        return;
    int x = nodes[id].x;
    int y = nodes[id].y;
    //    0 1 2
    //    3   4
    //    5 6 7
    //    8 9 10
    bool solid[11] = {TileDatabase::get()[world->getLoadedTile(x - 1, y + 1).tileId].isSolid,
                      TileDatabase::get()[world->getLoadedTile(x, y + 1).tileId].isSolid,
                      TileDatabase::get()[world->getLoadedTile(x + 1, y + 1).tileId].isSolid,
                      TileDatabase::get()[world->getLoadedTile(x - 1, y).tileId].isSolid,
                      TileDatabase::get()[world->getLoadedTile(x + 1, y).tileId].isSolid,
                      TileDatabase::get()[world->getLoadedTile(x - 1, y - 1).tileId].isSolid,
                      TileDatabase::get()[world->getLoadedTile(x, y - 1).tileId].isSolid,
                      TileDatabase::get()[world->getLoadedTile(x + 1, y - 1).tileId].isSolid,
                      TileDatabase::get()[world->getLoadedTile(x - 1, y - 2).tileId].isSolid,
                      TileDatabase::get()[world->getLoadedTile(x, y - 2).tileId].isSolid,
                      TileDatabase::get()[world->getLoadedTile(x + 1, y - 2).tileId].isSolid
    };

    //
    //       0
    //     1 2 3
    bool ladder[4] = {
            world->getObject(x, y) == nullptr ? false : world->getObject(x, y)->getComponent<LadderComponent>() !=
                                                        nullptr,
            world->getObject(x - 1, y - 1) == nullptr ? false :
            world->getObject(x - 1, y - 1)->getComponent<LadderComponent>() != nullptr,
            world->getObject(x, y - 1) == nullptr ? false :
            world->getObject(x, y - 1)->getComponent<LadderComponent>() != nullptr,
            world->getObject(x + 1, y - 1) == nullptr ? false :
            world->getObject(x + 1, y - 1)->getComponent<LadderComponent>() != nullptr
    };


    if(!solid[4] && (solid[7] || solid[10] || ladder[3]) && nodes[id].connections[Right] == 0) {
        branch(Right, nodes[id].x + 1, nodes[id].y, id);
    }
    if(!solid[3] && (solid[5] || solid[8] || ladder[1]) && nodes[id].connections[Left] == 0) {
        branch(Left, nodes[id].x - 1, nodes[id].y, id);
    }
    if(!solid[6] && (solid[9] || ladder[2])) {
        branch(Down, nodes[id].x, nodes[id].y - 1, id);
    }
    if(!solid[1] && ((!solid[0] && solid[3]) || (!solid[2] && solid[4]) || ladder[0]) && (solid[6] || ladder[2]) &&
       nodes[id].connections[Up] == 0) {
        branch(Up, nodes[id].x, nodes[id].y + 1, id);
    }

    int count = 0;
    for(auto connection : nodes[id].connections) {
        if(connection != 0) count++;
    }
    if(count == 0) {
        nodes[id] = {false, 0, 0, {0, 0, 0, 0}, {0, 0, 0, 0}};
    };

}

void Pathfinder::branch(Direction direction, int x, int y, ushort id) {

    std::vector<sf::Vector2i> tiles;

    bool end = false;
    Direction dir = direction;

    while(!end) {
        //    0 1 2
        //    3   4
        //    5 6 7
        //    8 9 10
        bool solid[11] = {TileDatabase::get()[world->getLoadedTile(x - 1, y + 1).tileId].isSolid,
                          TileDatabase::get()[world->getLoadedTile(x, y + 1).tileId].isSolid,
                          TileDatabase::get()[world->getLoadedTile(x + 1, y + 1).tileId].isSolid,
                          TileDatabase::get()[world->getLoadedTile(x - 1, y).tileId].isSolid,
                          TileDatabase::get()[world->getLoadedTile(x + 1, y).tileId].isSolid,
                          TileDatabase::get()[world->getLoadedTile(x - 1, y - 1).tileId].isSolid,
                          TileDatabase::get()[world->getLoadedTile(x, y - 1).tileId].isSolid,
                          TileDatabase::get()[world->getLoadedTile(x + 1, y - 1).tileId].isSolid,
                          TileDatabase::get()[world->getLoadedTile(x - 1, y - 2).tileId].isSolid,
                          TileDatabase::get()[world->getLoadedTile(x, y - 2).tileId].isSolid,
                          TileDatabase::get()[world->getLoadedTile(x + 1, y - 2).tileId].isSolid
        };
        //
        //       0
        //     1 2 3
        bool ladder[4] = {
                world->getObject(x, y) == nullptr ? false : world->getObject(x, y)->getComponent<LadderComponent>() !=
                                                            nullptr,
                world->getObject(x - 1, y - 1) == nullptr ? false :
                world->getObject(x - 1, y - 1)->getComponent<LadderComponent>() != nullptr,
                world->getObject(x, y - 1) == nullptr ? false :
                world->getObject(x, y - 1)->getComponent<LadderComponent>() != nullptr,
                world->getObject(x + 1, y - 1) == nullptr ? false :
                world->getObject(x + 1, y - 1)->getComponent<LadderComponent>() != nullptr
        };


        int temp_x = x;
        int temp_y = y;
        Direction temp_dir = direction;
        tiles.emplace_back(x, y);

        int count = 0;

        if(!solid[4] && (solid[6] || (ladder[2] && solid[7]) || direction == Up || (solid[7] && solid[9])) &&
           (solid[7] || (solid[10] && direction != Up) || ladder[3]) && direction != Left) {
            count++;
            temp_dir = Right;
            temp_x++;
        }
        if(!solid[3] && (solid[6] || (ladder[2] && solid[5]) || direction == Up || (solid[5] && solid[9])) &&
           (solid[5] || (solid[8] && direction != Up) || ladder[1]) && direction != Right) {
            count++;
            temp_dir = Left;
            temp_x--;
        }
        if(!solid[6] && (solid[9] || ladder[2]) && direction != Up) {
            count++;
            temp_dir = Down;
            temp_y--;
        }
        if(!solid[1] && ((!solid[0] && solid[3]) || (!solid[2] && solid[4]) || ladder[0]) && (solid[6] || ladder[2])
           && direction != Down) {
            count++;
            temp_dir = Up;
            temp_y++;
        }

        if(count > 1 || count == 0) {
            end = true;
            ushort newId;
            if(world->getTile(x, y).node != 0 && world->getTile(x, y).node < nodes.size()) {
                newId = world->getTile(x, y).node;
                switch(direction) {
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
                nodes[id].connections[dir] = newId;
            }
            else {
                ushort arr1[4] = {0, 0, 0, 0};
                ushort arr2[4] = {0, 0, 0, 0};
                switch(direction) {
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
                newId = nodes.size() - 1;

                world->setTileNode(x, y, static_cast<ushort>(nodes.size() - 1));
                nodes[id].connections[dir] = static_cast<ushort>(nodes.size() - 1);
                if(id != static_cast<short>(nodes.size() - 1)) {
                    branch_node(static_cast<ushort>(nodes.size() - 1));
                }
            }

            for(auto tile : tiles) {
                world->setTilePath(tile.x, tile.y, id, newId);
            }

        }
        else {
            x = temp_x;
            y = temp_y;
            direction = temp_dir;
        }
    }
    if(Settings::get<bool>("debug_path")) world->clearCache();
}


void Pathfinder::delete_branch(int x, int y, ushort node0, ushort node1) {
    if((world->getLoadedTile(x, y).node0 == node0 && world->getLoadedTile(x, y).node1 == node1)
       || (world->getLoadedTile(x, y).node0 == node1 && world->getLoadedTile(x, y).node1 == node0)) {
        world->setTilePath(x, y, 0, 0);
        delete_branch(x - 1, y, node0, node1);
        delete_branch(x + 1, y, node0, node1);
        delete_branch(x, y + 1, node0, node1);
        delete_branch(x, y - 1, node0, node1);
    }
}

void Pathfinder::updated_block(int x, int y) {

    std::set<ushort> nodes_to_branch;
    std::set<ushort> nodes_to_delete;

    auto check = [&](int x, int y) {
        if(world->getTile(x, y).node != 0) {
            auto node = world->getTile(x, y).node;
            nodes_to_delete.insert(node);
            for(auto connection : nodes[node].connections) {
                if(connection != 0) {
                    nodes_to_branch.insert(connection);
                    delete_branch(x - 1, y, node, connection);
                    delete_branch(x + 1, y, node, connection);
                    delete_branch(x, y + 1, node, connection);
                    delete_branch(x, y - 1, node, connection);
                }
            }
            nodes[node] = {false, 0, 0, {0, 0, 0, 0}, {0, 0, 0, 0}};
            world->setTileNode(x, y, 0);
        }
        if(world->getTile(x, y).node0 != 0) {
            nodes_to_branch.insert(world->getTile(x, y).node0);
            nodes_to_branch.insert(world->getTile(x, y).node1);
            nodes_to_delete.insert(world->getTile(x, y).node0);
            nodes_to_delete.insert(world->getTile(x, y).node1);
            delete_branch(x, y, world->getTile(x, y).node0, world->getTile(x, y).node1);
        }
    };

    check(x, y + 2);
    check(x - 1, y + 1);
    check(x, y + 1);
    check(x + 1, y + 1);
    check(x - 1, y);
    check(x, y);
    check(x + 1, y);
    check(x - 1, y - 1);
    check(x, y - 1);
    check(x + 1, y - 1);

    for(auto node : nodes_to_branch) {
        for(auto& connection : nodes[node].connections) {
            for(auto node_d : nodes_to_delete) {
                if(connection == node_d) {
                    connection = 0;
                }
            }
        }
    }
    for(auto node : nodes_to_branch) {
        branch_node(node);
    }
}


void Pathfinder::setWorld(World* world) {
    this->world = world;
}

void Pathfinder::save() {
    std::ofstream file(Settings::get<std::string>("save_path") + "nodes.td");
    if(file.good()) {
        std::string output;
        for(auto node : nodes) {
            output += std::to_string(node.enabled) + "\n";
            output += std::to_string(node.x) + "\n";
            output += std::to_string(node.y) + "\n";
            for(auto connection : node.connections) {
                output += std::to_string(connection) + "\n";
            }
            for(auto length : node.lengths) {
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
            for(ushort& connection : node.connections) {
                connection = (ushort)std::stoi(line);
                getline(file, line);
            }
            for(ushort& lenght : node.lengths) {
                lenght = (ushort)std::stoi(line);
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




