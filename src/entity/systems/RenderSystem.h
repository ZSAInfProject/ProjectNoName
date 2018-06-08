#ifndef NONAME_RENDERSYSTEM_H
#define NONAME_RENDERSYSTEM_H


#include "System.h"
#include "../components/Sprite.h"
#include "../components/Position.h"
#include "../../Game.h"
#include "../components/ObjectPosition.h"

class RenderSystem : public System {
public:
    stageEnum getStage() { return render; };
    void processEntity(std::shared_ptr<Entity> entity, std::chrono::microseconds dt) override {
        auto* spriteComponent = entity->getComponent<SpriteComponent>();
        auto* positionComponent = entity->getComponent<PositionComponent>();
        if(!spriteComponent)
            return;

        sf::Vector2f position;
        if(!positionComponent) {
            auto* objectPositionComponent = entity->getComponent<ObjectPositionComponent>();
            if (!objectPositionComponent) {
                return;;
            } else {
                position = sf::Vector2f(objectPositionComponent->position) * 16.0f;
            }
        }
        else {
            position = positionComponent->position;
        }

        position.y += spriteComponent->sprite.getGlobalBounds().height;
        spriteComponent->sprite.setPosition(position);
        Game::getRenderWindow().draw(spriteComponent->sprite);
    }
};

#endif //NONAME_RENDERSYSTEM_H
