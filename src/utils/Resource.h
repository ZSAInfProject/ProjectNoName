//
// Created by zakrent on 6/5/18.
//

#ifndef NONAME_RESOURCE_H
#define NONAME_RESOURCE_H

#include <string>

enum resourceType {
    unknown,
    stone,
    ironOre,
    copperOre,
};

struct Resource {
    enum resourceType type;
    unsigned int amount = 0;
};

enum resourceType getResourceType(std::string type);
std::string getResourceType(enum resourceType);

#endif //NONAME_RESOURCE_H
