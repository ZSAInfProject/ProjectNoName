#ifndef NONAME_BUILDING_H
#define NONAME_BUILDING_H

#include "Component.h"
#include "../../world/World.h"

class BuildingComponent : public Component {
public:
    static const componentId Id = componentId::Building;
    
    bool buildScheduled;
    sf::Vector2i buildingTarget;
    ChunkTile tile;

    nlohmann::json serialize() override {
        nlohmann::json json;
        json["type"] = "Building";
        return json;
    } 
    
    std::unique_ptr<Component> clone() override {
        return std::make_unique<BuildingComponent>(*this);
    }

    explicit BuildingComponent(nlohmann::json json){
        buildScheduled = false;
        buildingTarget = sf::Vector2i(0, 0);
        tile = {2,1};
    }
};

#endif
