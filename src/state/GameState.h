#ifndef NONAME_GAMESTATE_H
#define NONAME_GAMESTATE_H

#include "MinerMode.h"
#include "ArchitectMode.h"
#include "ManagementMode.h"
#include "State.h"
#include "../world/World.h"
#include "../entity/Entity.h"
#include "../entity/systems/System.h"
#include "../entity/systems/RenderSystem.h"
#include "../entity/EntityFactory.h"
#include "GameMode.h"

//! State when actual gameplay is present
/*!
 * Contains World, Entities etc. Get's player input. Runs physics.
 */
class GameState : public State{

public:
    static constexpr auto TAG = "GameState";

public:
    void update(std::chrono::microseconds deltaTime) override;
    void render(float deltaTime) override;
    void tick() override;
    void handleEvent(sf::Event event) override;

    //! Converts screen to world coordinates using player camera
    sf::Vector2f screen_to_global_offset(sf::Vector2f in);

    GameState();
    ~GameState() override;

    //! Returns reference to world
    World& getWorld();
    sf::View& getCamera();

    /*!
     * Returns pointer to the active game mode
     */
    std::shared_ptr<GameMode> getGameMode();

    /*!
     * Changes game mode, for list of game modes see GameMode
     * @param newMode Get newMode value from GameMode::gameModesEnum
     */
    void setGameMode(uint newMode);

    //! Returns a pointer to a GUI object
    std::shared_ptr<GUI> getGUI();

  private:
    World world;

    //! Vector containing all the entities currently active
    std::vector<std::shared_ptr<Entity>> entities;

    //! Vector containing entity/object systems
    std::vector<std::unique_ptr<System>> systems;

    //! Pointer to player entity
    std::shared_ptr<Entity> player;

    //! Camera centered at player
    sf::View camera;

    EntityFactory entityFactory;

    std::shared_ptr<GUI> gui;

    //! A pointer to the active game mode, default game mode for now is architect mode
    std::shared_ptr<GameMode> gameMode;

    //! Container for every game mode
    std::vector<std::shared_ptr<GameMode>> gameModes;

    //! Saves all entities to a file
    void saveEntities();
    //! Loads all entities from file \return True if succeeds
    bool loadEntities();

    //! Creates directory for save
    void createSavePath();
    //! Pushes all entity/object systems to system vector
    void loadSystems();

};

#endif //NONAME_GAMESTATE_H
