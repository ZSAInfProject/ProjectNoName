#ifndef NONAME_MOTIONSYSTEM_H
#define NONAME_MOTIONSYSTEM_H


#include "System.h"
#include "../components/Position.h"
#include "../components/Camera.h"
#include "../../Game.h"

class CameraSystem : public System{
public:
    static const stageEnum stage = render;

    explicit CameraSystem(sf::View& camera) : camera(camera) {};

    void processEntity(Entity *entity, std::chrono::microseconds dt) override {
        auto* cameraComponent = entity->getComponent<CameraComponent>();
        auto* positionComponent = entity->getComponent<PositionComponent>();
        if(!positionComponent || !cameraComponent)
            return;
        camera.setCenter(positionComponent->position);
    }

private:
    sf::View& camera;
};


#endif //NONAME_MOTIONSYSTEM_H
