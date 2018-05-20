#ifndef NONAME_SPRITE_H
#define NONAME_SPRITE_H

#include <SFML/Graphics/Sprite.hpp>
#include "Component.h"

class SpriteComponent : public Component{
public:
    static const componentId Id = componentId::Sprite;

    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2i size;

    std::unique_ptr<Component> clone() override {
        return std::make_unique<SpriteComponent>(*this);
    }


    SpriteComponent(SpriteComponent& old) = default;
    explicit SpriteComponent(nlohmann::json json) {
        texture.loadFromFile("res/" + json["path"].get<std::string>());
        size = {json["size"][0].get<int>(), json["size"][1].get<int>()};
        sprite = sf::Sprite(texture);
        sprite.setScale(1,1);
        sprite.setScale(size.x*32.0f/texture.getSize().y , -size.y*32.0f/texture.getSize().y);
    };
};

#endif //NONAME_SPRITE_H
