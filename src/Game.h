#ifndef NONAME_GAME_H
#define NONAME_GAME_H

#include "gui/GUI.h"
#include <stack>
#include "state/State.h"
#include "utils/Debug.h"
#include <mutex>
#include <memory>

//! Main program class
/*!
 * This class is a singleton. Contains main game loop.
 * Stores all states on stack and calls update(), render() and tick() on top state.
 * Call rate for each can be setup separately.
 */
class Game {

public:
    Debug debug;
    //! Push state on stack.
    void pushState(std::shared_ptr<State> state);
    //! Pop state from stack.
    void popState();
    //! Get reference to state on top of the stack
    static State& getState();

    //! Start main loop
    void run();

    //! Returns a reference to main render window
    static sf::RenderWindow& getRenderWindow();

    static bool canUpdateimGui();

    //! Minimum time for main loop iteration
    /*!
     * If main loop runs faster than the set up limit it will be slowed down to save CPU usage.
     */
    std::chrono::microseconds minimumLoopTime = std::chrono::microseconds(120);

    //! Period between consecutive tick() calls.
    /*!
     * tick() will be called periodically with a given period
     */
    std::chrono::microseconds tick_period = std::chrono::microseconds(1000000);

    static constexpr auto TAG = "GameSingleton";
private:
    bool imGuiUpdatedThisFrame;
    std::mutex renderMutex;

    //! Stack containing all states.
    std::stack<std::shared_ptr<State>> states;

    void render();
    std::chrono::system_clock::time_point previous_render = std::chrono::system_clock::now();
    sf::RenderWindow renderWindow;

    void update();
    std::chrono::system_clock::time_point previous_time = std::chrono::system_clock::now();

    void tick();
    std::chrono::system_clock::time_point previous_tick = std::chrono::system_clock::now();

    //Singleton stuff
public:
    //! Returns reference to singleton object.
    static Game& get() {
        static Game instance;
        return instance;
    }

    Game(Game const&) = delete;
    void operator= (Game const&) = delete;

private:
    Game() = default;
};

#endif //NONAME_GAME_H
