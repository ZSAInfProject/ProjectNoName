#ifndef NONAME_CAMERA_H
#define NONAME_CAMERA_H

#include "Component.h"

class CameraComponent : public Component{
public:
    static const componentId Id = componentId::Camera;

    nlohmann::json serialize() override {
        nlohmann::json json;
        json["type"] = "Camera";
        return json;
    };

    std::unique_ptr<Component> clone() override {
        return std::make_unique<CameraComponent>(*this);
    }

    explicit CameraComponent(nlohmann::json json) {};
};

#endif //NONAME_CAMERA_H
