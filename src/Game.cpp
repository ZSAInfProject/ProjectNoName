#include <SFML/Window/Event.hpp>
#include <SFML/System.hpp>
#include "state/GameState.h"
#include "Game.h"
#include "utils/Log.h"
#include "utils/Settings.h"
#include "utils/Controls.h"

void Game::pushState(std::shared_ptr<State> state) {
    Log::info(TAG , "New state pushed on stack");
    states.push(std::move(state));
}

void Game::popState() {
    Log::info(TAG ,"State popped from stack");
    states.pop();
}

State& Game::getState() {
    return *Game::get().states.top();
}

sf::RenderWindow& Game::getRenderWindow() {
    return Game::get().renderWindow;
}

void Game::run() {
    Log::info(TAG ,"Main loop started");
    renderWindow.create({Settings::get<unsigned int>("resolution_x"), Settings::get<unsigned int>("resolution_y")}, "ProjectNoName");
    pushState(std::make_shared<GameState>());

    renderWindow.setActive(false);

    std::thread renderThread([this](){
        renderWindow.setVerticalSyncEnabled(true);
        while(renderWindow.isOpen()){
            sf::Event event{};
            while (renderWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    renderWindow.close();
                switch(event.type){
                    case sf::Event::Closed:
                        renderWindow.close();
                        break;
                    case sf::Event::KeyPressed:
                        Controls::setKeyboardKey(event.key.code, true);
                        break;
                    case sf::Event::KeyReleased:
                        Controls::setKeyboardKey(event.key.code, false);
                        break;
                    case sf::Event::MouseButtonPressed:
                        Controls::setMouseButton(event.mouseButton.button, true);
                        break;
                    case sf::Event::MouseButtonReleased:
                        Controls::setMouseButton(event.mouseButton.button, false);
                        break;
                    case sf::Event::MouseMoved:
                        Controls::setMousePosition(sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
                        break;
                    default:
                        break;
                }
            }
            render();
        }
    });

    std::chrono::system_clock::time_point loopStart;
    while (renderWindow.isOpen())
    {

        loopStart = std::chrono::system_clock::now();

        update();

        tick();

        auto now = std::chrono::system_clock::now();
        auto loopTime = std::chrono::duration_cast<std::chrono::microseconds>(now - loopStart);
        if(loopTime < minimumLoopTime){
            sf::sleep(sf::microseconds(minimumLoopTime.count() - loopTime.count()));
        }
    }
    renderThread.join();
}

void Game::render() {
    if (!states.empty()) {
        getState().render();
    }
    renderWindow.display();
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
