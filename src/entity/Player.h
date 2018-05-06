#ifndef NONAME_PLAYER_H
#define NONAME_PLAYER_H


#include "Entity.h"

class Player : public Entity {
public:
    Player(GameState& game_state);

    void update(std::chrono::microseconds deltaTime) override;

    sf::Vector2f speed;

    bool touching_ground = false;
};


#endif //NONAME_PLAYER_H
