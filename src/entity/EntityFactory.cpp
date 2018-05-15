#include <fstream>
#include "EntityFactory.h"
#include "../utils/Log.h"

EntityFactory::EntityFactory(std::string file) {
    std::ifstream ifs("res/" + file);
    if (ifs.is_open()) {
        nlohmann::json j = nlohmann::json::parse(ifs);
        if (!j.empty()) {
            for (auto it = j.begin(); it != j.end(); it++) {
                entities[it.key()] = std::make_unique<Entity>(it.value());
            }
        }
    } else {
        Log::error(TAG, "Could not open Entities file");
    }
}

std::shared_ptr<Entity> EntityFactory::get(std::string name) {
    return std::make_shared<Entity>(*entities[name]);
}
