#include "Entity.h"
#include <utility>

Entity::Entity(nlohmann::json json) {
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
        auto v1 = json["hitbox"][0].get<float>();
        auto v2 = json["hitbox"][1].get<float>();
        auto v3 = json["hitbox"][2].get<float>();
        auto v4 = json["hitbox"][3].get<float>();
        hitbox.setPosition(v1,v2);
        hitbox.setSize(sf::Vector2f(v3,v4));
    }
    else {
        int v1 = 0; int v2 = 0; int v3 = 10; int v4 = 10;
        hitbox.setPosition(v1,v2);
        hitbox.setPosition(v3,v4);
    }

    if (json.find("sprite") != json.end()) {
        std::string path = json["sprite"].get<std::string>();
        sf::Texture texture;
        texture.loadFromFile(path);
        sprite.setTexture(texture);
    }
}

Entity::Entity(int ID, std::string name, sf::Vector2f position,sf::RectangleShape hitbox, sf::Sprite sprite) {
   this->ID = ID;
   this->name = std::move(name);
   this->sprite = std::move(sprite);
   this->hitbox = std::move(hitbox);
   this->position = position;
}

void Entity::render(sf::RenderWindow *window) {
    sprite.setPosition(position);
    window->draw(sprite);
}

void Entity::tick() {

}

void Entity::update(std::chrono::microseconds deltaTime) {

}

