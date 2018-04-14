#include <SFML/Window/Event.hpp>
#include <SFML/System.hpp>
#include "state/GameState.h"
#include "Game.h"
#include "utils/Log.h"

void Game::pushState(std::unique_ptr<State> state) {
    Log::info(TAG , "New state pushed on stack");
    states.push(std::move(state));
}

void Game::popState() {
    Log::info(TAG ,"State popped from stack");
    states.pop();
}

State& Game::getState() {
    return *states.top();
}

void Game::run() {
    Log::info(TAG ,"Main loop started");
    renderWindow.create({800, 600}, "ProjectNoName");
    pushState(std::make_unique<GameState>());

    std::chrono::system_clock::time_point loopStart;
    while (renderWindow.isOpen())
    {

        loopStart = std::chrono::system_clock::now();

        sf::Event event{};
        while (renderWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                renderWindow.close();
        }

        render();

        update();

        tick();

        auto now = std::chrono::system_clock::now();
        auto loopTime = std::chrono::duration_cast<std::chrono::microseconds>(now - loopStart);
        if(loopTime < minimumLoopTime){
            sf::sleep(sf::microseconds(minimumLoopTime.count() - loopTime.count()));
        }
    }
}

void Game::render() {
    auto now = std::chrono::system_clock::now();
    auto time_elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - previous_frame).count();

    if (time_elapsed >= 1000000/frame_per_second) {

        previous_frame = now - std::chrono::microseconds(time_elapsed - 1000000/frame_per_second);
        ChunkGenerator chunkGenerator(10);

        if (!states.empty()) {
            getState().render(&renderWindow);
        }
        renderWindow.display();
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
    Log::verbose(TAG, "tick");
    auto now = std::chrono::system_clock::now();
    auto time_elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - previous_tick);

    if (time_elapsed >= tick_period) {

        previous_tick = now - std::chrono::microseconds(time_elapsed - tick_period);

        if (!states.empty()) {
            getState().tick();
        }

    }
}
