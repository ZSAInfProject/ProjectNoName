//
// Created by zakrent on 5/7/18.
//

#ifndef NONAME_MOTIONSYSTEM_H
#define NONAME_MOTIONSYSTEM_H


#include "System.h"
#include "../components/Position.h"

class MotionSystem : public System{
public:
    void processEntity(Entity *entity) override {
        auto* positionComponent = entity->getComponent<PositionComponent>();
        if(!positionComponent)
            return;
        positionComponent->lastPosition = positionComponent->position;
        positionComponent->position += positionComponent->speed;
    }
};


#endif //NONAME_MOTIONSYSTEM_H
