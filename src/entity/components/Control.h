#ifndef NONAME_CONTROL_H
#define NONAME_CONTROL_H

#include "Component.h"

class ControlComponent : public Component{
public:
    static const componentId Id = componentId::Control;

    float speed = 1.0f;

    explicit ControlComponent(nlohmann::json json) {
        speed = json["speed"].get<float>();
    };
};

#endif //NONAME_CONTROL_H
