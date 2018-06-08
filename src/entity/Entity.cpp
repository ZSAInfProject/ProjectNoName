#include <utility>
#include "Entity.h"
#include "components/Position.h"
#include "components/Sprite.h"
#include "components/Control.h"
#include "components/Camera.h"
#include "components/ObjectPosition.h"
#include "../utils/Log.h"
#include "components/Inventory.h"
#include "components/Mining.h"

Entity::Entity(nlohmann::json json) {
    ID = 0;
    if (json.find("name") != json.end()) {
        name = json["name"].get<std::string>();
        if (json.find("components") != json.end()) {
            for (auto &component : json["components"]) {
                if (component.find("type") != component.end()) {
                    bool enabled;
                    if (component.find("enabled") != component.end()) {
                        enabled = component["enabled"].get<bool>();
                    }
                    else {
                        enabled = true;
                    }

                    if (component["type"].get<std::string>() == "Position")
                        addComponent<PositionComponent>(std::make_unique<PositionComponent>(component), enabled);
                    else if (component["type"].get<std::string>() == "Sprite")
                        addComponent<SpriteComponent>(std::make_unique<SpriteComponent>(component), enabled);
                    else if (component["type"].get<std::string>() == "Control")
                        addComponent<ControlComponent>(std::make_unique<ControlComponent>(component), enabled);
                    else if (component["type"].get<std::string>() == "Camera")
                        addComponent<CameraComponent>(std::make_unique<CameraComponent>(component), enabled);
                    else if (component["type"].get<std::string>() == "Inventory")
                        addComponent<InventoryComponent>(std::make_unique<InventoryComponent>(component), enabled);
                    else if (component["type"].get<std::string>() == "Mining")
                        addComponent<MiningComponent>(std::make_unique<MiningComponent>(component), enabled);
                    else if (component["type"].get<std::string>() == "ObjectPosition")
                        addComponent<ObjectPositionComponent>(std::make_unique<ObjectPositionComponent>(component), enabled);
                    else {
                        Log::warn(TAG, "Component type in entity: " + json["name"].get<std::string>() + " is invalid");
                    }
                } else {
                    Log::warn(TAG, "Component type in entity: " + json["name"].get<std::string>() + " does not have a type");
                }
            }
        } else {
            Log::warn(TAG, "Entity: " + json["name"].get<std::string>() + " does not have components");
        }
    } else {
        Log::warn(TAG, "Entity does not have a name");
    }
}

Entity::Entity(Entity &old) {
    for (auto &component : old.components) {
        components[component.first] = std::make_pair(std::move(component.second.first->clone()), component.second.second);
    }
}

nlohmann::json Entity::serialize() {
    nlohmann::json json;
    json["name"]=name;
    for(auto& component : components){
        auto comp = component.second.first->serialize();
        comp["enabled"] = component.second.second;
        json["components"].push_back(comp);
    }
    return json;
}
