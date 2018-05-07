#include <SFML/Window/Mouse.hpp>
#include <iostream>
#include <SFML/Window/Keyboard.hpp>
#include "GameState.h"
#include "../tile/TileDatabase.h"
#include "../Game.h"
#include "../utils/Log.h"
#include "../utils/Settings.h"
#include "../utils/Controls.h"

#ifdef __unix__
#include <sys/stat.h>
#include <sys/types.h>
#else
#error "System not supported."
#endif

long mod(long a, long b)
{ return (a%b+b)%b; }

GameState::GameState() : State(), world(10){
    camera.setSize(sf::Vector2f(800, -600));
    createSavePath();
    camera.setCenter(sf::Vector2f(0, 0));
    TileDatabase::get().loadTiles("tiles.json");
    TileDatabase::get().loadTexture("texture.png");

    player = std::make_shared<Player>(*this);
    entities.emplace_back(player);
}

void GameState::update(std::chrono::microseconds deltaTime) {
    camera.setCenter(entities[0]->position);

    if (Controls::isMouseButtonPressed(sf::Mouse::Left)) {
        sf::Vector2f position = sf::Vector2f(Controls::getMousePosition());

        position = screen_to_global_offset(position);
        sf::Vector2i tile = sf::Vector2i(position / (float)Chunk::TILE_SIZE);
        if (position.x < 0) tile.x -= 1;
        if (position.y < 0) tile.y -= 1;
        world.mineTile(tile.x, tile.y);

    }
    if (Controls::isMouseButtonPressed(sf::Mouse::Right)) {
        sf::Vector2f position = sf::Vector2f(Controls::getMousePosition());

        position = screen_to_global_offset(position);
        sf::Vector2i tile = sf::Vector2i(position / (float)Chunk::TILE_SIZE);
        if (position.x < 0) tile.x -= 1;
        if (position.y < 0) tile.y -= 1;
        world.setTile(tile.x, tile.y, {2,1});

    }

    for (auto& entity : entities) {
        entity->update(deltaTime);
    }

}

void GameState::render() {
    Game::getRenderWindow().setView(camera);
    world.render(camera);

    for (auto& entity : entities) {
        entity->render();
    }

    Game::getRenderWindow().setView(Game::getRenderWindow().getDefaultView());
}
void GameState::tick() {
    for (auto& entity : entities) {
        entity->tick();
    }

}

sf::Vector2f GameState::screen_to_global_offset(sf::Vector2f in) {
    auto out = in;
    out.y = Game::getRenderWindow().getSize().y - in.y;
    out -= sf::Vector2f(Game::getRenderWindow().getSize())/2.0f;
    out.x *= camera.getSize().x / Game::getRenderWindow().getSize().x;
    out.y *= -camera.getSize().y / Game::getRenderWindow().getSize().y;
    out += camera.getCenter();
    return out;
}

void GameState::createSavePath() {
#ifdef __unix__
    auto makeDir = [](const char* path) {
        if (mkdir(path, 0777) && errno != EEXIST) {
            Log::error(TAG, "Failed to create save directory " + std::to_string(errno));
        }
    };
    std::string savePath = Settings::get<std::string>("save_path");
    makeDir(savePath.c_str());
    makeDir((savePath+"chunks/").c_str());

#else
#error "System not supported."
#endif
}

World &GameState::getWorld() {
    return world;
}


