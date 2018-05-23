#include <SFML/Window/Mouse.hpp>
#include <iostream>
#include <SFML/Window/Keyboard.hpp>
#include <memory>
#include "GameState.h"
#include "../tile/TileDatabase.h"
#include "../utils/Controls.h"
#include "../entity/systems/MotionSystem.h"
#include "../entity/systems/ControlSystem.h"
#include "../entity/systems/CameraSystem.h"
#include "../entity/systems/MiningSystem.h"
#include "../entity/systems/BuildingSystem.h"

#ifdef __unix__

#include <sys/stat.h>

#else
#error "System not supported."
#endif


long mod(long a, long b) { return (a % b + b) % b; }


GameState::GameState() : State(), world(10), entityFactory("entities/entities.json"), pathfinder() {
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
    gui = std::make_shared<GUI>(gameMode->getTag(), 1.f);

    pathfinder.setWorld(&world);
    pathfinder.generate(0, 0);
}

void GameState::update(std::chrono::microseconds deltaTime) {
    Game::get().debug.reportEntityCount(static_cast<int>(entities.size()));
    auto objects = world.getObjectsForUpdate();
    for(auto& system : systems) {
        if(system->getStage() == stageEnum::update) {
            for(auto& entity : entities) {
                system->processEntity(entity, deltaTime);

            }
            for(auto& object : objects) {
                system->processEntity(object, deltaTime);
            }
        }
    }
}

void GameState::render(float deltaTime) {
    auto& renderWindow = Game::getRenderWindow();
    renderWindow.setView(camera);
    world.render(camera);
    auto objects = world.getObjectsForUpdate();
    for(auto& system : systems) {
        if(system->getStage() == stageEnum::render) {
            for(auto& entity : entities)
                system->processEntity(entity);
            for(auto& object : objects) {
                system->processEntity(object);
            }
        }
    }

    renderWindow.setView(Game::getRenderWindow().getDefaultView());
    gui->display(renderWindow, deltaTime);
    pathfinder.render(Game::getRenderWindow());
    Game::getRenderWindow().setView(Game::getRenderWindow().getDefaultView());
}

void GameState::tick() {
    auto objects = world.getObjectsForUpdate();
    for(auto& system : systems) {
        if(system->getStage() == stageEnum::tick) {
            for(auto& entity : entities)
                system->processEntity(entity);
            for(auto& object : objects) {
                system->processEntity(object);
            }
        }
    }
}

sf::Vector2f GameState::screen_to_global_offset(sf::Vector2f in) {
    auto out = in;
    out.y = Game::getRenderWindow().getSize().y - in.y;
    out -= sf::Vector2f(Game::getRenderWindow().getSize()) / 2.0f;
    out.x *= camera.getSize().x / Game::getRenderWindow().getSize().x;
    out.y *= -camera.getSize().y / Game::getRenderWindow().getSize().y;
    out += camera.getCenter();
    return out;
}

void GameState::createSavePath() {
#ifdef __unix__
    auto makeDir = [](const char* path) {
        if(mkdir(path, 0777) && errno != EEXIST) {
            Log::error(TAG, "Failed to create save directory " + std::to_string(errno));
        }
    };
    std::string savePath = Settings::get<std::string>("save_path");
    makeDir(savePath.c_str());
    makeDir((savePath + "chunks/").c_str());

#else
#error "System not supported."
#endif
}

World& GameState::getWorld() {
    return world;
}

sf::View& GameState::getCamera() {
    return camera;
}

void GameState::loadSystems() {
    systems.push_back(std::make_unique<ControlSystem>(*this));
    systems.push_back(std::make_unique<MotionSystem>());
    systems.push_back(std::make_unique<MiningSystem>(*this));
    systems.push_back(std::make_unique<BuildingSystem>(*this));
    systems.push_back(std::make_unique<CameraSystem>(camera));
    systems.push_back(std::make_unique<RenderSystem>());
}

void GameState::saveEntities() {
    nlohmann::json json;
    for(unsigned int i = 0; i < entities.size(); i++) {
        json["entities"][i] = entities[i]->serialize();
    }
    std::string filename = Settings::get<std::string>("save_path") + "entities.json";
    std::ofstream entityDate(filename);
    std::string data = json.dump();
    entityDate.write(data.c_str(), data.size());
    if(entityDate.fail()) {
        Log::error(TAG, "Failed to save entities");
    }
    else {
        Log::info(TAG, "Entities saved");
    }
}

GameState::~GameState() {
    saveEntities();
}

bool GameState::loadEntities() {
    std::string filename = Settings::get<std::string>("save_path") + "entities.json";
    std::ifstream ifs(filename);
    if(ifs.is_open()) {
        nlohmann::json j = nlohmann::json::parse(ifs);
        std::vector<nlohmann::json> entityData = j["entities"];
        for(auto entity : entityData) {
            entities.push_back(std::make_shared<Entity>(entity));
        }
        return true;
    }
    return false;
}

void GameState::setGameMode(uint newMode) {
    if(gameMode->getTag() == newMode)
        return;
    gui->changeMode(newMode);
    gameMode = gameModes.at(newMode);
}

std::shared_ptr<GameMode> GameState::getGameMode() {
    return gameMode;
}

std::shared_ptr<GUI> GameState::getGUI() {
    return gui;
}

void GameState::handleEvent(sf::Event event) {
    gui->handleEvent(event);
}
