#include "Entity.h"
#include "components/Position.h"
#include "components/Sprite.h"
#include "components/Control.h"
#include "components/Camera.h"
#include "../utils/Log.h"

Entity::Entity(nlohmann::json json) {
    ID = 0;
    if (json.find("name") != json.end()) {
        name = json["name"].get<std::string>();
        if (json.find("components") != json.end()) {
            for (auto &component : json["components"]) {
                if (component.find("type") != component.end()) {
                    if (component["type"].get<std::string>() == "Position")
                        addComponent<PositionComponent>(std::make_unique<PositionComponent>(component));
                    else if (component["type"].get<std::string>() == "Sprite")
                        addComponent<SpriteComponent>(std::make_unique<SpriteComponent>(component));
                    else if (component["type"].get<std::string>() == "Control")
                        addComponent<ControlComponent>(std::make_unique<ControlComponent>(component));
                    else if (component["type"].get<std::string>() == "Camera")
                        addComponent<CameraComponent>(std::make_unique<CameraComponent>(component));
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
        components[component.first] = std::move(component.second->clone());
    }
}
