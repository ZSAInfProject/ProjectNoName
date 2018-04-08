#ifndef NONAME_GAME_H
#define NONAME_GAME_H


#include <stack>
#include "state/State.h"

class Game {
public:
    static Game& get() {
        static Game instance;
        return instance;
    }

    Game(Game const&) = delete;
    void operator= (Game const&) = delete;

private:
    Game() = default;

    std::stack<std::unique_ptr<State>> states;

public:
    void pushState(std::unique_ptr<State> state);
    void popState();
    State& getState();

    void run();

private:
    std::chrono::microseconds minimumLoopTime = std::chrono::microseconds(120);

    void render();
    sf::RenderWindow renderWindow;
    std::chrono::system_clock::time_point previous_frame = std::chrono::system_clock::now();
    int frame_per_second = 120;

    void update();
    std::chrono::system_clock::time_point previous_time = std::chrono::system_clock::now();

    void tick();
    std::chrono::system_clock::time_point previous_tick = std::chrono::system_clock::now();
    std::chrono::microseconds tick_period = std::chrono::microseconds(1000000);
};


#endif //NONAME_GAME_H
