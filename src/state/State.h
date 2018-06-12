#ifndef NONAME_STATE_H
#define NONAME_STATE_H

#include <chrono>
#include <SFML/Graphics/RenderWindow.hpp>

//!Base class for program states
/*!
 * Represents a program state like menu or game. Kept on stack by Game.
 */
class State {
protected:
    State() = default;
public:
    //!Function called every loop iteration
    /*!
     * update() is called every main loop iteration. Should be used for implementing stuff like physics etc.
     * @param deltaTime Time which passed between update() calls
     */
    virtual void update(std::chrono::microseconds deltaTime) = 0;
    //! Function called every screen refresh
    /*!
     * render() is called every screen refresh. Should be used for rendering.
     * @param renderWindow SFML render window reference
     */
    virtual void render(float deltaTime) = 0;
    //! Function called periodicaly
    /*!
     *  tick() is called periodicaly. Period is set in Game singleton.
     *  Can be used for stuff that doesn't need to run very often, for example recalculating AI path.
     */
    virtual void tick() = 0;

    virtual void handleEvent(sf::Event event) = 0;


    State(State& other) = delete;
    State& operator= (State& other) = delete;
    virtual ~State() = default;
};

#endif //NONAME_STATE_H
