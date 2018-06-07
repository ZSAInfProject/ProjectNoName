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

    if(!loadEntities()) {
        entities.push_back(entityFactory.get("Player"));
    }

    //! the same arrangement as in GameMode::gameModesEnum
    gameModes.push_back(std::shared_ptr<GameMode>(new MinerMode()));
    gameModes.push_back(std::shared_ptr<GameMode>(new ArchitectMode()));
    gameModes.push_back(std::shared_ptr<GameMode>(new ManagementMode()));
    gameMode = gameModes.at(GameMode::architectMode);
    gui = std::shared_ptr<GUI>(new GUI(gameMode->getTag(), 1.f));
}

void GameState::update(std::chrono::microseconds deltaTime) {
    Game::get().debug.reportEntityCount(static_cast<int>(entities.size()));
    for(auto& system : systems){
        if(system->getStage() == stageEnum::update) {
            for (auto &entity : entities) {
                system->processEntity(entity, deltaTime);
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

void GameState::render(float deltaTime) {
    auto& renderWindow = Game::getRenderWindow();
    
    renderWindow.setView(camera);
    world.render(camera);
    for(auto& system : systems){
        if(system->getStage() == stageEnum::render) {
            for (auto &entity : entities)
                system->processEntity(entity);
        }
    }
    renderWindow.setView(Game::getRenderWindow().getDefaultView());
    gui->display(renderWindow, deltaTime);
}
void GameState::tick() {
    for(auto& system : systems){
        if(system->getStage() == stageEnum::tick) {
            for (auto &entity : entities)
                system->processEntity(entity);
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

void GameState::saveEntities() {
    nlohmann::json json;
    for(int i = 0; i < entities.size(); i++){
        json["entities"][i] = entities[i]->serialize();
    }
    std::string filename = Settings::get<std::string>("save_path")+"entities.json";
    std::ofstream entityDate(filename);
    std::string data = json.dump();
    entityDate.write(data.c_str(), data.size());
    if(entityDate.fail()){
        Log::error(TAG, "Failed to save entities");
    }
    else{
        Log::info(TAG, "Entities saved");
    }
}

GameState::~GameState() {
    saveEntities();
}

bool GameState::loadEntities() {
    std::string filename = Settings::get<std::string>("save_path")+"entities.json";
    std::ifstream ifs(filename);
    if (ifs.is_open()) {
        nlohmann::json j = nlohmann::json::parse(ifs);
        std::vector<nlohmann::json> entityData = j["entities"];
        for(auto entity : entityData){
            entities.push_back(std::make_shared<Entity>(entity));
        }
        return true;
    }
    return false;
}

void GameState::setGameMode(int newMode) {
    if(gameMode->getTag() == newMode)
        return;
    gui->changeMode(newMode);
    gameMode = gameModes.at(newMode);
}

std::shared_ptr<GameMode> GameState::getGameMode() {
    return gameMode;
}

std::shared_ptr<GUI> GameState::getGUI()
{
    return gui;
}
