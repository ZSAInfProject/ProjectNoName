#ifndef NONAME_STATE_H
#define NONAME_STATE_H

#include <chrono>
#include <SFML/Graphics/RenderWindow.hpp>

class State {
public:
    virtual void update(std::chrono::microseconds deltaTime) = 0;
    virtual void render(sf::RenderWindow* renderWindow) = 0;
    virtual void tick() = 0;

    State(State& other) = delete;
    State& operator= (State& other) = delete;
    virtual ~State() = default;
};

#endif //NONAME_STATE_H
