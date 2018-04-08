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
    void render();

    sf::RenderWindow renderWindow;
};


#endif //NONAME_GAME_H
