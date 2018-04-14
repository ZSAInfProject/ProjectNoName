#include "GameState.h"
#include "../tile/TileDatabase.h"

GameState::GameState() : State(), world(10){
    camera.reset(sf::FloatRect(-1000, 1000, 2000, -2000));
    TileDatabase::get().loadTiles("tiles.json");
    TileDatabase::get().loadTexture("texture.png");
}

void GameState::update(std::chrono::microseconds deltaTime) {
    auto currentCameraPos = camera.getCenter();
    //camera.setRotation(static_cast<float>(sin(camera.getCenter().x / 2000) * 360.0));
    camera.setCenter(currentCameraPos+sf::Vector2f(1*deltaTime.count()/10000.0,0.0));
}

void GameState::render(sf::RenderWindow *renderWindow) {
    renderWindow->setView(camera);
    world.render(*renderWindow, camera);
    renderWindow->setView(renderWindow->getDefaultView());
}
void GameState::tick() {
}
