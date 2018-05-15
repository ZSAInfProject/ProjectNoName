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

    explicit SpriteComponent(nlohmann::json json) {
        texture.loadFromFile("res/" + json["path"].get<std::string>());
        size = {json["size"][0].get<int>(), json["size"][1].get<int>()};
        sprite = sf::Sprite(texture);
        sprite.setScale(size.x / texture.getSize().x, -size.x / texture.getSize().x);
    };
};

#endif //NONAME_SPRITE_H
