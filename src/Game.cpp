#include <SFML/Window/Event.hpp>
#include "Game.h"

void Game::pushState(std::unique_ptr<State> state) {
    states.push(std::move(state));
}

void Game::popState() {
    states.pop();
}

State& Game::getState() {
    return *states.top();
}

void Game::run() {
    renderWindow.create({800, 600}, "ProjectNoName");

    while (renderWindow.isOpen())
    {

        sf::Event event{};
        while (renderWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                renderWindow.close();
        }

        render();

        update();

        tick();
    }
}

void Game::render() {
    auto now = std::chrono::system_clock::now();
    auto time_elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - previous_frame).count();

    if (time_elapsed >= 1000000/frame_per_second) {

        previous_frame = now - std::chrono::microseconds(time_elapsed - 1000000/frame_per_second);

        if (!states.empty()) {
            getState().render(&renderWindow);
        }
    }
}

void Game::update() {
    auto now = std::chrono::system_clock::now();
    auto deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - previous_time);
    previous_time = now;

    if (!states.empty()) {
        getState().update(deltaTime);
    }
}

void Game::tick() {
    auto now = std::chrono::system_clock::now();
    auto time_elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - previous_tick).count();

    if (time_elapsed >= tick_period) {

        previous_tick = now - std::chrono::microseconds(time_elapsed - tick_period);

        if (!states.empty()) {
            getState().tick();
        }

    }
}
