#ifndef NONAME_ENTITY_H
#define NONAME_ENTITY_H

#include <SFML/Graphics.hpp>
#include <chrono>
#include <unordered_map>
#include "../../deps/json.h"
#include "../state/State.h"
#include "components/Component.h"


//! In game moving entity
/*!
 * Represents entity - object in game that can move. It's position is not quantized.
 */
class Entity {
    int ID;
    std::unordered_map<uint, std::unique_ptr<Component>> components;
public:

    template<typename T>
    void addComponent(std::unique_ptr<Component> component)
    {
        components[int(T::Id)] = std::move(component);
    }

    template<typename T>
    T* getComponent()
    {
        uint id = T::Id;
        auto it = components.find(id);
        if (it == components.end())
            return nullptr;
        return dynamic_cast<T*>(it->second.get());
    }


    Entity() = default;

    Entity(Entity& old);

    explicit Entity(nlohmann::json json);

    std::string name;

    static constexpr auto TAG = "Entity";
};


#endif //NONAME_ENTITY_H
