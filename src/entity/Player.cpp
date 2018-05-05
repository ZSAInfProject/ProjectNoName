#include "Player.h"
#include <utility>
#include "../utils/Log.h"
#include "../Game.h"
#include "../state/GameState.h"
#include "../tile/TileDatabase.h"

Player::Player(GameState& game_state)
        : Entity(0, std::string("Player"), sf::Vector2f(0,100), sf::FloatRect(0, 32, 16, 32), sf::Texture(), game_state) {

    texture.loadFromFile("res/textures/player.png");
    sprite = sf::Sprite(texture);
    sprite.scale(0.5f, 0.5f);

    speed = {0.0f, 0.0f};
}

void Player::update(std::chrono::microseconds deltaTime) {

    auto div = [](int a, int b)->int{int ret = a/b; return ret>=0? ret: ret-1;};

    const float MOVEMENT_DIV = 2000.0f;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) && touching_ground){
        speed.y = 1.0f;
        touching_ground = false;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
        position += sf::Vector2f(-deltaTime.count()/MOVEMENT_DIV, 0.0f);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        position += sf::Vector2f(deltaTime.count()/MOVEMENT_DIV, 0.0f);
    }

    if (!touching_ground) {
        speed.y -= 0.005f * deltaTime.count() / MOVEMENT_DIV;
    }

    position.y += deltaTime.count() * speed.y / MOVEMENT_DIV;

    if (TileDatabase::get()[game_state.getWorld().getTile((int)position.x / Chunk::TILE_SIZE, div((int)position.y, Chunk::TILE_SIZE)).tileId].isSolid) {
        touching_ground = true;
        speed.y = 0;
        position.y = (int)(position.y / Chunk::TILE_SIZE) * Chunk::TILE_SIZE;
        if (TileDatabase::get()[game_state.getWorld().getTile((int)position.x / Chunk::TILE_SIZE, (int)position.y / Chunk::TILE_SIZE).tileId].isSolid) {
            position.y = ((int)(position.y / Chunk::TILE_SIZE) + 1 )* Chunk::TILE_SIZE;
        }
    }
    else {
        touching_ground = false;
    }
}
