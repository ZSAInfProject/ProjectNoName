#ifndef NONAME_RENDERSYSTEM_H
#define NONAME_RENDERSYSTEM_H

#include "System.h"

class RenderSystem : public System{
public:
    static const stageEnum stage = render;
    void processEntity(Entity *entity) override {
        return;
    }
};

#endif //NONAME_RENDERSYSTEM_H
