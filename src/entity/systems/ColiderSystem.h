//
// Created by piotr on 21.05.18.
//

#ifndef NONAME_COLIDERSYSTEM_H
#define NONAME_COLIDERSYSTEM_H

#include "System.h"
#include "../../world/World.h"
#include "../components/Position.h"
#include "../../tile/Tile.h"

class ColiderSystem: public System{
private:
    World& world;

    int cast(float f){
        if(f>0) return (int)f;
        else return ((int) f)-1;
    }

public:

    ColiderSystem(World& world) : world(world) {}

    stageEnum getStage() override { return update; };

    void processEntity(std::shared_ptr<Entity> entity, std::chrono::microseconds dt){
        auto* positionComponent = entity->getComponent<PositionComponent>();
        auto* coliderComponent = entity->getComponent<ColiderComponent>();
        if(!positionComponent || !coliderComponent) return;

        int sizeX = coliderComponent->size.x;
        int sizeY = coliderComponent->size.y;

        if(positionComponent->speed.x>0){
            positionComponent->position.x+=positionComponent->speed.x*dt.count()/1e6;
            for(int i = 0;i<=(coliderComponent->size.y)*2;i++){

                auto tile = world.getTile(cast(positionComponent->position.x+sizeX),cast(positionComponent->position.y+i/2));
                if(TileDatabase::get()[tile.tileId].isSolid){
                    positionComponent->position.x-=positionComponent->speed.x*dt.count()/1e6;
                }
            }
        }
        if(positionComponent->speed.x<0){
            positionComponent->position.x+=positionComponent->speed.x*dt.count()/1e6;
            for(int i = 0;i<=(coliderComponent->size.y)*2;i++){

                int sizeX = coliderComponent->size.x;

                auto tile = world.getTile(cast(positionComponent->position.x),cast(positionComponent->position.y+i/2));
                if(TileDatabase::get()[tile.tileId].isSolid){
                    positionComponent->position.x-=positionComponent->speed.x*dt.count()/1e6;
                }
            }
        }
        if(positionComponent->speed.y>0){
            positionComponent->position.y+=positionComponent->speed.y*dt.count()/1e6;
            for(int i = 0;i<=(coliderComponent->size.x)*2;i++){


                auto tile = world.getTile(cast(positionComponent->position.x+i/2),cast(positionComponent->position.y+sizeY));
                if(TileDatabase::get()[tile.tileId].isSolid){
                    positionComponent->position.y-=positionComponent->speed.y*dt.count()/1e6;
                }
            }
        }
        if(positionComponent->speed.y<0){
            positionComponent->position.y+=positionComponent->speed.y*dt.count()/1e6;
            for(int i = 0;i<=(coliderComponent->size.x)*2;i++){

                int sizeY = coliderComponent->size.y;

                auto tile = world.getTile(cast(positionComponent->position.x+i/2),cast(positionComponent->position.y));
                if(TileDatabase::get()[tile.tileId].isSolid){
                    positionComponent->position.y-=positionComponent->speed.y*dt.count()/1e6;
                }
            }
        }

        /*positionComponent->position.y+=positionComponent->speed.y*dt.count()/1e6;

        auto tile = world.getTile(cast(positionComponent->position.x),cast(positionComponent->position.y));
        if(TileDatabase::get()[tile.tileId].isSolid){
            positionComponent->position.y-=positionComponent->speed.y*dt.count()/1e6;
        }

        positionComponent->position.x+=positionComponent->speed.x*dt.count()/1e6;

        tile = world.getTile(cast(positionComponent->position.x),cast(positionComponent->position.y));
        if(TileDatabase::get()[tile.tileId].isSolid)
            positionComponent->position.x-=positionComponent->speed.x*dt.count()/1e6;*/


    }
};


#endif //NONAME_COLIDERSYSTEM_H
