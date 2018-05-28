#ifndef NONAME_CONTROL_H
#define NONAME_CONTROL_H

#include "Component.h"

class ControlComponent : public Component{
public:
    static const componentId Id = componentId::Control;

    float speed = 1;

    nlohmann::json serialize() override {
        nlohmann::json json;
        json["type"]="Control";
        json["speed"]=speed;
        return json;
    };

    std::unique_ptr<Component> clone() override {
        return std::make_unique<ControlComponent>(*this);
    }

    explicit ControlComponent(nlohmann::json json) {
        speed = json["speed"].get<float>();
    };
};

#endif //NONAME_CONTROL_H
