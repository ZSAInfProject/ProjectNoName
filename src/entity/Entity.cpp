#include "Entity.h"
#include "../utils/Log.h"
#include "../Game.h"
#include <utility>

Entity::Entity(nlohmann::json json, GameState& game_state) : game_state(game_state) {
    if(json.find("ID") != json.end()){
        ID = json["ID"].get<int>();
    }

    if(json.find("name") != json.end()){
        ID = json["name"].get<int>();
    }

    if (json.find("position") != json.end()) {
        position.x = json["position"][0].get<float>();
        position.y = json["position"][1].get<float>();
    }
    else {
        position.x = 0;
        position.y = 0;
    }

    if (json.find("hitbox") != json.end()) {
        hitbox.left = json["hitbox"][0].get<float>();
        hitbox.top = json["hitbox"][1].get<float>();
        hitbox.width = json["hitbox"][2].get<float>();
        hitbox.height = json["hitbox"][3].get<float>();
    }
    else {
        hitbox.left = 0;
        hitbox.top = 0;
        hitbox.width = 0;
        hitbox.height = 0;
    }

    if (json.find("sprite") != json.end()) {
        std::string path = json["sprite"].get<std::string>();
        sf::Texture texture;
        texture.loadFromFile(path);
        sprite.setTexture(texture);
    }
}

Entity::Entity(int ID, std::string name, sf::Vector2f position,
               sf::FloatRect hitbox, sf::Texture texture, GameState& game_state) : game_state(game_state) {
   this->ID = ID;
   this->name = std::move(name);
   this->texture = texture;
   this->sprite = sf::Sprite(texture);
   this->hitbox = hitbox;
   this->position = position;
}

void Entity::render() {
    sprite.setPosition(position.x, position.y);
    Game::getRenderWindow().draw(sprite);
}

void Entity::tick() {

}

void Entity::update(std::chrono::microseconds deltaTime) {
}

