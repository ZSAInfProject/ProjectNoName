//
// Created by zakrent on 5/7/18.
//

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
    virtual void processEntity(Entity *entity) = 0;
};


#endif //NONAME_SYSTEM_H
