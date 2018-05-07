#ifndef NONAME_SPRITE_H
#define NONAME_SPRITE_H

#include <SFML/Graphics/Sprite.hpp>
#include "Component.h"

class SpriteComponent : public Component{
public:
    static const componentId Id = componentId::Sprite;
    sf::Sprite sprite;
};

#endif //NONAME_SPRITE_H
