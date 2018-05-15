#include <SFML/Window/Mouse.hpp>
#include <iostream>
#include <SFML/Window/Keyboard.hpp>
#include "GameState.h"
#include "../tile/TileDatabase.h"
#include "../Game.h"
#include "../utils/Log.h"
#include "../utils/Settings.h"
#include "../utils/Controls.h"
#include "../entity/systems/MotionSystem.h"
#include "../entity/systems/ControlSystem.h"
#include "../entity/systems/RenderSystem.h"
#include "../entity/systems/CameraSystem.h"

#ifdef __unix__
#include <sys/stat.h>
#include <sys/types.h>
#else
#error "System not supported."
#endif

long mod(long a, long b)
{ return (a%b+b)%b; }

GameState::GameState() : State(), world(10), entityFactory("entities/entities.json"){
    camera.setSize(sf::Vector2f(800, -600));
    createSavePath();
    loadSystems();
    camera.setCenter(sf::Vector2f(0, 0));
    TileDatabase::get().loadTiles("tiles.json");
    TileDatabase::get().loadTexture("texture.png");
    entities.push_back(entityFactory.get("Player"));
}

void GameState::update(std::chrono::microseconds deltaTime) {
    for(auto& system : systems){
        if(system->stage == stageEnum::update) {
            for (auto &entity : entities) {
                system->processEntity(entity.get(), deltaTime);
            }
        }
    }

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

}

void GameState::render() {
    Game::getRenderWindow().setView(camera);
    for(auto& system : systems){
        if(system->stage == stageEnum::render) {
            for (auto &entity : entities)
                system->processEntity(entity.get());
        }
    }
    world.render(camera);
    Game::getRenderWindow().setView(Game::getRenderWindow().getDefaultView());
}
void GameState::tick() {
    for(auto& system : systems){
        if(system->stage == stageEnum::tick) {
            for (auto &entity : entities)
                system->processEntity(entity.get());
        }
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

sf::View &GameState::getCamera() {
    return camera;
}

void GameState::loadSystems() {
    systems.push_back(std::make_unique<ControlSystem>());
    systems.push_back(std::make_unique<MotionSystem>());
    systems.push_back(std::make_unique<CameraSystem>(camera));
    systems.push_back(std::make_unique<RenderSystem>());
}


