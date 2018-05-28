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
    static const componentId id = componentId::Unknown;
    virtual ~Component() = default;

    virtual std::unique_ptr<Component> clone() = 0;

    virtual  nlohmann::json serialize() = 0;

    Component() = default;
    Component(Component& old) = default;;
    explicit Component(nlohmann::json json) {};
};

#endif //NONAME_COMPONENT_H
