#ifndef NONAME_GAMESTATE_H
#define NONAME_GAMESTATE_H

#include "State.h"
#include "../world/World.h"

//! State when actual gameplay is present
/*!
 * Contains World, Entities etc. Get's player input. Runs physics.
 */
class GameState : public State{
    sf::View camera;
    World world;
    void createSavePath();
    static constexpr auto TAG = "GameState";
public:
    void update(std::chrono::microseconds deltaTime) override;
    void render(sf::RenderWindow* renderWindow) override;
    void tick() override;

    GameState();

private:
    //! Converts screen to world coordinates
    sf::Vector2f screen_to_global_offset(sf::Vector2f in);
};

#endif //NONAME_GAMESTATE_H
