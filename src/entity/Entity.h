#ifndef NONAME_ENTITY_H
#define NONAME_ENTITY_H

#include <SFML/Graphics.hpp>
#include <chrono>
#include "../utils/json.hpp"

//! In game moving entity
/*!
 * Represents entity - object in game that can move. It's position is not quantized.
 */
class Entity {

public:
    Entity(int ID, std::string name, sf::Vector2f position, sf::RectangleShape hitbox, sf::Sprite sprite);
    explicit Entity(nlohmann::json);

    void tick();
    void render(sf::RenderWindow *window);
    void update(std::chrono::microseconds deltaTime);

    int ID;
    std::string name;
    //! Position in world coordinates
    sf::Vector2f position;
    //! Hitbox (position with respect to entity origin, also sprite origin)
    sf::RectangleShape hitbox;
    sf::Sprite sprite;

};


#endif //NONAME_ENTITY_H
