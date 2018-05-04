#include <SFML/Window/Mouse.hpp>
#include <iostream>
#include <SFML/Window/Keyboard.hpp>
#include "GameState.h"
#include "../tile/TileDatabase.h"
#include "../Game.h"
#include "../utils/Log.h"

long mod(long a, long b)
{ return (a%b+b)%b; }

GameState::GameState() : State(), world(10){
    camera.setSize(sf::Vector2f(800, -600));
    camera.setCenter(sf::Vector2f(0, 0));
    TileDatabase::get().loadTiles("tiles.json");
    TileDatabase::get().loadTexture("texture.png");
    entities.push_back(std::make_unique<Player>());
    entities[0]->gameState = this;
}

void GameState::update(std::chrono::microseconds deltaTime) {
    camera.setCenter(entities[0]->position);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2f position = sf::Vector2f(sf::Mouse::getPosition(Game::get().getRenderWindow()));

        position = screen_to_global_offset(position);
        sf::Vector2i tile = sf::Vector2i(position / (float)Chunk::TILE_SIZE);
        if (position.x < 0) tile.x -= 1;
        if (position.y < 0) tile.y -= 1;
        world.mineTile(tile.x, tile.y);

    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        sf::Vector2f position = sf::Vector2f(sf::Mouse::getPosition(Game::get().getRenderWindow()));

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

void GameState::render(sf::RenderWindow *renderWindow) {
    renderWindow->setView(camera);
    world.render(*renderWindow, camera);

    for (auto& entity : entities) {
        entity->render(renderWindow);
    }

    renderWindow->setView(renderWindow->getDefaultView());
}
void GameState::tick() {
    for (auto& entity : entities) {
        entity->tick();
    }

}

sf::Vector2f GameState::screen_to_global_offset(sf::Vector2f in) {
    auto out = in;
    out.y = Game::get().getRenderWindow().getSize().y - in.y;
    out -= sf::Vector2f(Game::get().getRenderWindow().getSize())/2.0f;
    out.x *= camera.getSize().x / Game::get().getRenderWindow().getSize().x;
    out.y *= -camera.getSize().y / Game::get().getRenderWindow().getSize().y;
    out += camera.getCenter();
    return out;
}


