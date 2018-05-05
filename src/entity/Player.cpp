#include "Player.h"
#include <utility>
#include "../utils/Log.h"
#include "../Game.h"
#include "../state/GameState.h"
#include "../tile/TileDatabase.h"

Player::Player(GameState& game_state)
        : Entity(0, std::string("Player"), sf::Vector2f(0,0), sf::FloatRect(0, 32, 16, 32), sf::Texture(), game_state) {

    texture.loadFromFile("res/textures/player.png");
    sprite = sf::Sprite(texture);
    sprite.scale(0.5f, 0.5f);

    speed = {0.0f, 0.0f};
}

void Player::update(std::chrono::microseconds deltaTime) {

    const float MOVEMENT_DIV = 2000.0f;

    if((sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) && touching_ground){
        speed.y = 1.0f;
        touching_ground = false;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && speed.x > -0.6f){
        speed.x -= 0.005f * deltaTime.count() / MOVEMENT_DIV;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) && speed.x < 0.6f){
        speed.x += 0.005f * deltaTime.count() / MOVEMENT_DIV;
    }
    if(!sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        if (fabs(speed.x) > 0.005f * deltaTime.count() / MOVEMENT_DIV) {
            speed.x += speed.x > 0 ? (-0.005f * deltaTime.count() / MOVEMENT_DIV) : (0.005f * deltaTime.count() / MOVEMENT_DIV);
        } else {
            speed.x = 0;
        }
    }


    auto div = [](float a, int b)->int{int ret = a/b; return a>=0 ? ret : ret-1;};


    float old_pos_x = position.x;
    position.x += speed.x * deltaTime.count() / MOVEMENT_DIV;

    Tile tile_d = TileDatabase::get()[game_state.getWorld().getTile(                           //Collision detection X
            div(position.x + sprite.getGlobalBounds().width/2, Chunk::TILE_SIZE),
            div(position.y, Chunk::TILE_SIZE)).tileId];
    Tile tile_u = TileDatabase::get()[game_state.getWorld().getTile(
            div(position.x + sprite.getGlobalBounds().width/2, Chunk::TILE_SIZE),
            div(position.y, Chunk::TILE_SIZE) + 1).tileId];

    if (tile_d.isSolid && !tile_u.isSolid) {
        position.y = (div(position.y, Chunk::TILE_SIZE) + 1) * Chunk::TILE_SIZE + 0.0001f;
    }
    if (tile_d.isSolid && tile_u.isSolid) {
        position.x = old_pos_x;
        speed.x = 0;
    }

    speed.y -= 0.005f * deltaTime.count() / MOVEMENT_DIV; //Gravity

    float old_pos_y = position.y;
    position.y += speed.y * deltaTime.count() / MOVEMENT_DIV;

    if (speed.y < 0) {
        Tile tile = TileDatabase::get()[game_state.getWorld().getTile(                           //Collision detection Y down
                div(position.x + sprite.getGlobalBounds().width / 2, Chunk::TILE_SIZE),
                div(position.y, Chunk::TILE_SIZE)).tileId];

        if (tile.isSolid) {
            position.y = old_pos_y;
            touching_ground = true;
            speed.y = 0;
        } else {
            touching_ground = false;
        }
    }
    else {
        Tile tile = TileDatabase::get()[game_state.getWorld().getTile(                           //Collision detection Y up
                div(position.x + sprite.getGlobalBounds().width / 2, Chunk::TILE_SIZE),
                div(position.y, Chunk::TILE_SIZE) + 2).tileId];
        if (tile.isSolid) {
            position.y = old_pos_y;
            speed.y = 0;
        }
    }
}
