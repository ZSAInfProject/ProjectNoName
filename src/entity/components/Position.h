#ifndef NONAME_POSITION_H
#define NONAME_POSITION_H

#include <SFML/System.hpp>
#include "Component.h"
#include "../../../deps/json.h"

class PositionComponent : public Component{
public:
    static const componentId Id = componentId::Position;

    sf::Vector2f position;
    sf::Vector2f lastPosition;
    sf::Vector2f speed;

    nlohmann::json serialize() override {
        nlohmann::json json;
        json["type"] = "Position";
        json["position"][0] = position.x;
        json["position"][1] = position.y;
        json["speed"][0] = speed.x;
        json["speed"][1] = speed.y;
        return json;
    };

    std::unique_ptr<Component> clone() override {
        return std::make_unique<PositionComponent>(*this);
    }

    explicit PositionComponent(nlohmann::json json){
        position = {json["position"][0].get<float>(), json["position"][1].get<float>()};
        speed = {json["speed"][0].get<float>(), json["speed"][1].get<float>()};
        lastPosition = position;
    };
};

#endif //NONAME_POSITION_H
