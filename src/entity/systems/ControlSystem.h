#ifndef NONAME_CONTROLSYSTEM_H
#define NONAME_CONTROLSYSTEM_H

#include "System.h"
#include "../components/Position.h"
#include "../components/Control.h"
#include "../../utils/Controls.h"
#include "../components/Mining.h"

class ControlSystem : public System{
    GameState& gameState;
public:
    stageEnum getStage() override { return update; };
    void processEntity(std::shared_ptr<Entity> entity, std::chrono::microseconds dt) override {
        auto* positionComponent = entity->getComponent<PositionComponent>();
        auto* controlComponent = entity->getComponent<ControlComponent>();
        if(!positionComponent || !controlComponent)
            return;

        Game::get().debug.reportPlayerEntity(entity);

        if (Controls::isKeyboardButtonPressed(sf::Keyboard::W)) {
            positionComponent->speed.y = controlComponent->speed;
        }
        else if (Controls::isKeyboardButtonPressed(sf::Keyboard::S)) {
            positionComponent->speed.y = -controlComponent->speed;
        }
        else {
            positionComponent->speed.y = 0;
        }
        if (Controls::isKeyboardButtonPressed(sf::Keyboard::A)) {
            positionComponent->speed.x = -controlComponent->speed;
        }
        else if (Controls::isKeyboardButtonPressed(sf::Keyboard::D)) {
            positionComponent->speed.x = controlComponent->speed;
        }
        else {
            positionComponent->speed.x = 0;
        }

        auto* miningComponent = entity->getComponent<MiningComponent>();
        if (miningComponent && Controls::isMouseButtonPressed(sf::Mouse::Left)) {
            sf::Vector2f position = sf::Vector2f(Controls::getMousePosition());
            position = gameState.screen_to_global_offset(position);
            sf::Vector2i tile = sf::Vector2i(position / (float)Chunk::TILE_SIZE);
            if (position.x < 0) tile.x -= 1;
            if (position.y < 0) tile.y -= 1;
            miningComponent->miningTarget = tile;
            miningComponent->miningInProgress = true;
            miningComponent->miningLeft = TileDatabase::get()[gameState.getWorld().getTile(tile.x, tile.y).tileId].hardness;
        }
    }
    ControlSystem(GameState& _gameState) : gameState(_gameState){
    }
};

#endif //NONAME_CONTROLSYSTEM_H
