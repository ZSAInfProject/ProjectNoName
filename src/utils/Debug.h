#ifndef NONAME_DEBUG_H
#define NONAME_DEBUG_H


#include <chrono>
#include <vector>
#include <string>
#include <queue>
#include "Log.h"
#include "../entity/Entity.h"

//! Class responsible for updating visual debug items
class Debug : public LogSubscriber {
    int loadedChunks;
    int entityCount;
    float ups;
    float fps;
    bool active;
    bool toggled;

    std::vector<float> fpsHistory;
    std::vector<float> upsHistory;

    std::shared_ptr<Entity> player;
    std::vector<std::pair<enum Mode,std::string>> logMessages;
    std::vector<std::string> errorMessages;
    void updateErrors();
    void updateMainDebug();
public:
    //! Method called every update
    void update();
    //! Method called every tick
    void tick();

    //! Update update duration
    void reportUpdateTime(std::chrono::microseconds);
    //! Update render duration
    void reportRenderTime(std::chrono::milliseconds);
    //! Update player entity
    void reportPlayerEntity(std::shared_ptr<Entity>);
    //! Update loaded chunks count
    void reportLoadedChunks(int n);
    //! Update entity count
    void reportEntityCount(int n);

    void notify(enum Mode, std::string, std::string) override;

    Debug();
};


#endif //NONAME_DEBUG_H
