#ifndef NONAME_ENTITY_H
#define NONAME_ENTITY_H

#include <SFML/Graphics.hpp>
#include <chrono>
#include "../utils/json.hpp"
#include "../state/State.h"

class GameState;

//! In game moving entity
/*!
 * Represents entity - object in game that can move. It's position is not quantized.
 */
class Entity {

public:
    Entity(int ID, std::string name, sf::Vector2f position, sf::FloatRect hitbox, sf::Texture texture,
           GameState& game_state);
    explicit Entity(nlohmann::json json, GameState& game_state);

    virtual void tick();
    virtual void render();
    virtual void update(std::chrono::microseconds deltaTime);

    int ID;
    std::string name;
    //! Position in world coordinates
    sf::Vector2f position;
    //! Hitbox (position with respect to entity origin, also sprite origin)
    sf::FloatRect hitbox;
    sf::Sprite sprite;
    sf::Texture texture;

    GameState& game_state;

};


#endif //NONAME_ENTITY_H
