//
// Created by piotr on 13.04.18.
//


#ifndef NONAME_ENTITY_H
#define NONAME_ENTITY_H

#include <SFML/Graphics.hpp>
#include "../utils/json.hpp"


class Entity {

    /*
     * Skupic sie na NPC, cos ala Prison Architect, zatrudnie gornikow, komunikacja
     * mapa ID do
     * */

public:
    Entity(int ID, std::string name, sf::Vector2f position, sf::RectangleShape hitbox, sf::Sprite sprite);
    Entity(nlohmann::json);

    virtual void tick() =0;
    virtual void render() =0;
    virtual void update() =0;

    int ID;
    std::string name;
    sf::Vector2f position;
    sf::RectangleShape hitbox;
    sf::Sprite sprite;

};


#endif //NONAME_ENTITY_H
