#ifndef NONAME_ENTITY_H
#define NONAME_ENTITY_H

#include <SFML/Graphics.hpp>
#include "../utils/json.hpp"

//! In game moving entity
/*!
 * Represents entity - object in game that can move. It's position is not quantized.
 */
class Entity {

public:
    Entity(int ID, std::string name, sf::Vector2f position, sf::RectangleShape hitbox, sf::Sprite sprite);
    explicit Entity(nlohmann::json);

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
