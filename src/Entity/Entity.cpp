//
// Created by piotr on 13.04.18.
//

#include "Entity.h"


Entity::Entity(nlohmann::json json) {
    if(json.find("ID") != json.end()){
        ID = json["ID"].get<int>();
    }
    if(json.find("name") != json.end()){
        ID = json["name"].get<int>();
    }
    if (json.find("position") != json.end()) {
        position.x = json["position"][0].get<int>();
        position.y = json["position"][1].get<int>();
    }
    else{
        position.x = 0;
        position.y = 0;
    }
    if (json.find("hitbox") != json.end()) {
        int v1 = json["position"][0].get<int>();
        int v2 = json["position"][1].get<int>();
        int v3 = json["position"][2].get<int>();
        int v4 = json["position"][3].get<int>();
        hitbox.setPosition(v1,v2);
        hitbox.setPosition(v3,v4);
    }
    else{
        int v1 = 0; int v2 = 0; int v3 = 10; int v4 = 10;
        hitbox.setPosition(v1,v2);
        hitbox.setPosition(v3,v4);
    }
    if (json.find("sprite") != json.end()) {
        std::string adres = json["sprite"].get<std::string>();
        sf::Texture texture;
        texture.loadFromFile(adres);
        sprite.setTexture(texture);
    }



}

Entity::Entity(int ID, std::string name, sf::Vector2f position,sf::RectangleShape hitbox, sf::Sprite sprite) {
   this->ID = ID;
   this->name = name;
   this->sprite = sprite;
   this->hitbox = hitbox;
   this->position = position;
}

