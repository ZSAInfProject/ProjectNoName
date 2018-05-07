#ifndef NONAME_RENDERSYSTEM_H
#define NONAME_RENDERSYSTEM_H


#include "System.h"
#include "../components/Sprite.h"
#include "../components/Position.h"
#include "../../Game.h"

class RenderSystem : public System {
public:
    static const stageEnum stage = render;
    void processEntity(Entity *entity) override {
        auto* spriteComponent = entity->getComponent<SpriteComponent>();
        auto* positionComponent = entity->getComponent<PositionComponent>();
        if(!positionComponent || !spriteComponent)
            return;

        auto position = positionComponent->position * (float)Chunk::TILE_SIZE;
        position.y += spriteComponent->sprite.getGlobalBounds().height;
        spriteComponent->sprite.setPosition(position);
        Game::getRenderWindow().draw(spriteComponent->sprite);
    }
};

#endif //NONAME_RENDERSYSTEM_H
