#ifndef NONAME_CAMERA_H
#define NONAME_CAMERA_H

#include "Component.h"

class CameraComponent : public Component{
public:
    static const componentId Id = componentId::Camera;
};

#endif //NONAME_CAMERA_H
