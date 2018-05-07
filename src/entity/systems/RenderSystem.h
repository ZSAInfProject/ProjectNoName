//
// Created by zakrent on 5/7/18.
//

#ifndef NONAME_RENDERSYSTEM_H
#define NONAME_RENDERSYSTEM_H

#include "System.h"

class RenderSystem : public System{
public:
    void processEntity(Entity *entity) override {
        return;
    }
};

#endif //NONAME_RENDERSYSTEM_H
