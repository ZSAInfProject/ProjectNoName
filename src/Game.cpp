#include <SFML/Window/Event.hpp>
#include <SFML/System.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <SFGUI/SFGUI.hpp>
#include <SFML/Graphics.hpp>
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
    renderWindow.create({Settings::get<unsigned int>("resolution_x"), Settings::get<unsigned int>("resolution_y")}, "ProjectNoName",
        sf::Style::Close);
    pushState(std::make_shared<GameState>());

    renderWindow.setActive(false);
    imGuiUpdatedThisFrame = true;

    std::thread renderThread([this](){
        ImGui::SFML::Init(renderWindow);
        ImGui::NewFrame();
        renderWindow.setVerticalSyncEnabled(true);
        while(renderWindow.isOpen()){
            renderWindow.setActive(true);
            render();
            sf::Event event{};
            while (renderWindow.pollEvent(event)) {
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
                ImGui::SFML::ProcessEvent(event);
                if(!ImGui::IsAnyItemActive()) {
                    getState().handleEvent(event);
                }
            }
            renderWindow.display();
        }
        ImGui::SFML::Shutdown;
    });

    std::chrono::system_clock::time_point loopStart;
    while (renderWindow.isOpen())
    {
        loopStart = std::chrono::system_clock::now();

        renderMutex.lock();

        update();

        tick();

        auto now = std::chrono::system_clock::now();
        auto loopTime = std::chrono::duration_cast<std::chrono::microseconds>(now - loopStart);

        imGuiUpdatedThisFrame = true;

        renderMutex.unlock();

        if(loopTime < minimumLoopTime){
            sf::sleep(sf::microseconds(minimumLoopTime.count() - loopTime.count()));
        }
    }
    renderThread.join();
}

void Game::render() {
    //Wait for imGui update
    while(!imGuiUpdatedThisFrame && renderMutex.try_lock()){
        renderMutex.unlock();
        sf::sleep(sf::microseconds(10));
    }
    renderMutex.lock();
    auto now = std::chrono::system_clock::now();
    auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - previous_render);
    previous_render = now;
    debug.reportRenderTime(deltaTime);
    if (!states.empty()) {
        getState().render(deltaTime.count());
    }
    ImGui::SFML::Render(renderWindow);
    ImGui::SFML::Update(renderWindow, sf::milliseconds(static_cast<sf::Int32>(deltaTime.count())));
    imGuiUpdatedThisFrame = false;
    renderMutex.unlock();
}

void Game::update() {
    auto now = std::chrono::system_clock::now();
    auto deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - previous_time);
    previous_time = now;
    debug.reportUpdateTime(deltaTime);
    debug.update();
    if (!states.empty()) {
        getState().update(deltaTime);
    }
}

void Game::tick() {
    Log::verbose(TAG, "tick");
    auto now = std::chrono::system_clock::now();
    auto time_elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - previous_tick);
    if (time_elapsed >= tick_period) {
        debug.tick();
        previous_tick = now - std::chrono::microseconds(time_elapsed - tick_period);

        if (!states.empty()) {
            getState().tick();
        }

    }
}

bool Game::canUpdateimGui() {
    return !Game::get().imGuiUpdatedThisFrame;
}
