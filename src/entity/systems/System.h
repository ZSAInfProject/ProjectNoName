//
// Created by zakrent on 5/7/18.
//

#ifndef NONAME_SYSTEM_H
#define NONAME_SYSTEM_H

#include "../Entity.h"

class System{
public:
    virtual void processEntity(Entity *entity) = 0;
};


#endif //NONAME_SYSTEM_H
