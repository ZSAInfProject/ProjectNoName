#include <SFML/Window/Mouse.hpp>
#include <iostream>
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
}

void GameState::update(std::chrono::microseconds deltaTime) {
    auto currentCameraPos = camera.getCenter();
    //camera.setRotation(static_cast<float>(sin(camera.getCenter().x / 2000) * 360.0));
    //camera.setCenter(currentCameraPos+sf::Vector2f(1*deltaTime.count()/10000.0,0.0));

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2f position = sf::Vector2f(sf::Mouse::getPosition(Game::get().getRenderWindow()));
        position.y = Game::get().getRenderWindow().getSize().y - position.y;
        position -= sf::Vector2f(Game::get().getRenderWindow().getSize())/2.0f;
        position.x *= camera.getSize().x / Game::get().getRenderWindow().getSize().x;
        position.y *= -camera.getSize().y / Game::get().getRenderWindow().getSize().y;
        position += currentCameraPos;

        sf::Vector2i tile = sf::Vector2i(position / (float)Chunk::TILE_SIZE);
        if (position.x < 0) tile.x -= 1;
        if (position.y < 0) tile.y -= 1;
        world.setTile(tile.x, tile.y, 1);

    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        sf::Vector2f position = sf::Vector2f(sf::Mouse::getPosition(Game::get().getRenderWindow()));
        position.y = Game::get().getRenderWindow().getSize().y - position.y;
        position -= sf::Vector2f(Game::get().getRenderWindow().getSize())/2.0f;
        position.x *= camera.getSize().x / Game::get().getRenderWindow().getSize().x;
        position.y *= -camera.getSize().y / Game::get().getRenderWindow().getSize().y;
        position += currentCameraPos;

        sf::Vector2i tile = sf::Vector2i(position / (float)Chunk::TILE_SIZE);
        if (position.x < 0) tile.x -= 1;
        if (position.y < 0) tile.y -= 1;
        world.setTile(tile.x, tile.y, 2);

    }

}

void GameState::render(sf::RenderWindow *renderWindow) {
    renderWindow->setView(camera);
    world.render(*renderWindow, camera);
    renderWindow->setView(renderWindow->getDefaultView());
}
void GameState::tick() {
}


