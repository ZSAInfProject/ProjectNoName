#include "GameState.h"
#include "../tile/TileDatabase.h"

GameState::GameState() : State(), world(10){
    camera.reset(sf::FloatRect(-400, 400, 1920, -1080));
    TileDatabase::get().loadTiles("tiles.json");
    TileDatabase::get().loadTexture("texture.png");
}

void GameState::update(std::chrono::microseconds deltaTime) {
    auto currentCameraPos = camera.getCenter();
    //camera.setRotation(static_cast<float>(sin(camera.getCenter().x / 2000) * 360.0));
    camera.setCenter(currentCameraPos+sf::Vector2f(1*deltaTime.count()/10000.0,0.0));
}

void GameState::render(sf::RenderWindow *renderWindow) {
    float aspectRatio = -1.0f*camera.getSize().y/camera.getSize().x;
    int yRes = static_cast<int>(1.0f * renderWindow->getSize().x * aspectRatio);
    float yFill = 1.0f*yRes/renderWindow->getSize().y;
    float yDisplacement = yFill < 1 ? (1.0f-yFill)/2 : 0;
    camera.setViewport(sf::FloatRect(0, yDisplacement, 1, yFill));

    renderWindow->setView(camera);
    world.render(*renderWindow, camera);
    renderWindow->setView(renderWindow->getDefaultView());
}
void GameState::tick() {
}
