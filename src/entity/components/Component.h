#ifndef NONAME_COMPONENT_H
#define NONAME_COMPONENT_H

#include <bits/unique_ptr.h>
#include "../../../deps/json.h"

enum componentId {
    Unknown,
    Position,
    Sprite,
    Camera,
    Control,
    Inventory,
    Mining,
    Building,
    Ladder,
    ObjectPosition
};

class Component {
public:
    static const componentId id = componentId::Unknown;
    virtual ~Component() = default;

    virtual std::unique_ptr<Component> clone() = 0;

    virtual nlohmann::json serialize() = 0;

    Component() = default;
    Component(Component& old) = default;
};

#endif //NONAME_COMPONENT_H
