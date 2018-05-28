#ifndef NONAME_GAMESTATE_H
#define NONAME_GAMESTATE_H

#include "State.h"
#include "../world/World.h"
#include "../entity/Entity.h"
#include "../entity/systems/System.h"
#include "../entity/systems/RenderSystem.h"
#include "../entity/EntityFactory.h"

//! State when actual gameplay is present
/*!
 * Contains World, Entities etc. Get's player input. Runs physics.
 */
class GameState : public State{

    void saveEntities();
    void createSavePath();
    void loadSystems();
    bool loadEntities();
    static constexpr auto TAG = "GameState";
  
public:
    void update(std::chrono::microseconds deltaTime) override;
    void render() override;
    void tick() override;

    GameState();
    ~GameState();

    World& getWorld();
    sf::View& getCamera();

private:
    World world;

    //! Converts screen to world coordinates
    sf::Vector2f screen_to_global_offset(sf::Vector2f in);

    //! Vector containing all the entities currently active
    std::vector<std::shared_ptr<Entity>> entities;

    std::vector<std::unique_ptr<System>> systems;

    std::shared_ptr<Entity> player;

    sf::View camera;

    EntityFactory entityFactory;
};

#endif //NONAME_GAMESTATE_H
