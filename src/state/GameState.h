#ifndef NONAME_GAMESTATE_H
#define NONAME_GAMESTATE_H

#include "State.h"
#include "../world/World.h"

class GameState : public State{
    sf::View camera;
    World world;
public:
    void update(std::chrono::microseconds deltaTime) override;
    void render(sf::RenderWindow* renderWindow) override;
    void tick() override;

    GameState();
};

#endif //NONAME_GAMESTATE_H
