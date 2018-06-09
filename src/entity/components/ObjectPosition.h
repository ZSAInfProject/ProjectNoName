#ifndef NONAME_OBJECTPOSITION_H
#define NONAME_OBJECTPOSITION_H

#include <SFML/System.hpp>
#include "Component.h"
#include "../../../deps/json.h"

class ObjectPositionComponent : public Component{
public:
    static const componentId Id = componentId::ObjectPosition;

    sf::Vector2i position;

    nlohmann::json serialize() override {
        nlohmann::json json;
        json["type"] = "ObjectPosition";
        json["position"][0] = position.x;
        json["position"][1] = position.y;
        return json;
    };

    std::unique_ptr<Component> clone() override {
        return std::make_unique<ObjectPositionComponent>(*this);
    }

    explicit ObjectPositionComponent(nlohmann::json json){
        position = {json["position"][0].get<int>(), json["position"][1].get<int>()};
    };

    ObjectPositionComponent() {
        position = {0, 0};
    }
};

#endif //NONAME_OBJECTPOSITION_H
