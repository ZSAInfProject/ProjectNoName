//
// Created by zakrent on 6/8/18.
//

#ifndef NONAME_MINING_H
#define NONAME_MINING_H

#include "Component.h"
#include "../../../deps/json.h"

class MiningComponent : public Component{
public:
    static const componentId Id = componentId::Mining;

    bool miningInProgress;
    std::chrono::system_clock::time_point lastMine;
    sf::Vector2i miningTarget;
    float miningLeft;
    std::chrono::milliseconds miningDuration;

    nlohmann::json serialize() override {
        nlohmann::json json;
        json["type"] = "Mining";
        json["duration"] = miningDuration.count();
        return json;
    };

    std::unique_ptr<Component> clone() override {
        return std::make_unique<MiningComponent>(*this);
    }

    explicit MiningComponent(nlohmann::json json) {
        lastMine = std::chrono::system_clock::now();
        miningInProgress=false;
        miningLeft = 0;
        miningDuration = std::chrono::milliseconds(json["duration"].get<int>());
    };
};
#endif //NONAME_MINING_H
