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

    std::unique_ptr<Component> clone() override {
        return std::make_unique<PositionComponent>(*this);
    }

    explicit PositionComponent(nlohmann::json json){
        position = {0, 0};
        lastPosition = {0, 0};
        speed = {0, 0};
    };
};

#endif //NONAME_POSITION_H
