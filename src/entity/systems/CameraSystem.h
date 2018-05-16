#ifndef NONAME_CAMERASYSTEM_H
#define NONAME_CAMERASYSTEM_H

#include "System.h"
#include "../components/Position.h"
#include "../components/Camera.h"
#include "../../Game.h"

class CameraSystem : public System{
public:
    stageEnum getStage() override { return render; };

    explicit CameraSystem(sf::View& camera) : camera(camera) {};

    void processEntity(std::shared_ptr<Entity> entity, std::chrono::microseconds dt) override {
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
