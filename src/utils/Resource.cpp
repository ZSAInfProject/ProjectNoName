#include "Resource.h"

enum resourceType getResourceType(std::string type){
    if(type == "stone"){
        return resourceType::stone;
    }
    else if(type == "ironOre"){
        return resourceType::ironOre;
    }
    else if(type == "copperOre"){
        return resourceType::copperOre;
    }
    return resourceType::unknown;
}

std::string getResourceType(enum resourceType type) {
    switch(type){
        case unknown:
            return "unknown";
            break;
        case stone:
            return "stone";
            break;
        case ironOre:
            return "ironOre";
            break;
        case copperOre:
            return "copperOre";
            break;
    }
    return "unknown";
}
