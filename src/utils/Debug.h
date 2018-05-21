#ifndef NONAME_DEBUG_H
#define NONAME_DEBUG_H


#include <chrono>
#include <vector>
#include <string>
#include <queue>
#include "Log.h"
#include "../entity/Entity.h"

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
public:
    std::vector<std::pair<enum Mode,std::string>> logMessages;
    void update();
    void tick();
    void reportUpdateTime(std::chrono::microseconds);
    void reportRenderTime(std::chrono::milliseconds);
    void reportPlayerEntity(std::shared_ptr<Entity>);
    void reportLoadedChunks(int n);
    void reportEntityCount(int n);

    void notify(enum Mode, std::string, std::string) override;

    Debug();
};


#endif //NONAME_DEBUG_H
