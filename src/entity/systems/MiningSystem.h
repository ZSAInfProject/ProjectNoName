//
// Created by zakrent on 6/5/18.
//

#ifndef NONAME_MININGSYSTEM_H
#define NONAME_MININGSYSTEM_H

#include "System.h"
#include "../components/Control.h"
#include "../components/Inventory.h"
#include "../../utils/Controls.h"
#include "../../Game.h"
#include "../../world/chunk/Chunk.h"
#include "../../state/GameState.h"
#include "../../tile/TileDatabase.h"
#include "../components/Mining.h"

class MiningSystem : public System{
    GameState& gameState;

public:
    void processEntity(std::shared_ptr<Entity> entity, std::chrono::microseconds dt = std::chrono::microseconds(0)) override {
        auto* miningComponent = entity->getComponent<MiningComponent>();
        auto* inventoryComponent = entity->getComponent<InventoryComponent>();
        if(!inventoryComponent || !miningComponent)
            return;
        auto now = std::chrono::system_clock::now();
        auto mineTime = std::chrono::duration_cast<std::chrono::microseconds>(now - miningComponent->lastMine);
        if(mineTime > miningComponent->miningDuration && miningComponent->miningInProgress) {
            miningComponent->miningLeft -= 1;
            if(miningComponent->miningLeft <= 0){
                miningComponent->miningInProgress = false;
                int id = gameState.getWorld().mineTile(miningComponent->miningTarget.x, miningComponent->miningTarget.y);
                Resource res = TileDatabase::get()[id].drop;
                if (res.type != unknown) {
                    inventoryComponent->resources[res.type] += res.amount;
                }
            }
            miningComponent->lastMine = now;
        }
    }
    stageEnum getStage() override { return stageEnum::update; };

    MiningSystem(GameState& _gameState) : gameState(_gameState){
    };
};

#endif //NONAME_MININGSYSTEM_H
