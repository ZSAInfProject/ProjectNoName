#ifndef NONAME_SYSTEM_H
#define NONAME_SYSTEM_H

#include "../Entity.h"

enum stageEnum{
    update,
    tick,
    render
};

class System{
public:
    static const stageEnum stage = tick;
    virtual void processEntity(std::shared_ptr<Entity> entity, std::chrono::microseconds dt = std::chrono::microseconds(0)) = 0;
    virtual stageEnum getStage() { return stage; };
};


#endif //NONAME_SYSTEM_H
