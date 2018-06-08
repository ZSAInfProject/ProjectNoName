#include <fstream>
#include "EntityFactory.h"
#include "../utils/Log.h"

EntityFactory::EntityFactory(std::string file) {
    std::ifstream ifs("res/" + file);
    if (ifs.is_open()) {
        nlohmann::json j = nlohmann::json::parse(ifs);
        if (!j.empty()) {
            for (auto it = j.begin(); it != j.end(); it++) {
                std::ifstream entity_stream("res/" + it.value().get<std::string>());
                if (entity_stream.is_open()) {
                    auto entity_json = nlohmann::json::parse(entity_stream);
                    entities[it.key()] = std::make_unique<Entity>(entity_json);
                } else {
                    Log::error(TAG, "Could not open entity file:" + it.value().get<std::string>());
                }
            }
        }
    } else {
        Log::error(TAG, "Could not open Entities file");
    }
}

std::shared_ptr<Entity> EntityFactory::get(std::string name) {
    if (entities.find(name) == entities.end())
    {
        Log::error(TAG, "No entity named: " + name);
        return nullptr;
    }
    return std::make_shared<Entity>(*entities[name]);
}
