#ifndef NONAME_GAMESTATE_H
#define NONAME_GAMESTATE_H

#include "State.h"
#include "../world/World.h"
#include "../entity/Entity.h"
#include "../entity/Player.h"

//! State when actual gameplay is present
/*!
 * Contains World, Entities etc. Get's player input. Runs physics.
 */
class GameState : public State{

    void createSavePath();
    static constexpr auto TAG = "GameState";
  
public:
    void update(std::chrono::microseconds deltaTime) override;
    void render() override;
    void tick() override;

    GameState();

    World& getWorld();

private:
    World world;

    //! Converts screen to world coordinates
    sf::Vector2f screen_to_global_offset(sf::Vector2f in);

    //! Vector containing all the entities currently active
    std::vector<std::shared_ptr<Entity>> entities;

    std::shared_ptr<Player> player;

    sf::View camera;
};

#endif //NONAME_GAMESTATE_H
