#ifndef NONAME_MOTIONSYSTEM_H
#define NONAME_MOTIONSYSTEM_H


#include "System.h"
#include "../components/Position.h"

class MotionSystem : public System{
public:
    stageEnum getStage() override { return update; };
    void processEntity(std::shared_ptr<Entity> entity, std::chrono::microseconds dt) override {
        auto* positionComponent = entity->getComponent<PositionComponent>();
        if(!positionComponent)
            return;
        positionComponent->lastPosition = positionComponent->position;
        positionComponent->position.x += positionComponent->speed.x*dt.count()/1e6;
        positionComponent->position.y += positionComponent->speed.y*dt.count()/1e6;
    }
};


#endif //NONAME_MOTIONSYSTEM_H
