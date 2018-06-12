#ifndef NONAME_BUILDINGSYSTEM_H
#define NONAME_BUILDINGSYSTEM_H

#include "../components/Building.h"

class BuildingSystem : public System{
    GameState& gameState;
    public:
    void processEntity(std::shared_ptr<Entity> entity, std::chrono::microseconds dt) override {
        auto* buildingComponent = entity->getComponent<BuildingComponent>();
        auto* inventoryComponent = entity->getComponent<InventoryComponent>();
        if(!buildingComponent || !inventoryComponent)
            return;

        if(buildingComponent->buildScheduled){
            if(gameState.getWorld().getTile(buildingComponent->buildingTarget.x, buildingComponent->buildingTarget.y).tileId == 1){
                Resource cost = TileDatabase::get()[buildingComponent->tile.tileId].buildCost;
                if(inventoryComponent->resources[cost.type] >= cost.amount){
                    inventoryComponent->resources[cost.type] -= cost.amount;
                    gameState.getWorld().setTile(buildingComponent->buildingTarget.x, buildingComponent->buildingTarget.y, buildingComponent->tile);
                }
            }
            buildingComponent->buildScheduled = false;
        }
    }

    stageEnum getStage() override { return stageEnum::update; };

    BuildingSystem(GameState& _gameState) : gameState(_gameState){
    };
};

#endif
