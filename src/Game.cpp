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

        ////TODO: Implement update and tick
    }
}

void Game::render() {
    if (!states.empty()) {
        getState().render(&renderWindow);
    }
}
