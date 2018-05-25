//
// Created by piotr on 21.05.18.
//

#ifndef NONAME_COLIDER_H
#define NONAME_COLIDER_H

#include <bits/shared_ptr.h>
#include "Component.h"
#include "../Entity.h"

#include "Position.h"

class ColiderComponent: public Component{
public:
    static const componentId Id = componentId::Colider;

    sf::Vector2i size;
    explicit ColiderComponent(nlohmann::json json){
        size = {json["size"][0].get<int>(), json["size"][1].get<int>()};
    }

    std::unique_ptr<Component> clone() override {
        return std::make_unique<ColiderComponent>(*this);
    }

};

#endif //NONAME_COLIDER_H


