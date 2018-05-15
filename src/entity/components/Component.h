#ifndef NONAME_COMPONENT_H
#define NONAME_COMPONENT_H

enum componentId{
    Unknown,
    Position,
    Sprite,
    Camera,
    Control
};

class Component{
public:
    static const componentId Id = componentId::Unknown;
    virtual ~Component() = default;
    Component() = default;
    Component(Component& old) = default;;
    explicit Component(nlohmann::json json) {};
};

#endif //NONAME_COMPONENT_H
