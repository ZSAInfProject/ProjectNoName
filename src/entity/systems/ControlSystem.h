#ifndef NONAME_CONTROLSYSTEM_H
#define NONAME_CONTROLSYSTEM_H

#include "System.h"
#include "../components/Position.h"
#include "../components/Control.h"
#include "../../utils/Controls.h"

class ControlSystem : public System{
public:
    stageEnum getStage() override { return update; };
    void processEntity(std::shared_ptr<Entity> entity, std::chrono::microseconds dt) override {
        auto* positionComponent = entity->getComponent<PositionComponent>();
        auto* controlComponent = entity->getComponent<ControlComponent>();
        if(!positionComponent || !controlComponent)
            return;

        if (Controls::isKeyboardButtonPressed(sf::Keyboard::W)) {
            positionComponent->speed.y = controlComponent->speed;
        }
        if (Controls::isKeyboardButtonPressed(sf::Keyboard::S)) {
            positionComponent->speed.y = -controlComponent->speed;
        }
        if (Controls::isKeyboardButtonPressed(sf::Keyboard::A)) {
            positionComponent->speed.x = -controlComponent->speed;
        }
        if (Controls::isKeyboardButtonPressed(sf::Keyboard::D)) {
            positionComponent->speed.x = controlComponent->speed;
        }
    }
};

#endif //NONAME_CONTROLSYSTEM_H
