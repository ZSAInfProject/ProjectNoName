#ifndef NONAME_CAMERA_H
#define NONAME_CAMERA_H

#include "Component.h"

class CameraComponent : public Component{
public:
    static const componentId Id = componentId::Camera;

    explicit CameraComponent(nlohmann::json json) {};
};

#endif //NONAME_CAMERA_H
